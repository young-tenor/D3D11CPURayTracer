#pragma once
#include "Object.h"
#include "Triangle.h"

class Rect : public Object {
public:
	~Rect() override = default;

	void set_vertices(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);
	void set_uvs(const glm::vec2 &uv0, const glm::vec2 &uv1, const glm::vec2 &uv2, const glm::vec2 &uv3);

	Hit intersect(const Ray &ray) override;

public:
	Triangle triangle1;
	Triangle triangle2;
};

