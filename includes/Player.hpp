

#pragma once

# include "entt/entt.hpp"
# include "raylib.h"
# include "raymath.h"
# include "MiscComponents.hpp"

struct PlayerTag {};

void	InitPlayer(entt::registry &registry, Vector2 position, Vector2 offset);
void	HandlePlayerMovement(entt::registry &registry, float deltaTime, Vector2 screenSize);
