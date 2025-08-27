#include "steam_audio_server.hpp"

using namespace godot;

 SteamAudioServer::SteamAudioServer() {

}

 SteamAudioServer::~SteamAudioServer() {
     shutdown();
}

// Core Godot Methods
void SteamAudioServer::_bind_methods() {
 }

void SteamAudioServer::initialize() {
     if (Engine::get_singleton()->is_editor_hint())
         return;
     IPLerror err = iplContextCreate(&ctxSettings,&ctx);
     if (err!=IPL_STATUS_SUCCESS) {
         ERR_PRINT("Failed to create IPL context");
     }
     switch (static_cast<int>(proj_settings->get_setting("steam_audio/ray_tracer/RayTracer"))){
         case 0:
             break;
         case 1:
             err = iplEmbreeDeviceCreate(ctx,&embreeDeviceSettings,embreeDevice);
             if (err!=IPL_STATUS_SUCCESS) {
                 ERR_PRINT("Failed to create Embree device");
             }
         default:
             break;
     }

     iplSimulatorCreate(ctx,&simulationSettings,&simulator);
}

void SteamAudioServer::shutdown() {
     if (simulator!=nullptr)
        iplSimulatorRelease(&simulator);
     if (ctx!=nullptr)
         iplContextRelease(&ctx);
     if (embreeDevice!=nullptr)
         iplEmbreeDeviceRelease(embreeDevice);
     if (scene!=nullptr)
         iplSceneRelease(scene);
     if (hrtf!=nullptr)
         iplHRTFRelease(&hrtf);
}

void SteamAudioServer::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_POSTINITIALIZE:

            initialize();

            directThread.instantiate();
            indirectThread.instantiate();

            directThread->start(callable_mp(this,&SteamAudioServer::start_direct_thread),Thread::PRIORITY_NORMAL);
            indirectThread->start(callable_mp(this,&SteamAudioServer::start_indirect_thread),Thread::PRIORITY_NORMAL);
    }
}
void SteamAudioServer::start_direct_thread() {
     iplSimulatorRunDirect(simulator);
 }
void SteamAudioServer::start_indirect_thread() {
     iplSimulatorRunReflections(simulator);
 }

void SteamAudioServer::register_audio_source(SteamAudioSource *source) {

 }
