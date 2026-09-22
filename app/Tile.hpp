#pragma once

struct Tile {
  bool collisionable = false;
  cass::linear::Vector4<float> uvs;
};