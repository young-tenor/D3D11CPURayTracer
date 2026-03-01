#pragma once
#include <glm/glm.hpp>

class Object;

class Hit {
public:
	float d = -1.0f;
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
	Object *obj = nullptr;
};