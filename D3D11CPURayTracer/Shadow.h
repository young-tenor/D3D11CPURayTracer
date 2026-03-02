#pragma once
#include "App.h"
#include "Light.h";
#include "Object.h";

class Shadow : public App {
public:
	~Shadow() override = default;

	bool init(HWND h_wnd) override;
	void update() override;

private:
};