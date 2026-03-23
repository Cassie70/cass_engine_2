#pragma once
#include "Tile.hpp"
#include <memory>
#include "Renderer2D.hpp"
#include "SpriteSheet.hpp"
#include <filesystem>
#include <fstream>
#include <string>
class TileManager {
public:
    std::vector<std::vector<uint8_t>> mapTile;
private:
    Tile tiles[16];
    SpriteSheet atlas;
    Texture2D atlasTexture;
    

    void createTiles() { 
        tiles[0] = Tile{ false, atlas.GetUV(0,0) }; 
        tiles[1] = Tile{ true, atlas.GetUV(0,1) };
    }

    void readTileMap(const std::string& path) {
        std::ifstream file(path);

        if (!file.is_open()) {
            std::cout << "Fail to open tileMap file\n";
            return;
        }

        mapTile.clear();

        std::string line;

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::vector<uint8_t> row;

            std::string hexValue;

            while (ss >> hexValue) {
                uint8_t value = static_cast<uint8_t>(
                    std::stoi(hexValue, nullptr, 16)
                    );
                row.push_back(value);
            }

            mapTile.push_back(row);
        }

        file.close();
    }


public:
    TileManager(std::string atlasTexturePath, std::string atlasMapPath) : atlasTexture(atlasTexturePath, {}) {
        atlas = SpriteSheetParams{
            .textureWidth = (int)atlasTexture.GetWidth(),
            .textureHeight = (int)atlasTexture.GetHeight(),
            .spriteWidth = 16,
            .spriteHeight = 16,
            .rows = 1,
            .cols = 2
            };

        createTiles();
        readTileMap(atlasMapPath);
    }
    void draw(cass::Vector3<float> cameraPosition, int screenCols, int screenRows) {
        for (int i = 0; i < mapTile.size(); i++) {
            for (int j = 0; j < mapTile[i].size(); j++) {

                float y = mapTile.size() - i - 1;

                if (j > cameraPosition.x + (screenCols/2 + 1) || j < cameraPosition.x - (screenCols / 2 + 1) ||
                    y > cameraPosition.y + (screenRows/2 + 1) || y < cameraPosition.y - (screenRows / 2 + 1)) {
                    continue;
                }

                uint8_t tileID = mapTile[i][j];

                Renderer2D::DrawSprite({
                    .position = cass::Vector2<float>(j, y),
                    .size = {1,1},
                    .texture = &atlasTexture,
                    .uv = tiles[tileID].uvs
                    });
            }
        }
    }

    bool IsSolid(int x, int y) {
        int mapY = mapTile.size() - y - 1;

        if (mapY < 0 || mapY >= mapTile.size()) return false;
        if (x < 0 || x >= mapTile[mapY].size()) return false;

        uint8_t id = mapTile[mapY][x];
        return tiles[id].collisionable;
    }
};