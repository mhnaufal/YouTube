#include "renderer.h"
#include "tgaimage.h"
#include "geometry.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Don't know how this works :) https://haqr.eu/tinyrenderer/z-buffer/
auto signed_triangle_area(int ax, int ay, int bx, int by, int cx, int cy) -> double
{
    return .5 * ((by - ay) * (bx + ax) + (cy - by) * (cx + bx) + (ay - cy) * (ax + cx));
}

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

auto read_obj_file(TGAImage& frame_buffer, TGAImage::Format format) -> void
{
    std::ifstream fff("diablo3_pose.wobj");
    if (!fff.is_open())
    {
        std::cerr << "ERROR " << std::endl;
        return;
    }

    int vcount = 0;
    std::string line{};

// omp uninstalled :)
// #pragma omp parallel
    while (std::getline(fff, line))
    {
        char prefix;
        std::istringstream iss(line);

        // collect vertex buffer
        if (line.rfind("v ", 0) == 0)
        {
            double x, y, z;

            iss >> prefix >> x >> y >> z;       // separated by spaces
            VERETICES.at(vcount) = {x, y, z};
            vcount++;
        }
        else if (line.rfind("f ", 0) == 0)  // draw vertex based on faces
        {
            // f 2494/2675/2494 2491/2662/2491 2495/3259/2495
            int f1, f2, f3;         // 1st vertex geometric
            int vt1, vt2, vt3;      // 2nd vertex texture/UV
            int vn1, vn2, vn3;      // 3rd vertex normal
            char s1, s2;            // space

            iss >> prefix >> f1 >> s1 >> vt1 >> s2 >> vn1 >> f2 >> s1 >> vt2 >> s2 >> vn2 >> f3 >> s1 >> vt3 >> s2 >>
                vn3;

            // OBJ face indices are 1-based, so subtract 1 to index our 0-based vertex buffer
            // Scale back from orthogonal projection to screen projection (World to Clip space)
            auto xv1 = (VERETICES.at(f1 - 1).at(0) + 1) * WIDTH / 2;
            auto yv1 = (VERETICES.at(f1 - 1).at(1) + 1) * HEIGHT / 2;
            auto zv1 = (VERETICES.at(f1 - 1).at(2) + 1) * 255 / 2;
            vec3 vv1 = {xv1, yv1, zv1};

            auto xv2 = (VERETICES.at(f2 - 1).at(0) + 1) * WIDTH / 2;
            auto yv2 = (VERETICES.at(f2 - 1).at(1) + 1) * HEIGHT / 2;
            auto zv2 = (VERETICES.at(f2 - 1).at(2) + 1) * 255 / 2;
            vec3 vv2 = {xv2, yv2, zv2};

            auto xv3 = (VERETICES.at(f3 - 1).at(0) + 1) * WIDTH / 2;
            auto yv3 = (VERETICES.at(f3 - 1).at(1) + 1) * HEIGHT / 2;
            auto zv3 = (VERETICES.at(f3 - 1).at(2) + 1) * 255 / 2;
            vec3 vv3 = {xv3, yv3, zv3};

            triangle(xv1, yv1, zv1, xv2, yv2, zv2, xv3, yv3, zv3, frame_buffer, WHITE, format);
            line_v3(xv1, yv1, xv2, yv2, frame_buffer, RED);
            line_v3(xv1, yv1, xv3, yv3, frame_buffer, GREEN);
            line_v3(xv3, yv3, xv2, yv2, frame_buffer, YELLOW);
        }
    }

    fff.close();
}

auto triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, TGAImage& frame_buffer, TGAColor color, TGAImage::Format format) -> void
{
    // VERSION 2
    int lowest_x = std::min({ax, bx, cx});
    int highest_x = std::max({ax, bx, cx});
    int lowest_y = std::min({ay, by, cy});
    int highest_y = std::max({ay, by, cy});

    double total_area = signed_triangle_area(ax, ay, bx, by, cx, cy);
    if (total_area < 1) return; 

#pragma omp parallel for
    for (int i = lowest_x; i <= highest_x; i += 1)
    {
        for (int j = lowest_y; j <= highest_y; j += 1)
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
                if (frame_buffer.get(i, j) != nullptr)      // if current buffer index is empty, draw?
                {
                    if (format == TGAImage::Format::RGBA)
                    {
                        TGAColor rnd;
                        for (int c = 0; c < 3; c++) rnd[c] = std::rand() % 255;
                        frame_buffer.set(i, j, rnd);
                    }
                    else if (format == TGAImage::Format::GRAYSCALE)
                    {
                        double alpha    = signed_triangle_area(i, j, bx, by, cx, cy) / total_area;
                        double beta     = signed_triangle_area(i, j, cx, cy, ax, ay) / total_area;
                        double gamma    = signed_triangle_area(i, j, ax, ay, bx, by) / total_area;
                        if (alpha < 0 || beta < 0 || gamma < 0)
                            continue;
                        unsigned char grey = static_cast<unsigned char>(alpha * az + beta * bz + gamma * cz);
                        frame_buffer.set(i, j, {grey});
                    }
                }
            }
        }
    }

    // VERSION 1
    // line_v3(ax, ay, bx, by, frame_buffer, RED);
    // line_v3(bx, by, cx, cy, frame_buffer, BLUE);
    // line_v3(ax, ay, cx, cy, frame_buffer, GREEN);
}
