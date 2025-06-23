

#include "Game.hpp"
#include "Player.hpp"
#include "raylib.h"
#include <string> 

Game::Game(int screenWidth, int screenHeight): screenWidth(screenWidth), screenHeight(screenHeight) {

}

Game::~Game() {

}

void	RenderUI(entt::registry &registry) {
	auto view = registry.view<PlayerTag, Position, Velocity>();
	for (auto &entity : view) {
		Vector2 position = registry.get<Position>(entity).position;

		std::string positionSTR;
		positionSTR = "Position: X: " + std::to_string(position.x) + ", Y: " + std::to_string(position.y);
		DrawText(positionSTR.c_str(), 10, 10, 10, WHITE);
	}
}

void	Game::Run() {

	InitPlayer(registry, Vector2{0}, Vector2{float(screenWidth) / 2, float(screenHeight) * 0.9f});

	while (!WindowShouldClose()) {
		auto view = registry.view<PlayerTag>();
		entt::entity playerEntt = *view.begin();
		Camera2D cam = registry.get<CameraComponent>(playerEntt).camera;
		float deltaTime = GetFrameTime();

		HandlePlayerMovement(registry, deltaTime, screenWidth, screenHeight);

		BeginDrawing();
		ClearBackground(BLACK);

		BeginMode2D(cam);

		Vector2	position = registry.get<Position>(playerEntt).position;
		DrawRectangle(position.x, position.y, 15, 15, WHITE);

		EndMode2D();

		RenderUI(registry);
		EndDrawing();
	}
}
