#pragma once
#include "Object.h"

class Triangle : public Object {
public:
	Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
	Triangle(
		glm::vec3 v0,
		glm::vec3 v1,
		glm::vec3 v2,
		glm::vec2 uv0,
		glm::vec2 uv1,
		glm::vec2 uv2);
	~Triangle() override = default;

	Hit intersect(Ray ray) override;

public:
	glm::vec3 v0;
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec2 uv0;
	glm::vec2 uv1;
	glm::vec2 uv2;
};

