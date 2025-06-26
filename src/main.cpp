
#include "Game.hpp"

int main() {

	int screenWidth = 800;
	int screenHeight = 600;

	// playable area is x = 400 y = 600

	InitWindow(screenWidth, screenHeight, "Breakout");
	SetTargetFPS(60);
	Game game(screenWidth, screenHeight);

	game.Run();

	CloseWindow();
	return 0;
}
