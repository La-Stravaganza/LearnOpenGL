#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <GL/glew.h>
#include "Shader.h"

Shader::Shader(const GLchar *vsPath, const GLchar *fsPath) {
    // Read shaders from files
    std::ifstream vsStream(vsPath);
    std::stringstream vsBuffer;
    vsBuffer << vsStream.rdbuf();
    std::string vsStr = vsBuffer.str();
    const char* vertexShaderSource = vsStr.c_str();

    std::ifstream fgStream(fsPath);
    std::stringstream fgBuffer;
    fgBuffer << fgStream.rdbuf();
    std::string fgStr = fgBuffer.str();
    const char* fragmentShaderSource = fgStr.c_str();

    // Create a vertex shader and compile it
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check if the vertex shader is compiled correctly
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (success == false) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX:COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create a fragment shader and compile it
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check if the fragment shader is compiled correctly
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (success == false) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link two shaders with the shader program.
    this->program = glCreateProgram();
    glAttachShader(this->program, vertexShader);
    glAttachShader(this->program, fragmentShader);
    glLinkProgram(this->program);

    // check if the linking is successful.
    glGetShaderiv(this->program, GL_LINK_STATUS, &success);

    if (success == false) {
        glGetShaderInfoLog(this->program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM_LINKING_FAILED\n" << infoLog << std::endl;
    }

    // After linking, we don't need shader objects anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() {
    glUseProgram(this->program);
}

