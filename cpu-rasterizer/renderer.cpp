#include "renderer.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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

auto read_obj_file(TGAImage& frame_buffer) -> void
{
    std::ifstream fff("diablo3_pose.wobj");
    if (!fff.is_open())
    {
        std::cerr << "ERROR " << std::endl;
        return;
    }

    int i = 1;
    std::string line{};
    while (std::getline(fff, line))
    {
        char prefix;
        std::istringstream iss(line);

        if (line.rfind("v ", 0) == 0)
        {
            double x, y, z;

            iss >> prefix >> x >> y >> z;
            VERETICES.at(i - 1) = {x, y, z};
        }
        else if (line.rfind("f ", 0) == 0)
        {
            int f1, f2, f3;
            int vt1, vt2, vt3;
            int vn1, vn2, vn3;
            char s1, s2;

            iss >> prefix >> f1 >> s1 >> vt1 >> s2 >> vn1 >> f2 >> s1 >> vt2 >> s2 >> vn2 >> f3 >> s1 >> vt3 >> s2 >>
                vn3;

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

            triangle(x1, y1, x2, y2, x3, y3, frame_buffer, WHITE);
            line_v3(x1, y1, x2, y2, frame_buffer, RED);
            line_v3(x1, y1, x3, y3, frame_buffer, GREEN);
            line_v3(x3, y3, x2, y2, frame_buffer, YELLOW);
        }
        i = i + 1;
    }

    fff.close();
}

auto triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage& frame_buffer, TGAColor color) -> void
{
    int lowest_x = std::min({ax, bx, cx});
    int highest_x = std::max({ax, bx, cx});
    int lowest_y = std::min({ay, by, cy});
    int highest_y = std::max({ay, by, cy});
    double lowest_x_to_camera = lowest_x;

#pragma omp parallel for
    for (int i = lowest_x + 1; i <= highest_x; i += 1)
    {
        for (int j = lowest_y + 1; j <= highest_y; j += 1)
        {
            // https://www.youtube.com/watch?v=HYAgJN3x4GA: computational faster
            double s1 = cy - ay;
            double s2 = cx - ax;
            double s3 = by - ay;
            double s4 = j - ay;
            double w1 = (ax * s1 + s4 * s2 - i * s1) / (s3 * s2 - (bx - ax) * s1);
            double w2 = (s4 - w1 * s3) / s1;

            if (w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1)
            {
                int p1 = i - 0;
                if (frame_buffer.get(i, j) != nullptr)
                {
                    TGAColor rnd;
                    for (int c = 0; c < 3; c++) rnd[c] = std::rand() % 255;
                    frame_buffer.set(i, j, rnd);
                }
            }
        }
    }

    // line_v3(ax, ay, bx, by, frame_buffer, RED);
    // line_v3(bx, by, cx, cy, frame_buffer, BLUE);
    // line_v3(ax, ay, cx, cy, frame_buffer, GREEN);
}
