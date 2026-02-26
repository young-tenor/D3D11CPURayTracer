#include "pch.h"
#include "Rect.h"

Rect::Rect(DirectX::SimpleMath::Vector3 v0, DirectX::SimpleMath::Vector3 v1, DirectX::SimpleMath::Vector3 v2, DirectX::SimpleMath::Vector3 v3)
	: Mesh()
	, triangle1(v0, v1, v3)
	, triangle2(v1, v2, v3) {
}

Hit Rect::intersect(Ray ray) {
	Hit hit(-1.0f, { 0.0f, 0.0f, 0.0f });

	hit = triangle1.intersect(ray);
	if (hit.d >= 0.0f) {
		return hit;
	}
	hit = triangle2.intersect(ray);
	if (hit.d >= 0.0f) {
		return hit;
	}

	return hit;
}
