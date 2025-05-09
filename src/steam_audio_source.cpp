//
// Created by bryce on 5/9/2025.
//

#include "steam_audio_source.h"

#include "steam_audio.h"

using namespace godot;

 SteamAudioSource::SteamAudioSource() {

}
 SteamAudioSource::~SteamAudioSource() {
    if (steam_audio_source) {
        iplSourceRelease(&steam_audio_source);
        steam_audio_source = nullptr;
    }
}

void SteamAudioSource::_bind_methods() {
     ClassDB::bind_method(D_METHOD("play"), &SteamAudioSource::play);
     ClassDB::bind_method(D_METHOD("stop"), &SteamAudioSource::stop);
     ClassDB::bind_method(D_METHOD("set_stream","stream"), &SteamAudioSource::set_stream);
 }

void SteamAudioSource::play() {
     audio_player = memnew(AudioStreamPlayer3D);
     add_child(audio_player);
 }