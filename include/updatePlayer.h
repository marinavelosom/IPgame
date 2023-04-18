#ifndef UPDATEPLAYER_H
#define UPDATEPLAYER_H

#include <stdio.h>
#include "raylib.h"
#include "structPlayer.h"
#include "structEnvItem.h"

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta);

#endif