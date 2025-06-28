

#pragma once

# include "raylib.h"
# include "raymath.h"
# include "entt/entt.hpp"

# include "MiscComponents.hpp"
# include "Utils.hpp"

# include "Buffs/DisruptionBuff.hpp"
# include "Buffs/EnlargeBuff.hpp"
# include "Buffs/ExtraLifeBuff.hpp"
# include "Buffs/LazersBuff.hpp"
# include "Buffs/SlowBuff.hpp"

#include <cstdlib>



/* ------------ STRUCT ------------ */
enum BuffsEnum {
	ENLARGE		= 0,
	DISRUPTION	= 1,
	EXTRALIFE	= 2,
	SLOW		= 3,
	LAZERS		= 4,
};


/* ------------ COMPONENT ------------ */
struct Buff {
	BuffsEnum type;
};


/* ------------ SPAWNER ------------ */
void	SpawnBuffs(entt::registry &registry, Vector2 position);


/* ------------ SYSTEMS ------------ */
void	UpdateBuffObjectsSystem(entt::registry &registry, float deltaTime, entt::entity player, float screenHeight);
void	UpdateBuffsSystem(entt::registry &registry, float deltaTime, entt::entity player, ScreenData screenData);
