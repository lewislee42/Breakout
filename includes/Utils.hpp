

#pragma once

# include "raylib.h"
# include "entt/entt.hpp"
# include "MiscComponents.hpp"

struct CollisionReturn {
	bool			x;
	bool			y;
	entt::entity	entity;
};

CollisionReturn	BreakoutCollisionCheck(entt::registry &registry, Rectangle A, entt::entity ent);
