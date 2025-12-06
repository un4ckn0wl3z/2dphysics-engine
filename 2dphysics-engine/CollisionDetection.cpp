#include "CollisionDetection.h"
#include "Shape.h"

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact)
{
	bool aIsCircle = a->shape->GetType() == CIRCLE;
	bool bIsCircle = b->shape->GetType() == CIRCLE;

	if (aIsCircle && bIsCircle)
	{
		return IsCollidingCircleCicle(a, b, contact);
	}


}

bool CollisionDetection::IsCollidingCircleCicle(Body* pa, Body* pb, Contact& contact)
{
	CircleShape* a = dynamic_cast<CircleShape*>(pa->shape);
	CircleShape* b = dynamic_cast<CircleShape*>(pb->shape);

	const Vec2 ab = pb->position - pa->position;
	const float abRadius = a->radius + b->radius;
	bool isColliding = ab.MagnitudeSquared() <= (abRadius * abRadius);

	if (!isColliding) {
		return false;
	}

	// find contact point information
	contact.a = pa;
	contact.b = pb;
	contact.normal = ab;
	contact.normal.Normalize();

	contact.start = pb->position - contact.normal * b->radius;
	contact.end = pa->position + contact.normal * b->radius;
	contact.depth = (contact.end - contact.start).Magnitude();

	return true;

}

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b)
{
	return false;
}

bool CollisionDetection::IsCollidingPolygonCircle(Body* a, Body* b)
{
	return false;
}
