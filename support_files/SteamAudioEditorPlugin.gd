@tool
extends EditorPlugin

const AUTOLOAD_NAME = "STEAM_AUDIO"
func _enable_plugin() -> void:
    add_autoload_singleton(AUTOLOAD_NAME,"res://addons/SteamAudioGodot/steam_audio_manager.tscn")
    
func _enter_tree() -> void:
    # Initialization of the plugin goes here.
    pass

func _exit_tree() -> void:
    # Clean-up of the plugin goes here.
    pass

func _disable_plugin() -> void:
    remove_autoload_singleton(AUTOLOAD_NAME)
    
