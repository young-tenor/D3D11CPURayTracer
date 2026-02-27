#include "pch.h"
#include "Triangle.h"

Triangle::Triangle(DirectX::SimpleMath::Vector3 v0, DirectX::SimpleMath::Vector3 v1, DirectX::SimpleMath::Vector3 v2)
	: Object()
	, v0(v0)
	, v1(v1)
	, v2(v2) {
}

Triangle::Triangle(
	DirectX::SimpleMath::Vector3 v0,
	DirectX::SimpleMath::Vector3 v1,
	DirectX::SimpleMath::Vector3 v2,
	DirectX::SimpleMath::Vector2 uv0,
	DirectX::SimpleMath::Vector2 uv1,
	DirectX::SimpleMath::Vector2 uv2
)
	: Object()
	, v0(v0)
	, v1(v1)
	, v2(v2)
	, uv0(uv0)
	, uv1(uv1)
	, uv2(uv2) {
}

Hit Triangle::intersect(Ray ray) {
	Hit hit(-1.0f, { 0.0f, 0.0f, 0.0f });

	auto face_normal = (v1 - v0).Cross(v2 - v1);
	face_normal.Normalize();
	if (-ray.dir.Dot(face_normal) < 0.0f || std::abs(ray.dir.Dot(face_normal)) < 1e-3f) {
		return hit;
	}

	auto t = (v0.Dot(face_normal) - ray.orig.Dot(face_normal)) / ray.dir.Dot(face_normal);
	if (t < 0.0f) {
		return hit;
	}

	auto pos = ray.orig + t * ray.dir;
	auto cross0 = (v1 - pos).Cross(v2 - pos);
	auto cross1 = (v2 - pos).Cross(v0 - pos);
	auto cross2 = (v0 - pos).Cross(v1 - pos);
	if (cross0.Dot(face_normal) < 0.0f || cross1.Dot(face_normal) < 0.0f || cross2.Dot(face_normal) < 0.0f) {
		return hit;
	}

	hit.pos = pos;
	hit.d = t;
	hit.normal = face_normal;

	auto area0 = cross0.Length();
	auto area1 = cross1.Length();
	auto area2 = cross2.Length();
	auto area_sum = area0 + area1 + area2;
	hit.uv = (area0 * uv0 + area1 * uv1 + area2 * uv2) / area_sum;

	return hit;
}
