#include "pch.h"
#include "Reflection.h"

bool Reflection::init(HWND h_wnd) {
	if (!App::init(h_wnd)) {
		return false;
	}

	// cubemap
	const std::string nx = "./nx.jpg";
	const std::string px = "./px.jpg";
	const std::string ny = "./ny.jpg";
	const std::string py = "./py.jpg";
	const std::string nz = "./nz.jpg";
	const std::string pz = "./pz.jpg";

	cubemap = new Cubemap();
	if (!cubemap->init(nx, px, ny, py, nz, pz)) {
		std::cout << "cubemap->init() failed" << std::endl;
		return false;
	}

	// object
	sphere = new Sphere();
	sphere->radius = 0.5f;
	sphere->center = glm::vec3(0.0f, 0.0f, 0.5f);
	objects.push_back(sphere);

	return true;
}

void Reflection::update() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Cubemap");
	
	ImGui::Text("material");
	ImGui::SliderFloat("reflection", &sphere->reflection, 0.0f, 1.0f);

	ImGui::Separator();

	ImGui::Checkbox("super sampling", &super_sampling);

	ImGui::End();

	const auto clear_color = glm::vec4(0.1f, 0.2f, 0.4f, 1.0f);
	std::fill(canvas_data.begin(), canvas_data.end(), clear_color);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			const auto pos_world = screen_to_world(glm::vec3((float)j, (float)i, 0.0f));
			const auto cam_pos = glm::vec3(0.0f, 0.0f, -1.0f);
			const auto ray_dir = glm::normalize(pos_world - cam_pos);
			if (super_sampling) {
				canvas_data[i * width + j] = glm::vec4(trace_ray_super(pos_world, ray_dir, 3), 1.0f);
			} else {
				canvas_data[i * width + j] = glm::vec4(trace_ray(pos_world, ray_dir, 3), 1.0f);
			}
		}
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(canvas, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, canvas_data.data(), canvas_data.size() * sizeof(glm::vec4));
	context->Unmap(canvas, 0);
}

void Reflection::render() {
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
