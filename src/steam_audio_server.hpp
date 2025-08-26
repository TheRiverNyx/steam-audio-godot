#pragma once

#include <godot_cpp/classes/audio_frame.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/classes/mutex.hpp>
#include <phonon.h>
#include <steam_audio.hpp>
#include "steam_audio_dynamic_mesh.hpp"
#include "steam_audio_source.hpp"
#include "steam_audio_static_mesh.hpp"

using namespace godot;


class SteamAudioServer : public Object {
    GDCLASS(SteamAudioServer, Object) // Godot class declaration macro
private:
    IPLContext ctx=nullptr;
    IPLContextSettings ctxSettings;
    IPLAudioSettings audioSettings;
    IPLEmbreeDevice *embreeDevice=nullptr;
    IPLEmbreeDeviceSettings embreeDeviceSettings;
    IPLScene *scene=nullptr;
    IPLSceneSettings sceneSettings;
    IPLSimulator simulator;
    IPLSimulationSettings simulationSettings;
    IPLHRTF hrtf;
    IPLHRTFSettings hrtfSettings;

    ProjectSettings *proj_settings = ProjectSettings::get_singleton();

    Ref<Thread> directThread;
    Ref<Thread> indirectThread;

public:
    SteamAudioServer();                             // Constructor
    ~SteamAudioServer() override;                   // Destructor
    void _notification(int p_what);

protected:
    static void _bind_methods();
    void initialize();
    void shutdown();
    void start_direct_thread();
    void start_indirect_thread();
};
