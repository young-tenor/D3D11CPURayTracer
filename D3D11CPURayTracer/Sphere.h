#pragma once
#include "Object.h"

class Sphere : public Object {
public:
	Sphere(const float radius, const glm::vec3 &center);
	Sphere(const float radius, const glm::vec3 &center, const glm::vec3 &ambient);
	~Sphere() override = default;

	Hit intersect(const Ray &ray) override;

public:
	float radius = 0.0f;
	glm::vec3 center;
};