#pragma once
#include "App.h"
#include "Cubemap.h"
#include "Sphere.h"

class Reflection : public App {
public:
	~Reflection() override = default;

	bool init(HWND h_wnd) override;
	void update() override;

private:
	Sphere *sphere = nullptr;
};

