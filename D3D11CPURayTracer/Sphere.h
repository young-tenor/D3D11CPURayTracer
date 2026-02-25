#pragma once
#include <directxtk/SimpleMath.h>
#include "Hit.h"
#include "Ray.h"

class Sphere {
public:
	Sphere(float radius, DirectX::SimpleMath::Vector3 center);

	Hit intersect(Ray ray);

public:
	float radius = 0.0f;
	DirectX::SimpleMath::Vector3 center;

	DirectX::SimpleMath::Vector3 ambient = { 0.1f, 0.1f, 0.1f };
	DirectX::SimpleMath::Vector3 diffuse = { 0.7f, 0.7f, 0.7f };
	DirectX::SimpleMath::Vector3 specular = { 0.5f, 0.5f, 0.5f };
	float shininess = 32.0f;
};