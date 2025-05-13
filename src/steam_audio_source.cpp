#include "steam_audio_source.h"

#include "steam_audio_globals.h"

using namespace godot;
SteamAudioSource::SteamAudioSource() {
    iplSourceCreate(sa->get_simulator(),&settings,pSource);
}
SteamAudioSource::~SteamAudioSource() {
    iplSourceRelease(pSource);
}
void SteamAudioSource::_bind_methods() {
}

void SteamAudioSource::_process(double delta) {
    update_position();
}
bool SteamAudioSource::update_position() {
    IPLCoordinateSpace3 current_pos = sa->godot_to_ipl_space(get_transform());

    iplSource
}
