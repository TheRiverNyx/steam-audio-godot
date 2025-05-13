#include "register_types.h"

using namespace godot;

void initialize_steam_audio(ModuleInitializationLevel p_level) {
    if (p_level!=MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
    register_steam_audio_settings();
    GDREGISTER_CLASS(SteamAudio);
    GDREGISTER_CLASS(SteamAudioMaterial);
    GDREGISTER_RUNTIME_CLASS(SteamAudioListener);
    GDREGISTER_RUNTIME_CLASS(SteamAudioSource);
    GDREGISTER_RUNTIME_CLASS(SteamAudioStaticMesh)
    GDREGISTER_RUNTIME_CLASS(SteamAudioDynamicMesh)
}

void uninitialize_steam_audio(ModuleInitializationLevel p_level) {
    if (p_level!=MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

void register_steam_audio_settings() {
    ProjectSettings *settings = ProjectSettings::get_singleton();

    {//Raytracer enum
        settings->set("steam_audio/ray_tracer/RayTracer",0);
        Dictionary info;
        info["name"] = "steam_audio/ray_tracer/RayTracer";
        info["type"] = Variant::INT;
        info["hint"] = PROPERTY_HINT_ENUM;
        info["hint_string"] = "Steam RT,Embree RT";
        info["usage"] = PROPERTY_USAGE_DEFAULT;
        settings->add_property_info(info);
        settings->set("steam_audio/ray_tracer/RayTracer",1);
    }

    {//spatialization mode enum
        settings->set("steam_audio/spatializer/spatializer",0);
        Dictionary info;
        info["name"] = "steam_audio/spatializer/spatializer";
        info["type"] = Variant::INT;
        info["hint"] = PROPERTY_HINT_ENUM;
        info["hint_string"] = "Panning,HRTF,Ambisonic Pan,Ambisonic Binaural";
        info["usage"] = PROPERTY_USAGE_DEFAULT;
        settings->add_property_info(info);
        settings->set_initial_value("steam_audio/spatializer/spatializer",1);
    }

    {
        settings->set("steam_audio/spatializer/HRTF/Volume",0);
        Dictionary info;
        info["name"] = "steam_audio/spatializer/HRTF/Volume";
        info["type"] = Variant::FLOAT;
        info["hint"] = PROPERTY_HINT_RANGE;
        info["hint_string"] = "0.0,1.0,.01,slider";
        info["usage"] = PROPERTY_USAGE_DEFAULT;
        settings->add_property_info(info);
        settings->set("steam_audio/spatializer/HRTF/Volume",1);
    }

}

extern "C" {
    // Initialization.
    GDExtensionBool GDE_EXPORT steam_audio_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
        GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_steam_audio);
        init_obj.register_terminator(uninitialize_steam_audio);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}