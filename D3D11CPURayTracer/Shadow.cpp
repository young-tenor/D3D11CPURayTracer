#include "pch.h"
#include "Shadow.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Rect.h"

bool Shadow::init(HWND h_wnd) {
	if (!App::init(h_wnd)) {
		return false;
	}

	// light
	light = new Light(1.0f, { 0.0f, 1.0f, -1.0f });

	// meshes
	meshes.push_back(new Sphere(0.5f, { 0.0f, 0.0f, 1.0f }));
	meshes.push_back(new Rect({ -2.0f, -1.0f, 4.0f }, { 2.0f, -1.0, 4.0f }, { 2.0f, -1.0, 0.0f }, { -2.0f, -1.0, 0.0f }));

	return true;
}

void Shadow::update() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Shadow Ray");

	ImGui::Text("light");
	ImGui::DragFloat3("position", &light->pos.x, 0.1f);
	ImGui::SliderFloat("strength", &light->strength, 0.0f, 5.0f);

	ImGui::Separator();

	ImGui::Checkbox("draw shadow", &draw_shadow);

	ImGui::End();

	// meshes
	auto clear_color = DirectX::SimpleMath::Vector4{ 0.1f, 0.2f, 0.4f, 1.0f };
	std::fill(texture_data.begin(), texture_data.end(), clear_color);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			auto pos_world = screen_to_world({ (float)j, (float)i, 0.0f });
			auto cam_pos = DirectX::SimpleMath::Vector3{ 0.0f, 0.0f, -1.0f };
			auto ray_dir = pos_world - cam_pos;
			ray_dir.Normalize();
			Ray ray(cam_pos, ray_dir);

			Mesh *closest_mesh = nullptr;
			Hit closest_hit(-1.0f, { 0.0f, 0.0f, -1.0f });
			float min_d = 100.0f;
			for (auto &mesh : meshes) {
				Hit hit = mesh->intersect(ray);
				if (hit.d < 0.0f || hit.d > min_d) {
					continue;
				}
				closest_mesh = mesh;
				closest_hit = hit;
				min_d = hit.d;
			}
			if (!closest_mesh) {
				continue;
			}

			auto light_vec = light->pos - closest_hit.pos;
			light_vec.Normalize();

			if (draw_shadow) {
				Ray shadow_ray(closest_hit.pos + closest_hit.normal * 1e-3f, light_vec);
				bool is_shadowed = false;
				for (auto &mesh : meshes) {
					Hit hit = mesh->intersect(shadow_ray);
					if (hit.d < 0.0f) {
						continue;
					}
					is_shadowed = true;
					break;
				}

				if (is_shadowed) {
					auto color = closest_mesh->ambient;
					texture_data[i * width + j] = { color.x, color.y, color.z, 1.0f };
					continue;
				}
			}

			auto cam_dir = -ray.dir;
			cam_dir.Normalize();

			auto color = blinn_phong(closest_hit.normal, light_vec, cam_dir, light->strength, closest_mesh);

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