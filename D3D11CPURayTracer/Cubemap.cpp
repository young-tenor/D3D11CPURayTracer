#include "pch.h"
#include "Cubemap.h"

bool Cubemap::init(const std::string &nx, const std::string &px, const std::string &ny, const std::string &py, const std::string &nz, const std::string &pz) {
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
glm::vec3 Cubemap::sample(const glm::vec3 &dir) {
	glm::vec3 d = glm::normalize(dir);

	auto abs_x = glm::abs(d.x);
	auto abs_y = glm::abs(d.y);
	auto abs_z = glm::abs(d.z);

	bool is_x_positive = d.x > 0.0f;
	bool is_y_positive = d.y > 0.0f;
	bool is_z_positive = d.z > 0.0f;

	float max_axis, uc, vc;
	Texture *target = nullptr;

	if (abs_x >= abs_y && abs_x >= abs_z) {
		max_axis = abs_x;
		if (is_x_positive) {
			uc = -d.z; 
			vc = d.y; 
			target = px;
		} else {
			uc = d.z; 
			vc = d.y; 
			target = nx;
		}
	} else if (abs_y >= abs_x && abs_y >= abs_z) {
		max_axis = abs_y;
		if (is_y_positive) {
			uc = d.x; 
			vc = -d.z; 
			target = py;
		} else {
			uc = d.x; 
			vc = d.z; 
			target = ny;
		}
	} else {
		max_axis = abs_z;
		if (is_z_positive) {
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
	auto u = 0.5f * (uc / max_axis + 1.0f);
	auto v = 1.0f - 0.5f * (vc / max_axis + 1.0f);

	if (!target) {
		return glm::vec3(0.0f);
	}
	return target->sample_linear(glm::vec2(u, v), false);
}
