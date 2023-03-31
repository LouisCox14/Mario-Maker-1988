#pragma once
#include "Commons.h"
#include "Constants.h"
#include "Tile.h"
#include <vector>

class PhysicsObject
{
	public:
		PhysicsObject(Vector2D _startPosition = Vector2D(0, 0), float _screenScale = 1.0f, Vector2D _collSize = Vector2D(0, 0), Vector2D _drag = Vector2D(2, 2), float _gravityScale = 1);
		~PhysicsObject();

		void AddGravity(float deltaTime);
		void ResolveCollisions(const std::vector<Tile*>& tileMap);
		void UpdatePhysics(float deltaTime, const std::vector<Tile*>& tileMap);

		float screenScale;

		Vector2D position;
		Vector2D lastPosition;
		Vector2D velocity;

		Vector2D drag;

		BoxCollider coll;
		std::array<COLLISION_SIDES, 4> sidesColliding;

	protected:
		Vector2D oldPosition;
		Vector2D oldVelocity;

		float gravityScale;
};

