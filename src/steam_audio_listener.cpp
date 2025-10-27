#include "steam_audio_listener.hpp"

using namespace godot;

SteamAudioListener* SteamAudioListener::listener;

SteamAudioListener::SteamAudioListener() {
    listener=this;
}

SteamAudioListener::~SteamAudioListener() {
    listener=nullptr;
}

void SteamAudioListener::_bind_methods() {
}

SteamAudioListener* SteamAudioListener::get_listener() {
    return listener;
}

void SteamAudioListener::_notification(int p_what) {
    Transform3D current_transform;
    switch (p_what) {
        case NOTIFICATION_TRANSFORM_CHANGED:
            current_transform = get_global_transform();
            if (!last_transform.is_equal_approx(current_transform)) {
                needs_update = true;
                last_transform = current_transform;
            }
            break;
        case NOTIFICATION_ENTER_TREE:
            needs_update = true;
            break;
        default:
            break;
    }
}

void SteamAudioListener::update_listener(IPLSimulator iplSim, IPLSimulationSharedInputs iplSimInputs) {
    needs_update=false;

    Transform3D transform = get_global_transform();

    IPLCoordinateSpace3 space = SteamAudio::godot_to_ipl_space(transform);

    iplSimInputs.listener=space;

    iplSimulatorSetSharedInputs(iplSim,IPL_SIMULATIONFLAGS_DIRECT,&iplSimInputs);
    iplSimulatorSetSharedInputs(iplSim,IPL_SIMULATIONFLAGS_PATHING,&iplSimInputs);
    iplSimulatorSetSharedInputs(iplSim,IPL_SIMULATIONFLAGS_REFLECTIONS,&iplSimInputs);
}