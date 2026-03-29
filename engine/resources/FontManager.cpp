#include "FontManager.hpp"


FT_Library FontManager::s_FreeType;
std::vector<Font> FontManager::s_Fonts;

void FontManager::Init()
{
	if (FT_Init_FreeType(&s_FreeType))
		std::cout << "FreeType init failed\n";
}

void FontManager::Shutdown()
{
	s_Fonts.clear();

	if (s_FreeType)
	{
		FT_Done_FreeType(s_FreeType);
		s_FreeType = nullptr;
	}
}


uint32_t FontManager::Load(const std::string& path, uint32_t size)
{
	if (!s_FreeType)
	{
		std::cout << "[FontManager] ERROR: FreeType not initialized!\n";
		return 0;
	}

	FT_Face face;
	if (FT_New_Face(s_FreeType, path.c_str(), 0, &face))
	{
		std::cout << "Failed to load font\n";
		return 0;
	}

	FT_Set_Pixel_Sizes(face, 0, size);

	Font font;

	// 👉 aquí reutilizas TU código de atlas casi igual
	const uint32_t ATLAS_WIDTH = 1024;
	const uint32_t ATLAS_HEIGHT = 1024;
	const uint32_t PADDING = 1;

	std::vector<unsigned char> atlasBuffer(
		ATLAS_WIDTH * ATLAS_HEIGHT,
		0
	);

	uint32_t x = 0;
	uint32_t y = 0;
	uint32_t rowHeight = 0;

	font.LineHeight = (float)(face->size->metrics.height >> 6);

	// ===============================
	// Pack glyphs
	// ===============================

	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "[Renderer2D] Warning: Failed glyph '" << c << "'\n";
			continue;
		}

		FT_GlyphSlot g = face->glyph;

		// New row if needed
		if (x + g->bitmap.width + PADDING >= ATLAS_WIDTH) {
			x = 0;
			y += rowHeight + PADDING;
			rowHeight = 0;
		}

		if (y + g->bitmap.rows >= ATLAS_HEIGHT) {
			std::cout << "[Renderer2D] ERROR: Font atlas overflow!\n";
			break;
		}

		// Copy bitmap into atlas buffer
		for (uint32_t row = 0; row < g->bitmap.rows; row++)
		{
			memcpy(
				&atlasBuffer[(x + (y + row) * ATLAS_WIDTH)],
				&g->bitmap.buffer[row * g->bitmap.width],
				g->bitmap.width
			);
		}

		FTGlyph glyph;

		glyph.Size = { (float)g->bitmap.width, (float)g->bitmap.rows };
		glyph.Bearing = { (float)g->bitmap_left, (float)g->bitmap_top };
		glyph.Advance = (float)(g->advance.x >> 6);

		glyph.UV0 = {
			(float)x / ATLAS_WIDTH,
			(float)y / ATLAS_HEIGHT
		};

		glyph.UV1 = {
			(float)(x + g->bitmap.width) / ATLAS_WIDTH,
			(float)(y + g->bitmap.rows) / ATLAS_HEIGHT
		};

		font.Glyphs[c] = glyph;

		x += g->bitmap.width + PADDING;
		rowHeight = std::max(rowHeight, g->bitmap.rows);
	}

	// ===============================
	// Create atlas texture
	// ===============================

	font.atlas = std::make_unique<Texture2D>(
		ATLAS_WIDTH,
		ATLAS_HEIGHT,
		atlasBuffer.data()
	);

	FT_Done_Face(face);

	s_Fonts.push_back(std::move(font));

	uint32_t handle = (uint32_t)(s_Fonts.size() - 1);

	std::cout << "[FontManager] Font loaded successfully: "
		<< path << " (" << size << "px) -> ID: "
		<< handle << "\n";

	return handle;
}

Font* FontManager::Get(uint32_t handle)
{
	return &s_Fonts[handle];
}