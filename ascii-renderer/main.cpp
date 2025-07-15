#include <defs.h>
#include <Rasterizer.h>
#include <Vector2.h>

int main()
{
    initscr();
    start_color();
    cbreak();
    noecho();
    refresh();

    WINDOW *window;
    window = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);

    if (window == nullptr)
    {
        endwin();
        printf("Failed to create window\n");
        return 1;
    }

    Vector2 vec21(0, 0);
    Vector2 vec22(30, 0);
    Vector2 vec23(45, 45);
    Rasterizer rasterizer{WINDOW_WIDTH, WINDOW_HEIGHT};
    rasterizer.setWindow(*window);

    while (true)
    {
        rasterizer.rasterizerTriangle(vec21, vec22, vec23);
        wrefresh(window);
        wclear(window);
    }

    endwin();
    delwin(window);
    return 0;
}
