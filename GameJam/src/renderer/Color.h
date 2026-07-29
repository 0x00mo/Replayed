#pragma once
#include "../core/Types.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Color {
public:
	Color(glm::vec3 c) : m_R(c.r), m_G(c.g), m_B(c.b), m_A(0.0f) {}
	Color(f32 r, f32 g, f32 b) : m_R(r), m_G(g), m_B(b), m_A(0.0f) {}
	Color(f32 r, f32 g, f32 b, f32 a) : m_R(r), m_G(g), m_B(b), m_A(a) {}
	static Color RGBAFromHex(u32 value) {
		u8 red = (value & 0xFF000000) >> 24;
		u8 green = (value & 0x00FF0000) >> 16;
		u8 blue = (value & 0x0000FF00) >> 8;
		u8 alpha = (value & 0x000000FF);
		return Color((f32)red / 255.0f, (f32)green / 255.0f, (f32)blue / 255.0f, (f32)alpha / 255.0f);
	}
	static Color RGBFromHex(u32 value) {
		u8 red = (value & 0xFF0000) >> 16;
		u8 green = (value & 0x00FF00) >> 8;
		u8 blue = (value & 0x0000FF);
		return Color((f32)red / 255.0f, (f32)green / 255.0f, (f32)blue / 255.0f);
	}

	bool operator==(const Color& other) const {
		return (m_R == other.m_R) && (m_G == other.m_G) && (m_B == other.m_B);
	}

	inline glm::vec3 ToVec3() const { return glm::vec3(m_R, m_G, m_B); }
	inline glm::vec4 ToVec4() const { return glm::vec4(m_R, m_G, m_B, m_A); }

	inline f32 R() const { return m_R; }
	inline f32 G() const { return m_G; }
	inline f32 B() const { return m_B; }
	inline f32 A() const { return m_A; }
private:
	f32 m_R, m_G, m_B, m_A;
};