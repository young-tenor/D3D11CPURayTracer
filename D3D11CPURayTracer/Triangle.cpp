#include "pch.h"
#include "Triangle.h"

Triangle::Triangle(DirectX::SimpleMath::Vector3 v0, DirectX::SimpleMath::Vector3 v1, DirectX::SimpleMath::Vector3 v2)
    : Mesh()
    , v0(v0)
    , v1(v1)
    , v2(v2) {
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
    auto normal0 = (v1 - pos).Cross(v2 - pos);
    auto normal1 = (v2 - pos).Cross(v0 - pos);
    auto normal2 = (v0 - pos).Cross(v1 - pos);
    if (normal0.Dot(face_normal) < 0.0f || normal1.Dot(face_normal) < 0.0f || normal2.Dot(face_normal) < 0.0f) {
        return hit;
    }

    hit.pos = pos;
    hit.d = t;
    hit.normal = face_normal;

    return hit;
}
