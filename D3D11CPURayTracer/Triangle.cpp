#include "pch.h"
#include "Triangle.h"

Triangle::Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
	: Object()
	, v0(v0)
	, v1(v1)
	, v2(v2) {
}

Triangle::Triangle(
	glm::vec3 v0,
	glm::vec3 v1,
	glm::vec3 v2,
	glm::vec2 uv0,
	glm::vec2 uv1,
	glm::vec2 uv2)
	: Object()
	, v0(v0)
	, v1(v1)
	, v2(v2)
	, uv0(uv0)
	, uv1(uv1)
	, uv2(uv2) {
}

Hit Triangle::intersect(Ray ray) {
	Hit hit(-1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

	auto face_normal = glm::normalize(glm::cross(v1 - v0, v2 - v1));
	if (glm::dot(-ray.dir, face_normal) < 0.0f || std::abs(glm::dot(ray.dir, face_normal)) < 1e-3f) {
		return hit;
	}

	auto t = (glm::dot(v0, face_normal) - glm::dot(ray.orig, face_normal)) / glm::dot(ray.dir, face_normal);
	if (t < 0.0f) {
		return hit;
	}

	auto pos = ray.orig + t * ray.dir;
	auto cross0 = glm::cross(v1 - pos, v2 - pos);
	auto cross1 = glm::cross(v2 - pos, v0 - pos);
	auto cross2 = glm::cross(v0 - pos, v1 - pos);
	if (glm::dot(cross0, face_normal) < 0.0f || glm::dot(cross1, face_normal) < 0.0f || glm::dot(cross2, face_normal) < 0.0f) {
		return hit;
	}

	hit.pos = pos;
	hit.d = t;
	hit.normal = face_normal;

	auto area0 = glm::length(cross0);
	auto area1 = glm::length(cross1);
	auto area2 = glm::length(cross2);
	auto area_sum = area0 + area1 + area2;
	hit.uv = (area0 * uv0 + area1 * uv1 + area2 * uv2) / area_sum;

	hit.obj = this;

	return hit;
}
