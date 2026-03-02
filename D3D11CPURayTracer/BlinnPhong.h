#pragma once
#include "App.h";
#include "Light.h";
#include "Sphere.h";

class BlinnPhong : public App {
public:
	~BlinnPhong() override = default;

	bool init(HWND h_wnd) override;
	void update() override;

private:
	Sphere *sphere = nullptr;
};