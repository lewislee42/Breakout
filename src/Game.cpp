

#include "Game.hpp"
#include "raylib.h"

Game::Game(int screenWidth, int screenHeight): screenWidth(screenWidth), screenHeight(screenHeight) {

}

Game::~Game() {

}

void	RenderUI(entt::registry &registry) {
	auto view			= registry.view<PlayerTag, Position, Velocity, Points>();
	Vector2 position	= registry.get<Position>(*view.begin()).position;
	int		points		= registry.get<Points>(*view.begin()).points;

	std::string positionSTR;
	positionSTR = "Position: X: " + std::to_string(position.x) + ", Y: " + std::to_string(position.y);
	DrawText(positionSTR.c_str(), 10, 4, 8, DARKGRAY);
	std::string pointsSTR;
	pointsSTR = "Points: " + std::to_string(points);
	DrawText(pointsSTR.c_str(), 10, 12, 8, DARKGRAY);

	

	// auto view2 = registry.view<Position, Velocity>();
	// Vector2 ballPosition = registry.get<Position>(*view2.begin()).position;
	// std::string ballPositionSTR;
	// ballPositionSTR = "Ball Position: X: " + std::to_string(ballPosition.x) + ",\tY: " + std::to_string(ballPosition.y);
	// DrawText(ballPositionSTR.c_str(), 10, 12, 8, DARKGRAY);

}

void	RenderWorld(entt::registry &registry) {
	auto view = registry.view<Position, Dimensions>();

	for (auto &entity : view) {
		Vector2	position 		= registry.get<Position>(entity).position;
		Rectangle dimensions	= registry.get<Dimensions>(entity).dimensions;

		DrawRectangle(position.x, position.y, dimensions.width, dimensions.height, WHITE);
	}
}

void	InitBorder(entt::registry &registry, Vector2 screenSize) {
	entt::entity topWall = registry.create();
	entt::entity leftWall = registry.create();
	entt::entity rightWall = registry.create();
	entt::entity bottomWall = registry.create(); // not needed later
	
	float topRightX = float(screenSize.x) / 2;
	float topRightY = -(float(screenSize.y) / 2);
	float width = 12;
	registry.emplace<Dimensions>(topWall, Rectangle{-topRightX, topRightY, float(screenSize.x), width});
	registry.emplace<Position>(topWall, Vector2{-topRightX, topRightY});
	registry.emplace<Dimensions>(leftWall, Rectangle{-topRightX, topRightY, width, float(screenSize.y)});
	registry.emplace<Position>(leftWall, Vector2{-topRightX, topRightY});
	registry.emplace<Dimensions>(rightWall, Rectangle{topRightX - width, topRightY, width, float(screenSize.y)});
	registry.emplace<Position>(rightWall, Vector2{topRightX - width, topRightY});
}

void	Game::Run() {

	// some mathy math to make the center of the screen 0, 0 and the player spawns at an offset
	InitPlayer(registry, Vector2{0.0f, (float(screenHeight) / 2 * 0.9f)}, Vector2{float(screenWidth) / 2, float(screenHeight) / 2});

	Vector2 screenSize{float(screenWidth), float(screenHeight)};
	InitBall(registry, Vector2{0}, screenSize);

	InitBorder(registry, screenSize);
	InitWallOfBlocks(registry, screenSize);

	Shader shader = {0};
	shader = LoadShader(0, TextFormat("resources/crt_shader.fs", 330));
	RenderTexture2D target = LoadRenderTexture(screenSize.x, screenSize.y);


	while (!WindowShouldClose()) {
		auto view = registry.view<PlayerTag>();
		entt::entity playerEntt = *view.begin();
		Camera2D cam = registry.get<CameraComponent>(playerEntt).camera;
		float deltaTime = GetFrameTime();

		HandlePlayerMovement(registry, deltaTime, Vector2{float(screenWidth), float(screenHeight)});
		BallSystem(registry, deltaTime, Vector2{float(screenWidth), float(screenHeight)});
		HandleBlockHit(registry);

		BeginTextureMode(target);

			ClearBackground(BLACK);
			BeginMode2D(cam);
				RenderWorld(registry);
			EndMode2D();

			RenderUI(registry);
		EndTextureMode();

		BeginDrawing();
			ClearBackground(BLACK);
			BeginShaderMode(shader);
				DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height}, (Vector2){0, 0}, WHITE);
			EndShaderMode();
		EndDrawing();
	}
}
