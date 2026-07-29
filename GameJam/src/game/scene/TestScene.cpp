#include "TestScene.h"

constexpr f32 tileSize = 1.0f / 20.0f; // SCREENSHOT
constexpr f32 lightRadius = 300.0f;

TestScene::TestScene(std::vector<std::vector<Keyframe>> replays, f32 ellepsedSeconds, const std::filesystem::path& level) : m_Path(level), m_Level(LevelData(level)), m_Replays(replays) {
	m_Physics = new Physics();

	m_Player = new Player(m_Physics, glm::vec2(m_Level.GetPlayerStartPosition()) * tileSize + glm::vec2(0.0f, 0.02f));

	for (Platform platform : m_Level.GetPlatforms()) {
		m_Physics->AddStaticCollider(StaticCollider{ glm::vec2(platform.position.x, platform.position.y + 0.583f) * tileSize, glm::vec2(platform.size.x, 0.417f) * tileSize, false, "platform" });
	}
	for (Platform platform : m_Level.GetBorderColliders()) {
		m_Physics->AddStaticCollider(StaticCollider{ glm::vec2(platform.position.x, platform.position.y) * tileSize, glm::vec2(1.0, platform.size.y) * tileSize, false, "border" });
	}
	m_Physics->AddStaticCollider(StaticCollider{ glm::vec2(-1.0f, 0.0f) * tileSize, glm::vec2(1.0f, m_Level.GetSize().y) * tileSize, false, "platform" });
	m_Physics->AddStaticCollider(StaticCollider{ glm::vec2(m_Level.GetSize().x, 0.0f) * tileSize, glm::vec2(1.0f, m_Level.GetSize().y) * tileSize, false, "platform" });
	m_Physics->AddStaticCollider(StaticCollider{ glm::vec2(0.0f, m_Level.GetSize().y) * tileSize, glm::vec2(m_Level.GetSize().x, 1.0f) * tileSize, false, "platform" });

	m_Physics->AddStaticCollider(StaticCollider{ glm::vec2(m_Level.GetEndPosition()) * tileSize, glm::vec2(2.0f) * tileSize, true, "goal" });

	for (glm::vec2 l : m_Level.GetLights()) {
		glm::vec3 color1 = Color::RGBFromHex(0xd98115).ToVec3();
		glm::vec3 color2 = Color::RGBFromHex(0xd95715).ToVec3();
		glm::vec3 lightColor = glm::mix(color1, color2, RandomFloat(1.0f));

		glm::vec2 lightPosition = l * tileSize;
		m_Lights.push_back(Light{ glm::vec2(l.x + 0.5f, l.y + 0.75f) * tileSize, lightColor, lightRadius });
	}

	for (glm::ivec2 l : m_Level.GetLeverPositions()) {
		m_Levers.push_back(Lever{ l, false, Animation({
			ResourceManager::Sprites["lever1"],
			ResourceManager::Sprites["lever2"],
			ResourceManager::Sprites["lever3"],
			ResourceManager::Sprites["lever4"],
			ResourceManager::Sprites["lever5"],
		}, 0.5f, false) });
		m_Physics->AddStaticCollider(StaticCollider{ glm::vec2(l.x, l.y - 0.5f) * tileSize, glm::vec2(1.0f, 1.166f) * tileSize, true, "lever" + std::to_string(m_Levers.size()) });
	}

	for (glm::ivec2 j : m_Level.GetJumppadPositions()) {
		m_Jumppads.push_back(Jumppad{ j });
		m_Physics->AddStaticCollider(StaticCollider{ glm::vec2(j) * tileSize, glm::vec2(1.0f, 0.042f) * tileSize, true, "jumppad" });
	}

	for (glm::ivec2 p : m_Level.GetPresuarPlatePositions()) {
		m_PresuarePlates.push_back(PresuarePlate{ p, false });
		m_Physics->AddStaticCollider(StaticCollider{ glm::vec2(p) * tileSize, glm::vec2(1.0f, 0.166f) * tileSize, true, "presuareplate" + std::to_string(m_PresuarePlates.size()) });
	}

	for (glm::ivec2 t : m_Level.GetTrapPositions()) {
		m_Traps.push_back(Trap{ t, false, Animation({
			ResourceManager::Sprites["trap1"],
			ResourceManager::Sprites["trap2"],
			ResourceManager::Sprites["trap3"],
			ResourceManager::Sprites["trap4"],
			ResourceManager::Sprites["trap5"],
		}, 0.2f, false) });
		m_Physics->AddStaticCollider(StaticCollider{ glm::vec2(t) * tileSize, glm::vec2(1.0f, 0.75f) * tileSize, true, "trap" + std::to_string(m_Traps.size()) });
	}

	if (m_Level.HasDoor()) {
		if (!m_Level.IsDoorMirrored()) {
			m_Door = Door{ glm::vec2(m_Level.GetDoorPosition()) * tileSize, false, Animation({
					ResourceManager::Sprites["door_gear1"],
					ResourceManager::Sprites["door_gear2"],
					ResourceManager::Sprites["door_gear1"],
					ResourceManager::Sprites["door_gear2"],
					ResourceManager::Sprites["door_gear1"],
					ResourceManager::Sprites["door_gear2"],
					ResourceManager::Sprites["door_gear1"],
					ResourceManager::Sprites["door_gear2"],
				}, 1.0f, false),
				0.0f,
				Rigidbody{ &m_Door.position, &m_Door.grounded, glm::vec2(0.416f, 3.0f) * tileSize, 500.0f, &m_Door.velocity, &m_Door.impulse, false, false, "door" },
				true,
				glm::vec2(0.0f),
				glm::vec2(0.0f),
				false
			};
			m_Physics->AddRigidbody(&m_Door.rigidbody);
		}
		else {
			m_Door = Door{ (glm::vec2(m_Level.GetDoorPosition()) + glm::vec2(0.583f, 0.0f)) * tileSize, false, Animation({
					ResourceManager::Sprites["door_gear1"],
					ResourceManager::Sprites["door_gear2"],
					ResourceManager::Sprites["door_gear1"],
					ResourceManager::Sprites["door_gear2"],
					ResourceManager::Sprites["door_gear1"],
					ResourceManager::Sprites["door_gear2"],
					ResourceManager::Sprites["door_gear1"],
					ResourceManager::Sprites["door_gear2"],
				}, 1.0f, false),
				0.0f,
				Rigidbody{ &m_Door.position, &m_Door.grounded, glm::vec2(0.416f, 3.0f) * tileSize, 500.0f, &m_Door.velocity, &m_Door.impulse, false, false, "door" },
				true,
				glm::vec2(0.0f),
				glm::vec2(0.0f),
				true
			};
			m_Physics->AddRigidbody(&m_Door.rigidbody);
		}
	}

	for (glm::ivec2 d : m_Level.GetDispenserRightPositions()) {
		m_Dispensers.push_back(Dispenser{ d, true });
	}

	for (glm::ivec2 d : m_Level.GetDispenserLeftPositions()) {
		m_Dispensers.push_back(Dispenser{ d, false });
	}

	m_KeyFrameTime = 0.0f;
	m_DebugTime = 0.0f;
	m_TimeToShot = 5.0f;
	m_LeverIdx = 0;

	m_ReplayPlayers.reserve(100);
	for (u32 i = 0; i < m_Replays.size(); i++) {
		m_ReplayPlayers.push_back(ReplayPlayer{ });
		ReplayPlayer* replayPlayer = &m_ReplayPlayers.at(m_ReplayPlayers.size() - 1);
		replayPlayer->position;
		replayPlayer->velocity;
		replayPlayer->impulse;
		replayPlayer->grounded;
		Rigidbody rigidbody = Rigidbody{ &replayPlayer->position, &replayPlayer->grounded, glm::vec2(0.05f, 0.0958f), 100.0f, &replayPlayer->velocity, &replayPlayer->impulse, false, true, "replay" + std::to_string(i)};
		replayPlayer->rigidbody = rigidbody;
		replayPlayer->replayedFrames = 0;
		m_Physics->AddRigidbody(&replayPlayer->rigidbody);
		replayPlayer->alive = true;
	}

	m_Replays.push_back({});

	if (m_Replays.size() == 1) {
		m_Timer = Timer();
	}
	else {
		m_Timer = Timer(ellepsedSeconds);
	}

	m_Dead = false;

	m_Paused = false;
}

TestScene::~TestScene() {
	delete m_Physics;
}

void TestScene::Render(Renderer* renderer) {
	renderer->ClearBackground(Color::RGBFromHex(0x101010));
	
	glm::ivec2 levelSize = m_Level.GetSize();
	std::vector<u32>& tileData = m_Level.GetTileData();

	for (u32 y = 0; y < levelSize.y; y++) {
		for (u32 x = 0; x < levelSize.x; x++) {
			u32 tile = tileData.at(x + y * levelSize.x);
			renderer->RenderSprite(glm::vec2((f32)x, (f32)y) * tileSize, glm::vec2(tileSize), ResourceManager::Tiles[tile]);
			if (tile == STONE_TILE3) {
				renderer->RenderSpriteEmitter(glm::vec2((f32)x, (f32)y) * tileSize, glm::vec2(tileSize), ResourceManager::Sprites["STONE_TILE3_EMISSIVE"], Color::RGBFromHex(0x555577));
			}
		}
	}

	for (u32 i = 0; i < m_Lights.size(); i++) {
		renderer->RenderLight(m_Lights.at(i));
		renderer->RenderSprite(m_Lights.at(i).position - glm::vec2(0.5f, 0.75f) * tileSize, glm::vec2(tileSize), ResourceManager::Sprites["tourch"]);
		renderer->RenderEmitter(m_Lights.at(i).position - glm::vec2(0.083f) * tileSize, glm::vec2(0.167f) * tileSize, Color(m_Lights.at(i).color * 0.5f));
	}

	for (Platform platform : m_Level.GetPlatforms()) {
		renderer->RenderShadowCaster(glm::vec2(platform.position.x, platform.position.y + 0.583f) * tileSize, glm::vec2(platform.size.x, 0.417f) * tileSize);
	}

	renderer->EndBackground();

	for (Lever l : m_Levers) {
		glm::vec2 pos = glm::vec2(l.position.x, l.position.y - 0.5f) * tileSize;
		glm::vec2 size = glm::vec2(1.0f, 1.166f) * tileSize;
		renderer->RenderSprite(pos, size, l.animation.GetFrame());
		if (l.triggered) {
			renderer->RenderEmitter(pos + glm::vec2(0.01f, 0.343f) * tileSize, glm::vec2(0.6f) * tileSize, Color::RGBFromHex(0x333333));
		}
	}

	for (Jumppad j : m_Jumppads) {
		renderer->RenderSprite(glm::vec2(j.position) * tileSize, glm::vec2(1.0f, 0.166f) * tileSize, ResourceManager::Sprites["jumppad"]);
		renderer->RenderEmitter((glm::vec2(j.position) + glm::vec2(0.0f, 0.05f)) * tileSize, glm::vec2(1.0f, 0.1f) * tileSize, Color::RGBFromHex(0x052003));
	}

	for (PresuarePlate p : m_PresuarePlates) {
		renderer->RenderSprite(glm::vec2(p.position) * tileSize, glm::vec2(1.0f, 0.166f) * tileSize, ResourceManager::Sprites["presuareplate"]);
		if (p.triggered) {
			renderer->RenderEmitter((glm::vec2(p.position) + glm::vec2(0.0f, 0.05f)) * tileSize, glm::vec2(1.0f, 0.1f) * tileSize, Color::RGBFromHex(0x333333));
		}
	}

	renderer->RenderSprite(glm::vec2(m_Level.GetEndPosition()) * tileSize, glm::vec2(2.0) * tileSize, ResourceManager::Sprites["lotus_flower"]);
	renderer->RenderEmitter((glm::vec2(m_Level.GetEndPosition()) + glm::vec2(0.5f, 0.2f)) * tileSize, glm::vec2(1.0, 0.7f) * tileSize, Color::RGBFromHex(0xfff253));

	for (Trap t : m_Traps) {
		renderer->RenderSprite(glm::vec2(t.position) * tileSize, glm::vec2(1.0f, 1.0f) * tileSize, t.animation.GetFrame());
	}

	if (m_Level.HasDoor()) {
		if (!m_Door.mirrored) {
			renderer->RenderSprite(m_Door.position, glm::vec2(0.416f, 3.0f) * tileSize, ResourceManager::Sprites["door"]);
			renderer->RenderSprite(glm::vec2(m_Door.position) + glm::vec2(0.25f, 2.333f - m_Door.yOffset) * tileSize, glm::vec2(1.0f) * tileSize, m_Door.animation.GetFrame());
		}
		else {
			renderer->RenderSprite(m_Door.position, glm::vec2(0.416f, 3.0f) * tileSize, ResourceManager::Sprites["door_mirrored"]);
			renderer->RenderSprite(glm::vec2(m_Door.position) + glm::vec2(-0.833f, 2.333f - m_Door.yOffset) * tileSize, glm::vec2(1.0f) * tileSize, m_Door.animation.GetFrame());
		}
	}

	for (Dispenser d : m_Dispensers) {
		if (d.right) {
			renderer->RenderSprite(glm::vec2(d.position) * tileSize, glm::vec2(1.0f) * tileSize, ResourceManager::Sprites["dispenser_right"]);
		}
		else {
			renderer->RenderSprite(glm::vec2(d.position) * tileSize, glm::vec2(1.0f) * tileSize, ResourceManager::Sprites["dispenser_left"]);
		}
	}

	for (Arrow arrow : m_Arrows) {
		if (arrow.right) {
			renderer->RenderSprite(arrow.position, glm::vec2(0.5f, 0.292f) * tileSize, ResourceManager::Sprites["arrow_right"]);
		}
		else {
			renderer->RenderSprite(arrow.position, glm::vec2(0.5f, 0.292f) * tileSize, ResourceManager::Sprites["arrow_left"]);
		}
	}

	// SCREENSHOT
	m_Player->Render(renderer);

	for (u32 i = 0; i < m_ReplayPlayers.size(); i++) {
		ReplayPlayer& replayPlayer = m_ReplayPlayers.at(i);
		if (!replayPlayer.alive) continue;
		std::vector<Keyframe>& replay = m_Replays.at(i);
		if (replayPlayer.replayedFrames + 1 >= replay.size()) {
			replayPlayer.alive = false;
			continue;
		}
		Keyframe frame = replay.at(replayPlayer.replayedFrames);
		Sprite animFrame = frame.frame;
		animFrame.x += 96;
		replayPlayer.position = frame.position;
		renderer->RenderSprite(glm::vec2(frame.position.x - (animFrame.width - 12) / 24.0f * 0.05f, frame.position.y), glm::vec2(0.05f, 0.0958f) + glm::vec2((animFrame.width - 12) / 12.0f * 0.05f, 0.0f), animFrame);
	}

	renderer->BeginGUI();

	// SCREENSHOT
	if (m_LeverIdx != 0 && !m_Levers.at(m_LeverIdx - 1).triggered) {
		std::string text = "PRESS E TO PULL LEVER";
		renderer->RenderText(text, glm::vec2(0.5f - renderer->GetTextWidth(text, 0.003f) / 2.0f, 0.05f), 0.003f, Color::RGBFromHex(0x666666));
	}
	std::string time = m_Timer.ToText();
	renderer->RenderText(time, glm::vec2(0.05f, 0.5f), 0.001f, Color::RGBFromHex(0x666666));

	if (m_Paused) {
		renderer->RenderRectangle(glm::vec2(0.3f, 0.08f), glm::vec2(0.4f), Color::RGBFromHex(0xBBBBBB));
		renderer->RenderRectangle(glm::vec2(0.305f, 0.085f), glm::vec2(0.39f), Color::RGBFromHex(0x151515));

		std::string button1 = "RESUME";
		if (m_Cursor == 0) {
			renderer->RenderText(button1, glm::vec2(0.5f - renderer->GetTextWidth(button1, 0.003f) / 2.0f, 0.35f), 0.003f, Color::RGBFromHex(0xBBBBBB));
		}
		else {
			renderer->RenderText(button1, glm::vec2(0.5f - renderer->GetTextWidth(button1, 0.003f) / 2.0f, 0.35f), 0.003f, Color::RGBFromHex(0x777777));
		}
		std::string button2 = "RESTART";
		if (m_Cursor == 1) {
			renderer->RenderText(button2, glm::vec2(0.5f - renderer->GetTextWidth(button2, 0.003f) / 2.0f, 0.25f), 0.003f, Color::RGBFromHex(0xBBBBBB));
		}
		else {
			renderer->RenderText(button2, glm::vec2(0.5f - renderer->GetTextWidth(button2, 0.003f) / 2.0f, 0.25f), 0.003f, Color::RGBFromHex(0x777777));
		}
		std::string button3 = "MENU";
		if (m_Cursor == 2) {
			renderer->RenderText(button3, glm::vec2(0.5f - renderer->GetTextWidth(button3, 0.003f) / 2.0f, 0.15f), 0.003f, Color::RGBFromHex(0xBBBBBB));
		}
		else {
			renderer->RenderText(button3, glm::vec2(0.5f - renderer->GetTextWidth(button3, 0.003f) / 2.0f, 0.15f), 0.003f, Color::RGBFromHex(0x777777));
		}
	}
}

void TestScene::Update(f32 dt) {
	if (!m_Paused) {
		m_Player->Update(dt);

		m_Physics->Update(dt);

		for (PresuarePlate& p : m_PresuarePlates) {
			p.triggered = false;
		}

		for (u32 i = 0; i < m_ReplayPlayers.size(); i++) {
			ReplayPlayer& replayPlayer = m_ReplayPlayers.at(i);
			if (!replayPlayer.alive) continue;
			std::vector<Keyframe>& replay = m_Replays.at(i);
			while (replayPlayer.replayedFrames + 1 < replay.size() && replay.at(replayPlayer.replayedFrames + 1).ellapsedTime < m_EllapsedTime) {
				replayPlayer.replayedFrames++;
			}
			Keyframe frame = replay.at(replayPlayer.replayedFrames);

			if (frame.leverIdx != 0) {
				if (!m_Levers.at(frame.leverIdx - 1).triggered) {
					m_Levers.at(frame.leverIdx - 1).triggered = true;
					m_Levers.at(frame.leverIdx - 1).animation.Start();
				}
			}
			if (frame.presuarePlateIdx != 0) {
				m_PresuarePlates.at(frame.presuarePlateIdx - 1).triggered = true;
			}
			if (frame.trapIdx != 0) {
				if (!m_Traps.at(frame.trapIdx - 1).triggered) {
					m_Traps.at(frame.trapIdx - 1).triggered = true;
					m_Traps.at(frame.trapIdx - 1).animation.Start();
				}
			}
		}

		m_LeverIdx = 0;
		std::vector<TriggerCollision> triggers = m_Physics->GetTriggerCollisions();
		for (TriggerCollision t : triggers) {
			if (t.bodyA == "player") {
				//std::cout << t.bodyA << ", " << t.bodyB << std::endl;
				if (t.bodyB.starts_with("lever")) {
					m_LeverIdx = NumberAtEndOfString(t.bodyB);
				}
				else if (t.bodyB.starts_with("presuareplate")) {
					u32 idx = NumberAtEndOfString(t.bodyB) - 1;
					m_PresuarePlates.at(idx).triggered = true;
					m_PresuarePlateIdxReplay = idx + 1;
				}
				else if (t.bodyB.starts_with("trap")) {
					u32 idx = NumberAtEndOfString(t.bodyB) - 1;
					if (!m_Traps.at(idx).triggered) {
						m_Traps.at(idx).triggered = true;
						m_Traps.at(idx).animation.Start();
						m_TrapIdxReplay = idx + 1;

						m_Dead = true;
					}
				}
				else if (t.bodyB == "arrow") {
					m_Dead = true;
				}
				else if (t.bodyB == "goal") {
					m_Timer.Stop();
					std::string levelStr = m_Path.string();
					u32 levelIdx = levelStr.at(levelStr.size() - 5) - '0';
					Game::Get().GetConfig().levelsCompleted = levelIdx;
					Game::Get().GetConfig().times.at(levelIdx - 1) = m_Timer.ElapsedSeconds();
					Game::Get().TransitionScene(new MenuScene(), 1.5f);
				}
			}
			//else if (t.bodyA.starts_with("replay")) {
			//	if (t.bodyB.starts_with("lever")) {
			//		std::cout << "lever" << std::endl;
			//		m_LeverIdx = NumberAtEndOfString(t.bodyB) - 1;
			//		u32 replayIdx = NumberAtEndOfString(t.bodyA);
			//		if (m_Replays.at(replayIdx).at(m_ReplayPlayers.at(replayIdx).replayedFrames).pressedE) {
			//			m_Levers.at(m_LeverIdx).triggered = true;
			//			m_Levers.at(m_LeverIdx).animation.Start();
			//		}
			//	}
			//	else if (t.bodyB.starts_with("presuareplate")) {
			//		u32 idx = NumberAtEndOfString(t.bodyB) - 1;
			//		m_PresuarePlates.at(idx).triggered = true;
			//	}
			//	else if (t.bodyB.starts_with("trap")) {
			//		u32 idx = NumberAtEndOfString(t.bodyB) - 1;
			//		m_Traps.at(idx).triggered = true;
			//		m_Traps.at(idx).animation.Start();
			//	}
			//}
		}

		if (m_LeverIdx != 0 && Input::IsKeyPressed(GLFW_KEY_E)) {
			if (!m_Levers.at(m_LeverIdx - 1).triggered) {
				m_Levers.at(m_LeverIdx - 1).triggered = true;
				m_Levers.at(m_LeverIdx - 1).animation.Start();
				m_LeverIdxReplay = m_LeverIdx;
			}
		}

		bool doorOpen = true;
		for (PresuarePlate p : m_PresuarePlates) {
			doorOpen = doorOpen && p.triggered;
		}
		for (Lever l : m_Levers) {
			doorOpen = doorOpen && l.triggered;
		}
		if (doorOpen && !m_Door.opened) {
			m_Door.opened = true;
			m_Door.animation.Start();
		}

		if (m_Door.opened && m_Door.yOffset < 2.1f) {
			m_Door.yOffset += 2.0f * dt;
			m_Door.position.y += 2.0f * dt * tileSize;
		}

		if (m_TimeToShot > 0.0f) m_TimeToShot -= dt;
		if (m_TimeToShot < 0.0f) {
			m_Arrows.clear();
			m_Arrows.reserve(m_Dispensers.size());
			m_Physics->RemoveRigidbodysByName("arrow");

			for (Dispenser d : m_Dispensers) {
				m_Arrows.push_back(Arrow{ });
				Arrow* arrow = &m_Arrows.at(m_Arrows.size() - 1);
				glm::vec2 pos = glm::vec2(d.position) * tileSize;
				if (d.right) {
					pos += glm::vec2(1.0f, 0.5f - 0.146f) * tileSize;
				}
				else {
					pos += glm::vec2(-0.5f, 0.5f - 0.146f) * tileSize;
				}
				arrow->position = pos;
				arrow->right = d.right;
				arrow->grounded = false;
				arrow->velocity = glm::vec2(0.0f);
				arrow->impulse = glm::vec2(0.0f);
				Rigidbody rigidbody = Rigidbody{ &arrow->position, &arrow->grounded, glm::vec2(0.5f, 0.292f) * tileSize, 1.0f, &arrow->velocity, &arrow->impulse, false, true, "arrow" };
				arrow->rigidbody = rigidbody;
				m_Physics->AddRigidbody(&arrow->rigidbody);
			}

			m_TimeToShot += 5.0f;
		}

		for (Arrow& arrow : m_Arrows) {
			if (arrow.right) {
				arrow.position.x += 0.5f * dt;
			}
			else {
				arrow.position.x -= 0.5f * dt;
			}
		}

		m_EllapsedTime += dt;

		if (m_Dead) {
			Replay();
		}

		if (m_KeyFrameTime + dt < 1.0f / 60.0f) {
			m_KeyFrameTime += dt;
		}
		else if (m_DebugCounter < 60) {
			m_KeyFrameTime += dt - 1.0f / 60.0f;
			m_Replays.at(m_Replays.size() - 1).push_back(Keyframe{ m_Player->GetPosition(), m_Player->GetAnimationFrame(), m_EllapsedTime, m_LeverIdxReplay, m_TrapIdxReplay, m_PresuarePlateIdxReplay });
			m_LeverIdxReplay = 0;
			m_TrapIdxReplay = 0;
			m_PresuarePlateIdxReplay = 0;
			m_DebugCounter++;
		}

		if (m_DebugTime + dt < 1.0f) {
			m_DebugTime += dt;
		}
		else {
			m_DebugTime += dt - 1.0f;
			m_DebugCounter = 0;
		}

		if (!m_Timer.IsRunning()) {
			m_Timer.Start();
		}
	}

	if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
		m_Paused = true;
		m_Timer.Stop();
	}

	if (m_Paused) {
		if (Input::IsKeyPressed(GLFW_KEY_UP)) {
			m_Cursor = std::max(0, m_Cursor - 1);
		}
		if (Input::IsKeyPressed(GLFW_KEY_DOWN)) {
			m_Cursor = std::min(2, m_Cursor + 1);
		}
		if (Input::IsKeyDown(GLFW_KEY_ENTER)) {
			if (m_Cursor == 0) {
				m_Paused = false;
				m_Timer = Timer(m_Timer.ElapsedSeconds());
			}
			else if (m_Cursor == 1) {
				Game::Get().TransitionScene(new TestScene({ }, 0.0f, m_Path), 1.0f);
			}
			else if (m_Cursor == 2) {
				Game::Get().TransitionScene(new MenuScene(), 1.5f);
			}
		}
	}
}

void TestScene::Replay() {
	m_Replays.at(m_Replays.size() - 1).push_back(Keyframe{ m_Player->GetPosition(), m_Player->GetAnimationFrame(), m_EllapsedTime, m_LeverIdxReplay, m_TrapIdxReplay, m_PresuarePlateIdxReplay });
	Game::Get().TransitionScene(new TestScene(m_Replays, m_Timer.ElapsedSeconds(), m_Path), 0.75f);
}
