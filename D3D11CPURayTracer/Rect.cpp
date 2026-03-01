#include "pch.h"
#include "Rect.h"

void Rect::set_vertices(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3) {
	triangle1.v0 = v0;
	triangle1.v1 = v1;
	triangle1.v2 = v3;

	triangle2.v0 = v1;
	triangle2.v1 = v2;
	triangle2.v2 = v3;
}

void Rect::set_uvs(const glm::vec2 &uv0, const glm::vec2 &uv1, const glm::vec2 &uv2, const glm::vec2 &uv3) {
	triangle1.uv0 = uv0;
	triangle1.uv1 = uv1;
	triangle1.uv2 = uv3;

	triangle2.uv0 = uv1;
	triangle2.uv1 = uv2;
	triangle2.uv2 = uv3;
}

Hit Rect::intersect(const Ray &ray) {
	const auto hit1 = triangle1.intersect(ray);
	const auto hit2 = triangle2.intersect(ray);
	Hit closer_hit;
	if ((hit1.d >= 0.0f && (hit2.d < 0.0f || hit1.d < hit2.d))) {
		closer_hit = hit1;
	} else {
		closer_hit = hit2;
	}

	if (closer_hit.d >= 0.0f) {
		closer_hit.obj = this;
	}

	return closer_hit;
}
