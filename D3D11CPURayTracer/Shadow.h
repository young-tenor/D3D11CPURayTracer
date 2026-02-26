#pragma once
#include <directxtk/SimpleMath.h>
#include "App.h"
#include "Light.h";
#include "Mesh.h";

class Shadow : public App {
public:
	virtual bool init(HWND h_wnd) override;
	virtual void update() override;
	virtual void render() override;

private:
	Light *light = nullptr;
	std::vector<Mesh *> meshes;
	bool draw_shadow = false;
};