#pragma once
#include "App.h"
#include "Light.h";
#include "Object.h";

class Shadow : public App {
public:
	~Shadow() override = default;

	bool Init(HWND hWnd) override;
	void Update() override;

protected:
	void CPURender() override;

private:
};