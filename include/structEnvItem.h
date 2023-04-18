#ifndef ENVITEMSSTRUCT_H
#define ENVITEMSSTRUCT_H

#include "raylib.h"

typedef struct EnvItem {
    Rectangle rect;
    int blocking;
    Color color;
} EnvItem;

#endif