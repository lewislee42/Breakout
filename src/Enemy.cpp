

#include "Enemy.hpp"

#include "Ball.hpp"
#include "Player.hpp"
#include <cstdlib>


/* ------------ SPAWNER ------------ */
void	SpawnEnemy(entt::registry &registry, ScreenData screenData) {
	entt::entity enemy = registry.create();

	float min = screenData.topLeft.x + 12;
	float max = screenData.topLeft.x + screenData.playableArea.x - 12 - 20.;
	Vector2 position{min + (max - min) * (float(rand()) / (RAND_MAX + 1.0f)), screenData.topLeft.y + 14.0f};

	registry.emplace<Velocity>(enemy, Vector2{0., 1.}, 100.0f);
	registry.emplace<Position>(enemy, position);
	registry.emplace<Dimensions>(enemy, Rectangle{position.x, position.y, 20., 20.,});
	registry.emplace<Enemy>(enemy, Enemy{FALLING, 3.0f});
	registry.emplace<Points>(enemy, 10);
}


/* ------------ HELPER ------------ */
EnemyState	GetNewEnemyState() {
	float randomNumber = (float(rand()) / (RAND_MAX + 1.0f));
	if (randomNumber <= 0.5f)
		return FOLLOWING;
	else
		return FALLING;
}

void		RemoveEnemy(entt::registry &registry) {
	auto view = registry.view<Enemy>();

	for (auto entity : view) {
		registry.destroy(entity);
	}
}



/* ------------ SYSTEMS ------------ */
void	EnemySystem(entt::registry &registry, float deltaTime, float screenHeight) {
	auto view = registry.view<Enemy, Position, Dimensions, Velocity>();

	for (auto entity : view) {
		Enemy &enemy = registry.get<Enemy>(entity);
		Velocity &velocity = registry.get<Velocity>(entity);
		Vector2 &position = registry.get<Position>(entity).position;
		Rectangle &dimensions = registry.get<Dimensions>(entity).dimensions;
		
		// handle state change
		enemy.stateDuration -= deltaTime;
		if (enemy.stateDuration <= 0) {
			enemy.state = GetNewEnemyState();
			enemy.stateDuration = 1.0f + (3.5f - 1.0f) * (float(rand()) / (RAND_MAX + 1.0f));
		}

		// handle movement
		if (enemy.state == FALLING) {
			velocity.velocity = Vector2{0., 1.};
		}
		else if (enemy.state == FOLLOWING) {
	  		auto balls = registry.view<BallTag, Position>();

			for (auto ball : balls) {
				Vector2 ballPos = registry.get<Position>(ball).position;
				velocity.velocity = Vector2Normalize(Vector2Subtract(ballPos, position));
				break;
			}
		}

		position = Vector2Add(position, Vector2Scale(velocity.velocity, deltaTime * velocity.speed));
		dimensions.x = position.x;
		dimensions.y = position.y;

		if (position.y > (screenHeight / 2.0f) + 32) {
			registry.destroy(entity);
			continue ;
		}

		// handle collision with player
		auto players = registry.view<PlayerTag, Dimensions>();
		for (auto player : players) {
			Rectangle playerDim = registry.get<Dimensions>(player).dimensions;
			if (dimensions.x < playerDim.x + playerDim.width && dimensions.x + dimensions.width > playerDim.x && 
				dimensions.y < playerDim.y + playerDim.height && dimensions.y + dimensions.height > playerDim.y) {
				registry.emplace<EnemyHitTag>(entity);
			}
			break;
		}
	}
}

void	EnemyCollideSystem(entt::registry &registry, entt::entity player) {

	if (!registry.valid(player) || !registry.all_of<CurrentPoints>(player)) {
		std::cerr << "Error: invalid entity/missing component in EnemyCollideSystem" << std::endl;
		return;
	}
	auto view = registry.view<Enemy, EnemyHitTag, Points>();

	for (auto entity : view) {
		int &points = registry.get<Points>(entity).points;
		int &playerPoints = registry.get<CurrentPoints>(player).points;
		playerPoints += points;
		registry.destroy(entity);
	}
}

void	EnemySpawningSystem(entt::registry &registry, float deltaTime, ScreenData screenData) {
	static float timer;

	timer -= deltaTime;
	if (timer <= 0) {
		float random = float(rand()) / (RAND_MAX + 1.0f);
		if (random <= 0.75f)
			SpawnEnemy(registry, screenData);
		timer = 2.0f;
	}
}
