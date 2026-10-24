
#ifndef SCENE_H
#define SCENE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

#include "../models/ImportedModel.h"
#include "../models/Grid.h"
#include "../camera/OrbitCamera.h"
#include "WindowGL.h"
#include "ShaderProgram.h"

class Scene {
private:
    std::unique_ptr<WindowGL> m_window;
    std::unique_ptr<OrbitCamera> m_camera;
    std::unique_ptr<Grid> m_grid;
    std::unique_ptr<ShaderProgram> m_gridShader;

    std::vector<Model> m_models;

    GLuint m_modelProgram = 0;
    GLuint m_axesProgram = 0;

    GLuint m_axisVAO = 0;
    GLuint m_axisVBO = 0;

    int m_currentModel = 0;
    int m_renderMode = 2;

    bool m_initialized = false;

public:
    Scene();
    ~Scene();

    bool init(int width = 1000, int height = 700,
              const char* title = "Visualizador de modelos OBJ");

    void loadModels(int argc, char* argv[]);
    void render();
    void cleanup();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
};

#endif
