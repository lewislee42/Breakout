

#include "Player.hpp"
#include "MiscComponents.hpp"

void	InitPlayer(entt::registry &registry, Vector2 position, Vector2 offset) {
	entt::entity player = registry.create();

	registry.emplace<PlayerTag>(player);
	registry.emplace<Position>(player, position);
	registry.emplace<Velocity>(player, Vector2{0}, 100.0f);
	registry.emplace<CameraComponent>(player, Vector2{0}, offset);
	registry.emplace<Dimensions>(player, Rectangle{position.x, position.y, 32, 8});
	registry.emplace<Points>(player);
}

void	HandlePlayerMovement(entt::registry &registry, float deltaTime, Vector2 screenSize) {
	auto view = registry.view<PlayerTag, Position, Velocity, Dimensions>();
	for (auto &entity : view) {
		Vector2 velocity = {0};
		Velocity &currentVelocity	= registry.get<Velocity>(entity);
		Vector2	&currentPosition	= registry.get<Position>(entity).position;
		Rectangle &dimensions		= registry.get<Dimensions>(entity).dimensions;


		if (IsKeyDown(KEY_A))
			velocity.x -= currentVelocity.speed * deltaTime;
		if (IsKeyDown(KEY_D))
			velocity.x += currentVelocity.speed * deltaTime;
		
		currentVelocity.velocity = Vector2Add(currentVelocity.velocity, velocity);
		currentVelocity.velocity = Vector2Scale(currentVelocity.velocity, 0.8f);  // friction
		if (Vector2Length(currentVelocity.velocity) <= 0.1f)
			currentVelocity.velocity = {0};
		
		Vector2 wishPosition = Vector2Add(currentPosition, currentVelocity.velocity);


		if (wishPosition.x > (float(screenSize.x) / 2) - dimensions.width)
			wishPosition.x = (float(screenSize.x) / 2) - dimensions.width;
		if (wishPosition.x < -(float(screenSize.x) / 2))
			wishPosition.x = -(float(screenSize.x) / 2);

		currentPosition = wishPosition;
		dimensions.x = currentPosition.x;
		dimensions.y = currentPosition.y;
	}
}
