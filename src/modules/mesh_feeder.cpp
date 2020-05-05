#include <learnopengl/model.h>
#include <shader_editor/common.h>
#include <shader_editor/mesh_feeder.h>

static std::vector<Mesh> mesh_loaded;
static std::vector<Model> model_loaded;

static void add_test_mesh() {
    load_model("resources/objects/backpack/backpack.obj");
    LOG_MSG("added test mesh");
}

/*****************/
/* API functions */
/*****************/

std::vector<Mesh> &get_mesh_loaded() { return mesh_loaded; }

void load_model(const char *file_path) {
    model_loaded.emplace_back(file_path);

    // TODO: optimize this
    for (auto &mesh : model_loaded.back().meshes) {
        Mesh m = Mesh (mesh);
        LOG_MSG("load %d vertices.", (int)m.vertices.size());
        mesh_loaded.push_back(m);
    }
}

void mesh_feeder_init() { add_test_mesh(); }
