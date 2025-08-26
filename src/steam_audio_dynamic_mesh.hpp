#pragma once

#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/classes/convex_polygon_shape3d.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <phonon.h> // IPLStaticMesh, IPLStaticMeshSettings, IPLMaterial
#include "steam_audio.hpp"
#include "steam_audio_material.hpp"
#include <algorithm>

using namespace godot;

class SteamAudioDynamicMesh : public Node3D {
    GDCLASS(SteamAudioDynamicMesh, Node3D)

private:
    IPLScene subscene=nullptr;
    IPLScene global_scene=nullptr;
    IPLSceneSettings scene_settings;
    IPLInstancedMesh instanced_mesh=nullptr;
    IPLInstancedMeshSettings instanced_mesh_settings;
    int proxy_mode = PROXY_NONE;
    Ref<Mesh> custom_proxy_mesh;

    Transform3D last_transform;
    StringName bus;

    bool needs_update = false;

protected:
    static void _bind_methods();


public:
    SteamAudioDynamicMesh();
    ~SteamAudioDynamicMesh() override;

    void _notification(int what);

    // Setters & getters
    void set_proxy_mode(int mode);
    int  get_proxy_mode() const;

    void set_custom_proxy_mesh(Ref<Mesh> mesh);
    Ref<Mesh> get_custom_proxy_mesh() const;

    bool get_needs_update() {
        return needs_update;
    }
    void build_mesh(IPLScene scene, IPLContext ipl_context, IPLContextSettings ipl_context_settings);
    static Vector<SteamAudioDynamicMesh*> _instances;
    static const Vector<SteamAudioDynamicMesh*>& get_all_dynamic_meshes();

    // Regenerates Steam Audio proxies
    Ref<Mesh> get_proxy_mesh();

    void update_dynamic_mesh();
};
