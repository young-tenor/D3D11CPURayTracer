#include "pch.h"
#include "Cubemap.h"

bool Cubemap::Init(const std::string &nx, const std::string &px, const std::string &ny, const std::string &py, const std::string &nz, const std::string &pz) {
	this->nx = new Texture(nx);
	this->px = new Texture(px);
	this->ny = new Texture(ny);
	this->py = new Texture(py);
	this->nz = new Texture(nz);
	this->pz = new Texture(pz);
	if (!this->nx || !this->px || !this->ny || !this->py || !this->nz || !this->pz) {
		return false;
	}
	return true;
}

// https://en.wikipedia.org/wiki/Cube_mapping
glm::vec3 Cubemap::Sample(const glm::vec3 &dir) {
	glm::vec3 d = glm::normalize(dir);

	const float absX = glm::abs(d.x);
	const float absY = glm::abs(d.y);
	const float absZ = glm::abs(d.z);

	const bool xPositive = d.x > 0.0f;
	const bool yPositive = d.y > 0.0f;
	const bool zPositive = d.z > 0.0f;

	float maxAxis, uc, vc;
	Texture *target = nullptr;

	if (absX >= absY && absX >= absZ) {
		maxAxis = absX;
		if (xPositive) {
			uc = -d.z; 
			vc = d.y; 
			target = px;
		} else {
			uc = d.z; 
			vc = d.y; 
			target = nx;
		}
	} else if (absY >= absX && absY >= absZ) {
		maxAxis = absY;
		if (yPositive) {
			uc = d.x; 
			vc = -d.z; 
			target = py;
		} else {
			uc = d.x; 
			vc = d.z; 
			target = ny;
		}
	} else {
		maxAxis = absZ;
		if (zPositive) {
			uc = d.x; 
			vc = d.y; 
			target = pz;
		} else {
			uc = -d.x; 
			vc = d.y; 
			target = nz;
		}
	}

	// [-max_axis, max_axis] -> [0, 1]
	const float u = 0.5f * (uc / maxAxis + 1.0f);
	const float v = 1.0f - 0.5f * (vc / maxAxis + 1.0f);

	if (!target) {
		return glm::vec3(0.0f);
	}
	return target->SampleLinear(glm::vec2(u, v), false);
}
