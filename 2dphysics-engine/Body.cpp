#include "Body.h"
#include <iostream>

Body::Body(float x, float y, float mass)
{
	this->position = Vec2(x, y);
	this->mass = mass;

	if (this->mass != 0.0) {
		this->invMass = 1.0 / this->mass;
	}
	else {
		this->invMass = 0.0;
	}

	std::cout << "Body constructor called!" << std::endl;
}

Body::~Body()
{
	std::cout << "Body destructor called!" << std::endl;
}

void Body::AddForce(const Vec2& force)
{
	this->sumForces += force;
}

void Body::ClearForces()
{
	this->sumForces = Vec2(0.0, 0.0);
}

void Body::Integrate(float dt)
{
	// find the acceleration based on the forces and the mass
	this->acceleration = this->sumForces * this->invMass;

	this->velocity += this->acceleration * dt;
	this->position += this->velocity * dt;

	ClearForces();
}

