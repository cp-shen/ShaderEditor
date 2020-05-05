#include <shader_editor/mesh_feeder.h>

static std::vector<Mesh> mesh_loaded;

std::vector<Mesh>& get_mesh_loaded() {
    return mesh_loaded;
}

// TODO
