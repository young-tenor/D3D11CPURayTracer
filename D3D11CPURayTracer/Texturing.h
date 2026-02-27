#pragma once
#include "App.h"
#include "Rect.h"

class Texturing : public App {
public:
	virtual ~Texturing() override = default;

	virtual bool init(HWND h_wnd) override;
	virtual void update() override;
	virtual void render() override;

private:
	Rect *rect = nullptr;
	bool linear_sampling = false;
	DirectX::SimpleMath::Vector3 texture[4][4];
};

