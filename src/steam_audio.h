#pragma once

#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <phonon.h>
#include "steam_audio_dynamic_mesh.h"
#include "steam_audio_listener.h"
#include "steam_audio_material.h"
#include "steam_audio_source.h"
#include "steam_audio_static_mesh.h"


using namespace godot;

class SteamAudio : public Node
{
    GDCLASS( SteamAudio, Node ) // NOLINT(modernize-use-auto, hicpp-use-auto)

private:
    IPLContextSettings ctx_settings{};
    IPLAudioSettings audio_settings{};
    IPLContext ctx = nullptr;
    IPLEmbreeDeviceSettings embree_device_settings{};
    IPLEmbreeDevice embree_device = nullptr;
    IPLScene scene = nullptr;
    IPLSceneSettings scene_settings{};
    IPLSimulationSettings simulation_settings{};
    IPLSimulator simulator = nullptr;
    IPLStaticMesh mesh = nullptr;
    IPLHRTFSettings hrtf_settings{};
    IPLHRTF hrtf = nullptr;
    ProjectSettings *proj_settings = ProjectSettings::get_singleton();
    Array<SteamAudioDynamicMesh> dynamic_geometry;
    Array<SteamAudioStaticMesh> static_geometry;
    Array<SteamAudioSource> sources;
    Ref<Thread> reflections_thread;
    Ref<Thread> pathing_thread;
protected:
    static void _bind_methods();
    void _ready() override;
public:
    static SteamAudio *steam_audio;
    SteamAudio();
    ~SteamAudio() override;
    bool initialize();
    void shutdown();

    bool build_scene();

    void update_static_scene();
    void update_dynamic_scene();

    static Transform3D ipl_space_to_godot(const IPLCoordinateSpace3 &p_space);
    static IPLCoordinateSpace3 godot_to_ipl_space(const Transform3D &p_transform);

    [[nodiscard]] IPLContext get_context() const{return ctx;}

    [[nodiscard]] IPLEmbreeDevice get_embree_device() const{return embree_device;}
    [[nodiscard]] IPLEmbreeDeviceSettings get_embree_device_settings() const{return embree_device_settings;}

    [[nodiscard]] IPLScene get_scene() const{return scene;}
    [[nodiscard]] IPLSceneSettings get_scene_settings() const{return scene_settings;}

    [[nodiscard]] IPLSimulator get_simulator() const{return simulator;}
    [[nodiscard]] IPLSimulationSettings get_simulation_settings() const{return simulation_settings;}

    [[nodiscard]] IPLHRTF get_hrtf() const{return hrtf;}
    [[nodiscard]] IPLHRTFSettings get_hrtf_settings() const{return hrtf_settings;}

    [[nodiscard]] IPLContextSettings get_context_settings() const{return ctx_settings;}

};