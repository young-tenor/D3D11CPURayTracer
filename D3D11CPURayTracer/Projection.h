#pragma once
#include "App.h"
#include "Light.h";

class Projection : public App {
public:
	~Projection() override = default;

	bool Init(HWND hWnd) override;
	void Update() override;

protected:
	void CPURender() override;

private:
	bool perspective = false;
};