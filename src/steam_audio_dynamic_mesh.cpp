// steam_audio_static_mesh.cpp
#include "steam_audio_dynamic_mesh.hpp"

using namespace godot;

Vector<SteamAudioDynamicMesh*> SteamAudioDynamicMesh::_instances;

SteamAudioDynamicMesh::SteamAudioDynamicMesh() {
    _instances.push_back(this);
}

SteamAudioDynamicMesh::~SteamAudioDynamicMesh() {
    _instances.erase(this);
}

const Vector<SteamAudioDynamicMesh *> &SteamAudioDynamicMesh::get_all_dynamic_meshes() {
    return _instances;
}


void SteamAudioDynamicMesh::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_proxy_mode",        "mode"), &SteamAudioDynamicMesh::set_proxy_mode);
    ClassDB::bind_method(D_METHOD("get_proxy_mode"),             &SteamAudioDynamicMesh::get_proxy_mode);
    ADD_PROPERTY(
        PropertyInfo(Variant::INT, "proxy_mode",PROPERTY_HINT_ENUM,"None,Convex,Custom"), "set_proxy_mode","get_proxy_mode");

    ClassDB::bind_method(D_METHOD("set_custom_proxy_mesh", "mesh"), &SteamAudioDynamicMesh::set_custom_proxy_mesh);
    ClassDB::bind_method(D_METHOD("get_custom_proxy_mesh"),      &SteamAudioDynamicMesh::get_custom_proxy_mesh);
    ADD_PROPERTY(
            PropertyInfo(Variant::OBJECT, "custom_proxy_mesh",PROPERTY_HINT_RESOURCE_TYPE,"Mesh"), "set_custom_proxy_mesh","get_custom_proxy_mesh");
}

// Setters & getters ---------------------------------------------------
void SteamAudioDynamicMesh::set_proxy_mode(int mode) {
    proxy_mode = mode;
}

int SteamAudioDynamicMesh::get_proxy_mode() const {
    return proxy_mode;
}

void SteamAudioDynamicMesh::set_custom_proxy_mesh(Ref<Mesh> mesh) {
    custom_proxy_mesh = mesh;
}

Ref<Mesh> SteamAudioDynamicMesh::get_custom_proxy_mesh() const {
    return custom_proxy_mesh;
}

void SteamAudioDynamicMesh::_notification(int what) {
    Transform3D current;
    switch (what) {
        case NOTIFICATION_ENTER_TREE:
                needs_update=true;
            break;
        case NOTIFICATION_EXIT_TREE:
            break;
        case NOTIFICATION_TRANSFORM_CHANGED:
            current = get_global_transform();
            if (!last_transform.is_equal_approx(current)) {
                last_transform = current;
                needs_update=true;
            }
        default:
            break;
    }
}

    // Core functionality --------------------------------------------------
Ref<Mesh> SteamAudioDynamicMesh::get_proxy_mesh() {
    print_line("getting the proxy mesh");
    print_line(vformat("proxy_mode: %f", proxy_mode));

    MeshInstance3D *parent = cast_to<MeshInstance3D>(get_parent());
    if (!parent) {
        print_error("Parent is not a MeshInstance3D!");
        return {};
    }
    Ref<Mesh> target_mesh;
    Ref<Mesh> source_mesh = parent->get_mesh();
    if (!source_mesh.is_valid()) {
        print_error("Source mesh is invalid!");
        return {};
    }

    print_line("using parent mesh");
    print_line(vformat("Source mesh type: %s", source_mesh->get_class()));

    switch (proxy_mode) {
        case PROXY_NONE:
            target_mesh = source_mesh;
            break;
        case PROXY_AUTO:

            break;

        case PROXY_CUSTOM:

            break;
        default:
            print_error("Unknown proxy mode!");
            return {};
    }
    return target_mesh;
}
void SteamAudioDynamicMesh::update_dynamic_mesh() {
    IPLMatrix4x4 transform = SteamAudio::transform_to_ipl_matrix(get_global_transform());
    iplInstancedMeshUpdateTransform(instanced_mesh,global_scene,transform);
}


void SteamAudioDynamicMesh::build_mesh(IPLScene scene,IPLContext ipl_context,IPLContextSettings ipl_context_settings) {
    global_scene=scene;
    iplSceneCreate(ipl_context,&scene_settings,&subscene);
    if (!scene) {
        ERR_PRINT("Invalid IPLScene passed to build_mesh");
        return;
    }

    struct SurfaceData {
        PackedVector3Array vertices;
        PackedInt32Array indices;
        IPLMaterial material;
    };

    Vector<SurfaceData> surface_data;
    print_line("building a static mesh");
    Ref<Mesh> mesh = get_proxy_mesh();

    if (mesh.is_null()) {
        ERR_PRINT("Failed to get valid proxy mesh");
        return;
    }
    print_line("got the mesh");

    for (int surface_idx = 0; surface_idx < mesh->get_surface_count(); surface_idx++) {
        SurfaceData data;
        Array arrays = mesh->surface_get_arrays(surface_idx);

        if (arrays.is_empty()) {
            ERR_PRINT(vformat("Surface %d has no arrays data", surface_idx));
            continue;
        }

        // Get vertices and indices
        PackedVector3Array vertices = arrays[Mesh::ARRAY_VERTEX];
        PackedInt32Array indices = arrays[Mesh::ARRAY_INDEX];

        if (vertices.is_empty()) {
            ERR_PRINT(vformat("Surface %d has no vertices", surface_idx));
            continue;
        }

        // If indices array is empty, create one from vertices
        if (indices.is_empty()) {
            indices.resize(vertices.size());
            for (int i = 0; i < vertices.size(); i++) {
                indices[i] = i;
            }
        }

        // Create triangles if indices count is not a multiple of 3
        if (indices.size() % 3 != 0) {
            print_line("Surface " + itos(surface_idx) + " is not triangulated, converting to triangles");
            PackedInt32Array new_indices;

            // Simple triangulation for convex polygons
            for (int i = 0; i < indices.size() - 2; i++) {
                new_indices.push_back(indices[0]);
                new_indices.push_back(indices[i + 1]);
                new_indices.push_back(indices[i + 2]);
            }

            indices = new_indices;
        }

        data.vertices = vertices;
        data.indices = indices;

        // Get material for the surface
        Ref<Material> material = mesh->surface_get_material(surface_idx);
        if (material.is_valid() && material->has_meta("steam_audio_material")) {
            Ref<SteamAudioMaterial> steam_audio_material = material->get_meta("steam_audio_material");
            if (steam_audio_material.is_valid()) {
                SteamMaterial steam_material = steam_audio_material->get_steam_material();
                data.material = SteamAudio::to_ipl_material(steam_material);
            } else {
                WARN_PRINT(vformat("Invalid Steam Audio material for surface %d", surface_idx));
                data.material = SteamAudio::to_ipl_material(SteamAudioMaterial::get_default_material());
            }
        } else {
            data.material = SteamAudio::to_ipl_material(SteamAudioMaterial::get_default_material());
        }

        surface_data.push_back(data);
    }

    if (surface_data.is_empty()) {
        ERR_PRINT("No valid surfaces found in mesh");
        return;
    }

    // Calculate total counts
    int total_vertices = 0;
    int total_triangles = 0;
    for (const SurfaceData &data: surface_data) {
        total_vertices += data.vertices.size();
        total_triangles += data.indices.size() / 3;
    }

    if (total_vertices == 0 || total_triangles == 0) {
        ERR_PRINT("Mesh has no vertices or triangles");
        return;
    }

    // Prepare final data arrays
    PackedFloat32Array final_vertices;
    PackedInt32Array final_triangles;
    Vector<IPLMaterial> final_materials;
    PackedInt32Array material_indices;

    final_vertices.resize(total_vertices * 3);
    final_triangles.resize(total_triangles * 3);
    material_indices.resize(total_triangles);

    // Combine all surfaces
    int vertex_offset = 0;
    int triangle_offset = 0;
    int material_index = 0;

    for (const SurfaceData &data: surface_data) {
        // Copy vertices
        for (int i = 0; i < data.vertices.size(); i++) {
            const Vector3 &v = data.vertices[i];
            final_vertices[vertex_offset * 3 + 0] = v.x;
            final_vertices[vertex_offset * 3 + 1] = v.y;
            final_vertices[vertex_offset * 3 + 2] = v.z;
            vertex_offset++;
        }

        // Copy triangles
        int base_vertex = vertex_offset - data.vertices.size();
        for (int i = 0; i < data.indices.size(); i += 3) {
            final_triangles[triangle_offset * 3 + 0] = base_vertex + data.indices[i + 0];
            final_triangles[triangle_offset * 3 + 1] = base_vertex + data.indices[i + 1];
            final_triangles[triangle_offset * 3 + 2] = base_vertex + data.indices[i + 2];

            material_indices[triangle_offset] = material_index;
            triangle_offset++;
        }

        final_materials.push_back(data.material);
        material_index++;
    }

    // Create Steam Audio static mesh
    IPLStaticMeshSettings mesh_settings = {};
    mesh_settings.numVertices = total_vertices;
    mesh_settings.numTriangles = total_triangles;
    mesh_settings.numMaterials = final_materials.size();
    mesh_settings.vertices = const_cast<IPLVector3 *>(reinterpret_cast<const IPLVector3 *>(final_vertices.ptr()));
    mesh_settings.triangles = const_cast<IPLTriangle *>(reinterpret_cast<const IPLTriangle *>(final_triangles.ptr()));
    mesh_settings.materials = const_cast<IPLMaterial *>(final_materials.ptr());
    mesh_settings.materialIndices = const_cast<IPLint32 *>(material_indices.ptr());

    IPLStaticMesh static_mesh;
    IPLerror error = iplStaticMeshCreate(subscene, &mesh_settings, &static_mesh);
    if (error != IPL_STATUS_SUCCESS) {
        ERR_PRINT(vformat("Failed to create static mesh. Error code: %d", error));
        return;
    }

    print_line(vformat("Successfully created static mesh with %d vertices and %d triangles", total_vertices,
                       total_triangles));
    instanced_mesh_settings.subScene = subscene;
    instanced_mesh_settings.transform = SteamAudio::transform_to_ipl_matrix(get_global_transform());

    IPLerror imErr = iplInstancedMeshCreate(global_scene,&instanced_mesh_settings,&instanced_mesh);
    if (imErr != IPL_STATUS_SUCCESS) {
        ERR_PRINT(vformat("Failed to create instanced mesh. Error code: %d", imErr));
        return;
    }
}
