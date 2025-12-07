#include "Contact.h"

void Contact::ResolvePenetration()
{
	float da = (depth * b->mass) / (a->mass + b->mass);
	float db = (depth * a->mass) / (a->mass + b->mass);

	a->position -= normal * da;
	b->position += normal * db;
}
