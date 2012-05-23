/*###########################################################################
  # Copyright (c) 2010 Giovanni Pessiva <giovanni.pessiva@gmail.com>        #
  # This program is free software: you can redistribute it and/or modify    #
  # it under the terms of the GNU General Public License version 3          #
  # as published by the Free Software Foundation.                           #
  ###########################################################################*/

#include <iostream>
#include <string>
#include <sstream>
#include "Role.h"
#include "Choice.h"
#include "interface.h"
#include "loRe2.h"
using namespace std;

/** Getter of roleNumber
 * @return number of this Role
 */
int Role::getRoleNumber(){ return m_roleNumber; }

/** Getter of roleName
 * @return name of this Role
 */
string Role::getRoleName(){ return m_roleName; }

/** Count the cards in his Deck.
 * @return total number of single cards
 * @see Deck::getTotCards()
 */
int Role::getTotCards(){ return m_hand.getTotCards(); }

/** Getter of player's data
 * @return data of this player
 */
player_t *Role::getData(){ return m_data; }

/** Add a Card object to his Deck.
 * @param c    pointer to the Card to add
 * @see Deck::addCard(Card *c)
 */
void Role::giveCard(Card *c){ m_hand.addCard(c); }

/** Extract and return a specific Card.
 * @param value     value of the Card
 * @param n         number of different suits (cards inside the Card)
 * @return a new Card
 * @see Deck::extract(int value, int n)
 */
Card *Role::extractCard(int value,int n){ return m_hand.extract(value,n); }

/** Print user's Cards in a numbered list
 * @see Deck::print()
 */
void Role::printDeck(){ selectCard(NULL,&m_hand,NO_CARDS); }

/** Check if there are cards in his Deck
 * @return true if there is at least one Card
 */
bool Role::hasCards()
{
    if(m_hand.getTotGroups())
        return true;
    return false;
}

/** Ask this Role to play a Card
 * @param state     state of current game
 * @return Card that will be played, NULL if the player will not play any Card
 * @see playCard(gameState_t *state,Deck *hand,int brainLevel)
 */
Card *Role::play(gameState_t *state)
{
    Card *tmp;
    Card *chosen=chooseCard(state,&m_hand,m_data->brainLevel);
    if(chosen)
    {
        tmp=m_hand.extract(chosen->getValue(),chosen->getCount());
        delete chosen;
        return tmp;
    }else
        return NULL;
}

/** NoRole constructor.
 * @param data    player's data
 */
NoRole::NoRole(player_t *data)
{
    m_roleNumber=NO_ROLE;
    m_roleName="(nessuno)";
    m_data=data;
}

/** King constructor.
 * @param data    player's data
 */
King::King(player_t *data)
{
    m_roleNumber=R_KING;
    m_roleName="Lo Re";
    m_data=data;
}

/** Knight constructor.
 * @param data    player's data
 */
Knight::Knight(player_t *data)
{
    m_roleNumber=R_KNIGHT;
    m_roleName="Cavaliere";
    m_data=data;
}

/** Thief constructor.
 * @param data    player's data
 */
Thief::Thief(player_t *data)
{
    m_roleNumber=R_THIEF;
    m_roleName="Ladro";
    m_data=data;
}

/** Merchant constructor.
 * @param data    player's data
 */
Merchant::Merchant(player_t *data)
{
    m_roleNumber=R_MERCHANT;
    m_roleName="Mercante";
    m_data=data;
}

/** Chicken constructor.
 * @param data    player's data
 */
Chicken::Chicken(player_t *data)
{
    m_roleNumber=R_CHICKEN;
    m_roleName="Pollo";
    m_data=data;
}

/** Bitch constructor.
 * @param data    player's data
 */
Bitch::Bitch(player_t *data)
{
    m_roleNumber=R_BITCH;
    m_roleName="Puttana";
    m_data=data;
}

/** Slave constructor.
 * @param data    player's data
 */
Slave::Slave(player_t *data)
{
    m_roleNumber=R_SLAVE;
    m_roleName="Schiavo";
    m_data=data;
}
