#include "steam_audio_listener.h"

#include "phonon.h"

using namespace godot;

SteamAudioListener::SteamAudioListener()  = default;
SteamAudioListener::~SteamAudioListener() = default;

void SteamAudioListener::_bind_methods() {

}

void SteamAudioListener::_process(double delta) {
    Transform3D transform = get_global_transform();

    IPLVector3 pos = {
        static_cast<float>(transform.origin.x),
        static_cast<float>(transform.origin.y),
        static_cast<float>(transform.origin.z)
    };
    Vector3 godot_fwd = transform.basis.xform(Vector3(0, 0, -1));
    IPLVector3 fwd = {
        static_cast<float>(godot_fwd.x),
        static_cast<float>(godot_fwd.y),
        static_cast<float>(godot_fwd.z)
    };
    Vector3 godot_up = transform.basis.xform(Vector3(0, 1, 0));
    IPLVector3 up = {
        static_cast<float>(godot_up.x),
        static_cast<float>(godot_up.y),
        static_cast<float>(godot_up.z)
    };
    IPLVector3 right = {
        fwd.y * up.z - fwd.z * up.y,
        fwd.z * up.x - fwd.x * up.z,
        fwd.x * up.y - fwd.y * up.x
    };
    IPLCoordinateSpace3 listenerCS{};
    listenerCS.origin = pos;
    listenerCS.ahead = fwd;
    listenerCS.up = up;
    listenerCS.right = right;
    IPLSimulationSharedInputs sharedInputs{};
    sharedInputs.listener = listenerCS;
}
