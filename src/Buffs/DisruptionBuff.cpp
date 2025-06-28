

#include "Buffs/DisruptionBuff.hpp"

#include "Ball.hpp"


/* ------------ HANDLER ------------ */
void	HandleAddDisruptionBuff(entt::registry &registry) {
	auto view = registry.view<BallTag, Position, Velocity>();

	int ballAmount = 0;
	for (auto entity : view)
		ballAmount++;
	if (ballAmount >= 9) // stops too many balls from spawning
		return;

	for (auto entity : view) {
		Vector2 position = registry.get<Position>(entity).position;
		Vector2 velocity = registry.get<Velocity>(entity).velocity;

		float angle = DEG2RAD * 45.0f;

		Vector2 leftVector = {
			velocity.x * std::cosf(-angle) - velocity.y * std::sinf(-angle),
			velocity.x * std::sinf(-angle) + velocity.y * std::cosf(-angle)
		};
		Vector2 rightVector = {
			velocity.x * std::cosf(angle) - velocity.y * std::sinf(angle),
			velocity.x * std::sinf(angle) + velocity.y * std::cosf(angle)
		};
		InitBall(registry, position, Vector2Normalize(leftVector));
		InitBall(registry, position, Vector2Normalize(rightVector));
	}
}


