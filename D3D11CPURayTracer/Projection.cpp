#include "pch.h"
#include "Projection.h"
#include "Sphere.h"

bool Projection::Init(HWND hWnd) {
	if (!App::Init(hWnd)) {
		return false;
	}

	// objects
	Sphere *front = new Sphere();
	front->radius = 0.5f;
	front->center = glm::vec3(-0.33f, 0.0f, 0.5f);
	front->ambient = glm::vec3(0.1f, 0.02f, 0.02f);
	front->diffuse = glm::vec3(0.8f, 0.1f, 0.1f);
	front->specular = glm::vec3(1.0f, 0.8f, 0.8f);
	front->shininess = 128.0f;
	objects.push_back(front);

	Sphere *back = new Sphere();
	back->radius = 0.5f;
	back->center = glm::vec3(0.33f, 0.0f, 1.0f);
	back->ambient = glm::vec3(0.02f, 0.05f, 0.1f);
	back->diffuse = glm::vec3(0.1f, 0.4f, 0.9f);
	back->specular = glm::vec3(0.8f, 0.9f, 1.0f);
	back->shininess = 64.0f;
	objects.push_back(back);

	return true;
}

void Projection::Update() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Perspective Projection");

	if (ImGui::RadioButton("orthographic", perspective == false)) {
		perspective = false;
	}
	if (ImGui::RadioButton("perspective", perspective == true)) {
		perspective = true;
	}

	ImGui::End();
}

void Projection::CPURender() {
	const auto clearColor = glm::vec4(0.1f, 0.2f, 0.4f, 1.0f);
	std::fill(canvasData.begin(), canvasData.end(), clearColor);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			const auto posWorld = ScreenToWorld(glm::vec3((float)j + 0.5f, (float)i + 0.5f, 0.0f));

			glm::vec3 rayDir;
			if (perspective) {
				rayDir = glm::normalize(posWorld - camPos);
			} else {
				rayDir = glm::vec3(0.0f, 0.0f, 1.0f);
			}

			canvasData[i * width + j] = glm::vec4(TraceRay(posWorld, rayDir), 1.0f);
		}
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(canvas, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, canvasData.data(), canvasData.size() * sizeof(glm::vec4));
	context->Unmap(canvas, 0);
}
