#pragma once

#include <godot_cpp/classes/audio_listener3d.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include "steam_audio.hpp"
#include "phonon.h"

using namespace godot;

class SteamAudioListener:public AudioListener3D
{
    GDCLASS(SteamAudioListener,AudioListener3D)
private:
    bool needs_update = false;
    Transform3D last_transform;

protected:
    static void _bind_methods();
public:
    bool get_needs_update() {
        return needs_update;
    }
    static SteamAudioListener *listener;
    SteamAudioListener();
    ~SteamAudioListener() override;

    void _notification(int p_what);

    static SteamAudioListener* get_listener();
    void update_listener(IPLSimulator iplsim, IPLSimulationSharedInputs iplsiminputs);
    void set_listener_position(const Vector3 &p_position);
    void set_listener_orientation(const Vector3 &p_forward, const Vector3 &p_up);
    void set_listener_velocity(const Vector3 &p_velocity);
    void set_listener_gain(const float p_gain);
    void set_listener_doppler_factor(const float p_factor);
    void set_listener_distance_model(const int p_model);
    void set_listener_flags(const int p_flags);
    void set_listener_channel_count(const int p_count);
};