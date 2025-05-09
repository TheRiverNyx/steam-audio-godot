//
// Created by bryce on 5/8/2025.
//

#include "steam_audio_scene.h"

using namespace godot;

 SteamAudioScene::SteamAudioScene() = default;
 SteamAudioScene::~SteamAudioScene() {
     if (scene) {
         iplSceneRelease(&scene);
     }
 }

void SteamAudioScene::_bind_methods() {
     ClassDB::bind_method(D_METHOD("set_context","ctx"), &SteamAudioScene::set_context);
     ClassDB::bind_method(D_METHOD("add_mesh","mesh_node"), &SteamAudioScene::add_mesh);
     ClassDB::bind_method(D_METHOD("commit"), &SteamAudioScene::commit);
     ADD_PROPERTY(PropertyInfo(Variant::OBJECT,"context",PROPERTY_HINT_RESOURCE_TYPE,"SteamAudio"), "set_context","");
 }

void SteamAudioScene::set_context(const Ref<SteamAudio> &p_ctx) {
     ERR_FAIL_COND(!p_ctx.is_valid());
     ctx = p_ctx;
     if (scene) {
         iplSceneRelease(&scene);
         scene = nullptr;
     }
     IPLSceneSettings settings{};

     settings.type = ctx-> getEmbreeDevice() ? IPL_SCENETYPE_EMBREE : IPL_SCENETYPE_DEFAULT;
     settings.embreeDevice = ctx-> getEmbreeDevice();
     iplSceneCreate(ctx->getContext(), &settings, &scene);
 }

void SteamAudioScene::commit() const {
     ERR_FAIL_COND(!scene);
     ERR_FAIL_COND(!ctx.is_valid());
     iplSceneCommit(scene) ;
 }
void SteamAudioScene::add_mesh(MeshInstance3D *mesh_node) {
     ERR_FAIL_COND(!ctx.is_valid());
     ERR_FAIL_COND(!scene);
     ERR_FAIL_COND(!mesh_node);

     // 1) Fetch the ArrayMesh from the MeshInstance3D
     Ref<Mesh> mesh = mesh_node->get_mesh();
     ERR_FAIL_COND(!mesh.is_valid());

     ArrayMesh *raw_am = Object::cast_to<ArrayMesh>(mesh.ptr());
     ERR_FAIL_COND(!raw_am);
     Ref<ArrayMesh> am(raw_am);
     ERR_FAIL_COND(!am.is_valid());

     // 2) Pull out the raw arrays
     Array arrays = am->surface_get_arrays(0);
     PackedVector3Array verts = arrays[ArrayMesh::ARRAY_VERTEX];
     PackedInt32Array    idxs  = arrays[ArrayMesh::ARRAY_INDEX];

     int vertex_count   = verts.size();
     int index_count    = idxs.size();
     int triangle_count = index_count / 3;

     // 3) Build Steam Audio’s settings struct
     IPLStaticMeshSettings sm{};
     sm.numVertices   = vertex_count;
     sm.numTriangles  = triangle_count;
     sm.numMaterials  = 0;
     sm.vertices      = reinterpret_cast<IPLVector3*>(
                           const_cast<Vector3*>(verts.ptr())
                       );
     sm.materialIndices = nullptr;
     sm.materials       = nullptr;

     // 4) Convert the flat index list into IPLTriangle[]
     IPLTriangle *tris = static_cast<IPLTriangle*>(
         malloc(sizeof(IPLTriangle) * triangle_count)
     );
     for (int i = 0; i < triangle_count; ++i) {
         tris[i].indices[0] = idxs.ptr()[3*i + 0];
         tris[i].indices[1] = idxs.ptr()[3*i + 1];
         tris[i].indices[2] = idxs.ptr()[3*i + 2];
     }
     sm.triangles = tris;

     // 5) Create & add the static mesh to the scene
     IPLStaticMesh static_mesh = nullptr;
     iplStaticMeshCreate(scene, &sm, &static_mesh);
     iplStaticMeshAdd   (static_mesh, scene);
     iplStaticMeshRelease(&static_mesh);

     // 6) Clean up our temporary triangle array
     ::free(tris);
 }