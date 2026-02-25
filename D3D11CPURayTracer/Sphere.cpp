#include "pch.h"
#include "Sphere.h"

Sphere::Sphere(float radius, DirectX::SimpleMath::Vector3 center, DirectX::SimpleMath::Vector4 color)
	: radius(radius)
	, center(center)
	, color(color) {
}

// https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
Hit Sphere::intersect(Ray ray) {
	Hit hit(-1.0f, { 0.0f, 0.0f, 0.0f });

	float a = ray.dir.Dot(ray.dir);
	float b = 2.0f * ray.dir.Dot(ray.orig - center);
	float c = (ray.orig - center).Dot(ray.orig - center) - radius * radius;

	float nabla = b * b - 4.0f * a * c;
	if (nabla < 0.0f) {
		return hit;
	}

	hit.d = (-b - std::sqrt(nabla)) / (2.0f * a);
	if (hit.d < 0.0f) {
		hit.d = (-b + std::sqrt(nabla)) / (2.0f * a);
	}
	hit.pos = ray.orig + hit.d * ray.dir;

	return hit;
}

