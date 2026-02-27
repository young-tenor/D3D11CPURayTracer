#include "pch.h"
#include "Texturing.h"

bool Texturing::init(HWND h_wnd) {
	if (!App::init(h_wnd)) {
		return false;
	}

	// object
	rect = new Rect(
		glm::vec3(-0.5f, 0.5f, 1.0f),
		glm::vec3(0.5f, 0.5f, 1.0f),
		glm::vec3(0.5f, -0.5f, 1.0f),
		glm::vec3(-0.5f, -0.5f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f));

	// texture
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if ((i + j) % 2 == 0) {
				texture[i][j] = glm::vec3(0.0f, 0.0f, 0.0f);
			} else {
				texture[i][j] = glm::vec3(1.0f, 1.0f, 1.0f);
			}
		}
	}

	return true;
}

void Texturing::update() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Perspective Projection");

	if (ImGui::RadioButton("point sampling", linear_sampling == false)) {
		linear_sampling = false;
	}
	if (ImGui::RadioButton("linear sampling", linear_sampling == true)) {
		linear_sampling = true;
	}

	ImGui::End();

	// object
	auto clear_color = glm::vec4(0.1f, 0.2f, 0.4f, 1.0f);
	std::fill(canvas_data.begin(), canvas_data.end(), clear_color);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			auto pos_world = screen_to_world(glm::vec3((float)j, (float)i, 0.0f));
			Ray ray(pos_world, glm::vec3(0.0f, 0.0f, 1.0f));
			Hit hit = rect->intersect(ray);
			if (hit.d < 0.0f) {
				continue;
			}

			auto xy = glm::vec2(hit.uv.x * 4.0f - 0.5f, hit.uv.y * 4.0f - 0.5f);
			int x = std::round(xy.x);
			int y = std::round(xy.y);

			x = std::clamp(x, 0, 3);
			y = std::clamp(y, 0, 3);

			auto color = texture[y][x];
			canvas_data[i * width + j] = glm::vec4(color, 1.0f);
		}
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(canvas, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, canvas_data.data(), canvas_data.size() * sizeof(glm::vec4));
	context->Unmap(canvas, 0);
}

void Texturing::render() {
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
