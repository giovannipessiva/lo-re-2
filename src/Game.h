#ifndef GAME_H_
#define GAME_H_

#include "Role.h"
#include "gameState.h"

/** \brief main class of Lo Re.
 *
 * A Game object contains from 3 to 7 players (Role objects),
 * and provide high-level method to manage the game and player's datas.
 */
class Game{
    int m_n;
    Role **m_players;
    int roleMap[7];
    gameState_t m_state;

    int roleRemapping();
    void setDecks();
    void exchanges();
    int wonThisRole(int nextRole);
    int getPlayerIndex(int roleNum);
    Role *getThisRole(int roleNum);
    Role *getThisPlayer(int playerNum);

    public:
    Game();
    Game(int n);
    ~Game();
    int getN();
    void start();
    void save();

    #ifdef _TEST
    friend void test_game();
    #endif
};

#endif
