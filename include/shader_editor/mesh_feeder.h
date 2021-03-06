/***********************************************************/
/* Date: 2020-05-04                                        */
/* Desc: utility code to generate a mesh or load in a file */
/***********************************************************/

#ifndef MESH_FEEDER_H
#define MESH_FEEDER_H

#include <cassert>
#include <cstring> //memset
#include <learnopengl/mesh.h>
#include <learnopengl/model.h>
#include <memory>
#include <vector>

enum class mesh_input_enum {
    MODEL,
    CUBE,
    SPHERE,
};

class mesh_input_t {
    mesh_input_enum type;
};

std::vector<Mesh> &get_mesh_loaded();
std::vector<Model> &get_model_loaded();
void load_model(const char *);
void mesh_feeder_init();

// struct mesh_t {
//     float *vertices;   // 3 per vertex
//     float *normals;    // 3 per vertex
//     float *texcoords;  // 2 per vertex
//     float *colors;     // 3 per vertex
//     unsigned *indices; // 3 per triangle
//
//     unsigned n_vertices;
//     unsigned n_triangles;
//
//     mesh_t(unsigned n_vert, unsigned n_tri)
//         : n_vertices(n_vert), n_triangles(n_tri) {
//
//         assert(n_vert > 0 && n_tri > 0 && "mesh vert or tri count is 0");
//
//         vertices = new float[n_vert * 3];
//         normals = new float[n_vert * 3];
//         texcoords = new float[n_vert * 2];
//         colors = new float[n_vert * 3];
//         indices = new unsigned[n_tri * 3];
//
//         memset(vertices, 0, sizeof(float) * n_vert * 3);
//         memset(normals, 0, sizeof(float) * n_vert * 3);
//         memset(texcoords, 0, sizeof(float) * n_vert * 2);
//         memset(colors, 0, sizeof(float) * n_vert * 3);
//         memset(indices, 0, sizeof(unsigned) * n_tri * 3);
//     }
//
//     virtual ~mesh_t() {
//         delete vertices;
//         delete normals;
//         delete texcoords;
//         delete colors;
//         delete indices;
//     }
// };

#endif
