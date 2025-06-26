

#include "Ball.hpp"
#include "Blocks.hpp"
#include "PowerUps.hpp"
#include "Player.hpp"


Vector2	RandomBallDirection(Vector2 position) {
	srand(time(NULL));
	float u = float(rand()) / RAND_MAX;
	float v = float(rand()) / RAND_MAX;
	Vector2 leftPoint = Vector2{position.x - 200, position.y - 200};
	Vector2 rightPoint = Vector2{position.x + 200, position.y - 200};
	
	if (u + v > 1.0f) {
		u = 1.0f - u;
		v = 1.0f - v;
	}

	Vector2 ab = Vector2Subtract(leftPoint, position);
	Vector2 ac = Vector2Subtract(rightPoint, position);
	Vector2 p = Vector2Add(position, Vector2Add(Vector2Scale(ab, u), Vector2Scale(ac, v)));
	p = Vector2Subtract(p, position);
	// p.y *= -1;
	return p;
}

entt::entity	InitBall(entt::registry &registry, Vector2 position, Vector2 screenSize) {
	entt::entity ball = registry.create();

	registry.emplace<BallTag>(ball);
	registry.emplace<Position>(ball, position);
	// Vector2 RandomDirection = Vector2Normalize(RandomBallDirection(position, screenSize));
	registry.emplace<Velocity>(ball, Vector2{0}, 200.0f);
	registry.emplace<Dimensions>(ball, Rectangle{position.x, position.y, 8, 8});
	registry.emplace<BallBeingHeld>(ball);

	return ball;
}

void	BallSystem(entt::registry &registry, float deltaTime) {
	auto view = registry.view<BallTag, Position, Velocity, Dimensions>();

	for (auto &entity : view) {
		Vector2	&position		= registry.get<Position>(entity).position;
		Velocity &velocity		= registry.get<Velocity>(entity);
		Rectangle &dimensions	= registry.get<Dimensions>(entity).dimensions;

		if (Vector2Length(velocity.velocity) == 0 && registry.all_of<BallBeingHeld>(entity) == false)
			velocity.velocity = Vector2Normalize(RandomBallDirection(position));

		Vector2 wishPosition = Vector2Add(position, Vector2Scale(velocity.velocity, velocity.speed * deltaTime));
		Rectangle wishHitbox = dimensions;
		wishHitbox.x = wishPosition.x;
		wishHitbox.y = wishPosition.y;

		CollisionReturn collision = BallCollisionCheck(registry, wishHitbox, entity);
		bool isColPlayer = registry.all_of<BallCollidedWithPlayer>(entity);

		if (collision.x == true && !isColPlayer)
			velocity.velocity.x *= -1;
		if (collision.y == true && !isColPlayer)
			velocity.velocity.y *= -1;

		position = wishPosition;
		dimensions.x = position.x;
		dimensions.y = position.y;

		if (!registry.valid(collision.entity))
			continue ;

		if (registry.all_of<Block>(collision.entity) &&
			!registry.all_of<BlockHitTag>(collision.entity))
		{
			registry.emplace<BlockHitTag>(collision.entity);
		}
		else if (registry.all_of<PlayerTag, Velocity, Position>(collision.entity) &&
			!registry.all_of<BallCollidedWithPlayer>(entity))
		{
			Velocity playerVelocity = registry.get<Velocity>(collision.entity);
			Vector2 playerPosition = registry.get<Position>(collision.entity).position;
			velocity.velocity = Vector2Add(velocity.velocity, Vector2Scale(playerVelocity.velocity, deltaTime));
			registry.emplace<BallCollidedWithPlayer>(entity, 0.1f);
		}
		else if (!registry.all_of<PlayerTag>(collision.entity) && registry.all_of<BallCollidedWithPlayer>(entity)) {
			registry.remove<BallCollidedWithPlayer>(entity);
		}
	}
}

CollisionReturn	BallCollisionCheck(entt::registry &registry, Rectangle ball, entt::entity ent) {
	auto view = registry.view<Dimensions>(entt::exclude<PowerUpTag>);

	for (auto &entity : view) {
		Rectangle rec = registry.get<Dimensions>(entity).dimensions;
		if (entity == ent)
			continue;

		if (ball.x < rec.x + rec.width && ball.x + ball.width > rec.x && 
			ball.y < rec.y + rec.height && ball.y + ball.height > rec.y) {
			
			float overlapX = std::min(ball.x + ball.width, rec.x + rec.width) - std::max(ball.x, rec.x);
			float overlapY = std::min(ball.y + ball.height, rec.y + rec.height) - std::max(ball.y, rec.y);
			if (overlapX < overlapY)
				return CollisionReturn{true, 0, entity};
			else
				return CollisionReturn{0, true, entity};
		}
	}

	return {0};
}
