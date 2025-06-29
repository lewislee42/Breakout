

#pragma once

# include "raylib.h"
# include "raymath.h"
# include "entt/entt.hpp"

# include "MiscComponents.hpp"
# include "Utils.hpp"

# include <stdlib.h>
# include <time.h>
# include <cstdint>


/* ------------ DEFAULTS ------------ */
# define BALL_DEFAULT_SPEED 200.0f


/* ------------ TAGS ------------ */
struct BallTag {};

struct BallBeingHeldTag {};

struct BallCollidedWithPlayerTag {};

struct SmallBallTag {};

struct Bounces {
	int bounces = {0};
};


/* ------------ INIT FUNCTIONS ------------ */
entt::entity	InitBall(entt::registry &registry, Vector2 position, Vector2 velocity);
void			SpawnSmallBall(entt::registry &registry, entt::entity player);


/* ------------ SYSTEM FUNCTION ------------ */
void			UpdateBallSystem(entt::registry &registry, float deltaTime, float screenHeight);


/* ------------ HELPERS ------------ */
void			AttachBallToPlayer(entt::registry &registry, entt::entity ball, entt::entity player);
CollisionReturn	BallCollisionCheck(entt::registry &registry, Rectangle A, entt::entity ent);
void			RemoveBalls(entt::registry &registry, entt::entity player);
