//
// Created by bryce on 5/9/2025.
//

#pragma once
#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include "phonon.h"

using namespace godot;

class SteamAudioSource:Node3D {
    GDCLASS(SteamAudioSource, Node3D)
private:
    AudioStreamPlayer3D* audio_player = nullptr;
    IPLSource steam_audio_source = nullptr;

    bool initialized = false;
public:
    SteamAudioSource();
    ~SteamAudioSource();

    void _ready() override;
    void _process(double p_delta) override;

    void play();
    void stop();
    void set_stream(const Ref<AudioStream>& p_stream);
protected:
    static void _bind_methods();
};