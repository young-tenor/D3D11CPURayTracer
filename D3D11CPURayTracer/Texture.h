#pragma once
#include <glm/glm.hpp>

class Texture {
public:
	Texture(int width, int height);

	glm::vec3 clamped_color(int i, int j);
	glm::vec3 wrapped_color(int i, int j);
	glm::vec3 sample_point(glm::vec2 uv);
	glm::vec3 sample_linear(glm::vec2 uv);

public:
	int width, height;
	std::vector<glm::vec3> image;
};

