

# pragma once

# include "raylib.h"
# include "entt/entt.hpp"

# include "MiscComponents.hpp"


/* ------------ COMPONENT ------------ */
struct ExtraLifeBuff {};


/* ------------ HANDLER ------------ */
void	HandleAddExtraLifeBuff(entt::registry &registry, entt::entity player);
