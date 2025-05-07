//
// Created by bryce on 5/7/2025.
//

#pragma once

#include <godot_cpp/godot.hpp>
#include<godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/ref.hpp>
#include<godot_cpp/variant/utility_functions.hpp>
#include<phonon.h>

using namespace godot;

class SteamAudio : RefCounted
{
    GDCLASS( SteamAudio,RefCounted );
    private:
        IPLContext context = nullptr;
    protected:
        static void _bind_methods();
    public:
        SteamAudio();
        ~SteamAudio();
    bool initialize();
};