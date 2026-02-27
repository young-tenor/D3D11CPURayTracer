#pragma once
#include "App.h"
#include "Rect.h"
#include "Texture.h"

class Texturing : public App {
public:
	virtual ~Texturing() override = default;

	virtual bool init(HWND h_wnd) override;
	virtual void update() override;
	virtual void render() override;

private:
	Rect *rect = nullptr;
	Texture *texture = nullptr;
	bool linear_sampling = false;
	bool wrap = false;
};

