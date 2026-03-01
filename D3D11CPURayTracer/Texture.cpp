#include "pch.h"
#include "Texture.h"

Texture::Texture(const int width, const int height) : width(width), height(height) {
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
	i = ((i % height) + height) % height;
	j = ((j % width) + width) % width;
	return image[i * width + j];
}

glm::vec3 Texture::sample_point(const glm::vec2 &uv, const bool wrap) {
	auto xy = uv * glm::vec2(width, height) - glm::vec2(0.5f);
	int i = std::round(xy.y);
	int j = std::round(xy.x);
	glm::vec3 color;
	if (wrap) {
		color = wrapped_color(i, j);
	} else {
		color = clamped_color(i, j);
	}
	return color;
}

glm::vec3 Texture::sample_linear(const glm::vec2 &uv, const bool wrap) {
	auto xy = uv * glm::vec2(width, height) - glm::vec2(0.5f);
	int i = std::floor(xy.y);
	int j = std::floor(xy.x);
	float dy = xy.y - i;
	float dx = xy.x - j;

	glm::vec3 c00, c10, c01, c11;
	if (wrap) {
		c00 = wrapped_color(i, j);
		c01 = wrapped_color(i, j + 1);
		c10 = wrapped_color(i + 1, j);
		c11 = wrapped_color(i + 1, j + 1);
	} else {
		c00 = clamped_color(i, j);
		c01 = clamped_color(i, j + 1);
		c10 = clamped_color(i + 1, j);
		c11 = clamped_color(i + 1, j + 1);
	}

	auto top = c00 * (1.0f - dx) + c01 * dx;
	auto bot = c10 * (1.0f - dx) + c11 * dx;
	auto color = top * (1.0f - dy) + bot * dy;

	return color;
}
