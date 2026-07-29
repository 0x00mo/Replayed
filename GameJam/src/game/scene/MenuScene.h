#pragma once
#include "Scene.h"
#include "TestScene.h"
#include "HowToPlayScene.h"
#include "../Game.h"
#include "../../core/Input.h"

class MenuScene : public Scene {
public:
	MenuScene();

	void Render(Renderer* renderer) override;
	void Update(f32 dt) override;
private:
	i32 m_Cursor;
	std::vector<Texture> m_Textures;
	std::vector<glm::vec2> m_TexturePosition;
	std::vector<glm::vec2> m_TextureScales;
};