#pragma once
#include <glm/glm.hpp>
#include <string>

class Texture {
public:
	Texture(const int width, const int height);
	Texture(const std::string &filename);

	glm::vec3 clamped_color(int i, int j);
	glm::vec3 wrapped_color(int i, int j);
	glm::vec3 sample_point(const glm::vec2 &uv, const bool wrap);
	glm::vec3 sample_linear(const glm::vec2 &uv, const bool wrap);

public:
	int width, height;
	std::vector<glm::vec3> image;
};

