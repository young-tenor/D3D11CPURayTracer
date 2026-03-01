#pragma once
#include "Object.h"
#include "Triangle.h"

class Rect : public Object {
public:
	Rect(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);
	Rect(
		const glm::vec3 &v0,
		const glm::vec3 &v1,
		const glm::vec3 &v2,
		const glm::vec3 &v3,
		const glm::vec3 &ambient,
		const glm::vec2 &uv0,
		const glm::vec2 &uv1,
		const glm::vec2 &uv2,
		const glm::vec2 &uv3);
	Rect(
		const glm::vec3 &v0,
		const glm::vec3 &v1, 
		const glm::vec3 &v2,
		const glm::vec3 &v3, 
		const glm::vec3 &ambient, 
		const glm::vec3 &diffuse,
		const glm::vec3 &specular,
		const glm::vec2 &uv0,
		const glm::vec2 &uv1,
		const glm::vec2 &uv2,
		const glm::vec2 &uv3);
	~Rect() override = default;

	Hit intersect(const Ray &ray) override;

public:
	Triangle triangle1;
	Triangle triangle2;
};

