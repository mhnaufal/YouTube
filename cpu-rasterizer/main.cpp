#include "tgaimage.h"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

constexpr TGAColor WHITE = {255, 255, 255, 255}; // BGRA color
constexpr TGAColor RED = {0, 0, 255, 255};       // BGRA color
constexpr TGAColor GREEN = {0, 255, 0, 255};     // BGRA color
constexpr TGAColor BLUE = {255, 0, 0, 255};      // BGRA color
constexpr TGAColor YELLOW = {50, 200, 255, 255}; // BGRA color

std::vector<std::vector<double>> VERETICES(2520, std::vector<double>(3, 0));
// std::vector<int> FACES[5022];

constexpr int WIDTH = 800;
constexpr int HEIGHT = 800;
TGAImage frame_buffer(WIDTH, HEIGHT, TGAImage::RGBA);

auto line(int ax, int ay, int bx, int by, TGAImage& frame_buffer, TGAColor color) -> void
{
    for (float t = 0; t < 1; t += 0.001)
    {
        int x = ax + t * (bx - ax);
        int y = ay + t * (by - ay);
        frame_buffer.set(x, y, color);
    }
}

auto line_v2(int ax, int ay, int bx, int by, TGAImage& frame_buffer, TGAColor color) -> void
{
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    if (steep == true)
    {
        std::swap(ax, ay);
        std::swap(bx, by);
    }

    if (ax > bx)
    {
        std::swap(ax, bx);
        std::swap(ay, by);
    }

    for (int x = ax; x <= bx; x++)
    {
        if (bx - ax == 0)
            continue;

        float t = (x - ax) / static_cast<float>((bx - ax));
        int y = ay + t * (by - ay);

        if (steep)
            frame_buffer.set(y, x, color);
        else
            frame_buffer.set(x, y, color);
    }
}

auto line_v3(int ax, int ay, int bx, int by, TGAImage& frame_buffer, TGAColor color) -> void
{
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    if (steep == true)
    {
        std::swap(ax, ay);
        std::swap(bx, by);
    }

    if (ax > bx)
    {
        std::swap(ax, bx);
        std::swap(ay, by);
    }

    float y = ay;
    int ierr = 0;
    for (int x = ax; x <= bx; x++)
    {
        if (bx - ax == 0)
            continue;

        if (steep)
            frame_buffer.set(y, x, color);
        else
            frame_buffer.set(x, y, color);

        ierr += 2 * std::abs(by - ay);
        y += (by > ay ? 1 : -1) * (ierr > bx - ax);
        ierr -= 2 * (bx - ax) * (ierr > bx - ax);
    }
}

auto read_obj_file() -> void
{
    std::ifstream fff("diablo3_pose.wobj");
    if (!fff.is_open())
    {
        std::cerr << "ERROR " << std::endl;
        return;
    }

    int i = 1;
    std::string line{};
    while(std::getline(fff, line))
    {
        char prefix;
        std::istringstream iss(line);

        if (line.rfind("v ", 0) == 0)
        {
            double x, y, z;

            iss >> prefix >> x >> y >> z;
            VERETICES.at(i - 1) = {x, y, z};

        } else if (line.rfind("f ", 0) == 0)
        {
            int f1, f2, f3;
            int vt1, vt2, vt3;
            int vn1, vn2, vn3;
            char s1, s2;

            iss >> prefix 
                >> f1 >> s1 >> vt1 >> s2 >> vn1
                >> f2 >> s1 >> vt2 >> s2 >> vn2 
                >> f3 >> s1 >> vt3 >> s2 >> vn3;

            // Scale back from orthogonal projection to screen projection
            auto x1 = (VERETICES.at(f1).at(0) + 1) * WIDTH / 2;
            auto y1 = (VERETICES.at(f1).at(1) + 1) * HEIGHT / 2;
            auto z1 = (VERETICES.at(f1).at(2) + 1);

            auto x2 = (VERETICES.at(f2).at(0) + 1) * WIDTH / 2;
            auto y2 = (VERETICES.at(f2).at(1) + 1) * HEIGHT / 2;
            auto z2 = (VERETICES.at(f2).at(2) + 1);

            auto x3 = (VERETICES.at(f3).at(0) + 1) * WIDTH / 2;
            auto y3 = (VERETICES.at(f3).at(1) + 1) * HEIGHT / 2;
            auto z3 = (VERETICES.at(f3).at(2) + 1);

            line_v3(x1, y1, x2, y2, frame_buffer, RED);
            line_v3(x1, y1, x3, y3, frame_buffer, GREEN);
            line_v3(x3, y3, x2, y2, frame_buffer, YELLOW);
        }
        i = i + 1;
    }

    fff.close();
}

int main(int argc, char** argv)
{
    int ax = 17, ay = 13;
    int bx = 42, by = 107;
    int cx = 122, cy = 140;

    // frame_buffer.set(ax, ay, WHITE);
    // frame_buffer.set(bx, by, WHITE);
    // frame_buffer.set(cx, cy, WHITE);

    // line(ax, ay, bx, by, frame_buffer, GREEN);
    // line(ax, ay, cx, cy, frame_buffer, YELLOW);
    // line(bx, by, cx, cy, frame_buffer, RED);
    // frame_buffer.write_tga_file("main1.tga");

    // line_v3(ax, ay, bx, by, frame_buffer, GREEN);
    // line_v3(ax, ay, cx, cy, frame_buffer, YELLOW);
    // line_v3(bx, by, cx, cy, frame_buffer, RED);
    // frame_buffer.write_tga_file("main2.tga");

    read_obj_file();
    frame_buffer.write_tga_file("main3.tga");

    return EXIT_SUCCESS;
}