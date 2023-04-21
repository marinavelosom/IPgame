#ifndef STRUCTPLAYER_H
#define STRUCTPLAYER_H

#include "raylib.h"

typedef struct Player {
    Vector2 position;
    float speed;
    bool canJump;
} Player;

#endif