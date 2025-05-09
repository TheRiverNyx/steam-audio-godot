//
// Created by bryce on 5/7/2025.
//

#include "steam_audio.h"

using namespace godot;

 SteamAudio::~SteamAudio()
{
    if (context)
    {
        iplContextRelease(&context);
        context=nullptr;
    }
     if (embree_device) {
         iplEmbreeDeviceRelease(&embree_device);
         embree_device=nullptr;
     }
}
void SteamAudio::_bind_methods() {
    ClassDB::bind_method(D_METHOD("initialize"), &SteamAudio::initialize);
}

SteamAudio::SteamAudio() = default;

bool SteamAudio::initialize()
 {
     IPLContextSettings settings{};
     settings.version = STEAMAUDIO_VERSION;

     if ( IPLerror err = iplContextCreate( &settings, &context ); err != IPL_STATUS_SUCCESS)
     {
         UtilityFunctions::printerr("SteamAudio::initialize()", err);
         return false;
     }
    IPLEmbreeDeviceSettings emb_device_settings{};
    if (iplEmbreeDeviceCreate(context, &emb_device_settings, &embree_device) != IPL_STATUS_SUCCESS) {
        UtilityFunctions::printerr("SteamAudio::initialize(): failed to create Embree device");
        return false;
    }
     return true;
 }