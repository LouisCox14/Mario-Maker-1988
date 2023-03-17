#include "PhysicsObject.h"
#include "Constants.h"
#include "Commons.h"
#include "GameScreen.h"
#include <vector>
#include <iostream>

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
	velocity.y += GRAVITY * gravityScale * deltaTime;
}

void PhysicsObject::ResolveCollisions(const std::vector<Tile*>& tileMap)
{
	coll.position = position + (coll.size / 2);
	
	for (auto& a : sidesColliding) a = NONE;

	for (Tile* tile : tileMap)
	{
		if (tile->hasCollider)
		{
			if (coll.OverlapCheck(tile->coll))
			{
				switch (coll.GetCollisionSide(tile->coll))
				{
				case TOP:
					velocity.y = std::max(velocity.y, 0.0f);
					sidesColliding[(int)TOP] = TOP;
					break;
				case BOTTOM:
					velocity.y = std::min(velocity.y, 0.0f);
					sidesColliding[(int)BOTTOM] = BOTTOM;
					break;
				case RIGHT:
					velocity.x = std::min(velocity.x, 0.0f);
					sidesColliding[(int)RIGHT] = RIGHT;
					break;
				case LEFT:
					velocity.x = std::max(velocity.x, 0.0f);
					sidesColliding[(int)LEFT] = LEFT;
					break;
				}
			}
		}
	}
}

void PhysicsObject::UpdatePhysics(float deltaTime, const std::vector<Tile*>& tileMap)
{
	AddGravity(deltaTime);
	ResolveCollisions(tileMap);

	velocity += (velocity * -1) * drag * deltaTime;
	position += velocity * deltaTime;
}
