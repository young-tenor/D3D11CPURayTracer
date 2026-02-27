#pragma once

class Hit {
public:
	Hit(float d, glm::vec3 pos);

public:
	float d = -1.0f;
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};