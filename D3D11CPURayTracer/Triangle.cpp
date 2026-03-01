#include "pch.h"
#include "Triangle.h"

Hit Triangle::intersect(const Ray &ray) {
	Hit hit;

	const auto face_normal = glm::normalize(glm::cross(v1 - v0, v2 - v1));
	if (glm::dot(-ray.dir, face_normal) < 0.0f || std::abs(glm::dot(ray.dir, face_normal)) < 1e-3f) {
		return hit;
	}

	const auto t = (glm::dot(v0, face_normal) - glm::dot(ray.orig, face_normal)) / glm::dot(ray.dir, face_normal);
	if (t < 0.0f) {
		return hit;
	}

	const auto pos = ray.orig + t * ray.dir;
	const auto cross0 = glm::cross(v1 - pos, v2 - pos);
	const auto cross1 = glm::cross(v2 - pos, v0 - pos);
	const auto cross2 = glm::cross(v0 - pos, v1 - pos);
	if (glm::dot(cross0, face_normal) < 0.0f || glm::dot(cross1, face_normal) < 0.0f || glm::dot(cross2, face_normal) < 0.0f) {
		return hit;
	}

	hit.pos = pos;
	hit.d = t;
	hit.normal = face_normal;

	const float area0 = glm::length(cross0);
	const float area1 = glm::length(cross1);
	const float area2 = glm::length(cross2);
	const float area_sum = area0 + area1 + area2;
	hit.uv = (area0 * uv0 + area1 * uv1 + area2 * uv2) / area_sum;

	hit.obj = this;

	return hit;
}
