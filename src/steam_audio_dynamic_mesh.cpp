#include "steam_audio_dynamic_mesh.h"
#include "steam_audio_globals.h"   // holds global_context & global_scene

using namespace godot;

SteamAudioDynamicMesh::SteamAudioDynamicMesh() {}
SteamAudioDynamicMesh::~SteamAudioDynamicMesh() {
    if (instanced_mesh) {
        // remove & release the instance
        iplInstancedMeshRemove(instanced_mesh, global_scene);
        iplInstancedMeshRelease(&instanced_mesh);
    }
    if (proxy_subscene) {
        iplSceneRelease(&proxy_subscene);
    }
}

void SteamAudioDynamicMesh::_bind_methods() {
    // no exposed properties for now
}

void SteamAudioDynamicMesh::_ready() {
    build_subscene();

    // instance it in the main scene, with initial transform
    IPLInstancedMeshSettings ims{};
    ims.subScene  = proxy_subscene;
    ims.transform = to_ipl_matrix(get_global_transform());

    iplInstancedMeshCreate(global_scene, &ims, &instanced_mesh);
    iplInstancedMeshAdd(instanced_mesh, global_scene);
    iplSceneCommit(global_scene);
}

void SteamAudioDynamicMesh::_process(double /*delta*/) {
    if (!instanced_mesh) return;

    // update the world‐space transform
    IPLMatrix4x4 mat = to_ipl_matrix(get_global_transform());
    iplInstancedMeshUpdateTransform(instanced_mesh, global_scene, mat);

    // commit once per frame (best performance if after all updates) :contentReference[oaicite:0]{index=0}
    iplSceneCommit(global_scene);
}

void SteamAudioDynamicMesh::build_subscene() {
    // 1) create a fresh sub-scene
    IPLSceneSettings ss{};
    iplSceneCreate(global_context, &ss, &proxy_subscene);

    // 2) pull raw verts/indices out of this MeshInstance3D
    Ref<Mesh> mesh = cast_to<MeshInstance3D>(get_parent())->get_mesh();
    if (mesh.is_null()) return;

    // for simplicity, we assume a single surface:
    Array arr = mesh->surface_get_arrays(0);
    PackedVector3Array verts = arr[Mesh::ARRAY_VERTEX];
    PackedInt32Array    idxs  = arr[Mesh::ARRAY_INDEX];

    // fill out static‐mesh settings
    IPLStaticMeshSettings sms{};
    sms.numVertices  = verts.size();
    sms.vertices     = reinterpret_cast<IPLVector3*>(const_cast<Vector3*>(verts.ptr()));
    sms.numTriangles = idxs.size() / 3;
    // convert PoolIntArray → IPLTriangle*
    // …you’d build & fill an IPLTriangle array here…
    // sms.triangles = your_triangle_buffer;

    // add it to the proxy_subscene
    IPLStaticMesh sm = nullptr;
    iplStaticMeshCreate(proxy_subscene, &sms, &sm);
    iplStaticMeshAdd(sm, proxy_subscene);

    // finally commit the sub-scene so it’s ready to be instanced :contentReference[oaicite:1]{index=1}
    iplSceneCommit(proxy_subscene);
}

IPLMatrix4x4 SteamAudioDynamicMesh::to_ipl_matrix(const Transform3D &t) {
    // row-major: each m[i][j] is row i, column j
    IPLMatrix4x4 m{};
    // basis X column
    m.elements[0][0] = t.basis[0][0]; m.elements[1][0] = t.basis[0][1]; m.elements[2][0] = t.basis[0][2];
    // basis Y column
    m.elements[0][1] = t.basis[1][0]; m.elements[1][1] = t.basis[1][1]; m.elements[2][1] = t.basis[1][2];
    // basis Z column
    m.elements[0][2] = t.basis[2][0]; m.elements[1][2] = t.basis[2][1]; m.elements[2][2] = t.basis[2][2];
    // origin
    m.elements[0][3] = t.origin.x;    m.elements[1][3] = t.origin.y;    m.elements[2][3] = t.origin.z;
    // bottom row
    m.elements[3][0] = 0; m.elements[3][1] = 0; m.elements[3][2] = 0; m.elements[3][3] = 1;
    return m;
}
