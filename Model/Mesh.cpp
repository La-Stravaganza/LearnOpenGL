#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <GL/glew.h>
#define  GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Mesh.h"

using std::vector;
using std::string;

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->setup_mesh();
}

void Mesh::setup_mesh() {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, textureCoordinate)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

string Mesh::construct_name(const char* prefix, string name, unsigned int index) {
    string sPrefix(prefix);
    return sPrefix + name + std::to_string(index);
}

void Mesh::draw(Shader shader) {
    GLuint diffuseNumber = 1;
    GLuint specularNumber = 1;
    for (GLuint i = 0; i < this->textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        string textureName = this->textures[i].type;
        GLuint index = 0;
        if (textureName == "texture_diffuse") {
            index = diffuseNumber++;
        } else if (textureName == "texture_specular") {
            index = specularNumber++;
        }

        GLint location = glGetUniformLocation(shader.getProgram(), construct_name("material.", textureName, index).c_str());
        glUniform1i(location, i);
        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }

    GLint shininessLocation = glGetUniformLocation(shader.getProgram(), "material.shininess");
    glUniform1f(shininessLocation, 32.0f);

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    for (GLuint i = 0; i < this->textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

