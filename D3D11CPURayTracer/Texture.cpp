#include "pch.h"
#include "Texture.h"

Texture::Texture(float width, float height) : width(width), height(height) {
	image.resize(width * height);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < height; j++) {
			if ((i + j) % 2 == 0) {
				image[i * width + j] = glm::vec3(0.0f);
			} else {
				image[i * width + j] = glm::vec3(1.0f);
			}
		}
	}
}

glm::vec3 Texture::sample_point(glm::vec2 uv) {
	auto xy = uv * glm::vec2(width, height) - glm::vec2(0.5f);
	int i = std::round(xy.y);
	int j = std::round(xy.x);

	i = std::clamp(i, 0, 3);
	j = std::clamp(j, 0, 3);

	auto color = image[i * width + j];
	return color;
}
