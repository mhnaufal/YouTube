#pragma once

#include <curses.h>

class Vector2;
class Vector3;
class Vector4;
class Matrix44;

struct FrameBuffer
{
    FrameBuffer(int w, int h) : width(w), height(h) {}
    int width, height;
};

struct Rasterizer
{
public:
    Rasterizer(int width, int height);
    virtual ~Rasterizer();

    void rasterizerTriangle(const Vector2 &v1, const Vector2 &v2, const Vector2 &v3);
    inline FrameBuffer* getFrameBuffer() { return frameBuffer; }
    void setWindow(WINDOW& win);

private:
    WINDOW* window;
    FrameBuffer* frameBuffer;
    void initializeFrameBuffer(int width, int height);
    bool isPointInTriangle(int ptx, int pty, const Vector2 &v1, const Vector2 &v2, const Vector2 &v3);
};
