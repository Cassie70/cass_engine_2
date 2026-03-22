#pragma once
#include <memory>

struct Tile {
    bool collisionable;
    cass::Vector4<float> uvs;
};