#pragma once
#include "Object.h"

class Sphere : public Object {
public:
	~Sphere() override = default;

	Hit Intersect(const Ray &ray) override;

public:
	float radius = 0.0f;
	glm::vec3 center;
};