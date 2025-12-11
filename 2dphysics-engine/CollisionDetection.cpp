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

float FindMinSeparation(const PolygonShape& a, const PolygonShape& b) {
	float separation = std::numeric_limits<float>::lowest();
	for (int i = 0; i < a.worldVertices.size(); i++) {
		Vec2 va = a.worldVertices[i];
		Vec2 normal = a.EdgeAt(i).Normal();
		float minSep = std::numeric_limits<float>::max();

		for (int j = 0; j < b.worldVertices.size(); j++) {
			Vec2 vb = b.worldVertices[j];
			minSep = std::min(minSep, (vb - va).Dot(normal));
		}
		separation = std::max(separation, minSep);
	}
	return separation;
}

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact)
{
	const PolygonShape* aPolygonShape = dynamic_cast<PolygonShape*>(a->shape);
	const PolygonShape* bPolygonShape = dynamic_cast<PolygonShape*>(b->shape);

	if (FindMinSeparation(*aPolygonShape, *bPolygonShape) >= 0) {
		return false;
	}


	if (FindMinSeparation(*bPolygonShape, *aPolygonShape) >= 0) {
		return false;
	}

	return true;
}

bool CollisionDetection::IsCollidingPolygonCircle(Body* a, Body* b, Contact& contact)
{
	return false;
}
