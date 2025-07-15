#include <iostream>
#include <chrono>
#include <thread>

#include <curses.h>

#define PI (3.141592653f)

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define WINDOW_WIDTH (COLS)
#define WINDOW_HEIGHT (LINES)

#define CHAR_FILLED ("X")
#define CHAR_EMPTY (" ")
