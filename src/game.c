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
    // Local chunk
    currentPlayer->gamePlayerData.chunk_lx = -VIEWDISTANCE;
    currentPlayer->gamePlayerData.chunk_lz = -VIEWDISTANCE;

    currentPlayer->gamePlayerData.chunk_spawn_event = 1;
    currentPlayer->gamePlayerData.chunk_load_event = 1;
}
// fired in local context
void gamePlayerLocalTick(player_t *currentPlayer)
{
    // chunk generation stuff
    if (currentPlayer->gamePlayerData.chunk_load_event)
    {
        if (currentPlayer->chunk_px != 0 || currentPlayer->chunk_pz != 0)
        {
            // send only the new edge column/row.
            if (currentPlayer->chunk_px != 0)
            {
                int32_t chunk_x = currentPlayer->gamePlayerData.chunk_x + (currentPlayer->chunk_px * VIEWDISTANCE);
                int32_t chunk_z = currentPlayer->gamePlayerData.chunk_z + currentPlayer->gamePlayerData.chunk_lz;
                PlayS2Cchunk(currentPlayer, chunk_x, chunk_z, 5, 9);
                if (currentPlayer->gamePlayerData.chunk_lz < VIEWDISTANCE)
                {
                    currentPlayer->gamePlayerData.chunk_lz++;
                }
                else
                {
                    currentPlayer->gamePlayerData.chunk_lz = -VIEWDISTANCE;
                    currentPlayer->chunk_px = 0;
                }
            }
            else
            {
                int32_t chunk_x = currentPlayer->gamePlayerData.chunk_x + currentPlayer->gamePlayerData.chunk_lx;
                int32_t chunk_z = currentPlayer->gamePlayerData.chunk_z + (currentPlayer->chunk_pz * VIEWDISTANCE);
                PlayS2Cchunk(currentPlayer, chunk_x, chunk_z, 5, 9);
                if (currentPlayer->gamePlayerData.chunk_lx < VIEWDISTANCE)
                {
                    currentPlayer->gamePlayerData.chunk_lx++;
                }
                else
                {
                    currentPlayer->gamePlayerData.chunk_lx = -VIEWDISTANCE;
                    currentPlayer->chunk_pz = 0;
                }
            }
            if (currentPlayer->chunk_px == 0 && currentPlayer->chunk_pz == 0)
            {
                currentPlayer->gamePlayerData.chunk_load_event = 0;
            }
        }
        else
        {
            // full square load
            PlayS2Cchunk(currentPlayer, currentPlayer->gamePlayerData.chunk_lx + currentPlayer->gamePlayerData.chunk_x,
                         currentPlayer->gamePlayerData.chunk_lz + currentPlayer->gamePlayerData.chunk_z,
                         5, 9);
            if (currentPlayer->gamePlayerData.chunk_lx < VIEWDISTANCE)
            {
                currentPlayer->gamePlayerData.chunk_lx++;
            }
            else
            {
                currentPlayer->gamePlayerData.chunk_lx = -VIEWDISTANCE;
                currentPlayer->gamePlayerData.chunk_lz++;
            }
            if (currentPlayer->gamePlayerData.chunk_lz > VIEWDISTANCE)
            {
                // close the 'Loading terrain' screen and set the coordinates of the player
                if (currentPlayer->gamePlayerData.chunk_spawn_event)
                {
                    PlayS2Cpositionrotation(currentPlayer, SPAWN_X, SPAWN_Y, SPAWN_Z);
                    PlayS2Ccompassposition(currentPlayer, SPAWN_X, SPAWN_Y, SPAWN_Z);
                    PlayS2Cgameevent(EVENT_WAIT_LEVEL_CHUNKS, 1.0f);
                    PlayS2Cchunkcenter(currentPlayer, currentPlayer->gamePlayerData.chunk_x, currentPlayer->gamePlayerData.chunk_z);

                    currentPlayer->gamePlayerData.chunk_spawn_event = 0;
                }
                currentPlayer->gamePlayerData.chunk_load_event = 0;
            }
        }
    }
    else
    {
        // check if the conditions for a chunk update are met
        int32_t dx = currentPlayer->chunk_x - currentPlayer->gamePlayerData.chunk_x;
        int32_t dz = currentPlayer->chunk_z - currentPlayer->gamePlayerData.chunk_z;

        if (dx != 0 || dz != 0)
        {
            PlayS2Cchunkcenter(currentPlayer, currentPlayer->chunk_x, currentPlayer->chunk_z);
            currentPlayer->gamePlayerData.chunk_x = currentPlayer->chunk_x;
            currentPlayer->gamePlayerData.chunk_z = currentPlayer->chunk_z;
            currentPlayer->gamePlayerData.chunk_lx = -VIEWDISTANCE;
            currentPlayer->gamePlayerData.chunk_lz = -VIEWDISTANCE;

            if (dx > 1 || dx < -1 || dz > 1 || dz < -1)
            {
                // moved too far
                currentPlayer->chunk_px = 0;
                currentPlayer->chunk_pz = 0;
            }
            else
            {
                // moved one chunk: load only the new border(s)
                currentPlayer->chunk_px = (dx > 0) ? 1 : (dx < 0) ? -1
                                                                  : 0;
                currentPlayer->chunk_pz = (dz > 0) ? 1 : (dz < 0) ? -1
                                                                  : 0;
            }
            currentPlayer->gamePlayerData.chunk_load_event = 1;
        }
    }
}
