#pragma once
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

#include "Types.h"

class FileReader {
public:
	FileReader(const std::filesystem::path& path) {
		m_Input = std::ifstream(path, std::ios::binary);
		m_IsOpen = m_Input.is_open();
	}

	bool IsOpen() { return m_IsOpen; }

	template<typename T>
	T Read() {
		T result;
		m_Input.read((i8*)(&result), sizeof(T));
		return result;
	}

	template<typename T>
	std::vector<T> ReadArray(u32 count) {
		std::vector<T> result;
		result.reserve(count);

		for (u32 i = 0; i < count; i++) {
			T element;
			m_Input.read((i8*)(&element), sizeof(T));
			result.push_back(element);
		}
		return result;
	}

	std::string ReadString(u32 length) {
		std::vector<i8> array = ReadArray<i8>(length);
		return std::string(array.begin(), array.end());
	}
private:
	std::ifstream m_Input;
	bool m_IsOpen;
};