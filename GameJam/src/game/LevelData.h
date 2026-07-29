#pragma once
#include <vector>
#include <filesystem>
#include <assert.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../core/Types.h"
#include "../core/Utils.h"
#include "../renderer/Texture.h"
#include "../renderer/Color.h"

typedef struct {
	glm::ivec2 position;
	glm::ivec2 size;
} Platform;

constexpr u32 STONE_TILE1 = 1;
constexpr u32 STONE_TILE2 = 2;
constexpr u32 STONE_TILE3 = 3;
constexpr u32 CRACKED_STONE_TILE1 = 4;
constexpr u32 CRACKED_STONE_TILE2 = 5;
constexpr u32 CRACKED_STONE_TILE3 = 6;
constexpr u32 VINE_TILE1 = 7;
constexpr u32 VINE_TILE2 = 8;
constexpr u32 VINE_TILE3 = 9;
constexpr u32 VINE_END_TILE = 10;
constexpr u32 PLANK_TILE1 = 11;
constexpr u32 PLANK_TILE2 = 12;
constexpr u32 PLANK_TILE3 = 13;
constexpr u32 PLANK_LOG_TILE1 = 14;
constexpr u32 PLANK_LOG_TILE2 = 15;
constexpr u32 PLANK_LOG_TILE3 = 16;
constexpr u32 BORDER_TILE = 17;

const u32 STONE_COLOR = 0x959595;
const u32 CRACKED_STONE_COLOR = 0x4b4646;
const u32 VINE_COLOR1 = 0x30ff1d;
const u32 VINE_COLOR2 = 0x0e5008;
const u32 PLANK_COLOR = 0x000000;
const u32 PLAYER_COLOR = 0xff0000;
const u32 LIGHT_COLOR = 0x1800ff;
const u32 END_COLOR = 0xffeb00;
const u32 LEVER_COLOR = 0x971ba1;
const u32 DOOR_COLOR = 0xa1861b;
const u32 DOOR_MIRORED_COLOR = 0x7c7c7c;
const u32 DISPENSER_RIGHT_COLOR = 0xffad00;
const u32 DISPENSER_LEFT_COLOR = 0x703a3a;
const u32 JUMPPAD_COLOR = 0x0ffbff;
const u32 PRESEAR_PLAT_COLOR = 0xffffff;
const u32 TRAP_COLOR = 0xde72ff;
const u32 BORDER_COLOR = 0x9e6539;

class LevelData {
public:
	LevelData(const std::filesystem::path& path);
	std::vector<glm::ivec2> GetLights() { return m_Lights; }
	std::vector<Platform> GetPlatforms() { return m_Platforms; }
	std::vector<Platform> GetBorderColliders() { return m_BorderColliders; }
	glm::ivec2 GetPlayerStartPosition() { return m_PlayStartPosition; }
	glm::ivec2 GetEndPosition() { return m_EndPosition; }
	std::vector<u32>& GetTileData() { return m_Background; }
	glm::ivec2 GetSize() { return m_LevelSize; }
	bool HasDoor() { return m_HasDoor; }
	bool IsDoorMirrored() { return m_DoorMirrored; }
	glm::ivec2 GetDoorPosition() { return m_DoorPosition; }
	std::vector<glm::ivec2> GetLeverPositions() { return m_LeverPositions; }
	std::vector<glm::ivec2> GetDispenserRightPositions() { return m_DispenserRightPositions; }
	std::vector<glm::ivec2> GetDispenserLeftPositions() { return m_DispenserLeftPositions; }
	std::vector<glm::ivec2> GetJumppadPositions() { return m_JumppadPositions; }
	std::vector<glm::ivec2> GetPresuarPlatePositions() { return m_PresuarPlatePositions; }
	std::vector<glm::ivec2> GetTrapPositions() { return m_TrapPositions; }
private:
	std::vector<glm::ivec2> m_Lights;
	std::vector<Platform> m_Platforms;
	std::vector<Platform> m_BorderColliders;
	glm::ivec2 m_PlayStartPosition;
	glm::ivec2 m_EndPosition;
	std::vector<u32> m_Background;
	glm::ivec2 m_LevelSize;
	bool m_HasDoor;
	bool m_DoorMirrored;
	glm::ivec2 m_DoorPosition;
	std::vector<glm::ivec2> m_LeverPositions;
	std::vector<glm::ivec2> m_DispenserRightPositions;
	std::vector<glm::ivec2> m_DispenserLeftPositions;
	std::vector<glm::ivec2> m_JumppadPositions;
	std::vector<glm::ivec2> m_PresuarPlatePositions;
	std::vector<glm::ivec2> m_TrapPositions;
};