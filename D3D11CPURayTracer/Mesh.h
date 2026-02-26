#pragma once
#include <directxtk/SimpleMath.h>
#include "Hit.h"
#include "Ray.h"

class Mesh {
public:
	Mesh() = default;
	Mesh(DirectX::SimpleMath::Vector3 ambient);
	virtual ~Mesh() = default;

	virtual Hit intersect(Ray ray) = 0;

public:
	DirectX::SimpleMath::Vector3 ambient = { 0.1f, 0.1f, 0.1f };
	DirectX::SimpleMath::Vector3 diffuse = { 0.7f, 0.7f, 0.7f };
	DirectX::SimpleMath::Vector3 specular = { 0.5f, 0.5f, 0.5f };
	float shininess = 32.0f;
};