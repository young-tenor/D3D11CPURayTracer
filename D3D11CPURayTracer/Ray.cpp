#include "pch.h"
#include "Ray.h"

Ray::Ray(DirectX::SimpleMath::Vector3 orig, DirectX::SimpleMath::Vector3 dir)
	: orig(orig)
	, dir(dir) {
}

