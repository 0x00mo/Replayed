#pragma once
#include "Scene.h"
#include "MenuScene.h"
#include "../Game.h"
#include "../Physics.h"
#include "../LevelData.h"
#include "../gameobjects/Player.h"
#include "../../core/Timer.h"

typedef struct {
	glm::ivec2 position;
	bool triggered;
	Animation animation;
} Lever;

typedef struct {
	glm::ivec2 position;
} Jumppad;

typedef struct {
	glm::ivec2 position;
	bool triggered;
} PresuarePlate;

typedef struct {
	glm::ivec2 position;
	bool triggered;
	Animation animation;
} Trap;

typedef struct {
	glm::vec2 position;
	bool opened;
	Animation animation;
	f32 yOffset;
	Rigidbody rigidbody;
	bool grounded;
	glm::vec2 velocity;
	glm::vec2 impulse;
	bool mirrored;
} Door;

typedef struct {
	glm::ivec2 position;
	bool right;
} Dispenser;

typedef struct {
	glm::vec2 position;
	bool right;
	Rigidbody rigidbody;
	bool grounded;
	glm::vec2 velocity;
	glm::vec2 impulse;
} Arrow;

typedef struct {
	glm::vec2 position;
	Sprite frame;
	f32 ellapsedTime;
	u32 leverIdx;
	u32 trapIdx;
	u32 presuarePlateIdx;
} Keyframe;

typedef struct {
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 impulse;
	bool grounded;
	Rigidbody rigidbody;
	u32 replayedFrames;
	bool alive;
} ReplayPlayer;

class TestScene : public Scene {
public:
	TestScene(std::vector<std::vector<Keyframe>> replays, f32 ellepsedSeconds, const std::filesystem::path& level);
	~TestScene();
	void Render(Renderer* renderer) override;
	void Update(f32 dt) override;
private:
	void Replay();
private:
	Player* m_Player;
	LevelData m_Level;
	Physics* m_Physics;

	std::vector<Light> m_Lights;
	std::vector<Lever> m_Levers;
	std::vector<Jumppad> m_Jumppads;
	std::vector<PresuarePlate> m_PresuarePlates;
	std::vector<Trap> m_Traps;
	Door m_Door;
	std::vector<Dispenser> m_Dispensers;
	std::vector<Arrow> m_Arrows;

	std::vector<std::vector<Keyframe>> m_Replays;
	std::vector<ReplayPlayer> m_ReplayPlayers;
	f32 m_KeyFrameTime;
	f32 m_DebugTime;
	u32 m_DebugCounter;
	f32 m_EllapsedTime;
	u32 m_LeverIdxReplay;
	u32 m_TrapIdxReplay;
	u32 m_PresuarePlateIdxReplay;
	bool m_Dead;

	f32 m_TimeToShot;
	u32 m_LeverIdx;

	Timer m_Timer;
	std::filesystem::path m_Path;

	bool m_Paused;
	i32 m_Cursor;
};