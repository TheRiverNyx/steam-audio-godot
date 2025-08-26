#pragma once
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/class_db.hpp>
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/godot.hpp"
#include "phonon.h"

using namespace godot;
struct SteamMaterial {
    Vector3 absorption;
    float scattering;
    Vector3 transmission;
};
class SteamAudioMaterial : public Resource {
    GDCLASS(SteamAudioMaterial,Resource) // NOLINT(modernize-use-auto, hicpp-use-auto)
protected:
    static void _bind_methods();
private:
    SteamMaterial material;

public:
    static SteamMaterial default_material;
    static const SteamMaterial& get_default_material();
    void set_absorption(const Vector3 value);
    void set_scattering(const float value);
    void set_transmission(const Vector3 value);
    Vector3 get_absorption() const;
    [[nodiscard]] float get_scattering() const;
    Vector3 get_transmission() const;
    SteamMaterial get_steam_material() const;
};