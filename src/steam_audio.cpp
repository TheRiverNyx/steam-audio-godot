#include "steam_audio.h"

using namespace godot;

IPLContext global_context = nullptr;
IPLScene global_scene = nullptr;
IPLSimulator global_simulator = nullptr;

SteamAudio::SteamAudio() {
    steam_audio=this;
    initialize();

}

 SteamAudio::~SteamAudio()
{
    steam_audio = nullptr;
    shutdown();
}

void SteamAudio::_bind_methods() {
    ClassDB::bind_method(D_METHOD("initialize"), &SteamAudio::initialize);
    ClassDB::bind_method(D_METHOD("shutdown"), &SteamAudio::shutdown);

    ClassDB::bind_method(D_METHOD("get_hrtf"), &SteamAudio::get_hrtf);
    ClassDB::bind_method(D_METHOD("get_hrtf_settings"), &SteamAudio::get_hrtf_settings);

    ClassDB::bind_method(D_METHOD("get_simulator"), &SteamAudio::get_simulator);
    ClassDB::bind_method(D_METHOD("get_simulation_settings"), &SteamAudio::get_simulation_settings);

    ClassDB::bind_method(D_METHOD("get_context"),&SteamAudio::get_context);
    ClassDB::bind_method(D_METHOD("get_context_settings"),&SteamAudio::get_context_settings);

    ClassDB::bind_method(D_METHOD("get_scene"), &SteamAudio::get_scene);
    ClassDB::bind_method(D_METHOD("get_scene_settings"), &SteamAudio::get_scene_settings);

    ClassDB::bind_method(D_METHOD("get_embree_device"), &SteamAudio::get_embree_device);
    ClassDB::bind_method(D_METHOD("get_embree_device_settings"),&SteamAudio::get_embree_device_settings);
}

bool SteamAudio::initialize() {
    ctx_settings.version=STEAMAUDIO_VERSION;
    IPLContext ctx = nullptr;
    iplContextCreate(&ctx_settings, &ctx);
    iplSceneCreate(ctx,&scene_settings,&scene);
    iplSimulatorCreate(ctx,&simulation_settings,&simulator);
    int ray_mode = proj_settings->get_setting("steam_audio/ray_tracer");
    switch (ray_mode) {
        case 0://steam rt
            break;
        case 1: // embree rt
            iplEmbreeDeviceCreate(ctx,&embree_device_settings,&embree_device);
            break;
        default:
            ERR_PRINT("Unknown Raytracer");
            return false;
    }
    int spat_mode = proj_settings->get_setting("steam_audio/spatializer");
    switch (spat_mode) {
        case 0://panning
            break;
        case 1://HRTF
            iplHRTFCreate(ctx,&audio_settings,&hrtf_settings,&hrtf);
            break;
        case 2://ambisonic pan
            break;
        case 3://ambisonic binaural
            break;
        default:
            ERR_PRINT("Unknown Spatializer");
            return false;
    }
    print_line("Steam Audio Successfully initialized");

    return true;
}

void SteamAudio::update_static_scene() {

}

void SteamAudio::update_dynamic_scene() {

}


void SteamAudio::shutdown() {
    if (embree_device != nullptr) {
        iplEmbreeDeviceRelease(&embree_device);
        embree_device = nullptr;
    }
    if (ctx != nullptr) {
        iplContextRelease(&ctx);
        ctx = nullptr;
    }
    if (hrtf != nullptr) {
        iplHRTFRelease(&hrtf);
        hrtf = nullptr;
    }
    if (simulator != nullptr) {
        iplSimulatorRelease(&simulator);
        simulator = nullptr;
    }
    if (scene!= nullptr) {
        iplSceneRelease(&scene);
        scene = nullptr;
    }
}
bool SteamAudio::build_scene() {
    update_dynamic_scene();
    update_static_scene();
}
Array<NodePath> SteamAudio::get_nodes_with_child() {

}

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

void SteamAudio::_ready() {
    reflections_thread;
    build_scene();
    iplSimulatorRunDirect(simulator);

}

