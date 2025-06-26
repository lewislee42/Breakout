

#pragma once

# include "entt/entt.hpp"
# include "raylib.h"
# include "raymath.h"

# include "MiscComponents.hpp"

struct PlayerTag {};

struct PlayerHasBall {
	entt::entity ball;
};

struct Points {
	int points = {0};
};

entt::entity	InitPlayer(entt::registry &registry, Vector2 position, Vector2 offset, entt::entity ball);
void			HandlePlayerMovement(entt::registry &registry, float deltaTime, ScreenData screenData);
void			HandlePlayerHaveBall(entt::registry &registry);
