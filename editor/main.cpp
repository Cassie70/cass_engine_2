#include <Application.hpp>
#include <Renderer2D.hpp>
#include <FontManager.hpp>

using v3 = cass::Vector3<float>;

class SandBox : public Application {
private:
	OrthographicCamera m_Camera;
	OrthographicCamera ui_Camera;
	WindowProperties props;

	float m_TimeAccumulator = 0.0f;
	int m_FrameCount = 0;
	float accumulator = 0;
	float frameDt = 0;

	uint32_t arial24;

public:
	SandBox(const WindowProperties& props) :
		Application(props),
		m_Camera(
			-(props.Width) * 0.5f,
			(props.Width) * 0.5f,
			-(props.Height) * 0.5f,
			(props.Height) * 0.5f
		),
		ui_Camera(0, props.Width, 0, props.Height),
		props(props)
	{
		Application::SetClearColor(0xFF000000);
		FontManager::Init();
		arial24 = FontManager::Load("assets/arial.ttf", 24);
	}

protected:
	void OnUpdate(float deltaTime) override {

		Renderer2D::BeginScene(m_Camera);

		Renderer2D::EndScene();

		Renderer2D::BeginScene(ui_Camera);

		Renderer2D::EndScene();
	}
};

int main() {
	WindowProperties windowProps = {
		.Width = 1280,
		.Height = 720,
		.Title = "Hola cara de bola",
		.VSync = true,
		.Resizable = false
	};

	SandBox app(windowProps);

	app.Run();

	return 0;
}