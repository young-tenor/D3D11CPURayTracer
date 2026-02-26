#pragma once
#include "Mesh.h"
#include "Triangle.h"

class Rect : public Mesh {
public:
	Rect(DirectX::SimpleMath::Vector3 v0, DirectX::SimpleMath::Vector3 v1, DirectX::SimpleMath::Vector3 v2, DirectX::SimpleMath::Vector3 v3);
	virtual ~Rect() override = default;

	virtual Hit intersect(Ray ray) override;

public:
	Triangle triangle1;
	Triangle triangle2;
};

