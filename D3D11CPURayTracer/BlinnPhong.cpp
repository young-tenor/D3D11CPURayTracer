#include "pch.h"
#include "BlinnPhong.h"

bool BlinnPhong::Init(HWND hWnd) {
	if (!App::Init(hWnd)) {
		return false;
	}

	// light
	light = new Light(1.0f, glm::vec3(0.0f, 1.0f, -1.0f));

	// object
	sphere = new Sphere();
	sphere->radius = 0.5f;
	sphere->center = glm::vec3(0.0f, 0.0f, 0.5f);
	objects.push_back(sphere);

	return true;
}

void BlinnPhong::Update() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Blinn-Phong");

	ImGui::Text("material");
	if (ImGui::SliderFloat("ambient", &sphere->ambient.x, 0.0f, 1.0f)) {
		sphere->ambient.y = sphere->ambient.x;
		sphere->ambient.z = sphere->ambient.x;
	}
	if (ImGui::SliderFloat("diffuse", &sphere->diffuse.x, 0.0f, 1.0f)) {
		sphere->diffuse.y = sphere->diffuse.x;
		sphere->diffuse.z = sphere->diffuse.x;
	}
	if (ImGui::SliderFloat("specular", &sphere->specular.x, 0.0f, 1.0f)) {
		sphere->specular.y = sphere->specular.x;
		sphere->specular.z = sphere->specular.x;
	}
	ImGui::SliderFloat("shininess", &sphere->shininess, 1.0f, 256.0f);

	ImGui::Separator();

	ImGui::Text("light");
	ImGui::DragFloat3("position", &light->pos.x, 0.1f);
	ImGui::SliderFloat("strength", &light->strength, 0.0f, 5.0f);

	ImGui::End();
}

void BlinnPhong::CPURender() {
	const auto clearColor = glm::vec4(0.1f, 0.2f, 0.4f, 1.0f);
	fill(canvasData.begin(), canvasData.end(), clearColor);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			const auto posWorld = ScreenToWorld(glm::vec3((float)j + 0.5f, (float)i + 0.5f, 0.0f));
			const auto rayDir = glm::vec3(0.0f, 0.0f, 1.0f);
			canvasData[i * width + j] = glm::vec4(TraceRay(posWorld, rayDir), 1.0f);
		}
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(canvas, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, canvasData.data(), canvasData.size() * sizeof(glm::vec4));
	context->Unmap(canvas, 0);
}
