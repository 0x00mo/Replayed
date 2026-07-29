#include "Game.h"

Game* Game::m_Instance;
void Game::Init() {
	m_Instance = new Game();
	m_Instance->m_Window = new Window(1600, 900, "REPLAYED");
	glm::ivec2 windowSize = m_Instance->m_Window->GetSize();
	m_Instance->m_Renderer = new Renderer(windowSize.x, windowSize.y);
	//m_Instance->m_Renderer = new Renderer(1500, 1750); SCREENSHOT

	Input::Init(m_Instance->m_Window->GetHandle());

	m_Instance->m_Transition = false;

	FileReader file = FileReader("res/config.bin");
	if (file.IsOpen()) {
		m_Instance->m_Config.levelsCompleted = file.Read<u32>();
		m_Instance->m_Config.times = file.ReadArray<f32>(5);
	}
	else {
		m_Instance->m_Config.levelsCompleted = 0;
		m_Instance->m_Config.times = std::vector<f32>();
		m_Instance->m_Config.times.resize(5);
	}
}

void Game::Run() {
	f32 deltaTime = 0.0f;
	f32 lastFrame = 0.0f;

	while (!m_Window->ShouldClose()) {
		m_Window->PollEvents();
		Input::Update();

		f32 currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (m_Transition) {
			m_TransitionTime += deltaTime;
			f32 alpha = std::abs((m_TransitionTime - m_TransitionDuration / 2.0f) * (2.0f / m_TransitionDuration));
			m_Renderer->SetTransparency(alpha);
			if (m_TransitionTime > m_TransitionDuration / 2.0f && !m_SetScene) {
				SetScene(m_TransitionScene);
				m_SetScene = true;
			}
			if (m_TransitionTime > m_TransitionDuration) {
				m_Transition = false;
			}
		}

		if (!m_Transition) {
			m_CurrentScene->Update(deltaTime);
		}
		m_Renderer->BeginFrame();
		m_CurrentScene->Render(m_Renderer);
		m_Renderer->EndFrame();

		m_Window->SwapBuffers();
		//m_Renderer->RenderScreenShot(); SCREENSHOT
	}

	Exit();
}

void Game::SetScene(Scene* scene) {
	delete m_CurrentScene;
	m_CurrentScene = scene;
}

void Game::TransitionScene(Scene* scene, f32 duration) {
	m_TransitionTime = 0.0f;
	m_TransitionScene = scene;
	m_TransitionDuration = duration;
	m_Transition = true;
	m_SetScene = false;
}

void Game::Exit() {
	FileWriter file = FileWriter("res/config.bin");
	file.Write<u32>(m_Config.levelsCompleted);
	file.WriteArray<f32>(m_Config.times);

	delete m_CurrentScene;
	delete m_Renderer;
	m_Window->Terminate();
	delete m_Window;
	delete m_Instance;
	std::exit(0);
}
