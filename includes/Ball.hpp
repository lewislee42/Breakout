

#pragma once

# include "raylib.h"
# include "raymath.h"
# include "entt/entt.hpp"

# include "Utils.hpp"
# include "MiscComponents.hpp"

# include <stdlib.h>
# include <time.h>

struct BallTag {};

void	InitBall(entt::registry &registry, Vector2 position, Vector2 screenSize);
void	BallSystem(entt::registry &registry, float deltaTime, Vector2 screenSize);
