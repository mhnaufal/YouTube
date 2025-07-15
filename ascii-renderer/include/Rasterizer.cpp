#include "Rasterizer.h"
#include "curses.h"
#include "defs.h"
#include <math.h>

#include <Vector2.h>
#include <Vector3.h>
#include <Vector4.h>
#include <Matrix44.h>

Rasterizer::Rasterizer(int width, int height) : frameBuffer(nullptr)
{
    initializeFrameBuffer(width, height);
}

Rasterizer::~Rasterizer()
{
    if (frameBuffer)
        delete frameBuffer;
}

void Rasterizer::rasterizerTriangle(const Vector2 &v1, const Vector2 &v2, const Vector2 &v3)
{
    int minx, miny, maxx, maxy;

    minx = static_cast<int>(MAX(0, MIN(v3.x, MIN(v1.x, v2.x))));
    miny = static_cast<int>(MAX(0, MIN(v3.y, MIN(v1.y, v2.y))));
    maxx = static_cast<int>(MIN(WINDOW_WIDTH, ceil(MAX(v3.x, MAX(v1.x, v2.x))) + 1));
    maxy = static_cast<int>(MIN(WINDOW_HEIGHT, ceil(MAX(v3.y, MAX(v1.y, v2.y))) + 1));

    for (int i = minx; i < maxx; ++i)
    {
        for (int j = miny; j < maxy; ++j)
        {
            if (isPointInTriangle(i, j, v1, v2, v3))
            {
                mvwprintw(window, j, i, CHAR_FILLED);
            }
            else
            {
                mvwprintw(window, j, i, CHAR_EMPTY);
            }
        }
    }
}

void Rasterizer::setWindow(WINDOW &win)
{
    window = &win;
}

void Rasterizer::initializeFrameBuffer(int width, int height)
{
    frameBuffer = new FrameBuffer(width, height);
}

bool Rasterizer::isPointInTriangle(int ptx, int pty, const Vector2 &v1, const Vector2 &v2, const Vector2 &v3)
{
    bool is_inside = false;

    float w1_numerator = v1.x * (v3.y - v1.y) + (pty - v1.y) * (v3.x - v1.x) - ptx * (v3.y - v1.y);
    float w1_denominator = (v2.y - v1.y) * (v3.x - v1.x) - (v2.x - v1.x) * (v3.y - v1.y);
    float w1 = w1_numerator / w1_denominator;
    float w2_numerator = pty - v1.y - (w1) * (v2.y - v1.y);
    float w2_denominator = v3.y - v1.y;
    float w2 = w2_numerator / w2_denominator;

    is_inside = (w1 >= 0) && (w2 >= 0) && ((w1 + w2) <= 1);

    return is_inside;
}
