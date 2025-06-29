

#include "Ball.hpp"

#include "Blocks.hpp"
#include "Buffs/Buffs.hpp"
#include "Player.hpp"
#include "Enemy.hpp"


/* ------------ HELPERS ------------ */
CollisionReturn	BallCollisionCheck(entt::registry &registry, Rectangle ball, entt::entity ent) {
	auto view = registry.view<Dimensions>(entt::exclude<Buff, BallTag>);

	for (auto &entity : view) {
		Rectangle rec = registry.get<Dimensions>(entity).dimensions;
		if (entity == ent)
			continue;

		if (ball.x < rec.x + rec.width && ball.x + ball.width > rec.x && 
			ball.y < rec.y + rec.height && ball.y + ball.height > rec.y) {
			
			float overlapX = std::min(ball.x + ball.width, rec.x + rec.width) - std::max(ball.x, rec.x);
			float overlapY = std::min(ball.y + ball.height, rec.y + rec.height) - std::max(ball.y, rec.y);

			float deltaX = (ball.x + ball.width / 2) - (rec.x + rec.width / 2);
			float deltaY = (ball.y + ball.height / 2) - (rec.y + rec.height / 2);
			float resolveY = (deltaY < 0) ? -overlapY : overlapY;
			float resolveX = (deltaX < 0) ? -overlapX : overlapX;
			if (overlapX < overlapY)
				return CollisionReturn{resolveX, 0.0f, entity};
			else
				return CollisionReturn{0.0f, resolveY, entity};
		}
	}

	return {0};
}

void	HandleBallCollision(entt::registry &registry, float deltaTime, entt::entity ball, entt::entity collideEntt) {
	if (registry.all_of<Block>(collideEntt) && !registry.all_of<BlockHitTag>(collideEntt)) {
		registry.emplace<BlockHitTag>(collideEntt);
	}
	else if (registry.all_of<PlayerTag, Velocity, Position>(collideEntt) && !registry.all_of<BallCollidedWithPlayerTag>(ball)) {
		Velocity &ballVel	= registry.get<Velocity>(ball);
		Vector2 ballPos		= registry.get<Position>(ball).position;
		Velocity playerVel	= registry.get<Velocity>(collideEntt);
		Vector2 playerPos	= registry.get<Position>(collideEntt).position;
		Rectangle playerDim = registry.get<Dimensions>(collideEntt).dimensions;

		registry.emplace<BallCollidedWithPlayerTag>(ball);

		float relativePos = (ballPos.x - playerPos.x) / playerDim.width;

		float minDeg = -45. * DEG2RAD;
		float maxDeg = 45. * DEG2RAD;
		float angle = minDeg + (maxDeg - minDeg) * relativePos; 

		ballVel.velocity.x = sinf(angle); 
		ballVel.velocity.y = -cosf(angle);
		ballVel.velocity = Vector2Normalize(ballVel.velocity); 
	}
	else if (!registry.all_of<PlayerTag>(collideEntt) && registry.all_of<BallCollidedWithPlayerTag>(ball)) {
		registry.remove<BallCollidedWithPlayerTag>(ball);
	}
	else if (registry.all_of<Enemy>(collideEntt) && !registry.all_of<EnemyHitTag>(collideEntt)) {
		registry.emplace<EnemyHitTag>(collideEntt);
	}

	if (!registry.all_of<Bounces>(ball))
		return ;
	int &bounces = registry.get<Bounces>(ball).bounces;
	Velocity &ballVel	= registry.get<Velocity>(ball);
	bounces++;
	if (bounces % 10 == 0)
		ballVel.speed += 25;
}

void	RemoveBalls(entt::registry &registry, entt::entity player) {
	auto view = registry.view<BallTag>();

	for (auto entity : view) {
		registry.destroy(entity);
	}

	if (registry.valid(player) && registry.all_of<PlayerHasBall>(player))
		registry.remove<PlayerHasBall>(player);
}

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
	return Vector2Subtract(p, position);
}

void	AttachBallToPlayer(entt::registry &registry, entt::entity ball, entt::entity player) {
	registry.emplace<PlayerHasBall>(player, ball);
	registry.emplace<BallBeingHeldTag>(ball);
}



/* ------------ INIT FUNCTION ------------ */
entt::entity	InitBall(entt::registry &registry, Vector2 position, Vector2 velocity) {
	entt::entity ball = registry.create();

	registry.emplace<BallTag>(ball);
	registry.emplace<Position>(ball, position);
	registry.emplace<Velocity>(ball, velocity, BALL_DEFAULT_SPEED);
	registry.emplace<Dimensions>(ball, Rectangle{position.x, position.y, 8, 8});
	registry.emplace<Bounces>(ball, 0);

	return ball;
}

void	SpawnSmallBall(entt::registry &registry, entt::entity player) {
	entt::entity newBall = registry.create();

	if (!registry.valid(player) || !registry.all_of<PlayerTag, Position, Velocity, Dimensions>(player))
		return;

	Vector2 playerPos = registry.get<Position>(player).position;
	Velocity playerVel = registry.get<Velocity>(player);
	Rectangle playerDimen = registry.get<Dimensions>(player).dimensions;

	float widthHeight = 4;

	Vector2 startPosition{
		playerPos.x + (playerDimen.width / 2) - (widthHeight / 2), 
		playerPos.y - widthHeight - 4
	}; 
	Rectangle dimensions{startPosition.x, startPosition.y, 4, 4};

	registry.emplace<SmallBallTag>(newBall);
	registry.emplace<Dimensions>(newBall, dimensions);
	registry.emplace<Position>(newBall, startPosition);
	registry.emplace<Velocity>(newBall, Vector2{0.0f, -1.0f}, BALL_DEFAULT_SPEED);
}



/* ------------ SYSTEM ------------ */
void	UpdateBallSystem(entt::registry &registry, float deltaTime, float screenHeight) {
	auto view = registry.view<BallTag, Position, Velocity, Dimensions>();

	for (auto &entity : view) {
		Vector2	&position		= registry.get<Position>(entity).position;
		Velocity &velocity		= registry.get<Velocity>(entity);
		Rectangle &dimensions	= registry.get<Dimensions>(entity).dimensions;

		if (Vector2Length(velocity.velocity) == 0 && registry.all_of<BallBeingHeldTag>(entity) == false) // launches the ball
			velocity.velocity = Vector2Normalize(RandomBallDirection(position));

		Vector2 wishPosition = Vector2Add(position, Vector2Scale(velocity.velocity, velocity.speed * deltaTime));
		Rectangle wishHitbox = dimensions;
		wishHitbox.x = wishPosition.x;
		wishHitbox.y = wishPosition.y;

		CollisionReturn collision = BallCollisionCheck(registry, wishHitbox, entity);
		bool isColPlayer = registry.all_of<BallCollidedWithPlayerTag>(entity);

		if (collision.x != 0.0f && !isColPlayer) {
			wishPosition.x += collision.x;
			velocity.velocity.x *= -1;
		}
		if (collision.y != 0.0f && !isColPlayer) {
			wishPosition.y += collision.y; // offset it to not cause another collision on the next frame
			velocity.velocity.y *= -1;
		}

		position = wishPosition;
		if (position.y > (screenHeight / 2) + 32) {
			registry.destroy(entity);
			continue ;
		}

		dimensions.x = position.x;
		dimensions.y = position.y;

		if ((std::uint32_t)collision.entity == 0 || !registry.valid(collision.entity)) {
			continue ;
		}
		HandleBallCollision(registry, deltaTime, entity, collision.entity);
	}
}
