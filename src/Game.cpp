

#include "Game.hpp"
#include "Ball.hpp"
#include "Enemy.hpp"
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
	targetTexture(LoadRenderTexture(screenWidth, screenHeight)),
	gameState(GameState::MENU),
	shouldReset(false)
{
	srand(time(NULL));
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


void	Game::CheckGameState(entt::entity player) {
	auto blocks = registry.view<Block>();
	int loopCount = 0;
	for (auto entity : blocks) {
		if (loopCount > 0)
			break;
		loopCount++;
	}

	if (loopCount == 0) {
		gameState = GAME_WIN;
		return;
	}

	auto ball = registry.view<BallTag>();
	loopCount = 0;
	for (auto entity : ball) {
		if (loopCount > 0)
			break;
		loopCount++;
	}
	if (loopCount == 0) {
		if (registry.valid(player) && registry.all_of<Lives>(player) && (gameState != GAME_LOSE && gameState != GAME_RETRYING)) {
			int &lives = registry.get<Lives>(player).lives;
			lives--;
			if (lives <= 0) {
				gameState = GAME_LOSE;
				return;
			}
			else {
				gameState = GAME_RETRYING;
				return;
			}
		}
		return;
	}
}

void	Game::RunGame(entt::entity player, float deltaTime, Camera2D &cam) {

	if (gameState != GAME_LOSE && gameState != GAME_WIN && gameState != GAME_RETRYING) {
		PlayerHaveBallSystem(registry, player);
		PlayerMovementSystem(registry, deltaTime, player, screenData);

		UpdateBallSystem(registry, deltaTime, screenData.screenSize.y);

		UpdateBlocksSystem(registry, player);

		UpdateBuffObjectsSystem(registry, deltaTime, player, screenData.screenSize.y);
		UpdateBuffsSystem(registry, deltaTime, player, screenData);
		EnemySpawningSystem(registry, deltaTime, screenData);
		EnemySystem(registry, deltaTime, screenData.screenSize.y);
		EnemyCollideSystem(registry, player);
	}
	if ((gameState == GAME_LOSE || gameState == GAME_WIN || gameState == GAME_RETRYING) && shouldReset == false) {
		transitionTimer = LEVEL_CHANGE_DELAY_DURATION;
		shouldReset = true;
	}
	if ((gameState == GAME_LOSE || gameState == GAME_WIN || gameState == GAME_RETRYING) && shouldReset == true) {
		transitionTimer -= deltaTime;
		if (transitionTimer <= 0) {
			if (gameState == GAME_RETRYING) {
				shouldReset = true;
				prevGameState = GAME_RETRYING;
				gameState = TRANSITIONING;
			}
			else if (gameState == GAME_WIN) {
				shouldReset = true;
				prevGameState = GAME_WIN;
				gameState = TRANSITIONING;
				currentLevel++;
				if (currentLevel >= 3)
					gameState = MENU;
			}
			else if (gameState == GAME_LOSE) {
				prevGameState = GAME_LOSE;
				gameState = MENU;
			}
			return;
		}
	}

	// DEBUG
	if (IsKeyPressed(KEY_L)) {
		RemoveBlocks(registry);
	}

	BeginTextureMode(targetTexture);

		ClearBackground(BLACK);
		BeginMode2D(cam);
			RenderWorld(registry);
		EndMode2D();
		RenderUI(registry, screenData, gameState, currentLevel);

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
	CheckGameState(player);
}

void	Game::RunMenu(float deltaTime) {
	// look for player to press enter
	// render texts
	if (IsKeyPressed(KEY_ENTER)) {
		prevGameState = MENU;
		gameState = TRANSITIONING;
		shouldReset = true;
	
		return ;
	}

	int startX = screenData.screenSize.x / 2 - 86;
	int startY = 32;
	static float pressStartTimer;
	static bool	renderPressStart;

	pressStartTimer -= deltaTime;


	BeginTextureMode(targetTexture);

		ClearBackground(BLACK);
		startY += 16;
		DrawText("BREAKOUT", startX - 86, startY, 64, WHITE);

		startY += 64 + 32;
		DrawText("(ARKANIOD)", startX, startY, 32, WHITE);

		startY = screenData.screenSize.y - 86;
		if (pressStartTimer <= 0) {
			pressStartTimer = 0.6f;
			renderPressStart = !renderPressStart;
		}
		if (renderPressStart == true) {
			DrawText("PRESS ENTER TO START", startX - 12, startY, 16, WHITE);
		}


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

void	Game::RunTransition(entt::entity player, float deltaTime) {

	if (shouldReset == true) {
		RemoveBuffsFromPlayer(registry, player);
		RemoveBuffsObject(registry);
		RemoveEnemy(registry);
		if (prevGameState != GAME_RETRYING) {
			RemoveBlocks(registry);
			InitWallOfBlocks(registry, screenData, currentLevel);
			RemoveBalls(registry, player);
		}
		if (prevGameState == MENU) {
			ResetPlayerPointsLives(registry, player);
			currentLevel = 0;
		}

		ResetPlayerPos(registry, player, screenData);
		entt::entity ball = InitBall(registry, Vector2{0}, Vector2{0});
		AttachBallToPlayer(registry, ball, player);

		transitionTimer = TRANSITION_DURATION;
		shouldReset = false;
	}

	transitionTimer -= deltaTime;
	if (transitionTimer <= 0 ) {
		gameState = INGAME;
		return ;
	}


	int startX = screenData.screenSize.x / 2 - 32;
	int startY = screenData.screenSize.y / 2 - 12;
	std::string text = "Level " + std::to_string(currentLevel + 1);
	BeginTextureMode(targetTexture);

		ClearBackground(BLACK);
		DrawText(text.c_str(), startX, startY, 16, WHITE);

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

void	Game::Run() {
	entt::entity player = InitPlayer(registry, screenData);
	InitBorder(registry, screenData);
	float transitionDuration = 3.0f;

	while (!WindowShouldClose()) {
		Camera2D &cam = registry.get<CameraComponent>(player).camera;
		float deltaTime = GetFrameTime();

		if (gameState == MENU) {
			RunMenu(deltaTime);
		}
		else if (gameState == TRANSITIONING) {
			RunTransition(player, deltaTime);
		}
		else if (gameState == INGAME || gameState == GAME_LOSE || gameState == GAME_WIN || gameState == GAME_RETRYING) {
			RunGame(player, deltaTime, cam);
		}
	}
}
