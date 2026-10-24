
#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

std::string loadShaderFile(const char* path);

GLuint compileShader(GLenum type, const char* source);

GLuint createProgram(
    const char* vertexSource,
    const char* fragmentSource
);

class ShaderProgram {
private:
    GLuint m_program = 0;

public:
    ShaderProgram() = default;
    explicit ShaderProgram(GLuint program);

    void setProgram(GLuint program);
    void use() const;

    void setMat4x4(
        const char* name,
        const glm::mat4& matrix
    ) const;

    void setFloat(
        const char* name,
        float value
    ) const;

    GLuint getProgram() const;
};

#endif
