#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class RenderableModel {
protected:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLsizei m_vertexCount = 0;
    glm::mat4 m_modelMatrix = glm::mat4(1.0f);

public:
    RenderableModel() = default;
    virtual ~RenderableModel() = default;

    virtual void init() = 0;
    virtual void draw(const glm::mat4& viewProjection) = 0;

    RenderableModel(const RenderableModel&) = delete;
    RenderableModel& operator=(const RenderableModel&) = delete;
};

#endif