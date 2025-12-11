#pragma once

#include "Vec2.h"
#include <vector>

enum ShapeType {
	CIRCLE,
	POLYGON,
	BOX
};

struct Shape {

	virtual ~Shape() = default;
	virtual ShapeType GetType() const = 0;
	virtual Shape* Clone() const = 0;
	virtual float GetMomentOfInertia() const = 0;

};

struct CircleShape : public Shape {
	float radius;
	CircleShape(const float radius);
	virtual ~CircleShape();
	ShapeType GetType() const override;
	Shape* Clone() const override;
	float GetMomentOfInertia() const override;
};

struct PolygonShape : public Shape {
	std::vector<Vec2> localVertices;
	std::vector<Vec2> worldVertices;

	PolygonShape() = default;
	PolygonShape(const std::vector<Vec2> vertices);
	virtual ~PolygonShape();
	ShapeType GetType() const override;
	Shape* Clone() const override;
	Vec2 EdgeAt(int index) const;
	float GetMomentOfInertia() const override;

	// translate polygon vertices from local space to world space
	virtual void UpdateVertices(float angle, const Vec2& position);
};

struct BoxShape : public PolygonShape {
	float width;
	float height;
	BoxShape() = default;
	BoxShape(float width = 0.0, float height = 0.0);
	virtual ~BoxShape();
	ShapeType GetType() const override;
	Shape* Clone() const override;
	float GetMomentOfInertia() const override;


};