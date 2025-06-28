

#pragma once

# include "raylib.h"
# include "entt/entt.hpp"

# include "MiscComponents.hpp"
# include "Utils.hpp"

# include <cstdlib>


/* ------------ TAG ------------ */
struct BlockHitTag {};


/* ------------ COMPONENT------------ */
struct Block {
	int	points;
	int	health;
};


/* ------------ INIT FUNCTION ------------ */
void	InitWallOfBlocks(entt::registry &registry, ScreenData screenData);


/* ------------ SYSTEM FUNCTIOn ------------ */
void	UpdateBlocksSystem(entt::registry &registry, entt::entity player);

