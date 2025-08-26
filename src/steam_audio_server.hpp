#pragma once

#include <godot_cpp/classes/audio_frame.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <phonon.h>
#include <steam_audio.hpp>
#include "steam_audio_dynamic_mesh.hpp"
#include "steam_audio_source.hpp"
#include "steam_audio_static_mesh.hpp"

using namespace godot;


class SteamAudioServer : public Object {
    GDCLASS(SteamAudioServer, Object) // Godot class declaration macro
private:

public:
    SteamAudioServer();                             // Constructor
    ~SteamAudioServer() override;                   // Destructor

protected:
    static void _bind_methods();                    // Bind methods to Godot

};
