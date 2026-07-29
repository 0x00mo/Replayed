#include "Window.h"

Window::Window(i32 width, i32 height, const std::string& titel) : m_Width(width), m_Height(height), m_Titel(titel) {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_Handle = glfwCreateWindow(width, height, titel.c_str(), NULL, NULL);
    if (!m_Handle) {
        glfwTerminate();
        std::cerr << "Failed to create Window" << std::endl;
        exit(-1);
    }

    glfwMakeContextCurrent(m_Handle);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(-1);
    }

    glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMaximizeWindow(m_Handle);
}

bool Window::ShouldClose() {
    return glfwWindowShouldClose(m_Handle);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_Handle);
}

void Window::PollEvents() {
    glfwPollEvents();
}

void Window::Terminate() {
    glfwDestroyWindow(m_Handle);
    glfwTerminate();
}

glm::vec2 Window::GetSize() {
    glfwGetWindowSize(m_Handle, &m_Width, &m_Height);
    return glm::vec2(m_Width, m_Height);
}