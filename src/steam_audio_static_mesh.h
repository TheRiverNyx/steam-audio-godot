// steam_audio_proxy_mesh.h
#pragma once

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/classes/convex_polygon_shape3d.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/material.hpp>

#include "steam_audio_material.h"

using namespace godot;

// Proxy generation modes
enum ProxyMode {
    PROXY_NONE   = 0,
    PROXY_CONVEX = 1,
    PROXY_CUSTOM = 2,
};

class SteamAudioStaticMesh : public Node3D {
    GDCLASS(SteamAudioStaticMesh, Node3D)

private:
    int proxy_mode = PROXY_NONE;
    Ref<Mesh> custom_proxy_mesh;
    Ref<SteamAudioMaterial> default_material;
    Vector<IPLStaticMesh> static_meshes;

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

    void set_default_material(Ref<SteamAudioMaterial> mat);
    Ref<SteamAudioMaterial> get_default_material() const;

    // Regenerates Steam Audio proxies
    void generate_proxy_mesh();
};
