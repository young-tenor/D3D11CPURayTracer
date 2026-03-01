#pragma once

class Light {
public:
	Light(const float strength, const glm::vec3 &pos);

public:
	float strength = 1.0f;
	glm::vec3 pos;
};

