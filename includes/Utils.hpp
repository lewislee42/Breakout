

# pragma once

# include "raylib.h"
# include "entt/entt.hpp"


/* ------------ STRUCTS ------------ */
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

enum GameState {
	MENU = 0,
	TRANSITIONING = 1,
	INGAME = 2,
	GAME_RETRYING = 3,
	GAME_LOSE = 4,
	GAME_WIN = 5
};


bool	BreakoutCheckCollision(Rectangle rec1, Rectangle rec2);
