#include "CollisionDetection.h"
#include "Shape.h"

bool CollisionDetection::IsColliding(Body* a, Body* b)
{
	bool aIsCircle = a->shape->GetType() == CIRCLE;
	bool bIsCircle = b->shape->GetType() == CIRCLE;

	if (aIsCircle && bIsCircle)
	{
		return IsCollidingCircleCicle(a, b);
	}


}

bool CollisionDetection::IsCollidingCircleCicle(Body* pa, Body* pb)
{
	CircleShape* a = dynamic_cast<CircleShape*>(pa->shape);
	CircleShape* b = dynamic_cast<CircleShape*>(pb->shape);

	const Vec2 ab = pb->position - pa->position;
	const float abRadius = a->radius + b->radius;
	bool isColliding = ab.MagnitudeSquared() <= (abRadius * abRadius);

	return isColliding;
}

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b)
{
	return false;
}

bool CollisionDetection::IsCollidingPolygonCircle(Body* a, Body* b)
{
	return false;
}
