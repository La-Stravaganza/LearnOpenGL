#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <GL/glew.h>
#define  GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoordinate;
};

struct Texture {
    GLuint id; // GL_TEXTURE(id)
    std::string type; // diffuse or specular
    aiString path; // To avoid multiple loading
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
    void draw(Shader shader);
private:
    GLuint VAO, VBO, EBO;
    void setup_mesh(); // setup VAO, VBO, EBO
    static std::string construct_name(const char* prefix, std::string name, unsigned int index);
};

#endif //MESH_H
