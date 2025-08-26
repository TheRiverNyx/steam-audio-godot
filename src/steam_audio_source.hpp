#pragma once

#include <godot_cpp/classes/gd_extension.hpp>
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include "phonon.h"
#include <godot_cpp/classes/audio_stream_generator.hpp>
#include <godot_cpp/classes/audio_stream_generator_playback.hpp>
#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include "steam_audio.hpp"


using namespace godot;
class SteamAudioSource: public AudioStreamPlayer3D
{
    GDCLASS(SteamAudioSource,AudioStreamPlayer3D) // NOLINT(*-unhandled-self-assignment)

    AudioServer *audio_server = AudioServer::get_singleton();
    bool is_directivity_enabled = true;
    float dipole_weight=0.0f;
    float dipole_power=1.0f;
    int mix_rate=0;
    int buffer_size=0;

    IPLContext ipl_context = nullptr;
    IPLSource *pSource = nullptr;
    IPLSourceSettings settings{};
    IPLSimulationInputs inputs{};
    IPLDirectivity directivity{};

    Transform3D last_transform;
    bool needs_update = false;

    Ref<AudioStreamGeneratorPlayback> playback;
    Vector<AudioFrame> audio_buffer;
protected:
    static void _bind_methods();
    void _validate_property(PropertyInfo &property) const;
public:
    void _notification(int p_what);
    void _process(double p_delta) override;

    SteamAudioSource();
    ~SteamAudioSource() override;

    void update_source();

    void set_directivity_enabled(bool enabled){is_directivity_enabled=enabled;}
    void set_dipole_weight(float w){dipole_weight=w;}
    void set_dipole_power(float p){dipole_power=p;}

    [[nodiscard]] float get_dipole_weight() const{return dipole_weight;}
    [[nodiscard]] float get_dipole_power() const{return dipole_power;}
    [[nodiscard]] bool get_is_directivity_enabled() const{return is_directivity_enabled;}

    static Vector<SteamAudioSource*> _instances;
    static const Vector<SteamAudioSource*>& get_all_sources();

    [[nodiscard]] bool get_needs_update() const {
        return needs_update;}
};