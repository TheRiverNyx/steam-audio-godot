# SPDX-License-Identifier: Unlicense

target_sources( ${PROJECT_NAME}
    PRIVATE
        register_types.cpp
        steam_audio.cpp
        steam_audio_server.cpp
        steam_audio_material.cpp
        steam_audio_listener.cpp
        steam_audio_source.cpp
        steam_audio_dynamic_mesh.cpp
        steam_audio_static_mesh.cpp
        steam_audio_source.cpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE
        "src"
)
