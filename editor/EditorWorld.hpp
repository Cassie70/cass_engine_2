#pragma once

#include <SpriteSheet.hpp>
#include <Texture2D.hpp>
#include <camera/OrthographicCamera.hpp>
#include <cstdint>
#include <unordered_map>

class EditorWorld {
 public:
  EditorWorld();
  void Init();
  void Draw(
    cass::OrthographicCamera& camera, cass::Texture2D& atlas,
    cass::SpriteSheet& ss, float worldTileSize, int windowWidth,
    int windowHeight
  );
  void DrawGridInfinite(
    cass::OrthographicCamera& camera, float tileSize, uint32_t color,
    float weight, int windowWidth, int windowHeight
  );

  void SetTile(int x, int y, uint8_t tile);
  uint8_t GetTile(int x, int y) const;
  void PaintTileAt(cass::Vector2<float> mouseWorld, int atlasIndex);
  void EraseTileAt(cass::Vector2<float> mouseWorld);

  int GetWidth() const;
  int GetHeight() const;

 private:
  void RecalculateBounds();
  std::unordered_map<uint64_t, uint8_t> mapTile;
  int minX = 0;
  int maxX = 0;
  int minY = 0;
  int maxY = 0;
  bool isEmpty = true;
};
