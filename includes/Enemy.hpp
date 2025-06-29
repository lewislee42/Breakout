

# pragma once

# include "raylib.h"
# include "entt/entt.hpp"

# include "MiscComponents.hpp"
# include "Utils.hpp"


/* ------------ COMPONENTS ------------ */
enum EnemyState {
	FALLING = 0,
	FOLLOWING = 1
};

struct Enemy {
	EnemyState	state;
	float		stateDuration;
};

struct EnemyHitTag {};


/* ------------ SPAWNER ------------ */
void		SpawnEnemy(entt::registry &registry, ScreenData screenData);


/* ------------ HELPER ------------ */
EnemyState	GetNewEnemyState();
void		RemoveEnemy(entt::registry &registry);


/* ------------ SYSTEMS ------------ */
void		EnemySystem(entt::registry &registry, float deltaTime, float screenHeight);
void		EnemyCollideSystem(entt::registry &registry, entt::entity player);
void		EnemySpawningSystem(entt::registry &registry, float deltaTime, ScreenData screenData);

