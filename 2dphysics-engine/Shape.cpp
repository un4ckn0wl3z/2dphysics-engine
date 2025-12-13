#include "Shape.h"
#include <iostream>

CircleShape::CircleShape(const float radius)
{
	this->radius = radius;
	std::cout << "CircleShape constructor called!" << std::endl;
}

CircleShape::~CircleShape()
{
	std::cout << "CircleShape destructor called!" << std::endl;
}

ShapeType CircleShape::GetType() const
{
	return CIRCLE;
}

Shape* CircleShape::Clone() const
{
	return new CircleShape(radius);
}

float CircleShape::GetMomentOfInertia() const
{
	return 0.5 * (radius * radius);
}

PolygonShape::PolygonShape(const std::vector<Vec2> vertices)
{
	// TODO:
}

PolygonShape::~PolygonShape()
{
	// TODO:
}

ShapeType PolygonShape::GetType() const
{
	return POLYGON;
}

Shape* PolygonShape::Clone() const
{
	return new PolygonShape(localVertices);
}

Vec2 PolygonShape::EdgeAt(int index) const
{
	int currentVertex = index;
	int nextVertex = (index + 1) % worldVertices.size();
	return worldVertices[nextVertex] - worldVertices[currentVertex];
}

float PolygonShape::FindMinSeparation(const PolygonShape* other) const {
	float separation = std::numeric_limits<float>::lowest();
	for (int i = 0; i < this->worldVertices.size(); i++) {
		Vec2 va = this->worldVertices[i];
		Vec2 normal = this->EdgeAt(i).Normal();
		float minSep = std::numeric_limits<float>::max();

		for (int j = 0; j < other->worldVertices.size(); j++) {
			Vec2 vb = other->worldVertices[j];
			minSep = std::min(minSep, (vb - va).Dot(normal));
		}
		separation = std::max(separation, minSep);
	}
	return separation;
}

float PolygonShape::GetMomentOfInertia() const
{
	// TODO:
	return 0.0f;
}

void PolygonShape::UpdateVertices(float angle, const Vec2& position)
{
	for (int i = 0; i < localVertices.size(); i++) {
		// roate and translate to world 
		worldVertices[i] = localVertices[i].Rotate(angle);
		worldVertices[i] += position;

	}
}

BoxShape::BoxShape(float width, float height)
{
	
	this->width = width;
	this->height = height;

	// load 4 vertices
	localVertices.push_back(Vec2(-this->width / 2.0, -this->height / 2.0));
	localVertices.push_back(Vec2(+this->width / 2.0, -this->height / 2.0));
	localVertices.push_back(Vec2(+this->width / 2.0, +this->height / 2.0));
	localVertices.push_back(Vec2(-this->width / 2.0, +this->height / 2.0));

	worldVertices.push_back(Vec2(-this->width / 2.0, -this->height / 2.0));
	worldVertices.push_back(Vec2(+this->width / 2.0, -this->height / 2.0));
	worldVertices.push_back(Vec2(+this->width / 2.0, +this->height / 2.0));
	worldVertices.push_back(Vec2(-this->width / 2.0, +this->height / 2.0));


}

BoxShape::~BoxShape()
{
	std::cout << "BoxShape destructor called!" << std::endl;

}

ShapeType BoxShape::GetType() const
{
	return BOX;
}

Shape* BoxShape::Clone() const
{
	return new BoxShape(width, height);
}

float BoxShape::GetMomentOfInertia() const
{
	return (0.083333) * (width * width + height * height);
}
