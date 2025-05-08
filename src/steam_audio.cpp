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
     return true;
 }