#ifndef WINDOW_GL_H
#define WINDOW_GL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class WindowGL {
private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;
    const char* m_title;

public:
    WindowGL(int width, int height, const char* title);
    ~WindowGL();

    bool init();
    bool shouldClose() const;
    void pollEvents() const;
    void swapBuffers() const;

    GLFWwindow* getWindow() const;
    int getWidth() const;
    int getHeight() const;
};

#endif