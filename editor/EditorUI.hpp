#pragma once
#include <Event.hpp>
#include <SpriteSheet.hpp>
#include <Texture2D.hpp>
#include <Window.hpp>
#include <camera/OrthographicCamera.hpp>
#include <cass_linear.hpp>

class EditorUI {
public:
  EditorUI(cass::engine::Window &window);

  void Init(float tileSize, int columns);
  void Draw(
    cass::engine::OrthographicCamera &camera, cass::engine::Texture2D &atlas,
    cass::engine::SpriteSheet &ss
  );

  bool
  HandleMouseClick(float mouseX, float mouseY, cass::engine::SpriteSheet &ss);
  bool HandleEvent(cass::engine::Event &e, cass::engine::SpriteSheet &ss);
  void UpdateCursor(float mouseX);

  float GetPanelWidth() const { return panelWidth; }
  float GetStartX() const;

  bool HasSelection() const { return hasSelection; }
  int GetSelectedRow() const { return selectedRow; }
  int GetSelectedCol() const { return selectedCol; }

private:
  void DrawPanel(
    cass::engine::Texture2D &atlas, cass::engine::SpriteSheet &ss,
    int windowWidth, int windowHeight
  );

  cass::engine::Window &m_Window;
  uint32_t arial24;
  int layer = 0;

  float uiTileSize = 75.0f;
  int uiColumns = 5;
  float panelWidth = 0.0f;

  int selectedRow = -1;
  int selectedCol = -1;
  cass::linear::Vector2<float> selectedPos;
  bool hasSelection = false;
};
