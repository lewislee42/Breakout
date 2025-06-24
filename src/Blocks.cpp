

#include "Blocks.hpp"
#include "Player.hpp"
#include <iostream>

void	InitWallOfBlocks(entt::registry &registry, Vector2 screenSize) {
	// 24 pixels used for border already
	// 1 or 2 pixel gap?
	int blockWidth = 16;
	int blockHeight = 12;
	int blockGap = 2;
	int numOfRows = ((screenSize.y / 2) - 12) / (blockHeight + blockGap);
	int numOfColumns = (screenSize.x - 24) / (blockWidth + blockGap);
	int remainingSpace = int(screenSize.x - 24) % (blockWidth + blockGap);
	Vector2	startPos{-(float(screenSize.x) / 2) + 12 + blockGap + (remainingSpace / 2), -(float(screenSize.y) / 2) + 12 + blockGap};
	std::cout << "rows: " << numOfRows << std::endl;
	std::cout << "columns: " << numOfColumns << std::endl;
	for (int i = numOfRows - 1; i >= 0; i--) {
		if (i % 2 == 0)
			startPos.x = -(float(screenSize.x) / 2) + 12 + blockGap;
		else
			startPos.x = -(float(screenSize.x) / 2) + 12 + blockGap + (blockWidth / 2);

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

void	HandleBlockHit(entt::registry &registry) {
	auto view = registry.view<Block, BlockHitTag>();

	for (auto entity : view) {
		Block &block = registry.get<Block>(entity);
		block.health -= 1;
		if (block.health == 0) {
			auto view = registry.view<PlayerTag>();
			entt::entity player = *view.begin();
			int &points = registry.get<Points>(player).points;
			points += block.points;
			registry.destroy(entity);
		}
	}
}
