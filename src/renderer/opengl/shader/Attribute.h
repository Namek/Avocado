#pragma once
#include <GLES3/gl3.h>
#include <EGL/egl.h>

class Attribute {
    GLint id;

   public:
    Attribute(GLint id);
    ~Attribute();

    void enable();
    void disable();
    void pointer(GLint size, GLenum type, GLsizei stride, int pointer);
};
