#include "EditorWorld.hpp"

#include <Renderer2D.hpp>

using namespace cass;

EditorWorld::EditorWorld() {}

void EditorWorld::Init() {
  mapTile.clear();
  minX = 0;
  maxX = 0;
  minY = 0;
  maxY = 0;
  isEmpty = true;
}

void EditorWorld::RecalculateBounds() {
  if (mapTile.empty()) {
    isEmpty = true;
    minX = maxX = minY = maxY = 0;
    return;
  }

  auto it = mapTile.begin();
  int firstY = (int)(it->first >> 32);
  int firstX = (int)(it->first & 0xFFFFFFFF);
  minX = maxX = firstX;
  minY = maxY = firstY;

  for (++it; it != mapTile.end(); ++it) {
    int curY = (int)(it->first >> 32);
    int curX = (int)(it->first & 0xFFFFFFFF);
    if (curX < minX) minX = curX;
    if (curX > maxX) maxX = curX;
    if (curY < minY) minY = curY;
    if (curY > maxY) maxY = curY;
  }
}

void EditorWorld::SetTile(int x, int y, uint8_t tile) {
  uint64_t key = ((uint64_t)(uint32_t)y << 32) | (uint32_t)x;
  if (tile == 255) {
    if (mapTile.erase(key) > 0) {
      if (mapTile.empty()) {
        isEmpty = true;
        minX = maxX = minY = maxY = 0;
      } else if (x == minX || x == maxX || y == minY || y == maxY) {
        RecalculateBounds();
      }
    }
  } else {
    mapTile[key] = tile;
    if (isEmpty) {
      minX = maxX = x;
      minY = maxY = y;
      isEmpty = false;
    } else {
      if (x < minX) minX = x;
      if (x > maxX) maxX = x;
      if (y < minY) minY = y;
      if (y > maxY) maxY = y;
    }
  }
}

uint8_t EditorWorld::GetTile(int x, int y) const {
  uint64_t key = ((uint64_t)(uint32_t)y << 32) | (uint32_t)x;
  auto it = mapTile.find(key);
  if (it != mapTile.end()) {
    return it->second;
  }
  return 255;
}

void EditorWorld::PaintTileAt(Vector2<float> mouseWorld, int atlasIndex) {
  int tileX = (int)floor(mouseWorld.x / 16.0f);
  int tileY = (int)floor(mouseWorld.y / 16.0f);

  SetTile(tileX, tileY, atlasIndex);
}

void EditorWorld::EraseTileAt(Vector2<float> mouseWorld) {
  PaintTileAt(mouseWorld, 255);
}

int EditorWorld::GetWidth() const {
  if (isEmpty) return 0;
  return maxX - minX + 1;
}

int EditorWorld::GetHeight() const {
  if (isEmpty) return 0;
  return maxY - minY + 1;
}

void EditorWorld::Draw(
  OrthographicCamera& camera, Texture2D& atlas, SpriteSheet& ss,
  float worldTileSize, int windowWidth, int windowHeight
) {
  Renderer2D::BeginScene(camera);

  for (const auto& pair : mapTile) {
    uint8_t tile = pair.second;
    if (tile == 255) continue;  // vacío

    int y = (int)(pair.first >> 32);
    int x = (int)(pair.first & 0xFFFFFFFF);

    int row = tile / ss.cols;
    int col = tile % ss.cols;

    Renderer2D::DrawSprite(
      SpriteProperties{
        .position = {(float)x * worldTileSize, (float)y * worldTileSize},
        .size = {worldTileSize, worldTileSize},
        .texture = &atlas,
        .uv = ss.GetUV(row, col),
        .origin = {0, 0}
      }
    );
  }

  DrawGridInfinite(camera, 16.0f, 0xFF555555, 1.0f, windowWidth, windowHeight);

  Renderer2D::EndScene();
}

void EditorWorld::DrawGridInfinite(
  OrthographicCamera& camera, float tileSize, uint32_t color, float weight,
  int windowWidth, int windowHeight
) {
  Vector3<float> camPos = camera.GetPosition();
  float zoom = camera.GetZoom();

  float viewWidth = windowWidth * zoom;
  float viewHeight = windowHeight * zoom;

  float left = camPos.x - viewWidth * 0.5f;
  float right = camPos.x + viewWidth * 0.5f;
  float bottom = camPos.y - viewHeight * 0.5f;
  float top = camPos.y + viewHeight * 0.5f;

  float startX = floor(left / tileSize) * tileSize;
  float startY = floor(bottom / tileSize) * tileSize;

  // Verticales
  for (float x = startX; x <= right; x += tileSize) {
    Renderer2D::DrawCartesianLine(
      CartesianLineProperties{
        .start = {x, bottom}, .end = {x, top}, .argb = color, .weight = weight
      }
    );
  }

  // Horizontales
  for (float y = startY; y <= top; y += tileSize) {
    Renderer2D::DrawCartesianLine(
      CartesianLineProperties{
        .start = {left, y}, .end = {right, y}, .argb = color, .weight = weight
      }
    );
  }

  if (left <= 0 && right >= 0) {
    Renderer2D::DrawCartesianLine(
      CartesianLineProperties{
        .start = {0, bottom},
        .end = {0, top},
        .argb = 0xFFFF4444,  // rojo
        .weight = weight
      }
    );
  }

  // Eje X (horizontal en y = 0)
  if (bottom <= 0 && top >= 0) {
    Renderer2D::DrawCartesianLine(
      CartesianLineProperties{
        .start = {left, 0},
        .end = {right, 0},
        .argb = 0xFF44FF44,  // verde
        .weight = weight
      }
    );
  }
}
