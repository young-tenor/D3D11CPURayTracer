#pragma once
#include <directxtk/SimpleMath.h>

class Light {
public:
	Light(float strength, DirectX::SimpleMath::Vector3 pos);

public:
	float strength = 1.0f;
	DirectX::SimpleMath::Vector3 pos;
};

