

# pragma once

# include "raylib.h"
# include "entt/entt.hpp"


/* ------------ COMPONENT ------------ */
struct SlowBuff {
	float duration = {0};
	float speed = {0};
};


/* ------------ HANDLER ------------ */
void	HandleAddSlowBuff(entt::registry &registry, entt::entity player);


/* ------------ HANDLER ------------ */
void	SlowBuffSystem(entt::registry &registry, float deltaTime, entt::entity player);
