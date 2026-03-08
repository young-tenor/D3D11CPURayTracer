#pragma once
#include "App.h"
#include "Cubemap.h"
#include "Sphere.h"

class Reflection : public App {
public:
	~Reflection() override = default;

	bool Init(HWND hWnd) override;
	void Update() override;

protected:
	void CPURender() override;

private:
	Sphere *sphere = nullptr;
};

