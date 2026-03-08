#include "pch.h"
#include "Texturing.h"

bool Texturing::Init(HWND hWnd) {
	if (!App::Init(hWnd)) {
		return false;
	}

	// object
	rect = new Rect();
	rect->SetVertices(glm::vec3(-0.5f, 0.5f, 1.0f), glm::vec3(0.5f, 0.5f, 1.0f), glm::vec3(0.5f, -0.5f, 1.0f), glm::vec3(-0.5f, -0.5f, 1.0f));
	rect->SetUVs(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f));
	rect->texture = new Texture(4, 4);
	rect->ambient = glm::vec3(1.0f);
	rect->diffuse = glm::vec3(0.0f);
	rect->specular = glm::vec3(0.0f);
	objects.push_back(rect);

	return true;
}

void Texturing::Update() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Texturing");

	ImGui::Text("sampling mode");
	if (ImGui::RadioButton("point sampling", linearSampling == false)) {
		linearSampling = false;
	}
	if (ImGui::RadioButton("linear sampling", linearSampling == true)) {
		linearSampling = true;
	}

	ImGui::Separator();

	ImGui::Text("address mode");
	if (ImGui::RadioButton("clamp", wrap == false)) {
		wrap = false;
	}
	if (ImGui::RadioButton("wrap", wrap == true)) {
		wrap = true;
	}

	ImGui::Separator();

	ImGui::Text("etc");
	ImGui::Checkbox("expand", &expand);

	ImGui::End();
}

void Texturing::CPURender() {
	const auto clearColor = glm::vec4(0.1f, 0.2f, 0.4f, 1.0f);
	std::fill(canvasData.begin(), canvasData.end(), clearColor);

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
