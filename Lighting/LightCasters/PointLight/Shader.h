#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

class Shader {
private:
    GLuint program;
public:
    Shader(const GLchar* vsPath, const GLchar* fsPath);
    GLuint getProgram() const { return program; }
    void use() { glUseProgram(this->program); }
};

#endif //SHADER_H
