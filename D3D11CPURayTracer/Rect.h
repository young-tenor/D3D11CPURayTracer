#pragma once
#include "Object.h"
#include "Triangle.h"

class Rect : public Object {
public:
	Rect(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
	Rect(
		glm::vec3 v0,
		glm::vec3 v1,
		glm::vec3 v2,
		glm::vec3 v3,
		glm::vec3 ambient,
		glm::vec2 uv0,
		glm::vec2 uv1,
		glm::vec2 uv2,
		glm::vec2 uv3);
	virtual ~Rect() override = default;

	virtual Hit intersect(Ray ray) override;

public:
	Triangle triangle1;
	Triangle triangle2;
	glm::vec2 u;
	glm::vec2 v;
};

