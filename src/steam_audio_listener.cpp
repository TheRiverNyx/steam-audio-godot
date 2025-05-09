//
// Created by bryce on 5/8/2025.
//

#include "steam_audio_listener.h"

using namespace godot;

void SteamAudioListener::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_context", "ctx"), &SteamAudioListener::set_context);
    ClassDB::bind_method(D_METHOD("_process","delta"), &SteamAudioListener::_process);

    ADD_PROPERTY(
        PropertyInfo(Variant::OBJECT, "context", PROPERTY_HINT_RESOURCE_TYPE, "SteamAudio"),
        "set_context", ""
        );
}

void SteamAudioListener::set_context(const Ref<SteamAudio> &p_ctx) {
    ERR_FAIL_COND(!p_ctx.is_valid());
    ctx = p_ctx;
}

void SteamAudioListener::_process(double delta) {
    ERR_FAIL_COND(!ctx.is_valid());
    Transform3D transform = get_global_transform();
    IPLCoordinateSpace3 ls{};
    ls.origin = {transform.origin.x, transform.origin.y, transform.origin.z};
    Vector3 fwd = transform.basis.xform(Vector3(0,0,-1)), upv = transform.basis.xform(Vector3(0,1,0));
    ls.ahead = {fwd.x, fwd.y, fwd.z};
    ls.up = {upv.x, upv.y, upv.z};
}
