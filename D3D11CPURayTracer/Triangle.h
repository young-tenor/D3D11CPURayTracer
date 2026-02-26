#pragma once
#include "Mesh.h"

class Triangle : public Mesh {
public:
	Triangle(DirectX::SimpleMath::Vector3 v0, DirectX::SimpleMath::Vector3 v1, DirectX::SimpleMath::Vector3 v2);
	virtual ~Triangle() override = default;

	virtual Hit intersect(Ray ray) override;

public:
	DirectX::SimpleMath::Vector3 v0;
	DirectX::SimpleMath::Vector3 v1;
	DirectX::SimpleMath::Vector3 v2;
};

