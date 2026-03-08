#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Texture.h"

class Cubemap {
public:
	bool Init(const std::string &nx, const std::string &px, const std::string &ny, const std::string &py, const std::string &nz, const std::string &pz);
	glm::vec3 Sample(const glm::vec3 &dir);

public:
	Texture *nx = nullptr;
	Texture *px = nullptr;
	Texture *ny = nullptr;
	Texture *py = nullptr;
	Texture *nz = nullptr;
	Texture *pz = nullptr;
};

