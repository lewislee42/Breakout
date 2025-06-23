

#include "Player.hpp"

void	InitPlayer(entt::registry &registry, Vector2 position, Vector2 offset) {
	entt::entity player = registry.create();

	registry.emplace<PlayerTag>(player);
	registry.emplace<Position>(player, Vector2{0, 0});
	registry.emplace<Velocity>(player);
	registry.emplace<CameraComponent>(player, position, offset);
}

void	HandlePlayerMovement(entt::registry &registry, float deltaTime, int screenWidth, int screenHeight) {
	auto view = registry.view<PlayerTag, Position, Velocity>();
	for (auto &entity : view) {
		Vector2 velocity = {0};
		Vector2	&currentVelocity = registry.get<Velocity>(entity).velocity;
		Vector2	&currentPosition = registry.get<Position>(entity).position;
		float speed = 100;

		if (IsKeyDown(KEY_A))
			velocity.x -= speed * deltaTime;
		if (IsKeyDown(KEY_D))
			velocity.x += speed * deltaTime;
		
		currentVelocity = Vector2Add(currentVelocity, velocity);
		currentVelocity = Vector2Scale(currentVelocity, 0.8f);
		if (Vector2Length(currentVelocity) <= 0.1f)
			currentVelocity = {0};
		
		Vector2 wishPosition = Vector2Add(currentPosition, currentVelocity);

		int tempSIZE = 15;

		if (wishPosition.x > (screenWidth / 2) - tempSIZE)
			wishPosition.x = (screenWidth / 2) - tempSIZE;
		if (wishPosition.x < -(screenWidth / 2))
			wishPosition.x = -(screenWidth / 2);

		currentPosition = wishPosition;
	}
}
