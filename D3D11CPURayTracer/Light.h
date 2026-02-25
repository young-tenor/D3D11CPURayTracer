#pragma once
#include <directxtk/SimpleMath.h>

class Light {
public:
	Light(float strength, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 dir);

public:
	float strength = 1.0f;
	DirectX::SimpleMath::Vector3 pos;
	DirectX::SimpleMath::Vector3 dir;
};

