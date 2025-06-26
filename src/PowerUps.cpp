

#include "PowerUps.hpp"

void	SpawnPowerUps(entt::registry &registry, Vector2 position) {

	entt::entity powerUp = registry.create();

	position.x -= 6;
	registry.emplace<PowerUpTag>(powerUp);
	registry.emplace<Position>(powerUp, position);
	registry.emplace<Velocity>(powerUp, Vector2{0.0f, 1.0f}, 40.0f);
	registry.emplace<Dimensions>(powerUp, Rectangle{position.x, position.y, 12, 12});


}

void	PowerUpsSystem(entt::registry &registry, float deltaTime) {
	auto view = registry.view<PowerUpTag, Position, Velocity, Dimensions>();

	for (auto &entity : view) {
		Vector2 &position = registry.get<Position>(entity).position;
		Velocity velocity = registry.get<Velocity>(entity);
		Rectangle &dimensions = registry.get<Dimensions>(entity).dimensions;

		position = Vector2Add(position, Vector2Scale(velocity.velocity, velocity.speed * deltaTime));
		dimensions.x = position.x;
		dimensions.y = position.y;
	}
}

