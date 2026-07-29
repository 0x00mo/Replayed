#pragma once
#include <chrono>
#include <string>

#include "Utils.h"
#include "../core/Types.h"

class Timer {
public:
	Timer() : m_Running(false), m_EllapsedSeconds(0.0f) {}
	Timer(f32 ellapsedSeconds) : m_Running(false), m_EllapsedSeconds(ellapsedSeconds) {}

	void Start() { 
		m_StartTime = std::chrono::high_resolution_clock::now(); 
		m_EndTime = std::chrono::high_resolution_clock::now(); 
		m_Running = true;
	}

	void Stop() {
		if (m_Running) {
			m_EndTime = std::chrono::high_resolution_clock::now();
			m_Running = false;
		}
	}

	f32 ElapsedSeconds() {
		if (m_Running) {
			m_EndTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<f32> diff = m_EndTime - m_StartTime;
			return diff.count() + m_EllapsedSeconds;
		}
		else {
			std::chrono::duration<f32> diff = m_EndTime - m_StartTime;
			return diff.count() + m_EllapsedSeconds;
		}
	}

	std::string ToText() {
		f32 elapsed = ElapsedSeconds();
		return TimeToText(elapsed);
	}

	bool IsRunning() { return m_Running; }
private:
	std::chrono::high_resolution_clock::time_point m_StartTime;
	std::chrono::high_resolution_clock::time_point m_EndTime;
	bool m_Running;
	f32 m_EllapsedSeconds;
};