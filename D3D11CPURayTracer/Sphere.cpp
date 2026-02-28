#include "pch.h"
#include "Sphere.h"

Sphere::Sphere(float radius, glm::vec3 center)
	: Object()
	, radius(radius)
	, center(center) {
}

Sphere::Sphere(float radius, glm::vec3 center, glm::vec3 ambient)
	: Object(ambient)
	, radius(radius)
	, center(center) {
}

// https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
Hit Sphere::intersect(Ray ray) {
	Hit hit(-1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

	float a = glm::dot(ray.dir, ray.dir);
	float b = 2.0f * glm::dot(ray.dir, ray.orig - center);
	float c = glm::dot(ray.orig - center, ray.orig - center) - radius * radius;

	float nabla = b * b - 4.0f * a * c;
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