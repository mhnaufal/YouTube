#ifndef WATER_H
#define WATER_H

#include "vec2.h"

#include <stdint.h>

#define WATER_COUNT 10
#define WATER_CHAR '='

const Vec2 WATER_STARTING_POSITION = {40, 3};

typedef struct Water
{
    uint16_t FALLING_WATER_INDEX;
    char WATER_ARRAY[WATER_COUNT];
    Vec2 WATER_POSITIONS[WATER_COUNT];
} Water;

bool check_if_water_filled(Water InWater, uint16_t index);
void add_water(Water *InWater);
Vec2 get_water_position(Water InWater, uint16_t index);
void set_water_position(Water *InWater, uint16_t index, Vec2 position);

#endif // WATER_H