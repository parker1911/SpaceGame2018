#pragma once

#include <glm/vec3.hpp> // glm::vec3
#include <glm/glm.hpp> // vec3 normalize reflect dot pow
class Ray
{
public:
	Ray();
	~Ray();
	glm::vec2 raySphereIntersect(glm::vec3 r0, glm::vec3 rd, glm::vec3 s0, float sr);
};

