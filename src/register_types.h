//
// Created by bryce on 5/7/2025.
//
#pragma once

#include <godot_cpp/core/class_db.hpp>          // for ClassDB
using namespace godot;

// Called by the InitObject below to register your SteamAudio class.
void initialize_steam_audio(ModuleInitializationLevel p_level);
void uninitialize_steam_audio(ModuleInitializationLevel p_level);