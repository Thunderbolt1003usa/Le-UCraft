#include <math.h>
#include <stdarg.h>

#include "game.h"
#include "log.h"
#include "s2c.h"
#include "wrapper.h"
#include "UCraft.h"
#include "blocks/blocks.h"

// fired when the server is about to start
void gamePreload()
{
}
// fired when a player leaves
void gamePlayerLeft(player_t *currentPlayer)
{
}
// fired every tick in the global context
// NOTE: this will run even if there are no players so be careful when sending packets
void gameGlobalTick()
{
}
// fired every tick in the global context for the current player {so it will replicate for others}
void gamePlayerGlobalTick(player_t *currentPlayer)
{
}

// fired when the player is spawned in the local context {fired once}
void gamePlayerSpawned(player_t *currentPlayer)
{
    currentPlayer->gamePlayerData.chunk_x = -CHUNK_SIZE;
    currentPlayer->gamePlayerData.chunk_z = -CHUNK_SIZE;
    currentPlayer->gamePlayerData.chunk_next_event = 1;
}
// fired in local context
void gamePlayerLocalTick(player_t *currentPlayer)
{
    if (currentPlayer->gamePlayerData.chunk_next_event)
    {
        // dont send multiple chunks at the same time since the client will reject it
        // TODO: add compression
        PlayS2Cchunk(currentPlayer, currentPlayer->gamePlayerData.chunk_x, currentPlayer->gamePlayerData.chunk_z);
        if (currentPlayer->gamePlayerData.chunk_x < CHUNK_SIZE)
        {
            currentPlayer->gamePlayerData.chunk_x++;
        }
        else
        {
            currentPlayer->gamePlayerData.chunk_x = -CHUNK_SIZE;
            currentPlayer->gamePlayerData.chunk_z++;
        }
        if (currentPlayer->gamePlayerData.chunk_z > CHUNK_SIZE)
        {
            PlayS2Cgameevent(EVENT_WAIT_LEVEL_CHUNKS, 1.0f);
            currentPlayer->gamePlayerData.chunk_x = 0;
            currentPlayer->gamePlayerData.chunk_z = 0;
            currentPlayer->gamePlayerData.chunk_next_event = 0;
            currentPlayer->gamePlayerData.chunk_loaded_event = 1;
        }
    }
    if (currentPlayer->gamePlayerData.chunk_loaded_event)
    {
        PlayS2Cpositionrotation(currentPlayer, SPAWN_X, SPAWN_Y, SPAWN_Z);
        PlayS2Ccompassposition(currentPlayer, SPAWN_X, SPAWN_Y, SPAWN_Z);
        currentPlayer->gamePlayerData.chunk_loaded_event = 0;
    }
    // check if the conditions for a chunk update are met
    if (currentPlayer->gamePlayerData.chunk_x != currentPlayer->gamePlayerData.chunk_px || currentPlayer->gamePlayerData.chunk_z != currentPlayer->gamePlayerData.chunk_pz)
    {
        PlayS2Cchunkcenter(currentPlayer, currentPlayer->gamePlayerData.chunk_x, currentPlayer->gamePlayerData.chunk_z);
        currentPlayer->gamePlayerData.chunk_px = currentPlayer->gamePlayerData.chunk_x;
        currentPlayer->gamePlayerData.chunk_pz = currentPlayer->gamePlayerData.chunk_z;
    }
}