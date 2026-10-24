#include "WindowGL.h"
#include <iostream>

WindowGL::WindowGL(int width, int height, const char* title)
    : m_window(nullptr),
      m_width(width),
      m_height(height),
      m_title(title) {
}

WindowGL::~WindowGL() {
    if (m_window != nullptr) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

bool WindowGL::init() {
    if (!glfwInit()) {
        std::cerr << "Error: no se pudo inicializar GLFW." << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(
        m_width,
        m_height,
        m_title,
        nullptr,
        nullptr
    );

    if (m_window == nullptr) {
        std::cerr << "Error: no se pudo crear la ventana." << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error: no se pudo inicializar GLEW." << std::endl;
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        glfwTerminate();
        return false;
    }

    glfwSwapInterval(1);
    return true;
}

bool WindowGL::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void WindowGL::pollEvents() const {
    glfwPollEvents();
}

void WindowGL::swapBuffers() const {
    glfwSwapBuffers(m_window);
}

GLFWwindow* WindowGL::getWindow() const {
    return m_window;
}

int WindowGL::getWidth() const {
    return m_width;
}

int WindowGL::getHeight() const {
    return m_height;
}