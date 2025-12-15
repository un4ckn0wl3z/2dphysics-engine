#include "Body.h"
#include <iostream>
#include <math.h>

Body::Body(const Shape& shape, float x, float y, float mass)
{
	this->shape = shape.Clone();
	this->position = Vec2(x, y);
	this->mass = mass;
	this->velocity = Vec2(0, 0);
	this->acceleration = Vec2(0, 0);
	this->rotation = 0.0;
	this->angularVelocity = 0.0;
	this->angularAcceleration = 0.0;
	this->sumForces = Vec2(0, 0);
	this->sumTorque = 0.0;
	this->restitution = 1.0;

	if (this->mass != 0.0) {
		this->invMass = 1.0 / this->mass;
	}
	else {
		this->invMass = 0.0;
	}

	this->I = this->shape->GetMomentOfInertia() * this->mass;

	if (this->I != 0.0) {
		this->invI = 1.0 / this->I;
	}
	else {
		this->invI = 0.0;
	}

	std::cout << "Body constructor called!" << std::endl;
}

Body::~Body()
{
	delete shape;
	std::cout << "Body destructor called!" << std::endl;
}

bool Body::IsStatic() const
{
	const float epsilon = 0.005f;
	return fabs(this->invMass - 0.0) < epsilon;
}

void Body::AddForce(const Vec2& force)
{
	this->sumForces += force;
}

void Body::ClearForces()
{
	this->sumForces = Vec2(0.0, 0.0);
}

void Body::IntegrateLinear(float dt)
{
	if (this->IsStatic()) return;

	// find the acceleration based on the forces and the mass
	this->acceleration = this->sumForces * this->invMass;

	this->velocity += this->acceleration * dt;
	this->position += this->velocity * dt;

	ClearForces();
}

void Body::IntegrateAngular(float dt)
{
	if (this->IsStatic()) return;

	// find the acceleration based on the sumTorque and the I
	this->angularAcceleration = this->sumTorque * this->invI;

	this->angularVelocity += this->angularAcceleration * dt;
	this->rotation += this->angularVelocity * dt;

	ClearTorque();
}

void Body::AddTorque(float torque)
{
	this->sumTorque += torque;
}

void Body::ClearTorque()
{
	this->sumTorque = 0.0;
}

void Body::ApplyImpulse(const Vec2& j)
{
	if (this->IsStatic()) return;
	this->velocity += j * this->invMass;
}

void Body::ApplyImpulse(const Vec2& j, const Vec2& r)
{
	if (this->IsStatic()) return;
	this->velocity += j * this->invMass;
	this->angularVelocity = r.Cross(j) * invI;
}

void Body::Update(float dt)
{
	this->IntegrateLinear(dt);
	this->IntegrateAngular(dt);
	bool isPolygon =
		this->shape->GetType() == POLYGON ||
		this->shape->GetType() == BOX;

	if (isPolygon) {
		PolygonShape* polygon = dynamic_cast<PolygonShape*>(this->shape);
		polygon->UpdateVertices(this->rotation, this->position);
	}

}

