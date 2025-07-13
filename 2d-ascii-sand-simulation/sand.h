#ifndef SAND_H
#define SAND_H

#include "vec2.h"
#include "curses.h"

#include <stdint.h>

#define SAND_COUNT 100
#define SAND_CHAR 'O'

const Vec2 SAND_STARTING_POSITION = {30, 1};

typedef struct Sand
{
    uint16_t FALLING_SAND_INDEX;
    char SAND_ARRAY[SAND_COUNT];
    Vec2 SAND_POSITIONS[SAND_COUNT];
    int CURRENT_SAND_LEVEL;
} Sand;

inline bool check_if_sand_filled(Sand InSand, uint16_t index)
{
    if (index < 0 || index >= SAND_COUNT)
    {
        return false;
    }

    return InSand.SAND_ARRAY[index] == SAND_CHAR;
}

inline void add_sand(Sand *InSand, Vec2 final_sand_position)
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
    InSand->SAND_POSITIONS[InSand->FALLING_SAND_INDEX] = final_sand_position;
    InSand->FALLING_SAND_INDEX++;
    return;
}

inline Vec2 get_sand_position(Sand InSand, uint16_t index)
{
    Vec2 position = {0, 0};
    position.x = InSand.SAND_POSITIONS[index].x;
    position.y = InSand.SAND_POSITIONS[index].y;

    return position;
}

inline void set_sand_position(Sand *InSand, uint16_t index, Vec2 position)
{
    if (index < 0 || index >= SAND_COUNT)
    {
        return;
    }

    InSand->SAND_POSITIONS[index].x = position.x;
    InSand->SAND_POSITIONS[index].y = position.y;
}

inline void set_fallen_sand_position(Sand* InSand, Vec2 final_position)
{
    if (InSand->FALLING_SAND_INDEX == 0)
    {
        final_position = final_position;
    }
    else if (InSand->FALLING_SAND_INDEX == 1)
    {
        final_position.x = final_position.x - 1;
    }
    else
    {
        if (InSand->FALLING_SAND_INDEX % 2 == 0)
        {
            final_position.x = InSand->SAND_POSITIONS[InSand->FALLING_SAND_INDEX - 1].x + InSand->FALLING_SAND_INDEX;
            if (final_position.x < X_LEFT_WALL)
            {
                // final_position.x = (InSand->SAND_POSITIONS[0].x % X_LEFT_WALL) + final_position.x;
                final_position.x = final_position.x + 10;
            }
            else if (final_position.x > X_RIGHT_WALL)
            {
                // final_position.x = final_position.x - (X_RIGHT_WALL % InSand->SAND_POSITIONS[0].x);
                final_position.x = final_position.x - 10;
            }
        }
        else
        {
            final_position.x = InSand->SAND_POSITIONS[InSand->FALLING_SAND_INDEX - 1].x - InSand->FALLING_SAND_INDEX;
            if (final_position.x < X_LEFT_WALL)
            {
                // final_position.x = (InSand->SAND_POSITIONS[0].x % X_LEFT_WALL) + final_position.x;
                final_position.x = final_position.x + 10;
            }
            else if (final_position.x > X_RIGHT_WALL)
            {
                // final_position.x = final_position.x - (X_RIGHT_WALL % InSand->SAND_POSITIONS[0].x);
                final_position.x = final_position.x - 10;
            }
        }
    }

    // STUPID but works
    if (InSand->FALLING_SAND_INDEX % 20 == 0) {
        InSand->CURRENT_SAND_LEVEL--;
    }

    final_position.y = InSand->CURRENT_SAND_LEVEL;

    add_sand(InSand, final_position); // NOTE: index ketambah di sini
}

#endif // SAND_H