#include "Particle.h"
#include <iostream>

Particle::Particle(float x, float y, float mass)
{
	this->position = Vec2(x, y);
	this->mass = mass;
	std::cout << "Particle constructor called!" << std::endl;
}

Particle::~Particle()
{
	std::cout << "Particle destructor called!" << std::endl;
}

void Particle::AddForce(const Vec2& force)
{
	this->sumForces += force;
}

void Particle::ClearForces()
{
	this->sumForces = Vec2(0.0, 0.0);
}

void Particle::Integrate(float dt)
{
	// find the acceleration based on the forces and the mass
	this->acceleration = this->sumForces / this->mass;

	this->velocity += this->acceleration * dt;
	this->position += this->velocity * dt;

	ClearForces();
}

