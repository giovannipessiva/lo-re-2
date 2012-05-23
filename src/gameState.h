#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "player.h"
#include "Card.h"

/** \brief struct representing current situation of the game. */
typedef struct gameState{
    int n;

    Card *winning;      ///< last played Card
    void *winner;       ///< pointer to a Role object, owner of winning Card
    int stillPlaying;   ///< remaining number of players
    int twos;           ///< remaining number of twos (0-4)
    int aces;           ///< remaining number of aces (0-4)
} gameState_t;

#endif
