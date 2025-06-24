

#pragma once

# include "raylib.h"
# include "entt/entt.hpp"

# include "MiscComponents.hpp"

struct Block {
	int	points;
	int	health;
};
struct BlockHitTag {};

void	InitWallOfBlocks(entt::registry &registry, Vector2 screenSize);
void	HandleBlockHit(entt::registry &registry);

