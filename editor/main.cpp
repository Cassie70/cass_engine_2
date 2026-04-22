#include <Application.hpp>
#include <Renderer2D.hpp>
#include <FontManager.hpp>
#include <KeyEvent.hpp>
#include <GLFW/glfw3.h>
#include <Input.hpp>
#include <WindowResizeEvent .hpp>
#include <MouseScrolledEvent .hpp>
#include "CameraController.hpp"
#include <SpriteSheet.hpp>
#include <MousePressedEvent.hpp>

using v3 = cass::Vector3<float>;

class Editor : public Application {
private:
	OrthographicCamera m_Camera;
	OrthographicCamera ui_Camera;
	CameraController cameraController;

	float m_TimeAccumulator = 0.0f;
	int m_FrameCount = 0;
	float accumulator = 0;
	float frameDt = 0;

	float uiTileSize = 75.0f;
	int uiColumns = 5;
	float panelWidth = uiTileSize* uiColumns;

	int selectedRow = -1;
	int selectedCol = -1;

	cass::Vector2<float> selectedPos;
	bool hasSelection = false;

	uint32_t arial24;
	std::vector<std::vector<uint8_t>> mapTile;
	Texture2D atlasTexture;
	SpriteSheet ss;

public:
	Editor(const WindowProperties& props) :
		Application(props),
		m_Camera(
			-((float)props.Width) * 0.5f,
			((float)props.Width) * 0.5f,
			-((float)props.Height) * 0.5f,
			((float)props.Height) * 0.5f
		),
		ui_Camera(0.0f, (float)props.Width, 0.0f, (float)props.Height),
		cameraController(m_Camera),
		atlasTexture("assets/atlas.png", Texture2DParams{})
	{
		Application::SetClearColor(0xFF121212);
		FontManager::Init();

		arial24 = FontManager::Load("assets/arial.ttf", 24);

		ss = SpriteSheetParams{
			.textureWidth = (int)atlasTexture.GetWidth(),
			.textureHeight = (int)atlasTexture.GetHeight(),
			.spriteWidth = 16,
			.spriteHeight = 16,
			.rows = 6,
			.cols = 5
		};
	}

protected:

	void OnUpdate(float deltaTime) override {

		cameraController.HandleInputUpdate(deltaTime, Application::GetWindow().GetWidth(), Application::GetWindow().GetHeight());
		
		Renderer2D::BeginScene(m_Camera);

		DrawGridInfinite(16.0f,0xFF555555,1.0f);

		Renderer2D::DrawQuad({
			.transform = cass::Matrix4<float>().translate({0,0}).scale(16),
			.texture = &atlasTexture,
			.uv = ss.GetUV(1,1),
			.origin = {0,0},
			});
		Renderer2D::DrawQuad({
			.transform = cass::Matrix4<float>().translate({0,0}).scale(16),
			.argb = 0x80FFFFFF,
			.origin = {0,0},
		});
		Renderer2D::EndScene();

		Renderer2D::BeginScene(ui_Camera); 

		Renderer2D::DrawText({
			.font = arial24,
			.text = "Screen: " + Input::GetMousePosition().toString() + " | World: "  + cameraController.getWorldMouse().toString(),
			.position = { 50, 50},
			.scale = { 1.0f, 1.0f }
		});

		int index = 0;

		Renderer2D::DrawQuad({
			.transform = cass::Matrix4<float>().translate({getStartX(), 0}).scale({panelWidth,(float)Application::GetWindow().GetHeight()}),
			.argb = 0xff2f2f2f,
			.origin = {0,0}
		});

		for (int row = 0; row < ss.rows; row++) {
			for (int col = 0; col < ss.cols; col++) {
				int uiRow = index / uiColumns;
				int uiCol = index % uiColumns;

				int currentRow = index / ss.cols;
				int currentCol = index % ss.cols;

				bool isSelected = (row == selectedRow && col == selectedCol);

				float x = getStartX() + (uiCol * uiTileSize);
				float y = Application::GetWindow().GetHeight() - uiRow * uiTileSize;
				
				Renderer2D::DrawSprite({
					.position = {x, y},
					.size = {uiTileSize, uiTileSize},
					.texture = &atlasTexture,
					.uv = ss.GetUV(row, col),
					.origin = {0,1}
				});

				if (isSelected) {
					selectedPos = {x, y};
					hasSelection = true;
				}

				index++;
			}
		}

		if (hasSelection) {
			Renderer2D::DrawQuad({
				.transform = cass::Matrix4<float>()
					.translate({selectedPos.x, selectedPos.y})
					.scale({uiTileSize, uiTileSize}),
				.argb = 0x55ffffff,
				.origin = {0, 1}
			});
		}

		Renderer2D::EndScene();

		showInfo(deltaTime);
	}

	void OnEvent(Event& e) override
	{
		if (e.GetType() == EventType::WindowResize) {
			auto& resize = (WindowResizeEvent&)e;

			m_Camera.SetProjection(
				-resize.Width * 0.5f,
				resize.Width * 0.5f,
				-resize.Height * 0.5f,
				resize.Height * 0.5f
			);

			ui_Camera.SetProjection(0, resize.Width, 0, resize.Height);
		}

		cameraController.HandleInputEvent(e);


		if (e.GetType() == EventType::MousePressed) {
			auto& mouse = (MousePressedEvent&)e;

			if (mouse.GetButton() == GLFW_MOUSE_BUTTON_LEFT) {
				auto mousePos = Input::GetMousePosition();

				if (mousePos.x >= getStartX()) {
					float localX = mousePos.x - getStartX();
					float localY = mousePos.y;

					int col = (int)(localX / uiTileSize);
					int row = (int)(localY / uiTileSize);

					int index = row * uiColumns + col;

					if (index >= 0 && index < ss.rows * ss.cols)
					{
						selectedRow = index / uiColumns;
						selectedCol = index % uiColumns;
					}
					std::cout << "Selected: " << selectedRow << ", " << selectedCol << std::endl;
				}
			}
		}
	}

	void DrawGridInfinite(
		float tileSize,
		uint32_t color,
		float weight
	) {
		cass::Vector3<float> camPos = m_Camera.GetPosition();
		float zoom = m_Camera.GetZoom();

		float viewWidth = Application::GetWindow().GetWidth() * zoom;
		float viewHeight = Application::GetWindow().GetHeight() * zoom;

		float left = camPos.x - viewWidth * 0.5f;
		float right = camPos.x + viewWidth * 0.5f;
		float bottom = camPos.y - viewHeight * 0.5f;
		float top = camPos.y + viewHeight * 0.5f;

		float startX = floor(left / tileSize) * tileSize;
		float startY = floor(bottom / tileSize) * tileSize;

		// Verticales
		for (float x = startX; x <= right; x += tileSize) {
			Renderer2D::DrawCartesianLine({
				.start = { x, bottom },
				.end = { x, top },
				.argb = color,
				.weight = weight
			});
		}

		// Horizontales
		for (float y = startY; y <= top; y += tileSize) {
			Renderer2D::DrawCartesianLine({
				.start = { left, y },
				.end = { right, y },
				.argb = color,
				.weight = weight
			});
		}
	}

	void showInfo(float deltaTime)
	{
		m_TimeAccumulator += deltaTime;
		m_FrameCount++;

		if (m_TimeAccumulator >= 1.0f)
		{
			int fps = m_FrameCount;

			std::string title =
				"Sandbox | FPS: " + std::to_string(fps) +
				" | Draw Calls: " + std::to_string(Renderer2D::GetStats().DrawCalls) +
				" | Quads: " + std::to_string(Renderer2D::GetStats().QuadCount) +
				" | TexturesSlots: " + std::to_string(Renderer2D::GetStats().TextureCount);


			Application::GetWindow().SetTitle(title);

			m_FrameCount = 0;
			m_TimeAccumulator = 0.0f;
		}
	}

	float getStartX() {
		return Application::GetWindow().GetWidth() - panelWidth;
	}
};

int main() {
	WindowProperties windowProps = {
		.Width = 1280,
		.Height = 720,
		.Title = "Hola cara de bola",
		.VSync = true,
		.Maximized = true
	};

	Editor app(windowProps);

	app.Run();

	return 0;
}