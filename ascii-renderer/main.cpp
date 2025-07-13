#include <defs.h>
#include <curses.h>

int main()
{
    initscr();
    start_color();
    cbreak();
    noecho();
    refresh();

    WINDOW *window;
    int width = COLS, height = LINES;
    window = newwin(height, width, 0, 0);

    if (window == NULL)
    {
        endwin();
        printf("Failed to create window\n");
        return 1;
    }

    while (true)
    {
        box(window, 0, 0);
        mvwprintw(window, 1, 1, "Hello, World!");
        wrefresh(window);
        wclear(window);
    }

    endwin();
    delwin(window);
    return 0;
}
