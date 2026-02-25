#pragma once
#include <directxtk/SimpleMath.h>

class Circle {
public:
	Circle(float radius, DirectX::SimpleMath::Vector2 center, DirectX::SimpleMath::Vector4 color);

	bool is_inside(DirectX::SimpleMath::Vector2 point);

public:
	float radius = 0.0f;
	DirectX::SimpleMath::Vector2 center;
	DirectX::SimpleMath::Vector4 color;
};