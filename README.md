# D3D11 CPU Ray Tracer
C++와 D3D11로 진행한 CPU 레이 트레이서 프로젝트입니다. 엔진 프로그래밍보다는 그래픽스 알고리즘 구현에 집중했습니다. D3D11은 화면을 출력하는 용도로만 사용했습니다.

아래처럼 main 함수에서 App 포인터에 할당하는 객체를 바꿔가며 그래픽스 알고리즘을 검사할 수 있습니다.

```cpp
const auto blinn_phong = new BlinnPhong();
const auto projection = new Projection();
const auto shadow = new Shadow();
const auto texturing = new Texturing();
const auto reflection = new Reflection();
const auto super_sampling = new SuperSampling();

App *app = super_sampling;
```
### 목차
* [Blinn-Phong](#blinn-phong)
* [Perspective Projection](#perspective-projection)
* [Shadow](#shadow)
* [Texturing](#texturing)
* [Reflection](#reflection)
* [Super Sampling](#super-sampling)

## Blinn-Phong
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/8d246dc9-2ae5-4b25-bb43-6d47f4511b99" />

## Perspective Projection
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/2d477093-642c-406a-a644-1ef72f6fe335" />
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/50510c49-6185-4e38-834d-0a79f7e86107" />

## Shadow
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/423fefed-33ab-439e-acb4-596746b29910" />

## Texturing
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/eeb5acc9-d898-4b00-b0a3-3d598db73b67" />
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/5bf1348a-0007-4d70-b075-24a5995bebd9" />
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/2d372c88-d37e-43da-b81f-808b9f2e5989" />
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/865c41cd-e7b9-4ed0-8568-3eb56dd94f07" />

## Reflection
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/6be34fdf-9596-4902-9336-7ef2e41dc3d0" />

## Super Sampling
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/45f57f81-0202-44ce-82f1-c1accbf3e462" />
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/9b900378-207a-48c8-9051-0140ad45a935" />

## Credits
* **Cubemap Textures**: Created by [Emil Persson (Humus)](http://www.humus.name), licensed under [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/).
