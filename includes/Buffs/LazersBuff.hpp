

# pragma once

# include "raylib.h"
# include "entt/entt.hpp"

# include "MiscComponents.hpp"
# include "Utils.hpp"


/* ------------ COMPONENT ------------ */
struct LazersBuff {
	float duration = {0};
	float cooldown = {0};
	float currentCooldown = {0};
};


/* ------------ HANDLER ------------ */
void	HandleAddLazersBuff(entt::registry &registry, entt::entity player);


/* ------------ SYSTEM ------------ */
void	LazerBuffSystem(entt::registry &registry, float deltaTime, entt::entity player);
