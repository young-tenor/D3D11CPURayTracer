#pragma once
#include "App.h"
#include "Light.h";
#include "Object.h";

class Shadow : public App {
public:
	virtual bool init(HWND h_wnd) override;
	virtual void update() override;
	virtual void render() override;

private:
	Light *light = nullptr;
	std::vector<Object *> objects;
	bool draw_shadow = false;
	bool use_ray_bias = false;
};