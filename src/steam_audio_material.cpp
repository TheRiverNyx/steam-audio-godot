#include "steam_audio_material.hpp"

using namespace godot;

void SteamAudioMaterial::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_absorption"), &SteamAudioMaterial::get_absorption);
    ClassDB::bind_method(D_METHOD("set_absorption","value"), &SteamAudioMaterial::set_absorption);
    ClassDB::bind_method(D_METHOD("get_scattering"), &SteamAudioMaterial::get_scattering);
    ClassDB::bind_method(D_METHOD("set_scattering","value"), &SteamAudioMaterial::set_scattering);
    ClassDB::bind_method(D_METHOD("get_transmission"), &SteamAudioMaterial::get_transmission);
    ClassDB::bind_method(D_METHOD("set_transmission","value"), &SteamAudioMaterial::set_transmission);

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3,"absorption"), "set_absorption", "get_absorption");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT,"scattering"), "set_scattering", "get_scattering");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3,"transmission"), "set_transmission", "get_transmission");
}

SteamMaterial SteamAudioMaterial::default_material = {
    Vector3(0.10f,0.20f,0.30f),
    0.05f,
    Vector3(0.100f,0.050f,0.030f)
};

Vector3 SteamAudioMaterial::get_absorption() const {return material.absorption;}
void SteamAudioMaterial::set_absorption(Vector3 value) { material.absorption = value; }
float SteamAudioMaterial::get_scattering() const {return material.scattering;}
void SteamAudioMaterial::set_scattering(float value) { material.scattering = value; }
Vector3 SteamAudioMaterial::get_transmission() const {return material.transmission;}
void SteamAudioMaterial::set_transmission(Vector3 value) { material.transmission = value; }
const SteamMaterial &SteamAudioMaterial::get_default_material() {
    return default_material;
}
SteamMaterial SteamAudioMaterial::get_steam_material() const {
    return material;
}