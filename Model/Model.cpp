#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"
#include "Model.h"

using std::vector;
using std::string;

void Model::draw(Shader shader) {
    for (GLuint i = 0; i < this->meshes.size(); ++i) {
        this->meshes[i].draw(shader);
    }
}

void Model::load_model(string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (scene == nullptr || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        exit(-1);
    }

    this->directory = path.substr(0, path.find_last_of('/'));
    this->process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode *node, const aiScene *scene) {
    for (GLuint i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(this->process_mesh(mesh, scene));
    }

    for (GLuint i = 0; i < node->mNumChildren; i++) {
        this->process_node(node->mChildren[i], scene);
    }
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    for (GLuint i = 0; i < mesh->mNumVertices; ++i) {
        Vertex v;
        
        glm::vec3 temp;
        temp.x = mesh->mVertices[i].x;
        temp.y = mesh->mVertices[i].y;
        temp.z = mesh->mVertices[i].z;
        v.position = temp;
        
        temp.x = mesh->mNormals[i].x;
        temp.y = mesh->mNormals[i].y;
        temp.z = mesh->mNormals[i].z;
        v.normal = temp;

        if (mesh->mTextureCoords[0]) {
            glm::vec2 temp;
            temp.x = mesh->mTextureCoords[0][i].x;
            temp.y = mesh->mTextureCoords[0][i].y;
            v.textureCoordinate = temp;
        } else {
            v.textureCoordinate = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(v);
    }

    for (GLuint i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for(GLuint j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    vector<Texture> diffuseMaps = this->load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    vector<Texture> specularMaps = this->load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::load_material_textures(aiMaterial* mat, aiTextureType type, string typeName) {
    vector<Texture> textures;
    for(GLuint i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        GLboolean skip = GL_FALSE;
        for (GLuint j = 0; j < texturesLoaded.size(); ++j) {
            if (texturesLoaded[j].path == str) {
                textures.push_back(texturesLoaded[j]);
                skip = GL_TRUE;
                break;
            }
        }

        if (skip == GL_FALSE) {
            Texture texture;
            texture.id = load_texture_from_file(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
            this->texturesLoaded.push_back(texture);
        }
    }
    return textures;
}

GLuint Model::load_texture_from_file(const char* path, string directory) {
    //Generate texture ID and load texture data
    string filename = string(path);
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height;
    unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);

    return textureID;
}
