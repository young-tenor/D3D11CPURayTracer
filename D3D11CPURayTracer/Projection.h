#pragma once
#include "App.h"
#include "Light.h";
#include "Sphere.h";

class Projection : public App {
public:
	~Projection() override = default;

	bool init(HWND h_wnd) override;
	void update() override;
	void render() override;

private:
	std::vector<Object *> spheres;
	bool use_perspective = false;
};