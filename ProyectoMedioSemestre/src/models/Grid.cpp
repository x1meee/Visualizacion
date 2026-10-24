
#include "Grid.h"
#include "../obj_helpers/ShaderProgram.h"

#include <cmath>
#include <cstddef>
#include <vector>

Grid::~Grid()
{
    if (m_ebo != 0)
    {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
    }

    if (m_vbo != 0)
    {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }

    if (m_vao != 0)
    {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
}

void Grid::InitVertices(std::vector<Vertex>& Vertices)
{
    index = 0;
    m_y_min = 0.0f;
    m_y_max = 0.0f;

    for (int z = 0; z < m_depth; ++z)
    {
        for (int x = 0; x < m_width; ++x)
        {
            x_e = (static_cast<float>(x) - m_width / 2.0f) / factor;
            y_e = (static_cast<float>(z) - m_depth / 2.0f) / factor;

            const float y = std::cos(x_e) * std::sin(y_e);

            if (y < m_y_min)
                m_y_min = y;

            if (y > m_y_max)
                m_y_max = y;

            Vertices[index].position = glm::vec3(x_e, y, y_e);
            Vertices[index].color = glm::vec3(1.0f);

            ++index;
        }
    }

    const float heightRange = m_y_max - m_y_min;
    m_scale = heightRange > 0.0f ? 1.5f / heightRange : 1.0f;
}

// Genera los índices de los triángulos.
void Grid::InitIndices(std::vector<unsigned int>& Indices)
{
    std::size_t currentIndex = 0;

    for (int z = 0; z < m_depth - 1; ++z)
    {
        for (int x = 0; x < m_width - 1; ++x)
        {
            const unsigned int IndexBottomLeft =
                static_cast<unsigned int>(z * m_width + x);

            const unsigned int IndexTopLeft =
                static_cast<unsigned int>((z + 1) * m_width + x);

            const unsigned int IndexTopRight =
                static_cast<unsigned int>((z + 1) * m_width + x + 1);

            const unsigned int IndexBottomRight =
                static_cast<unsigned int>(z * m_width + x + 1);

            Indices[currentIndex++] = IndexBottomLeft;
            Indices[currentIndex++] = IndexTopLeft;
            Indices[currentIndex++] = IndexTopRight;

            Indices[currentIndex++] = IndexBottomLeft;
            Indices[currentIndex++] = IndexTopRight;
            Indices[currentIndex++] = IndexBottomRight;
        }
    }
}

void Grid::init()
{
    m_width = 80;
    m_depth = 80;
    factor = 10.0f;

    //Evita crear una cuadrícula con dimensiones no válidas
    if (m_width < 2 || m_depth < 2 || factor == 0.0f)
        return;

    const std::size_t vertexCount =
        static_cast<std::size_t>(m_width) * m_depth;

    const std::size_t indexCount =
        static_cast<std::size_t>(m_width - 1) *
        (m_depth - 1) * 6;

    m_vertices.resize(vertexCount);
    m_indices.resize(indexCount);

    InitVertices(m_vertices);
    InitIndices(m_indices);

    //Altura

    for (Vertex& vertex : m_vertices)
    {
        vertex.position.y *= m_scale;
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(m_vertices.size() * sizeof(Vertex)),
        m_vertices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(
            m_indices.size() * sizeof(unsigned int)),
        m_indices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, position))
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, color))
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    m_vertexCount = static_cast<GLsizei>(m_indices.size());
}

void Grid::draw(const glm::mat4& viewProjection)
{
    if (m_shaderProgram == nullptr)
        return;

    //Matriz combinada de vista y proyección.
    renderModel(
        glm::mat4(1.0f),
        viewProjection,
        GL_TRIANGLES
    );
}

//Configuración de matrices 
void Grid::renderModel(
    const glm::mat4& view,
    const glm::mat4& projection,
    int mode)
{
    if (m_shaderProgram == nullptr || m_vao == 0)
        return;

    m_shaderProgram->use();

    m_shaderProgram->setMat4x4("model", m_model_mat);
    m_shaderProgram->setMat4x4("view", view);
    m_shaderProgram->setMat4x4("projection", projection);
    m_shaderProgram->setFloat("maxH", m_y_max * m_scale);
    m_shaderProgram->setFloat("minH", m_y_min * m_scale);

    //Coloración por altura
    GLint gridModeLocation =
        glGetUniformLocation(
            m_shaderProgram->getProgram(),
            "useGridMode"
        );

    if (gridModeLocation != -1)
        glUniform1i(gridModeLocation, 1);

    glBindVertexArray(m_vao);

    glDrawElements(
        mode,
        static_cast<GLsizei>(m_indices.size()),
        GL_UNSIGNED_INT,
        nullptr
    );

    glBindVertexArray(0);
}

void Grid::updateModel(float deltaTime)
{
    if (deltaTime == 0.0f)
        return;
}
