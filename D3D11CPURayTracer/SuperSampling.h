#pragma once
#include "App.h"

class SuperSampling : public App {
public:
	~SuperSampling() override = default;

	bool Init(HWND hWnd) override;
	void Update() override;

protected:
	void CPURender() override;
};

