#pragma once
#include "Scene.h"
#include "MenuScene.h"
#include "../Game.h"

class HowToPlayScene : public Scene {
public:
	void Render(Renderer* renderer) override;
	void Update(f32 dt) override;
};