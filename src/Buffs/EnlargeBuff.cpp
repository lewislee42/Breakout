

#include "Buffs/EnlargeBuff.hpp"


/* ------------ HANDLER ------------ */
void	HandleAddEnlargeBuff(entt::registry &registry, entt::entity player, float dimensionsWidth) {
	EnlargeBuff &powerUp = registry.get_or_emplace<EnlargeBuff>(player);
	powerUp.duration = 10.0f;
	if (powerUp.originalWidth == 0)
		powerUp.originalWidth = dimensionsWidth;
	powerUp.newWidth = 64;
}


/* ------------ SYSTEMS ------------ */
void	EnlargeBuffSystem(entt::registry &registry, float deltaTime, entt::entity player, ScreenData screenData) {
	if (!registry.valid(player) || !registry.all_of<Dimensions, Position>(player)) {
		std::cerr << "Error: invalid entity/missing component in EnlargeBuffSystem" << std::endl;
		return;
	}

	EnlargeBuff *powerUp = registry.try_get<EnlargeBuff>(player);
	if (!powerUp)
		return;

	Rectangle &dimensions	= registry.get<Dimensions>(player).dimensions;
	Vector2 &position		= registry.get<Position>(player).position;

	powerUp->duration -= deltaTime;
	if (powerUp->duration <= 0) {
		registry.remove<EnlargeBuff>(player);
		dimensions.width = powerUp->originalWidth;
		return ;
	}

	if (dimensions.width == powerUp->newWidth)
		return;
	position.x -= (powerUp->newWidth / 2) - (dimensions.width / 2);
	dimensions.width = powerUp->newWidth;
	if (position.x < screenData.topLeft.x + 12)
		position.x = screenData.topLeft.x + 12;
	if (position.x > screenData.topLeft.x + screenData.playableArea.x - 12 - dimensions.width)
		position.x = screenData.topLeft.x + screenData.playableArea.x - 12 - dimensions.width;
	dimensions.x = position.x;
}

