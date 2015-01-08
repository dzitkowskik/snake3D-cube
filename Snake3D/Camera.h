#include "Angel.h"

#pragma once
class Camera
{
public:
	vec3 eyePoint;
	vec3 up_eye;
	float aspect;

	Camera(){}
	Camera(vec3 eye, vec3 up, float asp)
		:eyePoint(eye),up_eye(up),aspect(asp){}
	~Camera(){}
};