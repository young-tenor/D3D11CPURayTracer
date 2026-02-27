#pragma once

class Ray {
public:
	Ray(glm::vec3 orig, glm::vec3 dir);

public:
	glm::vec3 orig;
	glm::vec3 dir;
};