#pragma once
#include "App.h";
#include "Light.h";
#include "Sphere.h";

class BlinnPhong : public App {
public:
	~BlinnPhong() override = default;

	bool Init(HWND hWnd) override;
	void Update() override;

protected:
	void CPURender() override;

private:
	Sphere *sphere = nullptr;
};