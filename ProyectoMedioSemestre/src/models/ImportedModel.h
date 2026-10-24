#ifndef IMPORTEDMODEL_H
#define IMPORTEDMODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

struct Model {
    std::string path;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 minimum;
    glm::vec3 maximum;

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    // Matriz de transformación del modelo.
    glm::mat4 m_model_mat = glm::mat4(1.0f);

    // Actualizar la rotación del modelo.
    void update(glm::vec3 axis) {
        m_model_mat = glm::rotate(
            m_model_mat,
            glm::radians(0.7f),
            axis
        );
    }
};

bool loadOBJ(const std::string& path, Model& model);

#endif
