#pragma once
#include <ft2build.h>

#include FT_FREETYPE_H
#include <cass_linear.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Texture2D.hpp"

namespace cass {

struct FTGlyph {
  cass::Vector2<float> Size;
  cass::Vector2<float> Bearing;
  float Advance;

  cass::Vector2<float> UV0;
  cass::Vector2<float> UV1;

  uint32_t GlyphIndex = 0;
};

struct Font {
  std::unique_ptr<Texture2D> atlas;
  std::unordered_map<uint32_t, FTGlyph> Glyphs;
  float LineHeight;
  FT_Face face = nullptr;
};

class FontManager {
public:
  static void Init();
  static void Shutdown();

  static uint32_t Load(const std::string &path, uint32_t size);
  static Font *Get(uint32_t handle);

private:
  static FT_Library s_FreeType;

  static std::vector<Font> s_Fonts;
};
} // namespace cass