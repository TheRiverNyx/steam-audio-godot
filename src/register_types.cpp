// src/register_types.cpp
#include "register_types.h"
#include "steam_audio.h"   // your SteamAudio class declaration

using namespace godot;

void register_steam_audio_types() {
    ClassDB::register_class<SteamAudio>();
}

extern "C" GDE_EXPORT GDExtensionBool godot_steam_audio_init(
    GDExtensionInterfaceGetProcAddress p_get_proc_address,
    GDExtensionClassLibraryPtr p_library,
    GDExtensionInitialization *p_initialization
) {
    // Create the InitObject that drives registration.
    GDExtensionBinding::InitObject init_obj{ p_get_proc_address, p_library, p_initialization };

    // 1) Hook up your initializer
    init_obj.register_initializer( register_steam_audio_types );

    // 2) Hook up your terminator (must match the expected callback signature)
    init_obj.register_terminator( [](ModuleInitializationLevel level) {} );

    // 3) Choose the minimum init level (common choices: SCENE, EDITOR, etc.)
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    // 4) Actually perform the registrations
    return init_obj.init();
}
