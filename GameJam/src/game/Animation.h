#pragma once
#include <vector>

#include "../core/ResourceManager.h"
#include "../core/Window.h"

class Animation {
public:
	Animation() {}
	Animation(const std::vector<Sprite>& frames, f32 duration, bool loop) : m_Frames(frames), m_Duration(duration), m_Loop(loop) {}
	void Start();
	void Stop();

	Sprite GetFrame();
private:
	std::vector<Sprite> m_Frames;
	f32 m_Duration;
	bool m_Loop;
	
	bool m_Running = false;
	f32 m_StartTime;
};