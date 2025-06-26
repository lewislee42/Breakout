

#pragma once

# include "raylib.h"
# include "raymath.h"
# include "entt/entt.hpp"

# include "MiscComponents.hpp"

struct PowerUpTag {};

void	SpawnPowerUps(entt::registry &registry, Vector2 position);
void	PowerUpsSystem(entt::registry &registry, float deltaTime);



