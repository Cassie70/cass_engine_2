struct SpriteSheet
{
    int rows;
    int cols;
    float paddingX;
    float paddingY;
    float offsetX;
    float offsetY;

    // Función estática para crear desde pixeles
    static SpriteSheet FromPixels(
        int textureWidth, int textureHeight, // tamaño total de la textura
        int spriteWidth, int spriteHeight,   // tamaño de cada sprite
        int rows, int cols,                  // filas y columnas
        int paddingLeft = 0, int paddingTop = 0, // padding en pixeles
        int paddingRight = 0, int paddingBottom = 0
    )
    {
        SpriteSheet sheet{};
        sheet.rows = rows;
        sheet.cols = cols;

        // Convertimos el padding a UV
        sheet.paddingX = float(paddingLeft + paddingRight) / textureWidth;
        sheet.paddingY = float(paddingTop + paddingBottom) / textureHeight;

        // Offset inicial
        sheet.offsetX = float(paddingLeft) / textureWidth;
        sheet.offsetY = float(paddingTop) / textureHeight;

        return sheet;
    }
};
