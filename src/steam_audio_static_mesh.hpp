// steam_audio_proxy_mesh.h
#pragma once

#include <godot_cpp/classes/array_mesh.hpp>
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

using namespace godot;

// Proxy generation modes


class SteamAudioStaticMesh : public Node3D {
    GDCLASS(SteamAudioStaticMesh, Node3D)

private:
    int proxy_mode = PROXY_NONE;
    Ref<Mesh> custom_proxy_mesh;
    Vector<IPLStaticMesh> static_meshes;

    bool needs_update = false;

protected:
    static void _bind_methods();


public:
    SteamAudioStaticMesh();
    ~SteamAudioStaticMesh() override;

    // Setters & getters
    void set_proxy_mode(int mode);
    int  get_proxy_mode() const;

    void set_custom_proxy_mesh(Ref<Mesh> mesh);
    Ref<Mesh> get_custom_proxy_mesh() const;

    bool get_needs_update() {
        return needs_update;
    }
    void update_static_mesh();
    void build_mesh(IPLScene scene);
    static Vector<SteamAudioStaticMesh*> _instances;
    static const Vector<SteamAudioStaticMesh*>& get_all_static_meshes();

    // Regenerates Steam Audio proxies
    Ref<Mesh> get_proxy_mesh();
};
