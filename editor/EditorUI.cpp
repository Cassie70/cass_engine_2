#include "EditorUI.hpp"

#include <FontManager.hpp>
#include <Input.hpp>
#include <Mouse.hpp>
#include <MousePressedEvent.hpp>
#include <Renderer2D.hpp>

using namespace cass;

EditorUI::EditorUI(cass::Window &window) : m_Window(window) {}

void EditorUI::Init(float tileSize, int columns) {
  uiTileSize = tileSize;
  uiColumns = columns;
  panelWidth = uiTileSize * uiColumns;
  FontManager::Init();

  arial24 = FontManager::Load("assets/arial.ttf", 24);
}

float EditorUI::GetStartX() const { return m_Window.GetWidth() - panelWidth; }
void EditorUI::Draw(
  OrthographicCamera &camera, Texture2D &atlas, SpriteSheet &ss
) {
  int windowWidth = m_Window.GetWidth();
  int windowHeight = m_Window.GetHeight();

  DrawPanel(atlas, ss, windowWidth, windowHeight);
}

void EditorUI::DrawPanel(
  Texture2D &atlas, SpriteSheet &ss, int windowWidth, int windowHeight
) {
  int index = 0;

  Renderer2D::DrawQuad(
    QuadProperties{
      .transform = Matrix4<float>()
                     .translate({GetStartX(), 0})
                     .scale({panelWidth, (float)windowHeight}),
      .argb = 0xff2f2f2f,
      .origin = {0, 0}
    }
  );

  for (int row = 0; row < ss.rows; row++) {
    for (int col = 0; col < ss.cols; col++) {
      int uiRow = index / uiColumns;
      int uiCol = index % uiColumns;

      bool isSelected = (row == selectedRow && col == selectedCol);

      float x = GetStartX() + (uiCol * uiTileSize);
      float y = windowHeight - uiRow * uiTileSize;

      Renderer2D::DrawSprite(
        SpriteProperties{
          .position = {x, y},
          .size = {uiTileSize, uiTileSize},
          .texture = &atlas,
          .uv = ss.GetUV(row, col),
          .origin = {0, 1}
        }
      );

      if (isSelected) {
        selectedPos = {x, y};
        hasSelection = true;
      }

      index++;
    }
  }

  if (hasSelection) {
    Renderer2D::DrawQuad(
      QuadProperties{
        .transform = Matrix4<float>()
                       .translate({selectedPos.x, selectedPos.y})
                       .scale({uiTileSize, uiTileSize}),
        .argb = 0x55ffffff,
        .origin = {0, 1}
      }
    );
  }
}

bool EditorUI::HandleMouseClick(float mouseX, float mouseY, SpriteSheet &ss) {
  if (mouseX >= GetStartX()) {
    float localX = mouseX - GetStartX();
    float localY = mouseY;

    int col = (int)(localX / uiTileSize);
    int row = (int)(localY / uiTileSize);

    int index = row * uiColumns + col;

    if (index >= 0 && index < ss.rows * ss.cols) {
      selectedRow = index / ss.cols;
      selectedCol = index % ss.cols;
      hasSelection = true;
      return true;
    }
  }
  return false;
}

bool EditorUI::HandleEvent(Event &e, SpriteSheet &ss) {
  if (e.GetType() == EventType::MousePressed) {
    auto &mouse = static_cast<MousePressedEvent &>(e);

    if (mouse.GetButton() == Mouse::Left) {
      auto mousePos = Input::GetMousePosition();

      if (mousePos.x >= GetStartX()) {
        return HandleMouseClick(mousePos.x, mousePos.y, ss);
      }
    }
  }
  return false;
}

void EditorUI::UpdateCursor(float mouseX) {
  if (mouseX < GetStartX()) {
    if (cass::Input::IsMousePressed(cass::Mouse::Middle))
      m_Window.SetCursor(cass::CursorType::Crosshair);
    else
      m_Window.SetCursor(cass::CursorType::Arrow);
  } else {
    m_Window.SetCursor(cass::CursorType::Hand);
  }
}
