

# pragma once

# include "raylib.h"
# include "entt/entt.hpp"



struct ScreenData {
	Vector2 topLeft;
	Vector2 topRight;
	Vector2 screenSize;
	Vector2 playableArea;
	int		wallWidth;
};

struct CollisionReturn {
	float			x;
	float			y;
	entt::entity	entity;
};

bool	BreakoutCheckCollision(Rectangle rec1, Rectangle rec2);
