#include "pch.h"
#include "Circle.h"

Circle::Circle(float radius, DirectX::SimpleMath::Vector2 center, DirectX::SimpleMath::Vector4 color)
	: radius(radius)
	, center(center)
	, color(color) {
}

bool Circle::is_inside(DirectX::SimpleMath::Vector2 point) {
	return (point - center).Dot(point - center) <= radius * radius;
}