

# pragma once

# include "raylib.h"
# include "raymath.h"
# include "entt/entt.hpp"

# include "MiscComponents.hpp"
# include "Utils.hpp"

void	RenderUI(entt::registry &registry, ScreenData screenData);
void	RenderWorld(entt::registry &registry);
