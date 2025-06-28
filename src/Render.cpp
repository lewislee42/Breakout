

#include "Render.hpp"

#include "Player.hpp"
#include "Buffs/Buffs.hpp"

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

void	RenderUI(entt::registry &registry, ScreenData screenData) {
	auto view			= registry.view<PlayerTag, Position, Velocity, Points>();
	int	points			= registry.get<Points>(*view.begin()).points;
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


}

void	RenderWorld(entt::registry &registry) {
	auto view = registry.view<Position, Dimensions>(entt::exclude<Buff>);

	for (auto &entity : view) {
		Vector2	position 		= registry.get<Position>(entity).position;
		Rectangle dimensions	= registry.get<Dimensions>(entity).dimensions;

		DrawRectangle(position.x, position.y, dimensions.width, dimensions.height, WHITE);
	}

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

