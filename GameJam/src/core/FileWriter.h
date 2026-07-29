#pragma once
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

#include "Types.h"

class FileWriter {
public:
	FileWriter(const std::filesystem::path& path) {
		m_Output = std::ofstream(path, std::ios::binary);
		m_IsOpen = m_Output.is_open();
	}

	bool IsOpen() { return m_IsOpen; }

	template<typename T>
	void Write(T element) {
		m_Output.write((i8*)(&element), sizeof(T));
	}

	template<typename T>
	void WriteArray(std::vector<T> elements) {
		for (u32 i = 0; i < elements.size(); i++) {
			Write<T>(elements.at(i));
		}
	}

	void WriteString(const std::string& text) {
		WriteArray<i8>(std::vector<i8>(text.begin(), text.end()));
	}
private:
	std::ofstream m_Output;
	bool m_IsOpen;
};