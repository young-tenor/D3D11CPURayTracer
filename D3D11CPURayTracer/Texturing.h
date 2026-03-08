#pragma once
#include "App.h"
#include "Rect.h"
#include "Texture.h"

class Texturing : public App {
public:
	~Texturing() override = default;

	bool Init(HWND hWnd) override;
	void Update() override;

protected:
	void CPURender() override;

private:
	Rect *rect = nullptr;
};

