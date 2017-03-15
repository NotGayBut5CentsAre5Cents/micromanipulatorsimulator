#include "stdafx.h"
#include "Node.h"
#include "World.h"
#include "globals.h"
#include <ctime>

Node::Node(Vec2 pos, float size, float friction, float restitution, float mass, bool gravity) : 
	pos(pos), size(size), friction(friction), restitution(restitution), mass(mass), gravityForce(gravity ? Vec2(0, gravity_constant) : Vec2(0,0))
{
}

Node::~Node()
{
}

void Node::CollisionDetector()
{
	Ground* ground = World::ground;
	for (int i = 0; i < World::ground->points.size() - 1; i++)
	{
		bool inside1 = pointCircleCollision(ground->points[i], pos, size);
		bool inside2 = pointCircleCollision(ground->points[i + 1], pos, size);
		if (inside1 || inside2)
		{
			Vec2 direction = Vec2::Normalize(pos - ground->points[i]);
			vel = vel - direction * (Vec2::Dot(vel, direction)) * 2 * restitution; // mirror vector http://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
			forces += direction * mass * 0.1;
			continue;
		}


		float len = Vec2::Distance(ground->points[i], ground->points[i + 1]);
		float dot = (((pos.x - ground->points[i].x)*(ground->points[i + 1].x - ground->points[i].x)) + ((pos.y - ground->points[i].y)*(ground->points[i + 1].y - ground->points[i].y))) / pow(len, 2);
		float closestX = ground->points[i].x + (dot * (ground->points[i + 1].x - ground->points[i].x));
		float closestY = ground->points[i].y + (dot * (ground->points[i + 1].y - ground->points[i].y));
		Vec2 closestPoint = Vec2(closestX, closestY);
		if (!linePointCollision(ground->points[i], ground->points[i + 1], closestPoint, len))
		{
			continue;
		}
		float closestDist = Vec2::Distance(closestPoint, pos);
		if (closestDist <= size)
		{
			Vec2 direction = Vec2::Normalize(pos - closestPoint);
			vel = vel - direction * (Vec2::Dot(vel, direction)) * 2 * restitution; // mirror vector http://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
			forces += direction * mass * gravity_constant;
		}
	}
} 

bool Node::linePointCollision(Vec2 point, Vec2 point2, Vec2 closestPoint, float len)
{
	float dist1 = Vec2::Distance(point, closestPoint);
	float dist2 = Vec2::Distance(point2, closestPoint);

	float epsilon = 0.1f;

	if (dist1 + dist2 >= len - epsilon && dist1 + dist2 <= len + epsilon) {
		return true;
	}
	return false;
}

bool Node::pointCircleCollision(Vec2 point, Vec2 circle, float size)
{
	float distance = Vec2::Distance(point, circle);
	if (distance <= size
	) {
		return true;
	}
	return false;
}