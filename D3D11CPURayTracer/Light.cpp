#include "pch.h"
#include "Light.h"

Light::Light(float strength, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 dir)
	: strength(strength)
	, pos(pos)
	, dir(dir) {
	this->dir.Normalize();
}
