#include "Animation.h"

void Animation::Start() {
	if (!m_Running) {
		m_StartTime = glfwGetTime();
		m_Running = true;
	}
}

void Animation::Stop() {
	m_Running = false;
}

Sprite Animation::GetFrame() {
	if (!m_Running) {
		return m_Frames.at(0);
	}
	f32 ellapsedTime = glfwGetTime() - m_StartTime;

	f32 fps = (f32)m_Frames.size() / m_Duration;

	if (ellapsedTime > m_Duration - 0.1f && !m_Loop) {
		return m_Frames.at(m_Frames.size() - 1);
	}

	u32 frameIndex = (u32)std::roundf(ellapsedTime * fps);
	frameIndex %= m_Frames.size();
	return m_Frames.at(frameIndex);
}