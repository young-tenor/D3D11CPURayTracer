#include "pch.h"
#include "Rect.h"

Rect::Rect(DirectX::SimpleMath::Vector3 v0, DirectX::SimpleMath::Vector3 v1, DirectX::SimpleMath::Vector3 v2, DirectX::SimpleMath::Vector3 v3)
	: Object()
	, triangle1(v0, v1, v3)
	, triangle2(v1, v2, v3) {
}

Rect::Rect(
	DirectX::SimpleMath::Vector3 v0,
	DirectX::SimpleMath::Vector3 v1,
	DirectX::SimpleMath::Vector3 v2,
	DirectX::SimpleMath::Vector3 v3,
	DirectX::SimpleMath::Vector3 ambient,
	DirectX::SimpleMath::Vector2 uv0,
	DirectX::SimpleMath::Vector2 uv1,
	DirectX::SimpleMath::Vector2 uv2,
	DirectX::SimpleMath::Vector2 uv3
)
	: Object(ambient)
	, triangle1(v0, v1, v3, uv0, uv1, uv3)
	, triangle2(v1, v2, v3, uv1, uv2, uv3) {
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
