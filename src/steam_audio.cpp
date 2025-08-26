#include "steam_audio.hpp"

using namespace godot;


 SteamAudio::SteamAudio() {

}

void SteamAudio::_bind_methods() {

}
IPLContextSettings SteamAudio::ctx_default_settings = {
     STEAMAUDIO_VERSION,
     nullptr,
     nullptr,
     nullptr,
     IPL_SIMDLEVEL_NEON,
};
IPLSimulationSettings SteamAudio::simulation_default_settings = {
};
IPLAudioSettings SteamAudio::audio_default_settings = {
};
IPLSceneSettings SteamAudio::scene_default_settings = {
};
IPLEmbreeDeviceSettings SteamAudio::embree_device_default_settings = {
};
IPLHRTFSettings SteamAudio::hrtf_default_settings = {
};

IPLCoordinateSpace3 SteamAudio::godot_to_ipl_space(const Transform3D &t) {
    IPLCoordinateSpace3 s{};
    // pull columns via Basis.xform()
    Vector3 right = t.basis.xform(Vector3(1, 0, 0));
    Vector3 up    = t.basis.xform(Vector3(0, 1, 0));
    Vector3 ahead = t.basis.xform(Vector3(0, 0, -1)); // Godot forward is -Z

    s.right  = { right.x,  right.y,  right.z  };
    s.up     = { up.x,     up.y,     up.z     };
    s.ahead  = { ahead.x,  ahead.y,  ahead.z  };
    s.origin = { t.origin.x, t.origin.y, t.origin.z };

    return s;
}

// Steam Audio → Godot space
Transform3D SteamAudio::ipl_space_to_godot(const IPLCoordinateSpace3 &s) {
    // construct a Basis from column vectors
    Basis b(
        Vector3(s.right .x,  s.right .y,  s.right .z),
        Vector3(s.up    .x,  s.up    .y,  s.up    .z),
        Vector3(-s.ahead.x, -s.ahead.y, -s.ahead.z) // invert ahead back to -Z
    );

    return Transform3D(b, Vector3(s.origin.x, s.origin.y, s.origin.z));
}

IPLMatrix4x4 SteamAudio::transform_to_ipl_matrix(const Transform3D &t) {
    // First compute the three axes using Godot’s forward = –Z
    Vector3 right   = t.basis.xform(Vector3(1, 0, 0));
    Vector3 up      = t.basis.xform(Vector3(0, 1, 0));
    Vector3 forward = t.basis.xform(Vector3(0, 0, -1));  // ← same as .ahead above

    IPLMatrix4x4 m{};
    // Row-major: m[row][col]

    // X axis → column 0
    m.elements[0][0] = right.x;   m.elements[1][0] = right.y;   m.elements[2][0] = right.z;

    // Y axis → column 1
    m.elements[0][1] = up.x;      m.elements[1][1] = up.y;      m.elements[2][1] = up.z;

    // Forward (–Z) → column 2
    m.elements[0][2] = forward.x; m.elements[1][2] = forward.y; m.elements[2][2] = forward.z;

    // Translation → column 3
    m.elements[0][3] = t.origin.x; m.elements[1][3] = t.origin.y; m.elements[2][3] = t.origin.z;

    // Bottom row for homogeneous
    m.elements[3][0] = 0; m.elements[3][1] = 0; m.elements[3][2] = 0; m.elements[3][3] = 1;

    return m;
}

IPLMaterial SteamAudio::to_ipl_material(const SteamMaterial &material) {
    IPLMaterial iplmaterial ={};

    iplmaterial.absorption[0] = material.absorption.x;
    iplmaterial.absorption[1] = material.absorption.y;
    iplmaterial.absorption[2] = material.absorption.z;

    iplmaterial.scattering = material.scattering;

    iplmaterial.transmission[0] = material.transmission.x;
    iplmaterial.transmission[1] = material.transmission.y;
    iplmaterial.transmission[2] = material.transmission.z;

    return iplmaterial;
}
