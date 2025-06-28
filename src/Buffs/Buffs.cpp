

#include "Buffs/Buffs.hpp"

#include "Buffs/EnlargeBuff.hpp"
#include "Buffs/LazersBuff.hpp"
#include "Buffs/SlowBuff.hpp"
#include "Player.hpp"
#include "Ball.hpp"
#include "Blocks.hpp"
#include "Utils.hpp"



/* ------------ SPAWNER ------------ */
void	SpawnBuffs(entt::registry &registry, Vector2 position) {
	srand(time(NULL));
	entt::entity powerUp = registry.create();

	position.x -= 6;
	registry.emplace<Position>(powerUp, position);
	registry.emplace<Velocity>(powerUp, Vector2{0.0f, 1.0f}, 50.0f);
	registry.emplace<Dimensions>(powerUp, Rectangle{position.x, position.y, 12, 12});

	float randomNumber = (float)rand() / (RAND_MAX + 1.0f);
	if (randomNumber < 0.20)
		registry.emplace<Buff>(powerUp, ENLARGE);
	else if (randomNumber < 0.40)
		registry.emplace<Buff>(powerUp, DISRUPTION);
	else if (randomNumber < 0.60)
		registry.emplace<Buff>(powerUp, EXTRALIFE);
	else if (randomNumber < 0.80)
		registry.emplace<Buff>(powerUp, SLOW);
	else
		registry.emplace<Buff>(powerUp, LAZERS);
	
	// registry.emplace<Buff>(powerUp, SLOW);
}


/* ------------ SYSTEMS ------------ */
void	UpdateBuffObjectsSystem(entt::registry &registry, float deltaTime, entt::entity player, float screenHeight) {
	auto view = registry.view<Buff, Position, Velocity, Dimensions>();

	for (auto &entity : view) {
		Vector2 &position = registry.get<Position>(entity).position;
		Velocity velocity = registry.get<Velocity>(entity);
		Rectangle &dimensions = registry.get<Dimensions>(entity).dimensions;

		position = Vector2Add(position, Vector2Scale(velocity.velocity, velocity.speed * deltaTime));
		
		if (position.y > (screenHeight / 2) + 32) // 32 is for some padding
			registry.destroy(entity);

		dimensions.x = position.x;
		dimensions.y = position.y;

		if (!registry.valid(player) || !registry.all_of<PlayerTag, Dimensions, Velocity>(player)) {
			std::cerr << "Error: invalid entity/missing component in UpdateBuffObjectsSystem" << std::endl;
			continue;
		}

		Rectangle playerDimensions = registry.get<Dimensions>(player).dimensions;
		if (BreakoutCheckCollision(dimensions, playerDimensions)) { 
			BuffsEnum powerUpType = registry.get<Buff>(entity).type;
			
			switch (powerUpType) {
				case ENLARGE:
					HandleAddEnlargeBuff(registry, player, dimensions.width);
					break;
				case DISRUPTION:
					HandleAddDisruptionBuff(registry);
					break;
				case EXTRALIFE:
					HandleAddExtraLifeBuff(registry, player);
					break;
				case SLOW:
					HandleAddSlowBuff(registry, player);
					break;
				case LAZERS:
					HandleAddLazersBuff(registry, player);
					break;
				default:
					break;
			}
			registry.destroy(entity);
		}
	}
}

void	UpdateBuffsSystem(entt::registry &registry, float deltaTime, entt::entity player, ScreenData screenData) {
	EnlargeBuffSystem(registry, deltaTime, player, screenData);
	LazerBuffSystem(registry, deltaTime, player);
	SlowBuffSystem(registry, deltaTime, player);
}



