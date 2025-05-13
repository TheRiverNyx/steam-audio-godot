#include "steam_audio_material.h"

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
Vector3 SteamAudioMaterial::get_absorption() const {return absorption;}
void SteamAudioMaterial::set_absorption(Vector3 value) { absorption = value; }
float SteamAudioMaterial::get_scattering() const {return scattering;}
void SteamAudioMaterial::set_scattering(float value) { scattering = value; }
Vector3 SteamAudioMaterial::get_transmission() const {return transmission;}
void SteamAudioMaterial::set_transmission(Vector3 value) { transmission = value; }

IPLMaterial SteamAudioMaterial::to_ipl_material() const {
    IPLMaterial material ={};
    material.absorption[0] = absorption.x;
    material.absorption[1] = absorption.y;
    material.absorption[2] = absorption.z;

    material.scattering = scattering;

    material.transmission[0] = transmission.x;
    material.transmission[1] = transmission.y;
    material.transmission[2] = transmission.z;

    return material;
}


