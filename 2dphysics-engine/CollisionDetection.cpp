#include "CollisionDetection.h"
#include "Shape.h"

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact)
{
	bool aIsCircle = a->shape->GetType() == CIRCLE;
	bool bIsCircle = b->shape->GetType() == CIRCLE;

	bool aIsPolygon = a->shape->GetType() == POLYGON || a->shape->GetType() == BOX;
	bool bIsPolygon = b->shape->GetType() == POLYGON || b->shape->GetType() == BOX;;

	if (aIsCircle && bIsCircle)
	{
		return IsCollidingCircleCicle(a, b, contact);
	}

	if (aIsPolygon && bIsPolygon)
	{
		return IsCollidingPolygonPolygon(a, b, contact);
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
	contact.end = pa->position + contact.normal * a->radius;
	contact.depth = (contact.end - contact.start).Magnitude();

	return true;

}



bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact)
{
	const PolygonShape* aPolygonShape = dynamic_cast<PolygonShape*>(a->shape);
	const PolygonShape* bPolygonShape = dynamic_cast<PolygonShape*>(b->shape);
	Vec2 aAxis, bAxis;
	Vec2 aPoint, bPoint;

	float abSeparation = aPolygonShape->FindMinSeparation(bPolygonShape, aAxis, aPoint);

	if (abSeparation  >= 0) {
		return false;
	}

	float baSeparation = bPolygonShape->FindMinSeparation(aPolygonShape, bAxis, bPoint);

	if (baSeparation >= 0) {
		return false;
	}

	// populate the contact information
	contact.a = a;
	contact.b = b;
	if (abSeparation > baSeparation) {
		contact.depth = -abSeparation;
		contact.normal = aAxis.Normal();
		contact.start = aPoint;
		contact.end = contact.start + contact.normal * contact.depth;
	}
	else {
		contact.depth = -baSeparation;
		contact.normal = bAxis.Normal();
		contact.start = bPoint;
		contact.end = contact.start + contact.normal * contact.depth;
	}

	return true;
}

bool CollisionDetection::IsCollidingPolygonCircle(Body* a, Body* b, Contact& contact)
{
	return false;
}
