

#pragma once

# include "raylib.h"
# include "entt/entt.hpp"

# include "MiscComponents.hpp"
# include "Player.hpp"
# include "Ball.hpp"
# include "Blocks.hpp"
# include "Buffs/Buffs.hpp"
# include "Render.hpp"
# include "Utils.hpp"
# include "Enemy.hpp"

# include <string>

# define TRANSITION_DURATION 3.0f
# define LEVEL_CHANGE_DELAY_DURATION 2.0f

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

	GameState 		gameState;
	GameState 		prevGameState;
	int				currentLevel;
	bool			shouldReset;
	float			transitionTimer;

	void	RunGame(entt::entity player, float deltaTime, Camera2D &cam);
	void	RunMenu(float deltaTime);
	void	RunTransition(entt::entity player, float deltaTime);
	void	CheckGameState(entt::entity player);
};
