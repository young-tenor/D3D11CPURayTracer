#pragma once
#include "App.h"
#include "Rect.h"
#include "Texture.h"

class Texturing : public App {
public:
	~Texturing() override = default;

	bool init(HWND h_wnd) override;
	void update() override;

protected:
	void cpu_render() override;

private:
	Rect *rect = nullptr;
};

