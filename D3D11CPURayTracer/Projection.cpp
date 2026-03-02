#include "pch.h"
#include "Projection.h"
#include "Sphere.h"

bool Projection::init(HWND h_wnd) {
	if (!App::init(h_wnd)) {
		return false;
	}

	// objects
	Sphere *front = new Sphere();
	front->radius = 0.5f;
	front->center = glm::vec3(-0.25f, 0.0f, 0.5f);
	front->ambient = glm::vec3(0.1f, 0.02f, 0.02f);
	front->diffuse = glm::vec3(0.8f, 0.1f, 0.1f);
	front->specular = glm::vec3(1.0f, 0.8f, 0.8f);
	front->shininess = 128.0f;
	objects.push_back(front);

	Sphere *back = new Sphere();
	back->radius = 0.5f;
	back->center = glm::vec3(0.25f, 0.0f, 1.0f);
	back->ambient = glm::vec3(0.02f, 0.05f, 0.1f);
	back->diffuse = glm::vec3(0.1f, 0.4f, 0.9f);
	back->specular = glm::vec3(0.8f, 0.9f, 1.0f);
	back->shininess = 64.0f;
	objects.push_back(back);

	return true;
}

void Projection::update() {
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

	// objects
	const auto clear_color = glm::vec4(0.1f, 0.2f, 0.4f, 1.0f);
	std::fill(canvas_data.begin(), canvas_data.end(), clear_color);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			const auto pos_world = screen_to_world(glm::vec3((float)j + 0.5f, (float)i + 0.5f, 0.0f));

			glm::vec3 ray_dir;
			if (perspective) {
				ray_dir = glm::normalize(pos_world - cam_pos);
			} else {
				ray_dir = glm::vec3(0.0f, 0.0f, 1.0f);
			}

			canvas_data[i * width + j] = glm::vec4(trace_ray(pos_world, ray_dir), 1.0f);
		}
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(canvas, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, canvas_data.data(), canvas_data.size() * sizeof(glm::vec4));
	context->Unmap(canvas, 0);
}

void Projection::render() {
	const float clear_color[] = { 0.1f, 0.2f, 0.4f, 1.0f };
	context->ClearRenderTargetView(rtv, clear_color);

	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, &rtv, nullptr);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->VSSetShader(vs, nullptr, 0);
	context->PSSetShader(ps, nullptr, 0);

	context->PSSetShaderResources(0, 1, &canvas_srv);
	context->PSSetSamplers(0, 1, &sampler);

	context->Draw(3, 0);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swap_chain->Present(1, 0);
}