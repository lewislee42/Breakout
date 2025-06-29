

#include "Player.hpp"

#include "Ball.hpp"


/* ------------ INIT FUNCTION ------------ */
entt::entity	InitPlayer(entt::registry &registry, ScreenData screenData) {
	entt::entity player = registry.create();
	Vector2 position = Vector2{(screenData.playableArea.x / 2) + screenData.topLeft.x - (PLAYER_DEFAULT_WIDTH / 2.0f), screenData.screenSize.y / 2 - 16};
	Vector2 offset = Vector2{screenData.screenSize.x / 2, screenData.screenSize.y / 2};

	registry.emplace<PlayerTag>(player);
	registry.emplace<Position>(player, position);
	registry.emplace<Velocity>(player, Vector2{0}, PLAYER_DEFAULT_SPEED);
	registry.emplace<CameraComponent>(player, Vector2{0}, offset);
	registry.emplace<CurrentPoints>(player);
	registry.emplace<Lives>(player, 3);
	registry.emplace<Dimensions>(player, Rectangle{position.x, position.y, PLAYER_DEFAULT_WIDTH, PLAYER_DEFAULT_HEIGHT});

	return player;
}


void	ResetPlayerPos(entt::registry &registry, entt::entity player, ScreenData screenData) {
	if (!registry.valid(player) && !registry.all_of<PlayerTag, Position, Dimensions>(player)) {
		return ;
	}

	Vector2 &position = registry.get<Position>(player).position;
	Rectangle &dimensions = registry.get<Dimensions>(player).dimensions;
	position = Vector2{(screenData.playableArea.x / 2) + screenData.topLeft.x - (PLAYER_DEFAULT_WIDTH / 2.0f), screenData.screenSize.y / 2 - 16};
	dimensions.x = position.x;
	dimensions.y = position.y;
}

void	ResetPlayerPointsLives(entt::registry &registry, entt::entity player) {
	if (!registry.valid(player) || !registry.all_of<Lives, CurrentPoints>(player))
		return;
	int &lives = registry.get<Lives>(player).lives;
	int &points = registry.get<CurrentPoints>(player).points;
	lives = 3;
	points = 0;
}


/* ------------ SYSTEMS ------------ */
void	PlayerMovementSystem(entt::registry &registry, float deltaTime, entt::entity player, ScreenData screenData) {
	if (!registry.valid(player) || !registry.all_of<PlayerTag, Velocity, Position, Dimensions>(player)) {
		std::cerr << "Error: invalid entity/missing component in PlayerMovementSystem" << std::endl;
		return ;
	}

	Velocity &currentVelocity	= registry.get<Velocity>(player);
	Vector2	&currentPosition	= registry.get<Position>(player).position;
	Rectangle &dimensions		= registry.get<Dimensions>(player).dimensions;

	if (IsKeyDown(KEY_A))
		currentVelocity.velocity.x -= currentVelocity.speed * deltaTime;
	if (IsKeyDown(KEY_D))
		currentVelocity.velocity.x += currentVelocity.speed * deltaTime;
	
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

void	PlayerHaveBallSystem(entt::registry &registry, entt::entity player) {
	if (!registry.valid(player) || !registry.all_of<PlayerTag, Position, Dimensions, PlayerHasBall>(player))
		return ;

	entt::entity ball = registry.get<PlayerHasBall>(player).ball;
	if (!registry.valid(ball) || !registry.all_of<BallTag, Position, Dimensions>(ball)) {
		std::cerr << "Error: invalid entity/missing component in PlayerHaveBallSystem" << std::endl;
		return ;
	}

	Vector2	&ballPosition		= registry.get<Position>(ball).position;
	Rectangle &ballDimensions	= registry.get<Dimensions>(ball).dimensions;
	Vector2	&playerPosition		= registry.get<Position>(player).position;
	Rectangle &playerDimensions	= registry.get<Dimensions>(player).dimensions;
	ballPosition.x = playerPosition.x + (playerDimensions.width / 2) - (ballDimensions.width / 2);
	ballPosition.y = playerPosition.y - (playerDimensions.height) - 4;

	if (IsKeyDown(KEY_W)) {
		registry.remove<BallBeingHeldTag>(ball);
		registry.remove<PlayerHasBall>(player);
	}

}
