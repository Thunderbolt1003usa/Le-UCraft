#ifndef _WORLD_H
#define _WORLD_H

#include <stdint.h>
#include "player.h"


void worldGenerateChunk(player_t *currentPlayer, int32_t x, int32_t z, size_t from, size_t to);

#endif
