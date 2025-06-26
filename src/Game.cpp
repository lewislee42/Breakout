

#include "Game.hpp"
#include "PowerUps.hpp"
#include "raylib.h"

Game::Game(int screenWidth, int screenHeight):
	screenData{
		Vector2 { // topLeft
			-screenWidth / 2.0f,
			-screenHeight / 2.0f
		},
		Vector2 { // topRight
			((screenWidth / 3.0f) * 2) + (-screenWidth / 2.0f),
			-screenHeight / 2.0f
		},
		Vector2 { // screenSize
			float(screenWidth),
			float(screenHeight)
		},
		Vector2 { // playable area
			(screenWidth / 3.0f) * 2,
			float(screenHeight)
		},
		12
	},
	shader(LoadShader(0, TextFormat("resources/crt_shader.fs", 330))),
	targetTexture(LoadRenderTexture(screenWidth, screenHeight))
{
}

Game::~Game() {

}

void	RenderUI(entt::registry &registry, ScreenData screenData) {
	auto view			= registry.view<PlayerTag, Position, Velocity, Points>();
	Vector2 position	= registry.get<Position>(*view.begin()).position;
	int		points		= registry.get<Points>(*view.begin()).points;

	int startX = screenData.playableArea.x;
	int startY = 0;
	int renderArea = screenData.screenSize.x - screenData.playableArea.x;
	startY += 16;
	DrawText("BREAKOUT", startX + 16, startY, 32, WHITE);

	startY += 32;
	DrawText("(ARKANIOD)", startX + 16, startY, 16, WHITE);

	startY += 32 + 64;
	std::string pointsSTR;
	pointsSTR = "SCORE: " + std::to_string(points);
	DrawText(pointsSTR.c_str(), startX + 16, startY, 16, WHITE);

	

}

void	RenderWorld(entt::registry &registry) {
	auto view = registry.view<Position, Dimensions>(entt::exclude<PowerUpTag>);

	for (auto &entity : view) {
		Vector2	position 		= registry.get<Position>(entity).position;
		Rectangle dimensions	= registry.get<Dimensions>(entity).dimensions;

		DrawRectangle(position.x, position.y, dimensions.width, dimensions.height, WHITE);
	}

	auto powerUps = registry.view<PowerUpTag, Position, Dimensions>();

	for (auto &entity : powerUps) {
		Vector2	position 		= registry.get<Position>(entity).position;
		Rectangle dimensions	= registry.get<Dimensions>(entity).dimensions;

		DrawRectangleLines(position.x, position.y, dimensions.width, dimensions.height, WHITE);
		DrawRectangle(position.x + 2, position.y + 2, dimensions.width - 4, dimensions.height - 4, BLACK);
	}


}

void	InitBorder(entt::registry &registry, ScreenData screenData) {
	entt::entity topWall = registry.create();
	entt::entity leftWall = registry.create();
	entt::entity rightWall = registry.create();

	// Top wall initialization
	registry.emplace<Dimensions>(
		topWall,
		Rectangle{
			screenData.topLeft.x,
			screenData.topRight.y,
			screenData.playableArea.x,
			float(screenData.wallWidth), 
		}
	);
	registry.emplace<Position>(
		topWall,
		Vector2{
			screenData.topLeft.x,
			screenData.topRight.y
		}
	);

	// Left wall initialization
	registry.emplace<Dimensions>(
		leftWall,
		Rectangle{
			screenData.topLeft.x,
			screenData.topRight.y,
			float(screenData.wallWidth), 
			screenData.screenSize.y
		}
	);
	registry.emplace<Position>(
		leftWall,
		Vector2{
			screenData.topLeft.x, 
			screenData.topRight.y
		}
	);

	// Right wall initialization
	registry.emplace<Dimensions>(
		rightWall,
		Rectangle{
			screenData.topRight.x - screenData.wallWidth, 
			screenData.topRight.y,
			float(screenData.wallWidth), 
			screenData.screenSize.y
		}
	);
	registry.emplace<Position>(
		rightWall, 
		Vector2{
			screenData.topRight.x - screenData.wallWidth, 
			screenData.topRight.y
		}
	);
}

void	Game::Run() {

	// some mathy math to make the center of the screen 0, 0 and the player spawns at an offset

	entt::entity ball = InitBall(registry, Vector2{0}, screenData.screenSize);
	entt::entity player = InitPlayer(
		registry,
		Vector2{(screenData.playableArea.x / 2) + screenData.topLeft.x, screenData.screenSize.y / 2 - 16},
		Vector2{screenData.screenSize.x / 2, screenData.screenSize.y / 2},
		ball
	);

	InitBorder(registry, screenData);
	InitWallOfBlocks(registry, screenData);


	while (!WindowShouldClose()) {
		Camera2D cam = registry.get<CameraComponent>(player).camera;
		float deltaTime = GetFrameTime();

		HandlePlayerHaveBall(registry);
		HandlePlayerMovement(registry, deltaTime, screenData);
		BallSystem(registry, deltaTime);
		HandleBlockHit(registry);
		PowerUpsSystem(registry, deltaTime);

		BeginTextureMode(targetTexture);

			ClearBackground(BLACK);
			BeginMode2D(cam);
				RenderWorld(registry);
			EndMode2D();
			RenderUI(registry, screenData);

		EndTextureMode();

		BeginDrawing();
			ClearBackground(BLACK);
			BeginShaderMode(shader);
				DrawTextureRec(
					targetTexture.texture,
					(Rectangle){0, 0, (float)targetTexture.texture.width, (float)-targetTexture.texture.height},
					(Vector2){0, 0},
					WHITE
				);
			EndShaderMode();
		EndDrawing();
	}
}
