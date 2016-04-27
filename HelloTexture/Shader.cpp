#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <GL/glew.h>
#include "Shader.h"

Shader::Shader(const GLchar *vsPath, const GLchar *fsPath) {
    /**
     * Steps:
     * 1. Read the shader contents from file
     * 2. Create a vertex shader, compile it, check its compilation status
     * 3. So does the fragment shader
     * 4. Attach two shaders with the shader program, check its linking status
     * 5. Recycle resources
     */

    // 1. Read shaders from files
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

    // 2.1. Create a vertex shader and compile it
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // 2.2. Check if the vertex shader is compiled correctly
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == false) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX:COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 3.1. Create a fragment shader and compile it
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // 3.2. Check if the fragment shader is compiled correctly
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == false) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 4.1. Link two shaders with the shader program.
    this->program = glCreateProgram();
    glAttachShader(this->program, vertexShader);
    glAttachShader(this->program, fragmentShader);
    glLinkProgram(this->program);

    // 4.2. Check if the linking is successful.
    glGetShaderiv(this->program, GL_LINK_STATUS, &success);
    if (success == false) {
        glGetShaderInfoLog(this->program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM_LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 5. After linking, we don't need shader objects anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
