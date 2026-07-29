#include "HowToPlayScene.h"

void HowToPlayScene::Render(Renderer* renderer) {
	static std::vector<std::string> text = {
		"IN EVERY LEVEL THERE IS A DOOR YOU",
		"NEED TO OPEN TO GET TO THE GOAL.",
		"THE DOOR OPENS WHEN EVERY LEVER IS PULLED",
		"AND ALL PRESSURE PLATES ARE PRESSED",
		"AT THE SAME MOMENT. IF YOU DIE DUE TO TRAPS", 
		"YOU RESPAWN AND PLAY THE LEVEL AGAIN",
		"BUT AT THE SAME TIME A GHOST VERSION REPLAYS", 
		"WHAT YOU ALREADY DID.",
		"THERE ARE ALSO JUMPPADS WHICH BOOST YOU.",
		"YOU HAVE TO COMBINATE THE GHOSTS TO FIND AND TRIGGER",
		"ALL LEVERS AND PRESSURE PLATES AS FAST AS POSSIBLE.",
		"CONTROLLS:",
		"A: MOVE LEFT",
		"D: MOVE RIGHT",
		"SPACE: JUMP",
		"E: PULL LEVER",
		"ESC: BACK TO MENU AND PAUSE",
	};

	renderer->BeginGUI();
	renderer->ClearBackground(Color::RGBFromHex(0x151515));

	f32 yPos = 0.5f;
	for (std::string s : text) {
		renderer->RenderText(s, glm::vec2(0.5f - renderer->GetTextWidth(s, 0.002f) / 2.0f, yPos), 0.002f, Color::RGBFromHex(0xBBBBBB));
		yPos -= 0.03f;
	}
}

void HowToPlayScene::Update(f32 dt) {
	if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
		Game::Get().TransitionScene(new MenuScene(), 1.0f);
	}
}
