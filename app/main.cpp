#include "Application.hpp"
#include "Renderer.hpp"
#include <Renderer2D.hpp>

class SandBox : public Application {
private:
	OrthographicCamera m_Camera;

	Texture2D textura = Texture2D("C:\\Users\\Cass\\Desktop\\cass_engine\\app\\yop.jpg");

public:
	SandBox(const WindowProperties& props) : Application(props), m_Camera(
		0.0f, props.Width,
		0.0f, props.Height) {

		Renderer::SetClearColor(0xFF1E1E1E);
	}

protected:
	void OnUpdate(float deltaTime) override {
		Renderer2D::BeginScene(m_Camera);

		Renderer2D::DrawQuad({
			.transform =
				cass::Matrix4<float>()
					.translate({ 400, 300 })
					.scale({ 40, 40 }),
			.argb = 0xFFFFFFFF,
			.texture = &textura,
			.origin = { 0, 0 },
			.uv = { 0.5, 0.5, 1, 1 }
		});
	
		Renderer2D::EndScene();

		auto& stats = Renderer2D::GetStats();
		std::cout
			<< "Quads: " << stats.QuadCount
			<< " | DrawCalls: " << stats.DrawCalls
			<< " | " << stats.VertexCount
			<< std::endl;
	}
};

int main() {

	WindowProperties windowProps = {
		.Width = 800,
		.Height = 600
	};

	SandBox app(windowProps);

	app.Run();

	return 0;
}