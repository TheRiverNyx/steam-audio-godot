#pragma once

#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "steam_audio.hpp"
#include "steam_audio_dynamic_mesh.hpp"
#include "steam_audio_source.hpp"
#include "steam_audio_listener.hpp"
#include "steam_audio_server.hpp"
#include "steam_audio_material.hpp"
#include "steam_audio_static_mesh.hpp"

using namespace godot;

// Called by the InitObject to register your SteamAudio class.
void initialize_steam_audio(ModuleInitializationLevel p_level);
void uninitialize_steam_audio(ModuleInitializationLevel p_level);
//registers settings in godot
void register_steam_audio_settings();