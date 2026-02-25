#pragma once
#include <directxtk/SimpleMath.h>

class Hit {
public:
	Hit(float d, DirectX::SimpleMath::Vector3 pos);

public:
	float d = -1.0f;
	DirectX::SimpleMath::Vector3 pos;
};