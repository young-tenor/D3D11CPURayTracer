#include "pch.h"
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

Texture::Texture(const std::string &filename) {
	int channels;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 3);
	if (data) {
		image.resize(width * height);
		for (int i = 0; i < width * height; i++) {
			image[i] = glm::vec3(data[i * 3] / 255.0f, data[i * 3 + 1] / 255.0f, data[i * 3 + 2] / 255.0f);
		}
		stbi_image_free(data);
	}
}

glm::vec3 Texture::ClampedColor(int i, int j) {
	i = std::clamp(i, 0, height - 1);
	j = std::clamp(j, 0, width - 1);
	return image[i * width + j];
}

glm::vec3 Texture::WrappedColor(int i, int j) {
	i = ((i % height) + height) % height;
	j = ((j % width) + width) % width;
	return image[i * width + j];
}

glm::vec3 Texture::SamplePoint(const glm::vec2 &uv, const bool wrap) {
	const auto xy = uv * glm::vec2(width, height) - glm::vec2(0.5f);
	const int i = std::round(xy.y);
	const int j = std::round(xy.x);
	glm::vec3 color;
	if (wrap) {
		color = WrappedColor(i, j);
	} else {
		color = ClampedColor(i, j);
	}
	return color;
}

glm::vec3 Texture::SampleLinear(const glm::vec2 &uv, const bool wrap) {
	const auto xy = uv * glm::vec2(width, height) - glm::vec2(0.5f);
	const int i = std::floor(xy.y);
	const int j = std::floor(xy.x);
	const float dy = xy.y - i;
	const float dx = xy.x - j;

	glm::vec3 c00, c10, c01, c11;
	if (wrap) {
		c00 = WrappedColor(i, j);
		c01 = WrappedColor(i, j + 1);
		c10 = WrappedColor(i + 1, j);
		c11 = WrappedColor(i + 1, j + 1);
	} else {
		c00 = ClampedColor(i, j);
		c01 = ClampedColor(i, j + 1);
		c10 = ClampedColor(i + 1, j);
		c11 = ClampedColor(i + 1, j + 1);
	}

	//const auto top = c00 * (1.0f - dx) + c01 * dx;
	//const auto bot = c10 * (1.0f - dx) + c11 * dx;
	//const auto color = top * (1.0f - dy) + bot * dy;
	const auto top = glm::mix(c00, c01, dx);
	const auto bot = glm::mix(c10, c11, dx);
	const auto color = glm::mix(top, bot, dy);

	return color;
}
