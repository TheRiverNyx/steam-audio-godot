//
// Created by bryce on 5/7/2025.
//
#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>          // for ClassDB

using namespace godot;

// Called by the InitObject below to register your SteamAudio class.
void register_steam_audio_types();

// This is the exact symbol Godot will look up in your DLL.
extern "C" GDE_EXPORT GDExtensionBool godot_steam_audio_init(
    GDExtensionInterfaceGetProcAddress p_get_proc_address,
    GDExtensionClassLibraryPtr p_library,
    GDExtensionInitialization *p_initialization
);