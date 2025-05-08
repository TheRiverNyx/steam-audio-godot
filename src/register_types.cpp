// src/register_types.cpp
#include "register_types.h"
#include "steam_audio.h"   // your SteamAudio class declaration

using namespace godot;

void initialize_steam_audio(ModuleInitializationLevel p_level) {
    if (p_level!=MODULE_INITIALIZATION_LEVEL_SERVERS) {
        return;
    }
    GDREGISTER_CLASS(SteamAudio);
}
void uninitialize_steam_audio(ModuleInitializationLevel p_level) {
    if (p_level!=MODULE_INITIALIZATION_LEVEL_SERVERS) {
        return;
    }

}

extern "C" {
    // Initialization.
    GDExtensionBool GDE_EXPORT steam_audio_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_steam_audio);
        init_obj.register_terminator(uninitialize_steam_audio);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}