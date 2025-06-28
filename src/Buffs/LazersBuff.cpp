

#include "Buffs/LazersBuff.hpp"

#include "Ball.hpp"
#include "Blocks.hpp"


/* ------------ HANDLER ------------ */
void	HandleAddLazersBuff(entt::registry &registry, entt::entity player) {
	LazersBuff &powerUp = registry.get_or_emplace<LazersBuff>(player);
	powerUp.duration = 3.0f;
	powerUp.cooldown = 0.2f;
	powerUp.currentCooldown = 0.1f;
}


/* ------------ SYSTEM ------------ */
void	LazerBuffSystem(entt::registry &registry, float deltaTime, entt::entity player) {
	auto view = registry.view<SmallBallTag, Position, Velocity, Dimensions>();

	for (auto entity : view) {
		Vector2 &position =		registry.get<Position>(entity).position;
		Velocity &velocity =	registry.get<Velocity>(entity);
		Rectangle &dimensions =	registry.get<Dimensions>(entity).dimensions;

		// check if got collision
		CollisionReturn collision = BallCollisionCheck(registry, dimensions, entity);

		if (collision.y != 0 && registry.valid(collision.entity)) {
			if (registry.all_of<Block>(collision.entity))
				registry.emplace<BlockHitTag>(collision.entity);
			registry.destroy(entity);
			return;
		}

		position = Vector2Add(position, Vector2Scale(velocity.velocity, velocity.speed * deltaTime));
		dimensions.x = position.x;
		dimensions.y = position.y;
	}

	if (!registry.valid(player)) {
		std::cerr << "Error: invalid entity/missing component in LazerBuffSystem" << std::endl;
		return ;
	}

	LazersBuff *lazerBuff = registry.try_get<LazersBuff>(player);
	if (!lazerBuff)
		return;

	lazerBuff->duration -= deltaTime;
	if (lazerBuff->duration <= 0) {
		registry.remove<LazersBuff>(player);
		return;
	}

	lazerBuff->currentCooldown -= deltaTime;
	if (lazerBuff->currentCooldown <= 0 && IsKeyDown(KEY_W)) {
		lazerBuff->currentCooldown = lazerBuff->cooldown;
		SpawnSmallBall(registry, player);
	}
}

