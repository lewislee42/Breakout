

#include "Render.hpp"

#include "Player.hpp"
#include "Buffs/Buffs.hpp"
#include "Blocks.hpp"
#include "Enemy.hpp"
#include <string>


void DrawHearts(Vector2 position, int lives) {

	for (int i = 0; i < lives; i++) {
		DrawRectangle(position.x + 2, position.y, 6, 4, WHITE);
		DrawRectangle(position.x + 12, position.y, 6, 4, WHITE);
		DrawRectangle(position.x, position.y + 4, 20, 4, WHITE);
		DrawRectangle(position.x + 4, position.y + 8, 12, 4, WHITE);
		DrawRectangle(position.x + 8, position.y + 12, 4, 4, WHITE);
		position.x += 32;
	}
}

void	RenderUI(entt::registry &registry, ScreenData screenData, GameState gameState, int currentLevel) {
	auto view			= registry.view<PlayerTag, Position, Velocity, CurrentPoints>();
	int	points			= registry.get<CurrentPoints>(*view.begin()).points;
	int	lives			= registry.get<Lives>(*view.begin()).lives;

	int startX = screenData.playableArea.x + 16;
	int startY = 0;
	int renderArea = screenData.screenSize.x - screenData.playableArea.x;
	startY += 16;
	DrawText("BREAKOUT", startX, startY, 32, WHITE);

	startY += 32;
	DrawText("(ARKANIOD)", startX, startY, 16, WHITE);

	startY += 32 + 12;
	DrawHearts(Vector2{(float)startX, (float)startY}, lives);

	startY += 32 + 12;
	std::string pointsSTR;
	pointsSTR = "SCORE: " + std::to_string(points);
	DrawText(pointsSTR.c_str(), startX, startY, 16, WHITE);

	if (gameState == GAME_WIN) {
		std::string gameText = "YOU WIN!";
		if (currentLevel >= 2) {
			gameText = "YOU WIN!";
			startX = (screenData.playableArea.x / 2) - 32;
			startY = screenData.screenSize.y - 86;
		}
		else {
			gameText = "GOING TO NEXT LEVEL";
			startX = (screenData.playableArea.x / 2) - 118;
			startY = screenData.screenSize.y - 86;
		}
		DrawText(gameText.c_str(), startX, startY, 20, WHITE);
	}
	else if (gameState == GAME_LOSE) {
		std::string gameText = "GAME OVER";
		startX = (screenData.playableArea.x / 2) - 44;
		startY = screenData.screenSize.y - 86;
		DrawText(gameText.c_str(), startX, startY, 20, WHITE);
	}
}

void	RenderBlocks(entt::registry &registry) {
	auto blocks = registry.view<Block, Position, Dimensions>();

	for (auto entity : blocks) {
		Vector2	position 		= registry.get<Position>(entity).position;
		Rectangle dimensions	= registry.get<Dimensions>(entity).dimensions;
		BlockType blockType		= registry.get<Block>(entity).blockType;
		
		switch (blockType) {
			case NORMAL:
				DrawRectangle(position.x, position.y, dimensions.width, dimensions.height, WHITE);
				break;
			case REINFORCED:
				DrawRectangle(position.x, position.y, dimensions.width, dimensions.height, WHITE);
				DrawRectangle(position.x + 2, position.y + 2, dimensions.width - 4, dimensions.height - 4, BLACK);
				DrawRectangle(position.x + 4, position.y + 4, dimensions.width - 8, dimensions.height - 8, WHITE);
				break;
		}
	}
}

void	RenderBuffs(entt::registry &registry) {
	auto powerUps = registry.view<Buff, Position, Dimensions>();

	for (auto &entity : powerUps) {
		BuffsEnum powerUpType = registry.get<Buff>(entity).type;
		Vector2	position 		= registry.get<Position>(entity).position;
		Rectangle dimensions	= registry.get<Dimensions>(entity).dimensions;

		DrawRectangleLines(position.x, position.y, dimensions.width, dimensions.height, WHITE);
		DrawRectangle(position.x + 2, position.y + 2, dimensions.width - 4, dimensions.height - 4, BLACK);

		std::string text;
		switch (powerUpType) {
			case ENLARGE:
				text = "E";
				break;
			case DISRUPTION:
				text = "D";
				break;
			case EXTRALIFE:
				text = "P";
				break;
			case SLOW:
				text = "S";
				break;
			case LAZERS:
				text = "L";
				break;
			default:
				break;
		}

		DrawText(text.c_str(), position.x + (dimensions.width / 2) - 3, position.y + (dimensions.height / 2) - 5, 8, WHITE);
	}
}

void	RenderEnemy(entt::registry &registry) {
	auto view = registry.view<Enemy, Dimensions>();

	for (auto &entity : view) {
		Rectangle dimensions	= registry.get<Dimensions>(entity).dimensions;

		DrawRectangle(dimensions.x + 2, dimensions.y + 2, dimensions.width - 4, dimensions.height - 4, WHITE);
		DrawRectangleLines(dimensions.x, dimensions.y, dimensions.width, dimensions.height, BLACK);
		DrawRectangle(dimensions.x + 6, dimensions.y + 6, dimensions.width - 12, dimensions.height - 12, BLACK);

		DrawRectangle(dimensions.x + dimensions.width - 1, dimensions.y + 4, 6, 10, WHITE);
		DrawRectangleLines(dimensions.x + dimensions.width - 1, dimensions.y + 4, 6, 10, BLACK);
		DrawRectangle(dimensions.x - 5, dimensions.y + 4, 6, 10, WHITE);
		DrawRectangleLines(dimensions.x - 5, dimensions.y + 4, 6, 10, BLACK);
	}
}

void	RenderWorld(entt::registry &registry) {
	auto view = registry.view<Position, Dimensions>(entt::exclude<Buff, Block, Enemy>);

	for (auto &entity : view) {
		Vector2	position 		= registry.get<Position>(entity).position;
		Rectangle dimensions	= registry.get<Dimensions>(entity).dimensions;

		DrawRectangle(position.x, position.y, dimensions.width, dimensions.height, WHITE);
	}
	
	RenderBlocks(registry);
	RenderBuffs(registry);
	RenderEnemy(registry);
}

