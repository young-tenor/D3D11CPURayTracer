#pragma once
#include "App.h"

class SuperSampling : public App {
public:
	~SuperSampling() override = default;

	bool init(HWND h_wnd) override;
	void update() override;
};

