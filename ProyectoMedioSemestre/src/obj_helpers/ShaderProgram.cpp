#include "ShaderProgram.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::string loadShaderFile(const char* path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Error: no se pudo abrir el shader: "
                  << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

GLuint compileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];

        glGetShaderInfoLog(
            shader, sizeof(infoLog), nullptr, infoLog
        );

        std::cerr << "Error al compilar el shader:\n"
                  << infoLog << std::endl;

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createProgram(
    const char* vertexSource,
    const char* fragmentSource
)
{
    GLuint vertexShader = compileShader(
        GL_VERTEX_SHADER, vertexSource
    );

    if (vertexShader == 0)
        return 0;

    GLuint fragmentShader = compileShader(
        GL_FRAGMENT_SHADER, fragmentSource
    );

    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        return 0;
    }

    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        char infoLog[512];

        glGetProgramInfoLog(
            program, sizeof(infoLog), nullptr, infoLog
        );

        std::cerr << "Error al enlazar el programa:\n"
                  << infoLog << std::endl;

        glDeleteProgram(program);
        program = 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}


ShaderProgram::ShaderProgram(GLuint program)
    : m_program(program)
{
}

void ShaderProgram::setProgram(GLuint program)
{
    m_program = program;
}

void ShaderProgram::use() const
{
    glUseProgram(m_program);
}

void ShaderProgram::setMat4x4(
    const char* name,
    const glm::mat4& matrix
) const
{
    GLint location = glGetUniformLocation(m_program, name);

    if (location != -1)
    {
        glUniformMatrix4fv(
            location,
            1,
            GL_FALSE,
            &matrix[0][0]
        );
    }
}

void ShaderProgram::setFloat(
    const char* name,
    float value
) const
{
    GLint location = glGetUniformLocation(m_program, name);

    if (location != -1)
    {
        glUniform1f(location, value);
    }
}

GLuint ShaderProgram::getProgram() const
{
    return m_program;
}
