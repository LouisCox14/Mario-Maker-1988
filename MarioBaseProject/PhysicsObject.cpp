#include "PhysicsObject.h"
#include "Constants.h"
#include "Commons.h"

PhysicsObject::PhysicsObject(Vector2D _startPosition, Vector2D _collSize, Vector2D _drag, float _gravityScale)
{
	position = _startPosition;
	drag = _drag;
	gravityScale = _gravityScale;

	velocity = Vector2D(0, 0);
	coll = BoxCollider(_startPosition, _collSize);
}

PhysicsObject::~PhysicsObject()
{
	coll = BoxCollider(Vector2D(0, 0), Vector2D(0, 0));
	gravityScale = 0;
	velocity = Vector2D(0, 0);
}

void PhysicsObject::AddGravity(float deltaTime)
{
	if (position.y + coll.size.y < SCREEN_HEIGHT)
	{
		velocity.y += GRAVITY * gravityScale * deltaTime;
	}
	else
	{
		velocity.y = std::min(velocity.y, 0.0f);
	}
}

void PhysicsObject::UpdatePhysics(float deltaTime)
{
	AddGravity(deltaTime);

	velocity += (velocity * -1) * drag * deltaTime;
	position += velocity * deltaTime;
}
