#pragma once
#include "Object.h"
#include "Triangle.h"

class Rect : public Object {
public:
	~Rect() override = default;

	void SetVertices(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);
	void SetUVs(const glm::vec2 &uv0, const glm::vec2 &uv1, const glm::vec2 &uv2, const glm::vec2 &uv3);

	Hit Intersect(const Ray &ray) override;

public:
	Triangle triangle1;
	Triangle triangle2;
};

