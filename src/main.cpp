
#include "Game.hpp"

int main() {

	int screenWidth = 400;
	int screenHeight = 400;

	InitWindow(screenWidth, screenHeight, "Breakout");
	SetTargetFPS(60);
	Game game(screenWidth, screenHeight);

	game.Run();

	CloseWindow();
	return 0;
}
