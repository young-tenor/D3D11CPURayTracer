#include "pch.h"
#include "Shadow.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Rect.h"

bool Shadow::Init(HWND hWnd) {
	if (!App::Init(hWnd)) {
		return false;
	}

	// light
	light = new Light(1.0f, glm::vec3(0.0f, 1.0f, -1.0f));

	// objects
	Sphere *sphere = new Sphere();
	sphere->radius = 0.5f;
	sphere->center = glm::vec3(0.0f, 0.0f, 1.0f);
	objects.push_back(sphere);

	Rect *floor = new Rect();
	floor->SetVertices(glm::vec3(-2.0f, -1.0f, 4.0f), glm::vec3(2.0f, -1.0, 4.0f), glm::vec3(2.0f, -1.0, 0.0f), glm::vec3(-2.0f, -1.0, 0.0f));
	objects.push_back(floor);

	return true;
}

void Shadow::Update() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Shadow Ray");

	ImGui::Text("light");
	ImGui::DragFloat3("position", &light->pos.x, 0.1f);
	ImGui::SliderFloat("strength", &light->strength, 0.0f, 5.0f);

	ImGui::Separator();

	ImGui::Checkbox("draw shadow", &drawShadow);

	ImGui::End();
}

void Shadow::CPURender() {
	const auto clearColor = glm::vec4(0.1f, 0.2f, 0.4f, 1.0f);
	std::fill(canvasData.begin(), canvasData.end(), clearColor);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			const auto posWorld = ScreenToWorld(glm::vec3((float)j + 0.5f, (float)i + 0.5f, 0.0f));
			const auto rayDir = glm::normalize(posWorld - camPos);
			canvasData[i * width + j] = glm::vec4(TraceRay(posWorld, rayDir), 1.0f);
		}
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(canvas, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, canvasData.data(), canvasData.size() * sizeof(glm::vec4));
	context->Unmap(canvas, 0);
}
