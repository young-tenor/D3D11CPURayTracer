#pragma once

class Light {
public:
	Light(float strength, glm::vec3 pos);

public:
	float strength = 1.0f;
	glm::vec3 pos;
};

