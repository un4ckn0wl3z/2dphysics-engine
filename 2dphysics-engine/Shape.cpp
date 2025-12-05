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

BoxShape::BoxShape(float width, float height)
{
	// TODO:
	this->width = width;
	this->height = height;
}

BoxShape::~BoxShape()
{
	// TODO:
}

ShapeType BoxShape::GetType() const
{
	return BOX;
}
