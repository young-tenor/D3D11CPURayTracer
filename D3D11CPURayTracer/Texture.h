#pragma once
#include <glm/glm.hpp>

class Texture {
public:
	Texture(float width, float height);

	glm::vec3 sample_point(glm::vec2 uv);

public:
	float width, height;
	std::vector<glm::vec3> image;
};

