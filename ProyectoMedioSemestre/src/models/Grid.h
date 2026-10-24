
#ifndef GRID_H
#define GRID_H

#include "Model.h"
#include "ImportedModel.h"
#include <vector>

class ShaderProgram;

class Grid : public RenderableModel {
private:
    int m_width = 0;
    int m_depth = 0;
    float factor = 1.0f;

    float x_e = 0.0f;
    float y_e = 0.0f;

    float m_y_min = 0.0f;
    float m_y_max = 0.0f;
    float m_scale = 1.0f;

    int index = 0;

    glm::mat4 m_model_mat = glm::mat4(1.0f);

    ShaderProgram* m_shaderProgram = nullptr;

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    GLuint m_ebo = 0;

    void InitVertices(std::vector<Vertex>& Vertices);
    void InitIndices(std::vector<unsigned int>& Indices);

public:
    Grid() = default;
    ~Grid() override;

    //Conectar el programa de shaders desde Scene.
    void setShaderProgram(ShaderProgram* shaderProgram) {
        m_shaderProgram = shaderProgram;
    }

    void init() override;
    void draw(const glm::mat4& viewProjection) override;

    void renderModel(
        const glm::mat4& view,
        const glm::mat4& projection,
        int mode
    );

    void updateModel(float deltaTime);
};

#endif
