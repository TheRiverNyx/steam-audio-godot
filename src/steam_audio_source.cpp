#include "steam_audio_source.hpp"

using namespace godot;

// Static vector to track all active audio sources
Vector<SteamAudioSource *> SteamAudioSource::_instances;

// Constructor/Destructor
SteamAudioSource::SteamAudioSource() {
    // Disable default attenuation since Steam Audio handles it
    set_attenuation_model(ATTENUATION_DISABLED);
    _instances.push_back(this);
}

SteamAudioSource::~SteamAudioSource() {
    // Remove this instance from tracking vector
    _instances.erase(this);
}

// Static method to get all active audio sources
const Vector<SteamAudioSource *> &SteamAudioSource::get_all_sources() {
    return _instances;
}

// Godot virtual methods
void SteamAudioSource::_bind_methods() {
    // Bind directivity methods to make them accessible from GDScript
    ClassDB::bind_method(D_METHOD("set_dipole_weight", "weight"), &SteamAudioSource::set_dipole_weight);
    ClassDB::bind_method(D_METHOD("get_dipole_weight"), &SteamAudioSource::get_dipole_weight);
    ClassDB::bind_method(D_METHOD("set_dipole_power", "power"), &SteamAudioSource::set_dipole_power);
    ClassDB::bind_method(D_METHOD("get_dipole_power"), &SteamAudioSource::get_dipole_power);
    ClassDB::bind_method(D_METHOD("set_directivity_enabled", "arg"), &SteamAudioSource::set_directivity_enabled);
    ClassDB::bind_method(D_METHOD("is_directivity_enabled"), &SteamAudioSource::get_is_directivity_enabled);

    // Add properties to editor interface
    ADD_GROUP("Directivity", "directivity_");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "directivity_enabled"), "set_directivity_enabled",
                 "is_directivity_enabled");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "directivity_dipole_weight", PROPERTY_HINT_RANGE, "0,1,0.01"),
                 "set_dipole_weight", "get_dipole_weight");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "directivity_dipole_power", PROPERTY_HINT_RANGE, "0,10,0.1"),
                 "set_dipole_power", "get_dipole_power");
}

// Hide default AudioStreamPlayer3D properties that we don't use
void SteamAudioSource::_validate_property(PropertyInfo &property) const {
    if (property.name == StringName("attenuation_model") || property.name == StringName("unit_size") ||
        property.name.begins_with("attenuation") || property.name.begins_with("emission_")) {
        property.usage = PROPERTY_USAGE_NO_EDITOR;
    }
}
void SteamAudioSource::_notification(int p_what) {

}

void SteamAudioSource::_process(double p_delta) {

}

void SteamAudioSource::update_source() {

}