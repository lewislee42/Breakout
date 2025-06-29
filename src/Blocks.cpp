

#include "Blocks.hpp"

#include "Player.hpp"
#include "Buffs/Buffs.hpp"

#include <vector>

const std::vector level0 = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  
};


const std::vector level1 = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 
	2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 
	2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 
};

const std::vector level2 = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	2, 2, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 2, 2, 2, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 
};


const std::vector<int> &GetCurrentLevel(int currentLevelIndex) {
	switch (currentLevelIndex) {
		case 0:
			return level0;
		case 1:
			return level1;
		case 2:
			return level2;
		default:
			return level0;
	}
}

/* ------------ INIT FUNCTION ------------ */
void	InitBlock(entt::registry &registry, Vector2 pos, Vector2 posOnMap, int points, int type) {

	entt::entity block = registry.create();

	registry.emplace<Block>(block, (BlockType)type);
	registry.emplace<Points>(block, points);
	registry.emplace<Dimensions>(block, Rectangle{pos.x, pos.y, BLOCK_DEFAULT_WIDTH, float(BLOCK_DEFAULT_HEIGHT)});
	registry.emplace<Position>(block, Vector2{pos.x, pos.y});

	if (type == 1)
		registry.emplace<Health>(block, 1);
	else if (type == 2)
		registry.emplace<Health>(block, 2);
}

void	InitWallOfBlocks(entt::registry &registry, ScreenData screenData, int currentLevelIndex) { // need to add current level here
	const std::vector<int> &currentLevel = GetCurrentLevel(currentLevelIndex);
	int blockGap = 2;
	int numOfRows = 10;
	int numOfColumns = 19;
	Vector2	startPos{
		screenData.topLeft.x + 12 + blockGap,
		screenData.topLeft.y + 12 + blockGap + ((numOfRows-1) * (BLOCK_DEFAULT_HEIGHT + blockGap))
	};
	for (int i = numOfRows - 1; i >= 0; i--) {
		if (i % 2 == 0)
			startPos.x = -(screenData.screenSize.x / 2) + 12 + blockGap;
		else
			startPos.x = -(screenData.screenSize.x / 2) + 12 + blockGap + (BLOCK_DEFAULT_WIDTH / 2.0f);

		for (int j = 0; j < numOfColumns; j++) {

			if (currentLevel[(numOfColumns * i) + j] == 1)
				InitBlock(registry, startPos, Vector2{(float)j, (float)i}, 1 + ((numOfRows - 1 - i) * 2 ), 1);
			else if (currentLevel[(numOfColumns * i) + j] == 2)
				InitBlock(registry, startPos, Vector2{(float)j, (float)i}, 1 + ((numOfRows - 1 - i) * 2 ), 2);
			startPos.x += BLOCK_DEFAULT_WIDTH + blockGap;
		}
		startPos.y -= BLOCK_DEFAULT_HEIGHT + blockGap;
	}
}


/* ------------ HELPER ------------ */
void	RemoveBlocks(entt::registry &registry) {
	auto view = registry.view<Block>();

	for (auto entity : view) {
		registry.destroy(entity);
	}
}


/* ------------ SYSTEM ------------ */
void	UpdateBlocksSystem(entt::registry &registry, entt::entity player) {
	auto view = registry.view<Block, BlockHitTag, Position, Dimensions, Points, Health>();

	if (!registry.valid(player) || !registry.all_of<PlayerTag, CurrentPoints>(player)) {
		std::cerr << "Error: invalid entity/missing component in BlockHitSystem" << std::endl;
		return ;
	}

	for (auto entity : view) {
		int &points = registry.get<Points>(entity).points;
		int &health = registry.get<Health>(entity).health;

		health -= 1;
		if (health == 0) {
			// get player and award points
			int &playerPoints = registry.get<CurrentPoints>(player).points;
			playerPoints += points;

			Vector2 position = registry.get<Position>(entity).position;
			Rectangle dimensions = registry.get<Dimensions>(entity).dimensions;
			position.x += dimensions.width / 2;
			if (float(rand()) / RAND_MAX < 0.2)
				SpawnBuffs(registry, position);

			registry.destroy(entity);
			continue;
		}
		registry.remove<BlockHitTag>(entity);
	}
}
