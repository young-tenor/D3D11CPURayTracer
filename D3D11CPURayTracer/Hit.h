#pragma once
#include <glm/glm.hpp>

class Object;

class Hit {
public:
	Hit(const float d, const glm::vec3 &pos);

public:
	float d = -1.0f;
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
	Object *obj = nullptr;
};