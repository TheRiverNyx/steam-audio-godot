#pragma once

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <phonon.h>
#include "steam_audio_listener.hpp"
#include "steam_audio_material.hpp"

using namespace godot;

enum ProxyMode {
    PROXY_NONE   = 0,
    PROXY_AUTO = 1,
    PROXY_CUSTOM = 2,
};

class SteamAudio : public Resource
{
    GDCLASS( SteamAudio, Resource ) // NOLINT(modernize-use-auto, hicpp-use-auto)

private:
    static IPLContextSettings ctx_default_settings;
    static IPLAudioSettings audio_default_settings;
    static IPLEmbreeDeviceSettings embree_device_default_settings;
    static IPLSceneSettings scene_default_settings;
    static IPLSimulationSettings simulation_default_settings;
    static IPLHRTFSettings hrtf_default_settings;
    ProjectSettings *proj_settings = ProjectSettings::get_singleton();

protected:
    static void _bind_methods();

public:
    static SteamAudio *steam_audio;

    SteamAudio();
    ~SteamAudio() override = default;

    static Transform3D ipl_space_to_godot(const IPLCoordinateSpace3 &p_space);
    static IPLMatrix4x4 transform_to_ipl_matrix(const Transform3D &t);
    static IPLMaterial to_ipl_material(const SteamMaterial &);

    static IPLCoordinateSpace3 godot_to_ipl_space(const Transform3D &p_transform);

    [[nodiscard]] static IPLEmbreeDeviceSettings get_embree_device_settings() {return embree_device_default_settings;}

    [[nodiscard]] static IPLSceneSettings get_scene_settings() {return scene_default_settings;}

    [[nodiscard]] static IPLSimulationSettings get_simulation_settings() {return simulation_default_settings;}

    [[nodiscard]] static IPLHRTFSettings get_hrtf_settings() {return hrtf_default_settings;}

    [[nodiscard]] static IPLContextSettings get_context_settings() {return ctx_default_settings;}

};