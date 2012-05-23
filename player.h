#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>

/** \brief struct representing player's datas. */
typedef struct player
{
    std::string name;   ///< player's name
    int brainLevel;     ///< player's level (from HUMAN to AI_MAX)
    int time;           ///< how long he has kept his role
    int advancement;    ///< how much he is improved since his first role
} player_t;

#endif
