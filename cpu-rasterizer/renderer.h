#pragma once

#include "tgaimage.h"

constexpr TGAColor WHITE = {255, 255, 255, 255}; // BGRA color
constexpr TGAColor RED = {0, 0, 255, 255};       // BGRA color
constexpr TGAColor GREEN = {0, 255, 0, 255};     // BGRA color
constexpr TGAColor BLUE = {255, 0, 0, 255};      // BGRA color
constexpr TGAColor YELLOW = {50, 200, 255, 255}; // BGRA color

constexpr int WIDTH = 800;
constexpr int HEIGHT = 800;

inline std::vector<std::vector<double>> VERETICES(2520, std::vector<double>(3, 0));

auto line(int ax, int ay, int bx, int by, TGAImage& frame_buffer, TGAColor color) -> void;
auto line_v2(int ax, int ay, int bx, int by, TGAImage& frame_buffer, TGAColor color) -> void;
auto line_v3(int ax, int ay, int bx, int by, TGAImage& frame_buffer, TGAColor color) -> void;
auto read_obj_file(TGAImage& frame_buffer) -> void;
auto triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage& frame_buffer, TGAColor color) -> void;
