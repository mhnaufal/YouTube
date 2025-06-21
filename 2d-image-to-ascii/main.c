#include <stdio.h>
#include <stdbool.h>
#include <memory.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char *ASCII_CHARS = "Ñ@#W$9876543210?!abc;:+=-_,. ";
const int ASCII_CHAR_COUNT = 29;

int map(int value, int in_min, int in_max, int out_min, int out_max);

typedef struct ImageDesc
{
    int width;
    int height;
    int channel;
    unsigned char *pixels;
} ImageDesc;

int main(void)
{
    // LOAD PNG IMAGE
    int w, h, c;
    ImageDesc *image_desc = (ImageDesc *)malloc(sizeof(ImageDesc));
    image_desc->pixels = stbi_load("cat.png", &w, &h, &c, 0);

    if (image_desc->pixels == NULL)
    {
        return EXIT_FAILURE;
    }

    image_desc->width = w;
    image_desc->height = h;
    image_desc->channel = c;

    // CONVERT TO ASCII
    for(int row = 0; row < image_desc->height; ++row)
    {
        for(int col = 0; col < image_desc->width; ++col)
        {
            int pixel_index = (row * image_desc->width + col) * image_desc->channel;
            unsigned char r = image_desc->pixels[pixel_index + 0];
            unsigned char g = image_desc->pixels[pixel_index + 1];
            unsigned char b = image_desc->pixels[pixel_index + 2];

            int gray = (r + g + b) / 3;

            int ascii_index = map(gray, 0, 255, 0, ASCII_CHAR_COUNT);
            printf("%c", ASCII_CHARS[ascii_index]);
        }
        printf("\n");
    }

    stbi_image_free(image_desc->pixels);
    free(image_desc);

    return EXIT_SUCCESS;
}

int map(int value, int in_min, int in_max, int out_min, int out_max)
{
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
