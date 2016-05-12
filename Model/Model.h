#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"

class Model {
public:
    Model(const GLchar* path) { this->load_model(path); }
    void draw(Shader shader);
private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> texturesLoaded;
    void load_model(std::string path);
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> load_material_textures(aiMaterial* material, aiTextureType type, std::string typeName);
    static GLuint load_texture_from_file(const char* path, std::string directory);
};

#endif //MODEL_H
