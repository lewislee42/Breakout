

#pragma once

# include "raylib.h"
# include "entt/entt.hpp"

# include "MiscComponents.hpp"
# include "Player.hpp"
# include "Ball.hpp"
# include "Blocks.hpp"
# include "PowerUps.hpp"

# include <string>

class Game {
public:
	entt::registry	registry;

	Game(int screenWidth, int screenHeight);
	~Game();

	void	Run();

private:
	ScreenData screenData;

	Shader			shader;
	RenderTexture2D	targetTexture;
};
