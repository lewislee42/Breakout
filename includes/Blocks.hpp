

#pragma once

# include "raylib.h"
# include "entt/entt.hpp"

# include "MiscComponents.hpp"
# include "Utils.hpp"

# include <cstdlib>


/* ------------ DEFAULTS ------------ */
# define BLOCK_DEFAULT_HEIGHT	24.0f
# define BLOCK_DEFAULT_WIDTH	24.0f


enum BlockType {
	NORMAL = 1,
	REINFORCED = 2
};

/* ------------ TAG ------------ */
struct BlockHitTag {};

struct Block {
	BlockType blockType;
};


/* ------------ INIT FUNCTION ------------ */
void	InitWallOfBlocks(entt::registry &registry, ScreenData screenData, int currentLevelIndex);


/* ------------ HELPER FUNCTION ------------ */
void	RemoveBlocks(entt::registry &registry);


/* ------------ SYSTEM FUNCTIOn ------------ */
void	UpdateBlocksSystem(entt::registry &registry, entt::entity player);

