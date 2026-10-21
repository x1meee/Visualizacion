
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
    // Inicializar GLFW
    if (!glfwInit())
    {
        std::cerr << "Error al inicializar GLFW" << std::endl;
        return -1;
    }

    // Configurar OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Crear la ventana
    GLFWwindow* window = glfwCreateWindow(
        1000, 700,
        "Visualizador de modelos OBJ",
        nullptr, nullptr
    );

    if (window == nullptr)
    {
        std::cerr << "Error al crear la ventana" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Inicializar GLEW
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Error al inicializar GLEW" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Color de fondo
    glClearColor(0.08f, 0.08f, 0.12f, 1.0f);

    // Bucle principal
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Liberar recursos
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
