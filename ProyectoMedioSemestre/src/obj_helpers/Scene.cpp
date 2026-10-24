
#include "Scene.h"

#include <algorithm>
#include <iostream>
#include <string>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef PROJECT_ROOT
#define PROJECT_ROOT "."
#endif

Scene::Scene() = default;

Scene::~Scene()
{
    cleanup();
}

bool Scene::init(int width, int height, const char* title)
{
    m_window = std::make_unique<WindowGL>(width, height, title);

    if (!m_window->init())
    {
        m_window.reset();
        glfwTerminate();
        return false;
    }

    const std::string shaderPath =
        std::string(PROJECT_ROOT) + "/build/shaders/";

    const std::string vertexSource =
        loadShaderFile((shaderPath + "vert_shader.glsl").c_str());

    const std::string fragmentSource =
        loadShaderFile((shaderPath + "frag_shader.glsl").c_str());

    const std::string axesVertexSource =
        loadShaderFile((shaderPath + "axes_vertex.glsl").c_str());

    const std::string axesFragmentSource =
        loadShaderFile((shaderPath + "axes_frag.glsl").c_str());

    if (vertexSource.empty() || fragmentSource.empty() ||
        axesVertexSource.empty() || axesFragmentSource.empty())
    {
        cleanup();
        return false;
    }

    m_modelProgram = createProgram(
        vertexSource.c_str(),
        fragmentSource.c_str()
    );

    m_axesProgram = createProgram(
        axesVertexSource.c_str(),
        axesFragmentSource.c_str()
    );

    if (m_modelProgram == 0 || m_axesProgram == 0)
    {
        cleanup();
        return false;
    }

    // Se conserva el Grid, pero no se dibujará en la escena.
    m_gridShader = std::make_unique<ShaderProgram>(m_modelProgram);

    m_grid = std::make_unique<Grid>();
    m_grid->setShaderProgram(m_gridShader.get());
    m_grid->init();

    const GLfloat axesVertices[] = {
        0, 0, 0,  1, 0, 0,
        2, 0, 0,  1, 0, 0,

        0, 0, 0,  0, 1, 0,
        0, 2, 0,  0, 1, 0,

        0, 0, 0,  0, 0, 1,
        0, 0, 2,  0, 0, 1
    };

    glGenVertexArrays(1, &m_axisVAO);
    glGenBuffers(1, &m_axisVBO);

    glBindVertexArray(m_axisVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_axisVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(axesVertices),
        axesVertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(GLfloat), (void*)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(GLfloat),
        (void*)(3 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    m_camera = std::make_unique<OrbitCamera>(
        glm::vec3(3.2f, 2.6f, 4.2f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    glEnable(GL_DEPTH_TEST);

    m_initialized = true;

    std::cout << "Escena inicializada correctamente." << std::endl;
    return true;
}

void Scene::loadModels(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        Model model;

        if (loadOBJ(argv[i], model))
        {
            m_models.push_back(model);
        }
    }

    if (m_models.empty())
    {
        std::cerr << "Advertencia: no se cargaron modelos OBJ."
                  << std::endl;
    }
    else
    {
        std::cout << "Total de modelos cargados: "
                  << m_models.size() << std::endl;
    }
}

void Scene::render()
{
    if (!m_initialized || m_window == nullptr)
        return;

    GLFWwindow* window = m_window->getWindow();

    while (!m_window->shouldClose())
    {
        m_window->pollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
        {
            if (!m_models.empty())
            {
                m_currentModel =
                    (m_currentModel + 1) %
                    static_cast<int>(m_models.size());

                while (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
                    m_window->pollEvents();
            }
        }

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            m_renderMode = 0;

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
            m_renderMode = 1;

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
            m_renderMode = 2;

        if (!m_models.empty())
        {
            if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
                m_models[m_currentModel].update(glm::vec3(1, 0, 0));

            if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
                m_models[m_currentModel].update(glm::vec3(0, 1, 0));

            if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
                m_models[m_currentModel].update(glm::vec3(0, 0, 1));
        }

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        if (width == 0 || height == 0)
            continue;

        glViewport(0, 0, width, height);

        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(width) / static_cast<float>(height),
            0.1f,
            100.0f
        );

        const glm::mat4 view = m_camera->getMatrix();

        // Dibujar los ejes X, Y y Z.
        glUseProgram(m_axesProgram);

        const glm::mat4 axesMVP = projection * view;

        GLint axesLocation =
            glGetUniformLocation(m_axesProgram, "MVP");

        if (axesLocation != -1)
        {
            glUniformMatrix4fv(
                axesLocation,
                1,
                GL_FALSE,
                glm::value_ptr(axesMVP)
            );
        }

        glBindVertexArray(m_axisVAO);
        glDrawArrays(GL_LINES, 0, 6);
        glBindVertexArray(0);

        
        // Dibuja el modelo OBJ seleccionado
        if (!m_models.empty())
        {
            Model& model = m_models[m_currentModel];

            const glm::vec3 center =
                (model.minimum + model.maximum) * 0.5f;

            const glm::vec3 dimensions =
                model.maximum - model.minimum;

            const float largestDimension = std::max(
                dimensions.x,
                std::max(dimensions.y, dimensions.z)
            );

            const float scale = largestDimension > 0.0f
                ? 2.0f / largestDimension
                : 1.0f;

            const glm::mat4 normalize =
                glm::scale(
                    glm::mat4(1.0f),
                    glm::vec3(scale)
                );

            const glm::mat4 centerModel =
                glm::translate(glm::mat4(1.0f), -center);

            const glm::mat4 modelMatrix =
                model.m_model_mat * normalize * centerModel;

            const glm::mat4 mvp =
                projection * view * modelMatrix;

            glUseProgram(m_modelProgram);

            // Desactiva el modo de coloración por altura para los OBJ.
            GLint gridModeLocation =
                glGetUniformLocation(m_modelProgram, "useGridMode");

            if (gridModeLocation != -1)
                glUniform1i(gridModeLocation, 0);

            GLint mvpLocation =
                glGetUniformLocation(m_modelProgram, "MVP");

            if (mvpLocation != -1)
            {
                glUniformMatrix4fv(
                    mvpLocation,
                    1,
                    GL_FALSE,
                    glm::value_ptr(mvp)
                );
            }

            glPolygonMode(
                GL_FRONT_AND_BACK,
                m_renderMode == 0 ? GL_POINT :
                m_renderMode == 1 ? GL_LINE : GL_FILL
            );

            glBindVertexArray(model.vao);

            glDrawElements(
                GL_TRIANGLES,
                static_cast<GLsizei>(model.indices.size()),
                GL_UNSIGNED_INT,
                nullptr
            );

            glBindVertexArray(0);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        m_window->swapBuffers();
    }
}

void Scene::cleanup()
{
    // Liberar los recursos del Grid
    m_grid.reset();
    m_gridShader.reset();

    if (m_axisVBO != 0)
    {
        glDeleteBuffers(1, &m_axisVBO);
        m_axisVBO = 0;
    }

    if (m_axisVAO != 0)
    {
        glDeleteVertexArrays(1, &m_axisVAO);
        m_axisVAO = 0;
    }

    for (Model& model : m_models)
    {
        if (model.ebo != 0)
            glDeleteBuffers(1, &model.ebo);

        if (model.vbo != 0)
            glDeleteBuffers(1, &model.vbo);

        if (model.vao != 0)
            glDeleteVertexArrays(1, &model.vao);

        model.ebo = 0;
        model.vbo = 0;
        model.vao = 0;
    }

    m_models.clear();

    if (m_modelProgram != 0)
    {
        glDeleteProgram(m_modelProgram);
        m_modelProgram = 0;
    }

    if (m_axesProgram != 0)
    {
        glDeleteProgram(m_axesProgram);
        m_axesProgram = 0;
    }

    m_camera.reset();
    m_initialized = false;

    if (m_window != nullptr)
    {
        m_window.reset();
        glfwTerminate();
    }
}
