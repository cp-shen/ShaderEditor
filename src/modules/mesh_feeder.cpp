#include <shader_editor/common.h>
#include <shader_editor/mesh_feeder.h>

static std::vector<Mesh> mesh_loaded;
static std::vector<Model> model_loaded; // FIXME: remove this

static void load_test_model() {
    load_model("resources/objects/backpack/backpack.obj");
    LOG_MSG("test model loaded");
}

/*****************/
/* API functions */
/*****************/

void load_model(const char *file_path) {
    model_loaded.emplace_back(file_path);

    // TODO: optimize this
    for (auto &mesh : model_loaded.back().meshes) {
        Mesh m = Mesh(mesh);
        LOG_MSG("load %d vertices.", (int)m.vertices.size());
        mesh_loaded.push_back(std::move(m));
    }
}

void mesh_feeder_init() { load_test_model(); }

std::vector<Model> &get_model_loaded() { return model_loaded; }

std::vector<Mesh> &get_mesh_loaded() { return mesh_loaded; }
