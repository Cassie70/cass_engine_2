#include <Application.hpp>
#include <FontManager.hpp>
#include <Input.hpp>
#include <Mouse.hpp>
#include <Renderer2D.hpp>
#include <SpriteSheet.hpp>

#include "CameraController.hpp"
#include "EditorUI.hpp"
#include "EditorWorld.hpp"

using namespace cass;

class Editor : public Application {
private:
  OrthographicCamera m_Camera;
  OrthographicCamera ui_Camera;
  EditorWorld world;
  EditorUI ui;
  CameraController cameraController;

  float m_TimeAccumulator = 0.0f;
  int m_FrameCount = 0;

  float worldTileSize = 16.0f;

  Texture2D atlasTexture;
  SpriteSheet ss;

public:
  Editor(const WindowProperties &props)
      : Application(props),
        m_Camera(
          -((float)props.Width) * 0.5f, ((float)props.Width) * 0.5f,
          -((float)props.Height) * 0.5f, ((float)props.Height) * 0.5f
        ),
        ui_Camera(0.0f, (float)props.Width, 0.0f, (float)props.Height),
        ui(Application::GetWindow()), cameraController(m_Camera, ui_Camera),
        atlasTexture("assets/atlas.png", Texture2DParams{}) {
    Application::SetClearColor(0xFF121212);

    ss = SpriteSheetParams{
      .textureWidth = (int)atlasTexture.GetWidth(),
      .textureHeight = (int)atlasTexture.GetHeight(),
      .spriteWidth = 16,
      .spriteHeight = 16,
      .rows = 6,
      .cols = 5
    };

    world.Init();
    ui.Init(75.0f, 5);
  }

protected:
  void OnUpdate(float deltaTime) override {
    auto mousePos = Input::GetMousePosition();
    int winW = Application::GetWindow().GetWidth();
    int winH = Application::GetWindow().GetHeight();

    cameraController.HandleInputUpdate(deltaTime, winW, winH);

    if (Input::IsMousePressed(Mouse::Left)) {
      if (mousePos.x < ui.GetStartX()) {
        if (ui.HasSelection()) {
          int atlasIndex = ui.GetSelectedRow() * ss.cols + ui.GetSelectedCol();
          world.PaintTileAt(cameraController.getWorldMouse(), atlasIndex);
        }
      }
    } else if (Input::IsMousePressed(Mouse::Right)) {
      if (mousePos.x < ui.GetStartX()) {
        world.EraseTileAt(cameraController.getWorldMouse());
      }
    }

    ui.UpdateCursor(mousePos.x);

    world.Draw(m_Camera, atlasTexture, ss, worldTileSize, winW, winH);
    Renderer2D::BeginScene(ui_Camera);
    ui.Draw(ui_Camera, atlasTexture, ss);
    Renderer2D::EndScene();

    showInfo(deltaTime);
  }

  void OnEvent(Event &e) override {
    cameraController.HandleInputEvent(e);
    ui.HandleEvent(e, ss);
  }

  void showInfo(float deltaTime) {
    m_TimeAccumulator += deltaTime;
    m_FrameCount++;

    if (m_TimeAccumulator >= 1.0f) {
      int fps = m_FrameCount;

      std::string title =
        "Sandbox | FPS: " + std::to_string(fps) +
        " | Draw Calls: " + std::to_string(Renderer2D::GetStats().DrawCalls) +
        " | Quads: " + std::to_string(Renderer2D::GetStats().QuadCount) +
        " | TexturesSlots: " +
        std::to_string(Renderer2D::GetStats().TextureCount);

      Application::GetWindow().SetTitle(title);

      m_FrameCount = 0;
      m_TimeAccumulator = 0.0f;
    }
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