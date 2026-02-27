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
	light = new Light(1.0f, glm::vec3(0.0f, 1.0f, -1.0f));

	// objects
	objects.push_back(new Sphere(0.5f, glm::vec3(0.0f, 0.0f, 1.0f)));
	objects.push_back(new Rect(glm::vec3(-2.0f, -1.0f, 4.0f), glm::vec3(2.0f, -1.0, 4.0f), glm::vec3(2.0f, -1.0, 0.0f), glm::vec3(-2.0f, -1.0, 0.0f)));

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

	ImGui::BeginDisabled(!draw_shadow);
	ImGui::Checkbox("use ray bias", &use_ray_bias);
	ImGui::EndDisabled();

	ImGui::End();

	// objects
	auto clear_color = glm::vec4(0.1f, 0.2f, 0.4f, 1.0f);
	std::fill(canvas_data.begin(), canvas_data.end(), clear_color);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			auto pos_world = screen_to_world(glm::vec3((float)j, (float)i, 0.0f));
			auto cam_pos = glm::vec3(0.0f, 0.0f, -1.0f);
			auto ray_dir = glm::normalize(pos_world - cam_pos);
			Ray ray(pos_world, ray_dir);

			Object *closest_obj = nullptr;
			Hit closest_hit(-1.0f, glm::vec3(0.0f, 0.0f, -1.0f));
			float min_d = 100.0f;
			for (auto &object : objects) {
				Hit hit = object->intersect(ray);
				if (hit.d < 0.0f || hit.d > min_d) {
					continue;
				}
				closest_obj = object;
				closest_hit = hit;
				min_d = hit.d;
			}
			if (!closest_obj) {
				continue;
			}

			auto light_vec = glm::normalize(light->pos - closest_hit.pos);

			if (draw_shadow) {
				float bias = use_ray_bias ? 1e-3f : 0.0f;
				Ray shadow_ray(closest_hit.pos + closest_hit.normal * bias, light_vec);
				bool is_shadowed = false;
				for (auto &object : objects) {
					Hit hit = object->intersect(shadow_ray);
					if (hit.d < 0.0f) {
						continue;
					}
					is_shadowed = true;
					break;
				}

				if (is_shadowed) {
					auto color = closest_obj->ambient;
					canvas_data[i * width + j] = glm::vec4(color, 1.0f);
					continue;
				}
			}

			auto cam_dir = glm::normalize(-ray.dir);

			auto color = blinn_phong(closest_hit.normal, light_vec, cam_dir, light->strength, closest_obj);

			canvas_data[i * width + j] = glm::vec4(color, 1.0f);
		}
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(canvas, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, canvas_data.data(), canvas_data.size() * sizeof(glm::vec4));
	context->Unmap(canvas, 0);
}

void Shadow::render() {
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