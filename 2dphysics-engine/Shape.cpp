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
	return new PolygonShape(vertices);
}

float PolygonShape::GetMomentOfInertia() const
{
	// TODO:
	return 0.0f;
}

BoxShape::BoxShape(float width, float height)
{
	
	this->width = width;
	this->height = height;

	// load 4 vertices
	vertices.push_back(Vec2(-this->width / 2.0, -this->height / 2.0));
	vertices.push_back(Vec2(+this->width / 2.0, -this->height / 2.0));
	vertices.push_back(Vec2(+this->width / 2.0, +this->height / 2.0));
	vertices.push_back(Vec2(-this->width / 2.0, +this->height / 2.0));


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
