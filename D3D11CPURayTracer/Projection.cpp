#include "pch.h"
#include "Projection.h"

bool Projection::init(HWND h_wnd) {
	if (!App::init(h_wnd)) {
		return false;
	}

	// light
	light = new Light(1.0f, { 0.0f, 1.0f, 0.0f }, { 0.0f, -1.0f, 1.0f });

	// meshes
	spheres.push_back(new Sphere(0.5f, { -0.25f, 0.0f, 0.5f }, { 1.0f, 0.0f, 0.0f })); // left, front, red
	spheres.push_back(new Sphere(0.5f, { 0.25f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f })); // right, back, blue

	return true;
}

void Projection::update() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Perspective Projection");

	if (ImGui::RadioButton("orthographic", use_perspective == false)) {
		use_perspective = false;
	}
	if (ImGui::RadioButton("perspective", use_perspective == true)) {
		use_perspective = true;
	}

	ImGui::End();

	// meshes
	auto clear_color = DirectX::SimpleMath::Vector4{ 0.1f, 0.2f, 0.4f, 1.0f };
	std::fill(texture_data.begin(), texture_data.end(), clear_color);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			auto pos_world = screen_to_world({ (float)j, (float)i, 0.0f });
			if (use_perspective) {
				auto cam_pos = DirectX::SimpleMath::Vector3{ 0.0f, 0.0f, -1.0f };
				auto ray_dir = pos_world - cam_pos;
				ray_dir.Normalize();
				Ray ray(cam_pos, ray_dir);

				Mesh *closest_sphere = nullptr;
				Hit closest_hit(-1.0f, { 0.0f, 0.0f, -1.0f });
				float min_d = 100.0f;
				for (auto &sphere : spheres) {
					Hit hit = sphere->intersect(ray);
					if (hit.d < 0.0f || hit.d > min_d) {
						continue;
					}
					closest_sphere = sphere;
					closest_hit = hit;
					min_d = hit.d;
				}
				if (!closest_sphere) {
					continue;
				}

				auto light_dir = (light->pos - closest_hit.pos);
				light_dir.Normalize();

				auto cam_dir = -ray.dir;
				cam_dir.Normalize();

				auto color = blinn_phong(closest_hit.normal, light_dir, cam_dir, light->strength, closest_sphere);

				texture_data[i * width + j] = { color.x, color.y, color.z, 1.0f };
			} else {
				Ray ray(pos_world, { 0.0f, 0.0f, 1.0f });
				for (auto &sphere : spheres) {
					Hit hit = sphere->intersect(ray);
					if (hit.d < 0.0f) {
						continue;
					}

					auto light_dir = (light->pos - hit.pos);
					light_dir.Normalize();

					auto cam_dir = -ray.dir;
					cam_dir.Normalize();

					auto color = blinn_phong(hit.normal, light_dir, cam_dir, light->strength, sphere);

					texture_data[i * width + j] = { color.x, color.y, color.z, 1.0f };
				}
			}
		}
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, texture_data.data(), texture_data.size() * sizeof(DirectX::SimpleMath::Vector4));
	context->Unmap(texture, 0);
}

void Projection::render() {
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