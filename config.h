#ifndef _CONFIG_H
#define _CONFIG_H
#include "config_adv.h"

#define PORT 25565

#define MAX_PLAYERS 20

#define COMPRESSION
#define COMPRESSION_THRESHOLD 2048

#define ONLINE_MODE
#define ONLINE_MODE_AUTH // Requires ONLINE_MODE which authticates to mojang servers.

#define SPAWN_X 8.00
#define SPAWN_Y 60.00
#define SPAWN_Z 8.00

#define GAMEMODE 1   // 0: Survival, 1: Creative, 2: Adventure, 3: Spectator.
#define DIFFICULTY 2 //	0: peaceful, 1: easy, 2: normal, 3: hard
#define VIEWDISTANCE 2
#define WORLD_SEED 67
#define RESPAWNSCREEN 1

#define CLIENT_VERSION "1.21.10"
#define PROTOCOL_VERSION 773
#define LONG_PROTOCOL_VERSION "\\u00A7c UCraft: 1.21.10"
#define MOTD "\\u00A7a\\u00A7lUCraft\\u00A7r\\u00A7l server written in C :D"

#endif
