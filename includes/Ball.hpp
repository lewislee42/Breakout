

#pragma once

# include "raylib.h"
# include "raymath.h"
# include "entt/entt.hpp"

# include "MiscComponents.hpp"

# include <stdlib.h>
# include <time.h>

struct BallTag {};

struct BallBeingHeld {};


entt::entity	InitBall(entt::registry &registry, Vector2 position, Vector2 screenSize);
void			BallSystem(entt::registry &registry, float deltaTime);
CollisionReturn	BallCollisionCheck(entt::registry &registry, Rectangle A, entt::entity ent);

