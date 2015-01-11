#include "Angel.h"

#pragma once
struct Cube
{
	vec3 position;
	vec4 color;
	float scale;

	Cube(){ scale = 0.9f; }
	~Cube(){}
	Cube(Cube const& c)
	{
		this->position = c.position;
		this->color = c.color;
		this->scale = c.scale;
	}
	Cube(vec3 _position, vec4 _color=vec4(0,0,1,1), float _scale = 0.9f)
	{
		this->position = _position;
		this->color = _color;
		this->scale = _scale;
	}
	inline bool operator == (const Cube &other) const
	{
	return 
		other.position.x == this->position.x &&
		other.position.y == this->position.y &&
		other.position.z == this->position.z;
	}
};