#include "Angel.h"

inline vec3 toCartesian(vec3 spherical)
{
	float radius = spherical.x;
	float theta = spherical.y;
	float phi = spherical.z;
	float X = cos(theta) * cos(phi) * radius;
	float Y = sin(theta) * cos(phi) * radius;
	float Z = sin(phi) * radius;
	return vec3(X, Y, Z);
}

inline vec3 toSpherical(vec3 cartesian)
{
	float X = cartesian.x;
	float Y = cartesian.y;
	float Z = cartesian.z;
	float radius = sqrt(X * X + Y * Y + Z * Z);
	float theta = atan2(Y, X);
	float phi = acos(Z / radius);
	return vec3(radius, theta, phi);
}
