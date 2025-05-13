// steam_audio_static_mesh.cpp
#include "steam_audio_static_mesh.h"

#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/convex_polygon_shape3d.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <phonon.h> // IPLStaticMesh, IPLStaticMeshSettings, IPLMaterial

using namespace godot;

SteamAudioStaticMesh::SteamAudioStaticMesh() = default;

SteamAudioStaticMesh::~SteamAudioStaticMesh() {
    // release any previously created Steam Audio meshes
    for (auto &sm : static_meshes) {
        iplStaticMeshRelease(&sm);
    }
    static_meshes.clear();
}

void SteamAudioStaticMesh::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_proxy_mode",        "mode"), &SteamAudioStaticMesh::set_proxy_mode);
    ClassDB::bind_method(D_METHOD("get_proxy_mode"),             &SteamAudioStaticMesh::get_proxy_mode);
    ADD_PROPERTY(
        PropertyInfo(Variant::INT, "proxy_mode",PROPERTY_HINT_ENUM,"None,Convex,Custom"), "set_proxy_mode","get_proxy_mode");

    ClassDB::bind_method(D_METHOD("set_custom_proxy_mesh", "mesh"), &SteamAudioStaticMesh::set_custom_proxy_mesh);
    ClassDB::bind_method(D_METHOD("get_custom_proxy_mesh"),      &SteamAudioStaticMesh::get_custom_proxy_mesh);
    ADD_PROPERTY(
            PropertyInfo(Variant::OBJECT, "custom_proxy_mesh",PROPERTY_HINT_RESOURCE_TYPE,"Mesh"), "set_custom_proxy_mesh","get_custom_proxy_mesh");

    ClassDB::bind_method(D_METHOD("set_default_material",  "mat"),  &SteamAudioStaticMesh::set_default_material);
    ClassDB::bind_method(D_METHOD("get_default_material"),        &SteamAudioStaticMesh::get_default_material);

    // And still bind your proxy-generator:
    ClassDB::bind_method(D_METHOD("generate_proxy_mesh"), &SteamAudioStaticMesh::generate_proxy_mesh);

}

// Setters & getters ---------------------------------------------------
void SteamAudioStaticMesh::set_proxy_mode(int mode) {
    proxy_mode = mode;
}
int SteamAudioStaticMesh::get_proxy_mode() const {
    return proxy_mode;
}

void SteamAudioStaticMesh::set_custom_proxy_mesh(Ref<Mesh> mesh) {
    custom_proxy_mesh = mesh;
}
Ref<Mesh> SteamAudioStaticMesh::get_custom_proxy_mesh() const {
    return custom_proxy_mesh;
}

void SteamAudioStaticMesh::set_default_material(Ref<SteamAudioMaterial> mat) {
    default_material = mat;
}
Ref<SteamAudioMaterial> SteamAudioStaticMesh::get_default_material() const {
    return default_material;
}

// Core functionality --------------------------------------------------
void SteamAudioStaticMesh::generate_proxy_mesh() {
    // 1) Clean up any old meshes
    for (auto &sm : static_meshes) {
        iplStaticMeshRelease(&sm);
    }
    static_meshes.clear();

    // 2) Pick which Mesh to read triangles from
    Ref<Mesh> target;
    switch (proxy_mode) {
        case PROXY_NONE:
            target = cast_to<MeshInstance3D>(get_parent())->get_mesh();
            break;
        case PROXY_CONVEX: {
            Ref<Shape3D> hull_shape = cast_to<MeshInstance3D>(get_parent())->get_mesh()->create_convex_shape();
            ConvexPolygonShape3D *convex = cast_to<ConvexPolygonShape3D>(hull_shape.ptr());
            if (!convex) {
                ERR_PRINT("SteamAudioStaticMesh: convex cast failed");
                return;
            }
            Ref<ArrayMesh> debug_mesh = convex->get_debug_mesh();
            if (!debug_mesh.is_valid()) {
                ERR_PRINT("SteamAudioStaticMesh: debug mesh invalid");
                return;
            }
            target = debug_mesh;
            break;
        }
        case PROXY_CUSTOM:
            if (!custom_proxy_mesh.is_valid()) {
                ERR_PRINT("SteamAudioStaticMesh: custom_proxy_mesh not set");
                return;
            }
            target = custom_proxy_mesh;
            break;
        default:
            ERR_PRINT("SteamAudioStaticMesh: unknown proxy_mode");
            return;
    }

    if (!target.is_valid()) {
        ERR_PRINT("SteamAudioStaticMesh: target mesh invalid");
        return;
    }

    // 3) For each surface, build and add a Steam Audio static mesh
    const int surface_count = target->get_surface_count();
    for (int s = 0; s < surface_count; ++s) {
        Array arrays = target->surface_get_arrays(s);
        PackedVector3Array verts = arrays[Mesh::ARRAY_VERTEX];
        PackedInt32Array   idxs  = arrays[Mesh::ARRAY_INDEX];

        // --- build IPLMaterial from metadata or default_material ---
        Ref<Material> gm = target->surface_get_material(s);
        Ref<SteamAudioMaterial> sa_mat;
        if (gm.is_valid() && gm->has_meta("steam_audio")) {
            sa_mat = gm->get_meta("steam_audio");
        }
        if (!sa_mat.is_valid()) {
            sa_mat = default_material;
        }
        IPLMaterial material{};  // zero initialize all fields
        // absorption[0..2]
        material.absorption[0] = sa_mat->get_absorption().x;
        material.absorption[1] = sa_mat->get_absorption().y;
        material.absorption[2] = sa_mat->get_absorption().z;
        // scattering
        material.scattering    = sa_mat->get_scattering();
        // transmission[0..2]
        material.transmission[0] = sa_mat->get_transmission().x;
        material.transmission[1] = sa_mat->get_transmission().y;
        material.transmission[2] = sa_mat->get_transmission().z;

        // --- fill out static mesh settings ---
        IPLStaticMeshSettings settings{};

        // Number of vertices
        settings.numVertices = verts.size();

        // reinterpret Godot’s Vector3 buffer as Steam Audio’s IPLVector3[]
        const IPLVector3 *raw_vertices = reinterpret_cast<const IPLVector3 *>(verts.ptr());

        // drop the const so it matches IPLStaticMeshSettings::vertices (IPLVector3*)
        IPLVector3 *writable_vertices = const_cast<IPLVector3 *>(raw_vertices);
        settings.vertices = writable_vertices;

        // Number of triangles
        settings.numTriangles = idxs.size() / 3;

        // build triangle array
        std::vector<IPLTriangle> triangles;
        triangles.reserve(settings.numTriangles);
        for (int i = 0; i < settings.numTriangles; ++i) {
            IPLTriangle tri{};
            tri.indices[0] = static_cast<uint32_t>(idxs[3*i + 0]);
            tri.indices[1] = static_cast<uint32_t>(idxs[3*i + 1]);
            tri.indices[2] = static_cast<uint32_t>(idxs[3*i + 2]);
            triangles.push_back(tri);
        }
        settings.triangles    = triangles.data();
        settings.numMaterials = 1;
        settings.materials    = &material;

        // --- create & add to Steam Audio scene ---
        /*IPLStaticMesh sm = nullptr;
        IPLerror err = iplStaticMeshCreate(global_scene, &settings, &sm);
        if (err != IPL_STATUS_SUCCESS) {
            ERR_PRINT("SteamAudioStaticMesh: iplStaticMeshCreate failed");
            continue;
        }
         iplStaticMeshAdd(sm, global_scene);
        static_meshes.push_back(sm);*/
    }

    // 4) Commit all additions at once
   // iplSceneCommit(global_scene);
}
