#include "pch.h"
#include "BlinnPhong.h"

bool BlinnPhong::init(HWND h_wnd) {
	if (!App::init(h_wnd)) {
		return false;
	}

	// light
	light = new Light(1.0f, glm::vec3(0.0f, 1.0f, -1.0f));

	// object
	sphere = new Sphere(0.5f, glm::vec3(0.0f, 0.0f, 0.5f));

	return true;
}

void BlinnPhong::update() {
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

	// object
	auto clear_color = glm::vec4(0.1f, 0.2f, 0.4f, 1.0f);
	fill(canvas_data.begin(), canvas_data.end(), clear_color);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			auto pos_world = screen_to_world(glm::vec3((float)j, (float)i, 0.0f));
			Ray ray(pos_world, glm::vec3(0.0f, 0.0f, 1.0f));
			Hit hit = sphere->intersect(ray);
			if (hit.d < 0.0f) {
				continue;
			}
			auto light_vec = glm::normalize(light->pos - hit.pos);

			auto cam_dir = glm::normalize(-ray.dir);

			auto color = blinn_phong(hit.normal, light_vec, cam_dir, light->strength, sphere);

			canvas_data[i * width + j] = glm::vec4(color, 1.0f);
		}
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(canvas, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, canvas_data.data(), canvas_data.size() * sizeof(glm::vec4));
	context->Unmap(canvas, 0);
}

void BlinnPhong::render() {
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