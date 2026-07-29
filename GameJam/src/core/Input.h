#pragma once
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Types.h"

class Input {
public:
    static void Init(GLFWwindow* handle) { m_Handle = handle; }

    static void Update();
    static bool IsKeyPressed(i32 key);
    static bool IsKeyDown(i32 key);
    static bool IsKeyReleased(i32 key);
    static glm::vec2 GetMousePos();
private:
    static GLFWwindow* m_Handle;
	static std::unordered_map<i32, i32> m_PreviousKeyStates;
	static std::unordered_map<i32, i32> m_CurrentKeyStates;
};