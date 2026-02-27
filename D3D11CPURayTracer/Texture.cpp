#include "pch.h"
#include "Texture.h"

Texture::Texture(int width, int height) : width(width), height(height) {
	image.resize(width * height);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if ((i + j) % 2 == 0) {
				image[i * width + j] = glm::vec3(0.0f);
			} else {
				image[i * width + j] = glm::vec3(1.0f);
			}
		}
	}
}

glm::vec3 Texture::clamped_color(int i, int j) {
	i = std::clamp(i, 0, height - 1);
	j = std::clamp(j, 0, width - 1);
	return image[i * width + j];
}

glm::vec3 Texture::wrapped_color(int i, int j) {
	return glm::vec3();
}

glm::vec3 Texture::sample_point(glm::vec2 uv) {
	auto xy = uv * glm::vec2(width, height) - glm::vec2(0.5f);
	int i = std::round(xy.y);
	int j = std::round(xy.x);
	auto color = clamped_color(i, j);
	return color;
}

glm::vec3 Texture::sample_linear(glm::vec2 uv) {
	auto xy = uv * glm::vec2(width, height) - glm::vec2(0.5f);
	int i = std::floor(xy.y);
	int j = std::floor(xy.x);
	float dy = xy.y - i;
	float dx = xy.x - j;

	auto top = clamped_color(i, j) * (1.0f - dx) + clamped_color(i, j + 1) * dx;
	auto bot = clamped_color(i + 1, j) * (1.0f - dx) + clamped_color(i + 1, j + 1) * dx;
	auto color = top * (1.0f - dy) + bot * dy;

	return color;
}
