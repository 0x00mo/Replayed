#include "LevelData.h"

LevelData::LevelData(const std::filesystem::path& path) {
	Texture texture = Texture(path);
	m_LevelSize = glm::ivec2(texture.GetWidth(), texture.GetHeight());

	u8* data = texture.GetData();
	std::vector<u32> colorData;

	for (i32 y = 0; y < m_LevelSize.y; y++) {
		for (i32 x = 0; x < m_LevelSize.x; x++) {
			u32 idx = x + y * m_LevelSize.x;
			u32 r = (u32)data[idx * 4];
			u32 g = (u32)data[idx * 4 + 1];
			u32 b = (u32)data[idx * 4 + 2];
			colorData.push_back((r << 16) | (g << 8) | b);
		}
	}

	for (i32 y = 0; y < m_LevelSize.y; y++) {
		for (i32 x = 0; x < m_LevelSize.x; x++) {
			u32 c = colorData.at(x + y * m_LevelSize.x);

			if (c == STONE_COLOR) {
				u32 rand = RandomInt(2);
				if (rand == 0) {
					m_Background.push_back(STONE_TILE1);
				} else if (rand == 1) {
					m_Background.push_back(STONE_TILE2);
				}
				else {
					m_Background.push_back(STONE_TILE3);
				}
			} 
			else if (c == CRACKED_STONE_COLOR) {
				u32 rand = RandomInt(2);
				if (rand == 0) {
					m_Background.push_back(CRACKED_STONE_TILE1);
				}
				else if (rand == 1) {
					m_Background.push_back(CRACKED_STONE_TILE2);
				}
				else {
					m_Background.push_back(CRACKED_STONE_TILE3);
				}
			}
			else if (c == VINE_COLOR1) {
				u32 rand = RandomInt(2);
				if (rand == 0) {
					m_Background.push_back(VINE_TILE1);
				}
				else if (rand == 1) {
					m_Background.push_back(VINE_TILE2);
				}
				else {
					m_Background.push_back(VINE_TILE3);
				}
			}
			else if (c == VINE_COLOR2) {
				m_Background.push_back(VINE_END_TILE);
			}
			else if (c == PLANK_COLOR) {
				bool hasLeftPlank = true;
				bool hasRightPlank = true;
				if (x - 1 >= 0) {
					u32 leftColor = colorData.at(x - 1 + y * m_LevelSize.x);
					if (leftColor != PLANK_COLOR) {
						hasLeftPlank = false;
					}
				}
				else {
					hasLeftPlank = false;
				}
				if (x + 1 < m_LevelSize.x) {
					u32 rightColor = colorData.at(x + 1 + y * m_LevelSize.x);
					if (rightColor != PLANK_COLOR) {
						hasLeftPlank = false;
					}
				}
				else {
					hasLeftPlank = false;
				}
				if (!hasLeftPlank && !hasRightPlank) {
					m_Background.push_back(PLANK_LOG_TILE3);
				}
				else if (!hasLeftPlank) {
					m_Background.push_back(PLANK_LOG_TILE2);
				}
				else if (!hasRightPlank) {
					m_Background.push_back(PLANK_LOG_TILE1);
				}
				else {
					u32 rand = RandomInt(1);
					if (rand == 0) {
						m_Background.push_back(PLANK_TILE1);
					}
					else if (rand == 1) {
						m_Background.push_back(PLANK_TILE2);
					}
					else {
						m_Background.push_back(PLANK_TILE3);
					}
				}
			}
			else if (c == LIGHT_COLOR) {
				m_Lights.push_back(glm::ivec2(x, y));
				m_Background.push_back(STONE_TILE1);
			}
			else if (c == PLAYER_COLOR) {
				m_PlayStartPosition = glm::ivec2(x, y);
				m_Background.push_back(STONE_TILE2);
			}
			else if (c == END_COLOR) {
				m_EndPosition = glm::ivec2(x, y);
				m_Background.push_back(STONE_TILE3);
			}
			else if (c == LEVER_COLOR) {
				m_LeverPositions.push_back(glm::ivec2(x, y));
				m_Background.push_back(STONE_TILE1);
			}
			else if (c == DOOR_COLOR) {
				m_DoorPosition = glm::ivec2(x, y);
				m_HasDoor = true;
				m_DoorMirrored = false;
				m_Background.push_back(STONE_TILE2);
			}
			else if (c == DOOR_MIRORED_COLOR) {
				m_DoorPosition = glm::ivec2(x, y);
				m_HasDoor = true;
				m_DoorMirrored = true;
				m_Background.push_back(STONE_TILE2);
			}
			else if (c == DISPENSER_RIGHT_COLOR) {
				m_DispenserRightPositions.push_back(glm::ivec2(x, y));
				m_Background.push_back(STONE_TILE1);
			}
			else if (c == DISPENSER_LEFT_COLOR) {
				m_DispenserLeftPositions.push_back(glm::ivec2(x, y));
				m_Background.push_back(STONE_TILE1);
			}
			else if (c == JUMPPAD_COLOR) {
				m_JumppadPositions.push_back(glm::ivec2(x, y));
				m_Background.push_back(STONE_TILE2);
			}
			else if (c == PRESEAR_PLAT_COLOR) {
				m_PresuarPlatePositions.push_back(glm::ivec2(x, y));
				m_Background.push_back(STONE_TILE2);
			}
			else if (c == TRAP_COLOR) {
				m_TrapPositions.push_back(glm::ivec2(x, y));
				m_Background.push_back(STONE_TILE3);
			}
			else if (c == BORDER_COLOR) {
				m_Background.push_back(BORDER_TILE);
			}
			else {
				assert(false && "UNREACHABLE");
			}
		}
	}

	bool isPlatform = false;
	glm::ivec2 platformStart;
	for (i32 y = 0; y < m_LevelSize.y; y++) {
		if (isPlatform) {
			isPlatform = false;
			m_Platforms.push_back(Platform{ platformStart, glm::ivec2(m_LevelSize.x, y - 1) - platformStart });
		}
		for (i32 x = 0; x < m_LevelSize.x; x++) {
			u32 c = colorData.at(x + y * m_LevelSize.x);

			if (isPlatform) {
				if (c != PLANK_COLOR) {
					isPlatform = false;
					m_Platforms.push_back(Platform{ platformStart, glm::ivec2(x, y) - platformStart });
				}
			}
			else if (c == PLANK_COLOR) {
				isPlatform = true;
				platformStart = glm::ivec2(x, y);
			}
		}
	}

	bool isBorderCollider = false;
	glm::ivec2 colliderStart;
	for (i32 x = 0; x < m_LevelSize.x; x++) {
		if (isBorderCollider) {
			isBorderCollider = false;
			m_BorderColliders.push_back(Platform{ colliderStart, glm::ivec2(x - 1, m_LevelSize.y) - colliderStart });
		}
		for (i32 y = 0; y < m_LevelSize.y; y++) {
			u32 c = colorData.at(x + y * m_LevelSize.x);

			if (isBorderCollider) {
				if (c != BORDER_COLOR) {
					isBorderCollider = false;
					m_BorderColliders.push_back(Platform{ colliderStart, glm::ivec2(x, y) - colliderStart });
				}
			}
			else if (c == BORDER_COLOR) {
				isBorderCollider = true;
				colliderStart = glm::ivec2(x, y);
			}
		}
	}
}
