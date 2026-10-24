
#include "ImportedModel.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include <cstddef>

bool loadOBJ(const std::string& path, Model& model)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Error: no se pudo abrir el modelo: "
                  << path << std::endl;
        return false;
    }

    model.path = path;
    model.vertices.clear();
    model.indices.clear();

    std::vector<glm::vec3> positions;

    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream stream(line);
        std::string type;
        stream >> type;

        if (type == "v")
        {
            glm::vec3 position;
            stream >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }
        else if (type == "f")
        {
            std::vector<unsigned int> face;
            std::string vertexData;

            while (stream >> vertexData)
            {
                std::istringstream vertexStream(vertexData);
                std::string positionIndex;
                std::getline(vertexStream, positionIndex, '/');

                if (positionIndex.empty())
                    continue;

                int index = std::stoi(positionIndex);

                if (index < 0)
                    index = static_cast<int>(positions.size()) + index;
                else
                    index -= 1;

                if (index >= 0 &&
                    index < static_cast<int>(positions.size()))
                {
                    face.push_back(static_cast<unsigned int>(index));
                }
            }

            for (std::size_t i = 1; i + 1 < face.size(); ++i)
            {
                model.indices.push_back(face[0]);
                model.indices.push_back(face[i]);
                model.indices.push_back(face[i + 1]);
            }
        }
    }

    if (positions.empty() || model.indices.empty())
    {
        std::cerr << "Error: el modelo no contiene geometría válida: "
                  << path << std::endl;
        return false;
    }

    model.minimum = positions[0];
    model.maximum = positions[0];

    for (const glm::vec3& position : positions)
    {
        model.minimum.x = std::min(model.minimum.x, position.x);
        model.minimum.y = std::min(model.minimum.y, position.y);
        model.minimum.z = std::min(model.minimum.z, position.z);

        model.maximum.x = std::max(model.maximum.x, position.x);
        model.maximum.y = std::max(model.maximum.y, position.y);
        model.maximum.z = std::max(model.maximum.z, position.z);
    }

    float minY = model.minimum.y;
    float maxY = model.maximum.y;
    float height = maxY - minY;

    if (height == 0.0f)
        height = 1.0f;

    model.vertices.reserve(positions.size());

    for (const glm::vec3& position : positions)
    {
        float t = (position.y - minY) / height;
        glm::vec3 color;

        if (t < 0.5f)
        {
            float factor = t * 2.0f;
            color = glm::vec3(
                factor,
                factor,
                1.0f
            );
        }
        else
        {
            float factor = (t - 0.5f) * 2.0f;
            color = glm::vec3(
                1.0f,
                1.0f - factor,
                0.0f
            );
        }

        model.vertices.push_back({position, color});
    }

    glGenVertexArrays(1, &model.vao);
    glGenBuffers(1, &model.vbo);
    glGenBuffers(1, &model.ebo);

    glBindVertexArray(model.vao);

    glBindBuffer(GL_ARRAY_BUFFER, model.vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        model.vertices.size() * sizeof(Vertex),
        model.vertices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        model.indices.size() * sizeof(unsigned int),
        model.indices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, position)
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, color)
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    std::cout << "Modelo cargado: " << path << std::endl;
    std::cout << "Vertices: " << model.vertices.size() << std::endl;
    std::cout << "Indices: " << model.indices.size() << std::endl;

    std::cout << "Valores minimos:" << std::endl;
    std::cout << "X: " << model.minimum.x << std::endl;
    std::cout << "Y: " << model.minimum.y << std::endl;
    std::cout << "Z: " << model.minimum.z << std::endl;

    std::cout << "Valores maximos:" << std::endl;
    std::cout << "X: " << model.maximum.x << std::endl;
    std::cout << "Y: " << model.maximum.y << std::endl;
    std::cout << "Z: " << model.maximum.z << std::endl;

    std::cout << "------------------------" << std::endl;

    return true;
}
