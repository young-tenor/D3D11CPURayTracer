#pragma once
#include <glm/glm.hpp>
#include "Hit.h"
#include "Ray.h"
#include "Texture.h"

class Object {
public:
	Object() = default;
	Object(glm::vec3 ambient);
	Object(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	virtual ~Object() = default;

	virtual Hit intersect(Ray ray) = 0;

public:
	glm::vec3 ambient = glm::vec3(0.1f);
	glm::vec3 diffuse = glm::vec3(0.7f);
	glm::vec3 specular = glm::vec3(0.5f);
	float shininess = 32.0f;
	Texture *texture = nullptr;
};