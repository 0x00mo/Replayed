#include "Input.h"

GLFWwindow* Input::m_Handle;
std::unordered_map<i32, i32> Input::m_PreviousKeyStates;
std::unordered_map<i32, i32> Input::m_CurrentKeyStates;

void Input::Update() {
    for (i32 key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; key++) {
        m_PreviousKeyStates[key] = m_CurrentKeyStates[key];
        m_CurrentKeyStates[key] = glfwGetKey(m_Handle, key);
    }
}

bool Input::IsKeyPressed(i32 key) {
    return (m_CurrentKeyStates[key] == GLFW_PRESS) && (m_PreviousKeyStates[key] != GLFW_PRESS);
}

bool Input::IsKeyDown(i32 key) {
    return (m_CurrentKeyStates[key] == GLFW_PRESS);
}

bool Input::IsKeyReleased(i32 key) {
    return (m_CurrentKeyStates[key] == GLFW_RELEASE) && (m_PreviousKeyStates[key] == GLFW_PRESS);
}

glm::vec2 Input::GetMousePos() {
    double x, y;
    glfwGetCursorPos(m_Handle, &x, &y);
    return glm::vec2(x, y);
}