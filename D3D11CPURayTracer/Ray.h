#pragma once
#include <directxtk/SimpleMath.h>

class Ray {
public:
	Ray(DirectX::SimpleMath::Vector3 orig, DirectX::SimpleMath::Vector3 dir);

public:
	DirectX::SimpleMath::Vector3 orig;
	DirectX::SimpleMath::Vector3 dir;
};