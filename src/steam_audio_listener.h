#pragma once

#include<godot_cpp/godot.hpp>
#include <godot_cpp/classes/audio_listener3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/wrapped.hpp>

using namespace godot;

class SteamAudioListener:public AudioListener3D
{
    GDCLASS(SteamAudioListener,AudioListener3D)


protected:
    static void _bind_methods();
    void _process(double p_delta) override;
public:
    SteamAudioListener();
    ~SteamAudioListener() override;

private:

};