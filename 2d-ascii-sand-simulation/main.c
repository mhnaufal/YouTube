/*
 * TODO: compile to Linux
 */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <memory.h>

#include <windows.h>
#include <signal.h>

#include "curses.h"

#define SAND_COUNT 10
#define SAND_CHAR 'N'

typedef struct Vec2
{
    int x;
    int y;
} Vec2;
const Vec2 STARTING_POSITION = {30, 1};

typedef struct Sand
{
    uint16_t FALLING_SAND_INDEX;
    char SAND_ARRAY[SAND_COUNT];
    Vec2 SAND_POSITIONS[SAND_COUNT];
} Sand;
bool check_if_sand_filled(Sand InSand, uint16_t index);
void add_sand(Sand *InSand);
Vec2 get_sand_position(Sand InSand, uint16_t index);
void set_sand_position(Sand *InSand, uint16_t index, Vec2 position);

void trap(int sig);

int main(void)
{
    WINDOW *window;
    int width = COLS, height = LINES;
    chtype ch;
    bool is_simulation_running = false;

    Sand sand;
    sand.FALLING_SAND_INDEX = 0;
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

        // Draw the falling sand
        wattron(window, COLOR_PAIR(1));
            Vec2 failing_sand_position = get_sand_position(sand, sand.FALLING_SAND_INDEX);

            if (failing_sand_position.x <= 0 || failing_sand_position.y <= 0)
            {
                set_sand_position(&sand, sand.FALLING_SAND_INDEX, STARTING_POSITION);
                failing_sand_position = get_sand_position(sand, sand.FALLING_SAND_INDEX);
            }

            mvwprintw(window, failing_sand_position.y, failing_sand_position.x, "%c", SAND_CHAR);
            failing_sand_position.y++;
            set_sand_position(&sand, sand.FALLING_SAND_INDEX, failing_sand_position);
        wattroff(window, COLOR_PAIR(1));

        if (failing_sand_position.y >= 30)
        {
            add_sand(&sand);
        }

        /*
        // Draw the already fallen sand
        for (uint16_t i = 0; i < SAND_COUNT; ++i)
        {
            bool is_current_sand_filled = check_if_sand_filled(sand, i);
            if (is_current_sand_filled)
            {
                Vec2 fallen_sand_position = get_sand_position(sand, i);
                mvwprintw(window, fallen_sand_position.y, fallen_sand_position.x, "%c", SAND_CHAR | COLOR_PAIR(99));
            }
        }
        */

        // Check if all sand already fallen
        if (sand.FALLING_SAND_INDEX >= SAND_COUNT)
        {
            ch = wgetch(window);
            if (ch == 'q' || ch == 'Q' || ch == '\033')
            {
                is_simulation_running = false;
                break;
            }
        }

        mvwprintw(window, 10, 60, "Falling Sand Index: %d\n", sand.FALLING_SAND_INDEX);
        Sleep(30);

        wrefresh(window);
        wclear(window);
    }

    wrefresh(window);

    curs_set(2);

    endwin();
    delwin(window);

    return 0;
}

bool check_if_sand_filled(Sand InSand, uint16_t index)
{
    if (index < 0 || index >= SAND_COUNT)
    {
        return false;
    }

    return InSand.SAND_ARRAY[index] == SAND_CHAR;
}

void add_sand(Sand *InSand)
{
    if (InSand->FALLING_SAND_INDEX < 0 || InSand->FALLING_SAND_INDEX >= SAND_COUNT)
    {
        return;
    }

    if (InSand->SAND_ARRAY[InSand->FALLING_SAND_INDEX] == SAND_CHAR)
    {
        printw("Sand already filled at index %d\n", InSand->FALLING_SAND_INDEX);
        return;
    }

    InSand->SAND_ARRAY[InSand->FALLING_SAND_INDEX] = SAND_CHAR;
    InSand->FALLING_SAND_INDEX++;
    return;
}

Vec2 get_sand_position(Sand InSand, uint16_t index)
{
    Vec2 position = {0, 0};
    position.x = InSand.SAND_POSITIONS[index].x;
    position.y = InSand.SAND_POSITIONS[index].y;

    return position;
}

void set_sand_position(Sand *InSand, uint16_t index, Vec2 position)
{
    if (index < 0 || index >= SAND_COUNT)
    {
        return;
    }

    InSand->SAND_POSITIONS[index].x = position.x;
    InSand->SAND_POSITIONS[index].y = position.y;
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
