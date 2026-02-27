#pragma once
#include "Object.h"

class Triangle : public Object {
public:
	Triangle(DirectX::SimpleMath::Vector3 v0, DirectX::SimpleMath::Vector3 v1, DirectX::SimpleMath::Vector3 v2);
	Triangle(
		DirectX::SimpleMath::Vector3 v0,
		DirectX::SimpleMath::Vector3 v1,
		DirectX::SimpleMath::Vector3 v2,
		DirectX::SimpleMath::Vector2 uv0,
		DirectX::SimpleMath::Vector2 uv1,
		DirectX::SimpleMath::Vector2 uv2
	);
	virtual ~Triangle() override = default;

	virtual Hit intersect(Ray ray) override;

public:
	DirectX::SimpleMath::Vector3 v0;
	DirectX::SimpleMath::Vector3 v1;
	DirectX::SimpleMath::Vector3 v2;
	DirectX::SimpleMath::Vector2 uv0;
	DirectX::SimpleMath::Vector2 uv1;
	DirectX::SimpleMath::Vector2 uv2;
};

