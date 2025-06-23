
#include "raylib.h"

int main() {
	InitWindow(400, 400, "Breakout");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(WHITE);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
