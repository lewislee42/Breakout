

#pragma once

# include "raylib.h"
# include "entt/entt.hpp"

# include "MiscComponents.hpp"

# include "cstdlib"

struct Block {
	int	points;
	int	health;
};

struct BlockHitTag {};

void	InitWallOfBlocks(entt::registry &registry, ScreenData screenData);
void	HandleBlockHit(entt::registry &registry);

