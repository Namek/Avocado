#pragma once
#include <string>
#include <GLES3/gl3.h>
#include <EGL/egl.h>

enum class ShaderType { Vertex, Fragment };

class Shader {
   private:
    std::string error = "";
    std::string name;
    bool compiled = false;
    ShaderType type;
    GLuint shaderId = 0;

    void destroy();

   public:
    Shader(std::string name, ShaderType shaderType);
    ~Shader();

    bool compile();
    GLuint get();

    std::string getError();
    bool isCompiled();

    bool reload();
};
