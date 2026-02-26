#pragma once
#include "Mesh.h"

class Sphere : public Mesh {
public:
	Sphere(float radius, DirectX::SimpleMath::Vector3 center);
	Sphere(float radius, DirectX::SimpleMath::Vector3 center, DirectX::SimpleMath::Vector3 ambient);
	virtual ~Sphere() override = default;

	virtual Hit intersect(Ray ray) override;

public:
	float radius = 0.0f;
	DirectX::SimpleMath::Vector3 center;
};