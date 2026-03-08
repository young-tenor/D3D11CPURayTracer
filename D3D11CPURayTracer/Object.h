#pragma once
#include <glm/glm.hpp>
#include "Hit.h"
#include "Ray.h"
#include "Texture.h"

class Object {
public:
	virtual ~Object() = default;

	virtual Hit Intersect(const Ray &ray) = 0;

public:
	glm::vec3 ambient = glm::vec3(0.1f);
	glm::vec3 diffuse = glm::vec3(0.7f);
	glm::vec3 specular = glm::vec3(0.5f);
	float shininess = 32.0f;
	float reflection = 0.0f;
	Texture *texture = nullptr;
};