#include "game/Game.h"
#include "game/scene/MenuScene.h"

int main() {

	Game::Init();
	Game::Get().SetScene(new MenuScene());
	Game::Get().Run();

	return 0;
}