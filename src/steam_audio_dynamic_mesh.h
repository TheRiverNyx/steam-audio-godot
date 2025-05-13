#pragma once

#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <phonon.h>

using namespace godot;

class SteamAudioDynamicMesh : public Node3D {
    GDCLASS(SteamAudioDynamicMesh, Node3D)

private:
    IPLScene           proxy_subscene = nullptr;
    IPLInstancedMesh   instanced_mesh = nullptr;

    // helper: convert Godot Transform3D → IPLMatrix4x4
    static IPLMatrix4x4 to_ipl_matrix(const Transform3D &t);

    // builds a one-off IPLScene containing this mesh’s raw geometry
    void build_subscene();

public:
    SteamAudioDynamicMesh();
    ~SteamAudioDynamicMesh() override;

    void _ready() override;
    void _process(double delta) override;

protected:
    static void _bind_methods();
};