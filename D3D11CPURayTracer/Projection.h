#pragma once
#include "App.h"
#include "Light.h";

class Projection : public App {
public:
	~Projection() override = default;

	bool init(HWND h_wnd) override;
	void update() override;

private:
	bool perspective = false;
};