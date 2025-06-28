

#pragma once

# include "entt/entt.hpp"
# include "raylib.h"
# include "raymath.h"

# include "MiscComponents.hpp"
# include "Utils.hpp"

/* ------------ DEFAULTS ------------ */
# define PLAYER_DEFAULT_WIDTH 32
# define PLAYER_DEFAULT_HEIGHT 8
# define PLAYER_DEFAULT_SPEED 100.0f


/* ------------ TAGS ------------ */
struct PlayerTag {};


/* ------------ COMPONENTS ------------ */
struct PlayerHasBall {
	entt::entity ball;
};

struct Points {
	int points = {0};
};

struct Lives {
	int lives = {3};
};


/* ------------ INIT FUNCTION ------------ */
entt::entity	InitPlayer(entt::registry &registry, Vector2 position, Vector2 offset);


/* ------------ SYSTEM FUNCTIONS ------------ */
void			PlayerMovementSystem(entt::registry &registry, float deltaTime, entt::entity player, ScreenData screenData);
void			PlayerHaveBallSystem(entt::registry &registry, entt::entity player);

