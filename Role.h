#ifndef ROLE_H_
#define ROLE_H_

#include "Deck.h"
#include "Choice.h"
#include "player.h"
#include "option.h"
#include "gameState.h"
#include "loRe2.h"
#include <string>
using namespace std;

/** \brief virtual Class for all the roles.
 *
 * An Role object represents a player during the game.
 * It provides methods to manage  interactions with other players.
 */
class Role{
    protected:
    int m_roleNumber;
    string m_roleName;
    Deck m_hand;
    player_t *m_data;
    int evaluateExchange(int given,Card *received);
    int countOffers(option_t **offers,int length);

    public:
    bool hasCards();
    int getTotCards();
    player_t *getData();
    void giveCard(Card *c);
    Card *extractCard(int value,int n);
    void printDeck();
    int getRoleNumber();
    string getRoleName();
    option_t *offer(Choice *sale);
    virtual Choice *exchange() =0;
    Card *play(gameState_t *state);
    static Card *chooseCard(gameState_t *state,Deck *hand,int brainLevel);

    friend void test_role();
};

/** \brief Role for players who do not have (yet) a role */
class NoRole : public Role{
    public:
    NoRole(player_t *data);
    Choice *exchange();
};

/** \brief Role for the player who won last game */
class King : public Role{
    public:
    King(player_t *data);
    Choice *exchange();
};

/** \brief Role for the player who finished second at last game */
class Knight : public Role{
    public:
    Knight(player_t *data);
    Choice *exchange();
};

/** \brief Role for the player who finished third at last game */
class Thief : public Role{
    public:
    Thief(player_t *data);
    Choice *exchange();
};

/** \brief Role for the player who finished fourth at last game */
class Merchant : public Role{
    public:
    Merchant(player_t *data);
    Choice *exchange();
    Card *valueOffers(option_t **offers,int length,Choice *sale);
};

/** \brief Role for the player who finished fifth at last game */
class Chicken : public Role{
    public:
    Chicken(player_t *data);
    Choice *exchange();
};

/** \brief Role for the player who finished sixth at last game */
class Bitch : public Role{
    public:
    Bitch(player_t *data);
    Choice *exchange();
};

/** \brief Role for the player who lost last game */
class Slave : public Role{
    public:
    Slave(player_t *data);
    Choice *exchange();
};

#endif
