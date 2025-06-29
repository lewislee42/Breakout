

#pragma once

# include "entt/entt.hpp"
# include "raylib.h"

# include <iostream>

struct CameraComponent {
	Camera2D	camera = {0};
	
	CameraComponent(Vector2 position, Vector2 offset) {
		camera.offset	= offset;
		camera.target	= position;
		camera.zoom		= 1.0f;
	}
};

struct Position {
	Vector2	position = {0};
};

struct Velocity {
	Vector2 velocity	= {0};
	float	speed		= {0};

	Velocity(Vector2 velocity, float speed): speed(speed), velocity(velocity) {}
};

struct Dimensions {
	Rectangle	dimensions;
};

struct Points {
	int points;
};

struct Health {
	int health;
};
