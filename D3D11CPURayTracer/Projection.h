#pragma once
#include "App.h"
#include "Light.h";

class Projection : public App {
public:
	~Projection() override = default;

	bool init(HWND h_wnd) override;
	void update() override;
	void render() override;

private:
	bool perspective = false;
};