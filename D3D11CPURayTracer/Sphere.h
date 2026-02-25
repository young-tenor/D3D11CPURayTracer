#pragma once
#include <directxtk/SimpleMath.h>
#include "Hit.h"
#include "Ray.h"

class Sphere {
public:
	Sphere(float radius, DirectX::SimpleMath::Vector3 center, DirectX::SimpleMath::Vector4 color);

	Hit intersect(Ray ray);

public:
	float radius = 0.0f;
	DirectX::SimpleMath::Vector3 center;
	DirectX::SimpleMath::Vector4 color;
};