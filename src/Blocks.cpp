

#include "Blocks.hpp"

#include "Player.hpp"
#include "Buffs/Buffs.hpp"


/* ------------ INIT FUNCTION ------------ */
void	InitWallOfBlocks(entt::registry &registry, ScreenData screenData) {
	int blockWidth = 28;
	int blockHeight = 12;
	int blockGap = 2;
	int numOfRows = (screenData.playableArea.y / 2 - 12) / (blockHeight + blockGap);
	int numOfColumns = (screenData.playableArea.x - 24) / (blockWidth + blockGap);
	Vector2	startPos{screenData.topLeft.x + 12 + blockGap, screenData.topLeft.y + 12 + blockGap};
	for (int i = numOfRows - 1; i >= 0; i--) {
		if (i % 2 == 0)
			startPos.x = -(float(screenData.screenSize.x) / 2) + 12 + blockGap;
		else
			startPos.x = -(float(screenData.screenSize.x) / 2) + 12 + blockGap + (blockWidth / 2.0f);

		for (int j = 0; j < numOfColumns; j++) {
			entt::entity block = registry.create();
			registry.emplace<Block>(block, Block{1 + (i * 2), 1});
			registry.emplace<Dimensions>(block, Rectangle{startPos.x, startPos.y, float(blockWidth), float(blockHeight)});
			registry.emplace<Position>(block, Vector2{startPos.x, startPos.y});
			startPos.x += blockWidth + blockGap;
		}
		startPos.y += blockHeight + blockGap;
	}
}


/* ------------ SYSTEM ------------ */
void	UpdateBlocksSystem(entt::registry &registry, entt::entity player) {
	auto view = registry.view<Block, BlockHitTag, Position, Dimensions>();

	if (!registry.valid(player) || !registry.all_of<PlayerTag, Points>(player)) {
		std::cerr << "Error: invalid entity/missing component in BlockHitSystem" << std::endl;
		return ;
	}

	for (auto entity : view) {
		Block &block = registry.get<Block>(entity);

		block.health -= 1;
		if (block.health == 0) {
			// get player and award points
			int &points = registry.get<Points>(player).points;
			points += block.points;

			Vector2 position = registry.get<Position>(entity).position;
			Rectangle dimensions = registry.get<Dimensions>(entity).dimensions;
			position.x += dimensions.width / 2;
			// if (float(rand()) / RAND_MAX < 0.2)
				SpawnBuffs(registry, position);

			registry.destroy(entity);
		}
	}
}
