#include "pch.h"
#include "Rect.h"

Rect::Rect(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
	: Object()
	, triangle1(v0, v1, v3)
	, triangle2(v1, v2, v3) {
}

Rect::Rect(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 ambient, glm::vec2 uv0, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3)
	: Object(ambient)
	, triangle1(v0, v1, v3, uv0, uv1, uv3)
	, triangle2(v1, v2, v3, uv1, uv2, uv3) {
}

Rect::Rect(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec2 uv0, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3)
	: Object(ambient, diffuse, specular)
	, triangle1(v0, v1, v3, uv0, uv1, uv3)
	, triangle2(v1, v2, v3, uv1, uv2, uv3) {
}

Hit Rect::intersect(Ray ray) {
	auto hit1 = triangle1.intersect(ray);
	auto hit2 = triangle2.intersect(ray);
	Hit closer_hit(-1.0f, glm::vec3(0.0f, 0.0f, -1.0f));
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
