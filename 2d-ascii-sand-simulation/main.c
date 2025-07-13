/*
 * TODO: compile to Linux
 */

#include "sand.h"
#include "water.h"

#include <stdio.h>
#include <stdbool.h>
#include <memory.h>

#include <windows.h>
#include <signal.h>
#include <winuser.h>

#include "curses.h"

void trap(int sig);

int main(void)
{
    WINDOW *window;
    int width = COLS, height = LINES;
    chtype ch;
    bool is_simulation_running = false;

    Sand sand;
    sand.FALLING_SAND_INDEX = 0;
    sand.CURRENT_SAND_LEVEL = 30;
    memset(sand.SAND_ARRAY, '0', sizeof(sand.SAND_ARRAY));
    memset(sand.SAND_POSITIONS, 0, sizeof(sand.SAND_POSITIONS));

    printf("\x1B[2J");
    initscr();
    start_color();
    cbreak();
    noecho();
    refresh();

    signal(SIGINT, trap);

    window = newwin(height, width, 0, 0);
    if (window == NULL)
    {
        endwin();
        printf("Failed to create window\n");
        return 1;
    }
    is_simulation_running = true;

    init_pair(1, COLOR_RED, COLOR_WHITE); // Sand color
    init_pair(2, COLOR_BLUE, COLOR_WHITE);   // Water color
    init_pair(3, COLOR_WHITE, COLOR_HIGHLIGHTTEXT);

    curs_set(0);
    wbkgd(window, COLOR_PAIR(2));

    while (is_simulation_running)
    {
        if (!is_simulation_running)
        {
            is_simulation_running = false;
            break;
        }

        box(window, 0, 0);

        //* Draw the falling sand
        wattron(window, COLOR_PAIR(1));
            Vec2 failing_sand_position = get_sand_position(sand, sand.FALLING_SAND_INDEX);

            if (failing_sand_position.x <= 0 || failing_sand_position.y <= 0)
            {
                set_sand_position(&sand, sand.FALLING_SAND_INDEX, SAND_STARTING_POSITION);
                failing_sand_position = get_sand_position(sand, sand.FALLING_SAND_INDEX);
            }

            mvwprintw(window, failing_sand_position.y, failing_sand_position.x, "%c", SAND_CHAR);
            failing_sand_position.y++;
            failing_sand_position.y++;
            set_sand_position(&sand, sand.FALLING_SAND_INDEX, failing_sand_position);
        wattroff(window, COLOR_PAIR(1));

        //* Calculate final sand position
        if (failing_sand_position.y >= sand.CURRENT_SAND_LEVEL)
        {
            set_fallen_sand_position(&sand, failing_sand_position);
        }

        //* Draw the fallen sands
        wattron(window, COLOR_PAIR(3));
            for (uint16_t i = 0; i < sand.FALLING_SAND_INDEX; ++i)
            {
                bool is_current_sand_filled = check_if_sand_filled(sand, i);
                if (is_current_sand_filled)
                {
                    Vec2 fallen_sand_position = get_sand_position(sand, i);
                    mvwprintw(window, fallen_sand_position.y, fallen_sand_position.x, "%c", SAND_CHAR);
                }

                if (i >= 1)
                    mvwprintw(window, 20, 60, "=> Position: %d %d\n", sand.SAND_POSITIONS[i].x, sand.SAND_POSITIONS[i].y);
            }
        wattroff(window, COLOR_PAIR(3));

        //* Check if all sand already fallen
        if (sand.FALLING_SAND_INDEX >= SAND_COUNT)
        {
            ch = wgetch(window);
            if (ch == 'q' || ch == 'Q' || ch == '\033')
            {
                is_simulation_running = false;
            }
        }

        mvwprintw(window, 10, 60, "=> Falling Sand Count: %d\n", sand.FALLING_SAND_INDEX);
        mvwprintw(window, 15, 60, "=> Fallen Sand Count: %d\n", SAND_COUNT - sand.FALLING_SAND_INDEX);
        Sleep(10);

        wrefresh(window);
        wclear(window);
    }

    wrefresh(window);

    curs_set(2);

    endwin();
    delwin(window);

    return 0;
}

void trap(int sig)
{
    if (sig == SIGINT)
    {
        curs_set(2);
        endwin();
        exit(0);
    }
}
