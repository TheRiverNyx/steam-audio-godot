# SPDX-License-Identifier: Unlicense

target_sources( ${PROJECT_NAME}
    PRIVATE
        register_types.cpp
        register_types.h
        steam_audio.cpp
        steam_audio.h
        steam_audio_scene.cpp
        steam_audio_scene.h
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE
        "src"
)
