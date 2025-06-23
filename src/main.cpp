
#include "Game.hpp"

int main() {

	int screenWidth = 400;
	int screenHeight = 400;

	Game game(screenWidth, screenHeight);
	InitWindow(screenWidth, screenHeight, "Breakout");
	SetTargetFPS(60);

	game.Run();

	CloseWindow();
	return 0;
}
