#include "pch.h"
#include "Shadow.h"

bool Shadow::init(HWND h_wnd) {
	if (!App::init(h_wnd)) {
		return false;
	}

	// light
	light = new Light(1.0f, { 0.0f, 1.0f, 0.0f }, { 0.0f, -1.0f, 1.0f });

	// mesh
	sphere = new Sphere(0.5f, { 0.0f, 0.0f, 0.5f });

	return true;
}

void Shadow::update() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Shadow Ray");

	ImGui::Checkbox("Draw Shadow", &draw_shadow);

	ImGui::End();

	// mesh
	auto clear_color = DirectX::SimpleMath::Vector4{ 0.1f, 0.2f, 0.4f, 1.0f };
	std::fill(texture_data.begin(), texture_data.end(), clear_color);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			auto pos_world = screen_to_world({ (float)j, (float)i, 0.0f });
			auto cam_pos = DirectX::SimpleMath::Vector3{ 0.0f, 0.0f, -1.0f };
			auto ray_dir = pos_world - cam_pos;
			ray_dir.Normalize();
			Ray ray(cam_pos, ray_dir);

			Hit hit = sphere->intersect(ray);
			if (hit.d < 0.0f) {
				continue;
			}

			auto normal = (hit.pos - sphere->center);
			normal.Normalize();

			auto light_dir = (light->pos - hit.pos);
			light_dir.Normalize();

			auto cam_dir = -ray.dir;
			cam_dir.Normalize();

			auto color = blinn_phong(normal, light_dir, cam_dir, light->strength, sphere);

			texture_data[i * width + j] = { color.x, color.y, color.z, 1.0f };
		}
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, texture_data.data(), texture_data.size() * sizeof(DirectX::SimpleMath::Vector4));
	context->Unmap(texture, 0);
}

void Shadow::render() {
	const float clear_color[] = { 0.1f, 0.2f, 0.4f, 1.0f };
	context->ClearRenderTargetView(rtv, clear_color);

	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, &rtv, nullptr);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->VSSetShader(vs, nullptr, 0);
	context->PSSetShader(ps, nullptr, 0);

	context->PSSetShaderResources(0, 1, &texture_srv);
	context->PSSetSamplers(0, 1, &sampler);

	context->Draw(3, 0);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swap_chain->Present(1, 0);
}