#pragma once
#include <string>
#include <format>
#include <random>

#include "Types.h"

static std::string TimeToText(f32 elapsed) {
	u32 hour = (u32)(elapsed / 3600.0f);
	elapsed -= 3600.0f * (f32)hour;
	u32 minutes = (u32)(elapsed / 60.0f);
	elapsed -= 60.0f * (f32)minutes;
	return std::format("{}:{:02}:{:04.1f}", hour, minutes, elapsed);
}

static u32 RandomInt(u32 max) {
	static std::random_device rd;  
	static std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> dist(0, max);
	return dist(gen);
}

static f32 RandomFloat(float max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0.0f, max);
	return dist(gen);
}

static u32 NumberAtEndOfString(const std::string& str) {
	if (str.empty()) return 0;

	int i = str.size() - 1;
	while (i >= 0 && std::isdigit(str[i])) {
		i--;
	}

	std::string numberPart = str.substr(i + 1);

	if (numberPart.empty()) {
		return 0;
	}

	return std::stoi(numberPart);
}