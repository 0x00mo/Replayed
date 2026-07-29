#include "MenuScene.h"

MenuScene::MenuScene() {
	m_Textures = {
		Texture("res/textures/lvl1.png"),
		Texture("res/textures/lvl2.png"),
		Texture("res/textures/lvl3.png"),
		Texture("res/textures/lvl4.png"),
		Texture("res/textures/lvl5.png")
	};

	m_Cursor = 0;
	m_TextureScales = {
		glm::vec2(m_Textures.at(0).GetWidth(), m_Textures.at(0).GetHeight()) * 0.00012f,
		glm::vec2(m_Textures.at(1).GetWidth(), m_Textures.at(1).GetHeight()) * 0.00012f,
		glm::vec2(m_Textures.at(2).GetWidth(), m_Textures.at(2).GetHeight()) * 0.00012f,
		glm::vec2(m_Textures.at(3).GetWidth(), m_Textures.at(3).GetHeight()) * 0.00012f,
		glm::vec2(m_Textures.at(4).GetWidth(), m_Textures.at(4).GetHeight()) * 0.00012f,
	};
	m_TexturePosition = {
		glm::vec2(0.3f - m_TextureScales.at(0).x / 2.0f, 0.25f),
		glm::vec2(0.5f - m_TextureScales.at(1).x / 2.0f, 0.28f),
		glm::vec2(0.7f - m_TextureScales.at(2).x / 2.0f, 0.06f),
		glm::vec2(0.3f - m_TextureScales.at(3).x / 2.0f, 0.02f),
		glm::vec2(0.5f - m_TextureScales.at(4).x / 2.0f, 0.02f),
	};
}

void MenuScene::Render(Renderer* renderer) {
	renderer->BeginGUI();
	renderer->ClearBackground(Color::RGBFromHex(0x151515));
	std::string title = "REPLAYED";
	renderer->RenderText(title, glm::vec2(0.5f - renderer->GetTextWidth(title, 0.007f) / 2.0f, 0.43f), 0.007f, Color::RGBFromHex(0x999999));

	for (u32 i = 0; i < 5; i++) {
		glm::vec2 position = m_TexturePosition.at(i);
		glm::vec2 scale = m_TextureScales.at(i);
		glm::vec2 boxPosition = position - scale * 0.1f;
		glm::vec2 boxScale = scale * 1.2f;
		if (m_Cursor == i) {
			renderer->RenderRectangle(boxPosition, boxScale, Color::RGBFromHex(0xBBBBBB));
			std::string text = "LEVEL " + std::to_string(i + 1);
			renderer->RenderText(text, boxPosition + glm::vec2((boxScale.x - renderer->GetTextWidth(text, 0.0015f)) / 2.0f, boxScale.y + 0.01f), 0.0015f, Color::RGBFromHex(0xBBBBBB));
			f32 time = Game::Get().GetConfig().times.at(i);
			if (time > 0.0f) {
				std::string timeStr = TimeToText(time);
				renderer->RenderText(timeStr, boxPosition + glm::vec2((boxScale.x - renderer->GetTextWidth(timeStr, 0.00075f)) / 2.0f, boxScale.y + 0.035f), 0.00075f, Color::RGBFromHex(0xBBBBBB));
			}
		}
		renderer->RenderRectangle(position, scale, Color::RGBFromHex(0x151515));
		if (i <= Game::Get().GetConfig().levelsCompleted) {
			renderer->RenderTexture(position, scale, m_Textures.at(i).GetID(), Color::RGBFromHex(0xFFFFFF));
		}
		else {
			renderer->RenderTexture(position, scale, m_Textures.at(i).GetID(), Color::RGBFromHex(0x666666));
		}
	}

	std::string text = "H: HOW TO PLAY";
	renderer->RenderText(text, glm::vec2(0.825f - renderer->GetTextWidth(text, 0.001f) / 2.0f, 0.35f), 0.001f, Color::RGBFromHex(0xBBBBBB));
}

void MenuScene::Update(f32 dt) {
	if (Input::IsKeyPressed(GLFW_KEY_LEFT)) {
		m_Cursor = std::max(0, m_Cursor - 1);
	}
	if (Input::IsKeyPressed(GLFW_KEY_RIGHT)) {
		m_Cursor = std::min((i32)std::min((u32)4, Game::Get().GetConfig().levelsCompleted), m_Cursor + 1);
	}
	if (Input::IsKeyPressed(GLFW_KEY_ENTER)) {
		Game::Get().TransitionScene(new TestScene({ }, 0.0f, "res/textures/level" + std::to_string(m_Cursor + 1) + ".png"), 1.5f);
	}
	if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
		Game::Get().Exit();
	}
	if (Input::IsKeyPressed(GLFW_KEY_H)) {
		Game::Get().TransitionScene(new HowToPlayScene(), 1.0f);
	}
}
