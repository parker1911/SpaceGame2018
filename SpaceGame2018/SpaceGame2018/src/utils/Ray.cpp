#include "Ray.h"
#include <iostream>


Ray::Ray()
{
}


Ray::~Ray()
{
}

//
//#include <iostream>
//
//
//int main()
//{
//	glm::vec3 rayOrigin(-5, -2, 0);
//	glm::vec3 rayDir(2, 2, 0);
//	glm::vec3 sphereOrigin(0, 0, 0);
//	float sphereRad = 3;
//
//	glm::vec2 intersec = raySphereIntersect(rayOrigin, rayDir, sphereOrigin, sphereRad);
//
//	if (intersec.x == -1)
//	{
//		std::cout << intersec.y << "ray doesn't hit sphere!! " << std::endl;
//
//	}
//	else
//	{
//		glm::vec3 insectpointNear = rayOrigin + rayDir * intersec.x;
//		std::cout << intersec.x << " " << insectpointNear.x << " " << insectpointNear.y << " " << insectpointNear.z << " " << std::endl;
//		glm::vec3 insectpointFar = rayOrigin + rayDir * intersec.y;
//		std::cout << intersec.y << " " << insectpointFar.x << " " << insectpointFar.y << " " << insectpointFar.z << " " << std::endl;
//
//	}
//
//
//	float rayCloses = intersec.x - intersec.y;
//
//	std::cout << " rayclose " << rayCloses;
//
//
//
//
//
//	std::getchar();
//
//
//	return 0;
//}



glm::vec2 Ray::raySphereIntersect(glm::vec3 r0, glm::vec3 rd, glm::vec3 s0, float sr)
{
	// - r0: ray origin
	// - rd: normalized ray direction
	// - s0: sphere center
	// - sr: sphere radius
	// - Returns distance from r0 to first intersecion with sphere,
	//   or -1.0 if no intersection.
	float a = dot(rd, rd);


	glm::vec3 s0_r0 = r0 - s0;
	float b = 2.0 * dot(rd, s0_r0);
	float c = dot(s0_r0, s0_r0) - (sr * sr);
	if (b*b - 4.0*a*c < 0.0) {
		return glm::vec2(-1, -1.0);
	}

	glm::vec2 intsec;

	intsec.x = (-b - sqrt((b*b) - 4.0*a*c)) / (2.0*a);
	intsec.y = (-b + sqrt((b*b) - 4.0*a*c)) / (2.0*a);

	std::cout << "testing raycaster";
	return intsec;
}



