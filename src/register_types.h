#pragma once

#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "steam_audio.h"
#include "steam_audio_listener.h"
#include "steam_audio_material.h"
#include "steam_audio_source.h"
#include "steam_audio_static_mesh.h"
#include "steam_audio_dynamic_mesh.h"

using namespace godot;

// Called by the InitObject to register your SteamAudio class.
void initialize_steam_audio(ModuleInitializationLevel p_level);
void uninitialize_steam_audio(ModuleInitializationLevel p_level);
//registers settings in godot
void register_steam_audio_settings();