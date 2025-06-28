

#include "Buffs/SlowBuff.hpp"

#include "Ball.hpp"


/* ------------ HANDLER ------------ */
void	HandleAddSlowBuff(entt::registry &registry, entt::entity player) {
	SlowBuff &powerUp = registry.get_or_emplace<SlowBuff>(player);
	auto view = registry.view<BallTag, Velocity>();
	float speed = registry.get<Velocity>(*view.begin()).speed;

	powerUp.duration = 6.0f;
	powerUp.speed = 100.0f;
}


/* ------------ SYSTEM ------------ */
void	SlowBuffSystem(entt::registry &registry, float deltaTime, entt::entity player) {
	if (!registry.valid(player)) {
		std::cerr << "Error: invalid entity/missing component in SlowBuffSystem" << std::endl;
		return;
	}

	SlowBuff *powerUp = registry.try_get<SlowBuff>(player);
	if (!powerUp)
		return;

	auto view = registry.view<BallTag, Velocity>();

	powerUp->duration -= deltaTime;
	for (auto entity : view) {
		Velocity &velocity = registry.get<Velocity>(entity);

		if (powerUp->duration <= 0) {
			velocity.speed = BALL_DEFAULT_SPEED;
			registry.remove<SlowBuff>(player);
			continue;
		}

		if (velocity.speed == powerUp->speed)
			continue;
		velocity.speed = powerUp->speed;
	}
}

