#include "pch.h"
#include "Sphere.h"

// https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
Hit Sphere::intersect(const Ray &ray) {
	Hit hit;

	const float a = glm::dot(ray.dir, ray.dir);
	const float b = 2.0f * glm::dot(ray.dir, ray.orig - center);
	const float c = glm::dot(ray.orig - center, ray.orig - center) - radius * radius;

	const float nabla = b * b - 4.0f * a * c;
	if (nabla < 0.0f) {
		return hit;
	}

	hit.d = (-b - std::sqrt(nabla)) / (2.0f * a);
	if (hit.d < 0.0f) {
		hit.d = (-b + std::sqrt(nabla)) / (2.0f * a);
	}
	hit.pos = ray.orig + hit.d * ray.dir;
	hit.normal = glm::normalize(hit.pos - center);
	hit.obj = this;

	return hit;
}