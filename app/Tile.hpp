#pragma once
#include <memory>

struct Tile {
    bool collisionable = false;
    cass::Vector4<float> uvs;
};