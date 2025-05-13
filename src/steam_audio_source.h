#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/audio_stream_generator.hpp>
#include <godot_cpp/classes/audio_stream_generator_playback.hpp>
#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>
#include "steam_audio.h"
#include "phonon.h"

using namespace godot;

class SteamAudioSource: public Node3D
{
    GDCLASS(SteamAudioSource,Node3D)

    IPLSource *pSource = nullptr;
    SteamAudio *sa = SteamAudio::steam_audio;
    IPLSourceSettings settings{};
protected:
    static void _bind_methods();
    void _process(double p_delta) override;
public:
    SteamAudioSource();
    ~SteamAudioSource();
    bool update_position();

};