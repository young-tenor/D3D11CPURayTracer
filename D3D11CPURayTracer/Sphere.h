#pragma once
#include "Object.h"

class Sphere : public Object {
public:
	Sphere(float radius, glm::vec3 center);
	Sphere(float radius, glm::vec3 center, glm::vec3 ambient);
	virtual ~Sphere() override = default;

	virtual Hit intersect(Ray ray) override;

public:
	float radius = 0.0f;
	glm::vec3 center;
};