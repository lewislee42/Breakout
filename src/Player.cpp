

#include "Player.hpp"
#include "Ball.hpp"
#include "MiscComponents.hpp"

entt::entity	InitPlayer(entt::registry &registry, Vector2 position, Vector2 offset, entt::entity ball) {
	entt::entity player = registry.create();

	registry.emplace<PlayerTag>(player);
	registry.emplace<Position>(player, position);
	registry.emplace<Velocity>(player, Vector2{0}, 100.0f);
	registry.emplace<CameraComponent>(player, Vector2{0}, offset);
	registry.emplace<Dimensions>(player, Rectangle{position.x, position.y, 32, 8});
	registry.emplace<Points>(player);
	registry.emplace<PlayerHasBall>(player, ball);

	return player;
}

void	HandlePlayerMovement(entt::registry &registry, float deltaTime, ScreenData screenData) {
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


		if (wishPosition.x < screenData.topLeft.x + screenData.wallWidth)
			wishPosition.x = screenData.topLeft.x + screenData.wallWidth;
		if (wishPosition.x > screenData.topRight.x - screenData.wallWidth - dimensions.width) 
			wishPosition.x = screenData.topRight.x - screenData.wallWidth - dimensions.width;

		currentPosition = wishPosition;
		dimensions.x = currentPosition.x;
		dimensions.y = currentPosition.y;
	}
}

void	HandlePlayerHaveBall(entt::registry &registry) {
	auto view = registry.view<PlayerTag, PlayerHasBall, Dimensions>();

	for (auto &entity : view) {
		entt::entity ball			= registry.get<PlayerHasBall>(entity).ball;
		if (!registry.all_of<Position, Dimensions>(ball))
			continue;

		Vector2	&ballPosition		= registry.get<Position>(ball).position;
		Rectangle &ballDimensions	= registry.get<Dimensions>(ball).dimensions;
		Vector2	&playerPosition		= registry.get<Position>(entity).position;
		Rectangle &playerDimensions	= registry.get<Dimensions>(entity).dimensions;
		ballPosition.x = playerPosition.x + (playerDimensions.width / 2) - (ballDimensions.width / 2);
		ballPosition.y = playerPosition.y - (playerDimensions.height) - 4;

		if (IsKeyDown(KEY_W) && registry.valid(ball)) {
			registry.remove<BallBeingHeld>(ball);
			registry.remove<PlayerHasBall>(entity);
		}

	}
}
