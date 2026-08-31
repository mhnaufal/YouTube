#include "renderer.h"
#include "tgaimage.h"
#include <iostream>

int main(int argc, char** argv)
{
    TGAImage frame_buffer(WIDTH, HEIGHT, TGAImage::RGBA);
    TGAImage depth_buffer(WIDTH, HEIGHT, TGAImage::GRAYSCALE);

    int ax = 20, ay = 20;
    int bx = 100, by = 680;
    int cx = 450, cy = 250;

    /* */
    // frame_buffer.set(ax, ay, WHITE);
    // frame_buffer.set(bx, by, WHITE);
    // frame_buffer.set(cx, cy, WHITE);

    /* */
    // line(ax, ay, bx, by, frame_buffer, GREEN);
    // line(ax, ay, cx, cy, frame_buffer, YELLOW);
    // line(bx, by, cx, cy, frame_buffer, RED);
    // frame_buffer.write_tga_file("main1.tga");

    /* */
    // line_v3(ax, ay, bx, by, frame_buffer, GREEN);
    // line_v3(ax, ay, cx, cy, frame_buffer, YELLOW);
    // line_v3(bx, by, cx, cy, frame_buffer, RED);
    // frame_buffer.write_tga_file("main2.tga");

    /* Backbuffer */
    // read_obj_file(frame_buffer, TGAImage::RGBA);
    read_obj_file(depth_buffer, TGAImage::GRAYSCALE);
    // frame_buffer.write_tga_file("main3.tga");
    depth_buffer.write_tga_file("main4.tga");

    /* */
    // triangle(ax, ay, bx, by, cx, cy, frame_buffer, GREEN);
    // frame_buffer.write_tga_file("main4.tga");

    return EXIT_SUCCESS;
}