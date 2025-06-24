

#pragma once

# include "raylib.h"
# include "entt/entt.hpp"
# include "MiscComponents.hpp"

struct CollisionReturn {
	bool x;
	bool y;
};

CollisionReturn	BreakoutCollisionCheck(entt::registry &registry, Rectangle A, entt::entity ent);
