//
// Created by bryce on 5/8/2025.
//

#pragma once
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/audio_listener3d.hpp>
#include <godot_cpp/godot.hpp>
#include "steam_audio.h"
#include <phonon.h>

using namespace godot;

class SteamAudioListener : public AudioListener3D {
    GDCLASS(SteamAudioListener,AudioListener3D)
    Ref<SteamAudio> ctx;
protected:
    static void _bind_methods();
public:
    void set_context(const Ref<SteamAudio> &p_ctx);
    void _process(double delta) override;
};