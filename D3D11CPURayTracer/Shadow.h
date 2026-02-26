#pragma once
#include <directxtk/SimpleMath.h>
#include "App.h"
#include "Light.h";
#include "Sphere.h";

class Shadow : public App {
public:
	virtual bool init(HWND h_wnd) override;
	virtual void update() override;
	virtual void render() override;

private:
	Light *light = nullptr;
	Sphere *sphere = nullptr;
	bool draw_shadow = false;
};