#pragma once
#include "Commons.h"
#include "Constants.h"

class PhysicsObject
{
	public:
		PhysicsObject(Vector2D _startPosition = Vector2D(0, 0), Vector2D _collSize = Vector2D(0, 0), Vector2D _drag = Vector2D(2, 2), float _gravityScale = 1);
		~PhysicsObject();

		void AddGravity(float deltaTime);
		void UpdatePhysics(float deltaTime);

		Vector2D position;
		Vector2D velocity;

		BoxCollider coll;

	protected:
		Vector2D oldPosition;
		Vector2D oldVelocity;

		Vector2D drag;
		float gravityScale;
};

