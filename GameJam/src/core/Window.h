#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<string>
#include<iostream>

#include "Types.h"

class Window {
public:
	Window() {}
	Window(i32 width, i32 height, const std::string& titel);
	bool ShouldClose();
	void SwapBuffers();
	void PollEvents();
	void Terminate();
	glm::vec2 GetSize();
	GLFWwindow* GetHandle() { return m_Handle; }
private:
	GLFWwindow* m_Handle;
	i32 m_Width;
	i32 m_Height;
	std::string m_Titel;
};
