

#include "Game.hpp"
#include "Ball.hpp"

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
	entt::entity ball = InitBall(registry, Vector2{0}, Vector2{0});
	entt::entity player = InitPlayer(
		registry,
		Vector2{(screenData.playableArea.x / 2) + screenData.topLeft.x, screenData.screenSize.y / 2 - 16},
		Vector2{screenData.screenSize.x / 2, screenData.screenSize.y / 2}
	);
	AttachBallToPlayer(registry, ball, player);

	InitBorder(registry, screenData);
	InitWallOfBlocks(registry, screenData);


	while (!WindowShouldClose()) {
		Camera2D cam = registry.get<CameraComponent>(player).camera;
		float deltaTime = GetFrameTime();

		PlayerHaveBallSystem(registry, player);
		PlayerMovementSystem(registry, deltaTime, player, screenData);

		UpdateBallSystem(registry, deltaTime, screenData.screenSize.y);

		UpdateBlocksSystem(registry, player);

		UpdateBuffObjectsSystem(registry, deltaTime, player, screenData.screenSize.y);
		UpdateBuffsSystem(registry, deltaTime, player, screenData);


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
