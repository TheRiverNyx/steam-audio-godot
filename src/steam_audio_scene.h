#pragma once

#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>
#include <godot_cpp/classes/object.hpp>
#include "godot_cpp/godot.hpp"
#include "phonon.h"
#include "steam_audio.h"

using namespace godot;

class SteamAudioScene : public RefCounted {
    GDCLASS(SteamAudioScene, RefCounted);

    IPLScene scene = nullptr;
    Ref<SteamAudio> ctx;

protected:
    static void _bind_methods();
public:
    SteamAudioScene();
    ~SteamAudioScene() override;

    void set_context(const Ref<SteamAudio> &p_ctx);

    void add_mesh(MeshInstance3D *mesh_node);

    void commit() const;
};