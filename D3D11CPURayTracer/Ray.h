#pragma once

class Ray {
public:
	Ray(const glm::vec3 &orig, const glm::vec3 &dir);

public:
	glm::vec3 orig;
	glm::vec3 dir;
};