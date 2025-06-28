

# pragma once

# include "raylib.h"
# include "entt/entt.hpp"

# include "MiscComponents.hpp"
# include "Utils.hpp"


/* ------------ COMPONENT ------------ */
struct EnlargeBuff {
	float duration = {0};
	float originalWidth = {0};
	float newWidth = {0};
};


/* ------------ HANDLER ------------ */
void	HandleAddEnlargeBuff(entt::registry &registry, entt::entity player, float dimensionsWidth);


/* ------------ SYSTEM ------------ */
void	EnlargeBuffSystem(entt::registry &registry, float deltaTime, entt::entity player, ScreenData screenData);

