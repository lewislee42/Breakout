

#include "Ball.hpp"


Vector2	RandomBallDirection(Vector2 position, Vector2 screenSize) {
	srand(time(NULL));
	float u = float(rand()) / RAND_MAX;
	float v = float(rand()) / RAND_MAX;
	
	if (u + v > 1.0f) {
		u = 1.0f - u;
		v = 1.0f - v;
	}

	Vector2 ab = Vector2Subtract(position, Vector2{screenSize.x / 2, screenSize.y / 2});
	Vector2 ac = Vector2Subtract(position, Vector2{-screenSize.x / 2, screenSize.y / 2});
	Vector2 p = Vector2Add(position, Vector2Add(Vector2Scale(ab, u), Vector2Scale(ac, v)));
	return Vector2Scale(p, -1);
}

void	InitBall(entt::registry &registry, Vector2 position, Vector2 screenSize) {
	entt::entity ball = registry.create();

	registry.emplace<BallTag>(ball);
	registry.emplace<Position>(ball, position);
	Vector2 RandomDirection = Vector2Normalize(RandomBallDirection(position, screenSize));
	registry.emplace<Velocity>(ball, RandomDirection, 100.0f);
	registry.emplace<Dimensions>(ball, Rectangle{position.x, position.y, 8, 8});
}

void	BallSystem(entt::registry &registry, float deltaTime, Vector2 screenSize) {
	auto view = registry.view<BallTag, Position, Velocity, Dimensions>();

	for (auto &entity : view) {
		Vector2	&position		= registry.get<Position>(entity).position;
		Velocity &velocity		= registry.get<Velocity>(entity);
		Rectangle &dimensions	= registry.get<Dimensions>(entity).dimensions;

		Vector2 wishPosition = Vector2Add(position, Vector2Scale(velocity.velocity, velocity.speed * deltaTime));
		Rectangle wishHitbox = dimensions;
		wishHitbox.x = wishPosition.x;
		wishHitbox.y = wishPosition.y;
		CollisionReturn collision = BreakoutCollisionCheck(registry, wishHitbox, entity);
		if (collision.x == true) {
			wishPosition.x = position.x;
			velocity.velocity.x *= -1;
		}
		if (collision.y == true) {
			wishPosition.y = position.y;
			velocity.velocity.y *= -1;
		}
		position = wishPosition;
		dimensions.x = position.x;
		dimensions.y = position.y;
	}
}
