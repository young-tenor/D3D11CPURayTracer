#pragma once
#include <glm/glm.hpp>
#include <string>

class Texture {
public:
	Texture(const int width, const int height);
	Texture(const std::string &filename);

	glm::vec3 ClampedColor(int i, int j);
	glm::vec3 WrappedColor(int i, int j);
	glm::vec3 SamplePoint(const glm::vec2 &uv, const bool wrap);
	glm::vec3 SampleLinear(const glm::vec2 &uv, const bool wrap);

public:
	int width, height;
	std::vector<glm::vec3> image;
};

