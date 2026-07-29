#pragma once
#include <filesystem>

#include "../core/Types.h"
#include "../core/GLValidation.h"

class Texture {
public:
	Texture() {}
	Texture(const std::filesystem::path& path);

	u32 GetID() { return m_ID; }
	u8* GetData() { return m_Data; }
	u32 GetWidth() { return m_Width; }
	u32 GetHeight() { return m_Height; }
private:
	u32 m_ID;
	u8* m_Data;
	u32 m_Width;
	u32 m_Height;
};