#include "PhysicsObject.h"
#include "Constants.h"
#include "Commons.h"
#include "GameScreen.h"
#include <vector>
#include <iostream>

PhysicsObject::PhysicsObject(Vector2D _startPosition, float _screenScale, Vector2D _collSize, Vector2D _drag, float _gravityScale)
{
	position = _startPosition;
	lastPosition = _startPosition;
	drag = _drag;
	gravityScale = _gravityScale;
	screenScale = _screenScale;

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
	sidesColliding = { NONE, NONE, NONE, NONE };
	coll.position = position + (coll.size / 2);

	for (Tile* tile : tileMap)
	{
		if (tile->hasCollider && Distance(tile->position, position) < TILE_RESOLUTION * CAMERA_SCALE * 2)
		{
			for (float dist = 0.0f; dist <= 1.0f; dist += std::min(2 / Distance(lastPosition, position), 1.0f))
			{
				coll.position = VectorLerp(lastPosition, position, dist) + (coll.size / 2);
				if (coll.OverlapCheck(tile->coll))
				{
					switch (coll.GetCollisionSide(tile->coll))
					{
					case TOP:
						velocity.y = std::max(velocity.y, 0.0f);
						position.y = std::max(position.y, coll.position.y - (coll.size.y / 2));
						sidesColliding[(int)TOP] = TOP;
						break;
					case BOTTOM:
						velocity.y = std::min(velocity.y, 0.0f);
						position.y = std::min(position.y, coll.position.y - (coll.size.y / 2));
						sidesColliding[(int)BOTTOM] = BOTTOM;
						break;
					case RIGHT:
						velocity.x = std::min(velocity.x, 0.0f);
						position.x = std::min(position.x, coll.position.x - (coll.size.x / 2));
						sidesColliding[(int)RIGHT] = RIGHT;
						break;
					case LEFT:
						velocity.x = std::max(velocity.x, 0.0f);
						position.x = std::max(position.x, coll.position.x - (coll.size.x / 2));
						sidesColliding[(int)LEFT] = LEFT;
						break;
					}
				}
			}
		}
	}

	coll.position = position + (coll.size / 2);
}

void PhysicsObject::UpdatePhysics(float deltaTime, const std::vector<Tile*>& tileMap)
{
	AddGravity(deltaTime);
	ResolveCollisions(tileMap);

	lastPosition = position;

	velocity += (velocity * -1) * drag * deltaTime;
	position += velocity * deltaTime * screenScale;
}
