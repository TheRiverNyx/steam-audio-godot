#pragma once

#include <godot_cpp/classes/resource.hpp>
#include "godot_cpp/godot.hpp"
#include <godot_cpp/core/class_db.hpp>
#include "phonon.h"

using namespace godot;

class SteamAudioMaterial : public Resource {
    GDCLASS(SteamAudioMaterial,Resource) // NOLINT(modernize-use-auto, hicpp-use-auto)
protected:
    static void _bind_methods();
private:
    Vector3 absorption = Vector3(0.2, 0.2, 0.2);
    float scattering = 0.0;
    Vector3 transmission = Vector3(0.0, 0.0, 0.0);
public:
    void set_absorption(const Vector3 value);
    void set_scattering(const float value);
    void set_transmission(const Vector3 value);
    Vector3 get_absorption() const;
    [[nodiscard]] float get_scattering() const;
    Vector3 get_transmission() const;

    [[nodiscard]] IPLMaterial to_ipl_material() const;
};