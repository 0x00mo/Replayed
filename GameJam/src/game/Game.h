#pragma once
#include "../core/Window.h"
#include "../core/Input.h"
#include "../core/FileReader.h"
#include "../core/FileWriter.h"
#include "../renderer/Renderer.h"
#include "../renderer/Color.h"
#include "scene/Scene.h"

typedef struct {
	u32 levelsCompleted;
	std::vector<f32> times;
} Config;

class Game {
public:
	static void Init();
	static Game& Get() { return *m_Instance; }

	Config& GetConfig() { return m_Config; }

	void Run();
	void SetScene(Scene* scene);
	void TransitionScene(Scene* scene, f32 duration);
	void Exit();
private:
	static Game* m_Instance;

	Window* m_Window;
	Renderer* m_Renderer;
	Scene* m_CurrentScene;

	f32 m_TransitionTime;
	f32 m_TransitionDuration;
	Scene* m_TransitionScene;
	bool m_Transition;
	bool m_SetScene;

	Config m_Config;
};