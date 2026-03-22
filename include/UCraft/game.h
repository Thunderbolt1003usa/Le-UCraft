#ifndef _GAME_H
#define _GAME_H
#include "config.h"
#include "wrapper.h"

typedef struct player_t player_t;

// per player stuff
struct gamePlayerData_t
{
    // Events
    uint8_t chunk_load_event : 1;
    uint8_t chunk_spawn_event : 1;
    // Chunk
    int32_t chunk_x, chunk_z, chunk_lx, chunk_lz;
};
typedef struct gamePlayerData_t gamePlayerData_t;

void gamePreload();
void gamePlayerSpawned(player_t *currentPlayer);
void gameGlobalPlayerMoved(player_t *currentPlayer);
void gamePlayerLeft(player_t *currentPlayer);
void gameGlobalTick();
void gamePlayerGlobalTick(player_t *currentPlayer);
void gamePlayerLocalTick(player_t *currentPlayer);

#endif
