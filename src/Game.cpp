/*###########################################################################
  # Copyright (c) 2010 Giovanni Pessiva <giovanni.pessiva@gmail.com>        #
  # This program is free software: you can redistribute it and/or modify    #
  # it under the terms of the GNU General Public License version 3          #
  # as published by the Free Software Foundation.                           #
  ###########################################################################*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "Card.h"
#include "Role.h"
#include "Game.h"
#include "player.h"
#include "gameState.h"
#include "interface.h"
#include "loRe2.h"
using namespace std;

/** Game constructor 1.
 * Create a new Game, loading players'data from file NAME_FILE
 * @see Game::Game(int n)
 */
Game::Game()
{
    int n,i,tmp;
    string line;
    player_t *data;
    ifstream loadFile(FILE_NAME);
    if(loadFile.is_open())
    {
        getline(loadFile,line);
        n=strToInt(line,3,7);
        m_players=new Role *[n];

        for(i=0; i<n && !loadFile.eof(); i++)
        {
            data=new player_t;
            getline(loadFile,line);
            data->name=line;

            getline(loadFile,line);
            data->brainLevel=strToInt(line,0,AI_MAX);

            getline(loadFile,line,'-');
            tmp=strToInt(line,0,7);

            getline(loadFile,line,'-');
            data->time=strToInt(line,0,999);

            getline(loadFile,line);
            data->advancement=strToInt(line,-6,6);

            switch(tmp)
            {
                case R_KING:        m_players[i]=new King(data);        break;
                case R_KNIGHT:      m_players[i]=new Knight(data);      break;
                case R_THIEF:       m_players[i]=new Thief(data);       break;
                case R_MERCHANT:    m_players[i]=new Merchant(data);    break;
                case R_CHICKEN:     m_players[i]=new Chicken(data);     break;
                case R_BITCH:       m_players[i]=new Bitch(data);       break;
                case R_SLAVE:       m_players[i]=new Slave(data);       break;
                case NO_ROLE:       m_players[i]=new NoRole(data);      break;
                default:    cerr <<"Error input file: invalid role " <<tmp;
            };

            msg_printData(data,m_players[i]->getRoleName());
        }
        if(i!=n)
            cerr <<"Input error while reading " <<FILE_NAME  <<", not enough lines: " <<i+1 <<"/" <<n <<endl;
        loadFile.close();
    }else{
        msg_noFile();
        n=0;
    }

    loadFile.close();

    m_n=n;
    m_state.stillPlaying=n;
    m_state.n=0;
    m_state.winning=NULL;
    m_state.winner=NULL;
    m_state.twos=4;
    m_state.aces=4;
}

/** Game constructor 2.
 * Create a new Game with n players, asking the user to insert players'data
 * @param n     number of players
 * @see Game::Game()
 */
Game::Game(int n)
{
    m_n=n;
    m_state.stillPlaying=n;
    m_state.n=0;
    m_state.winning=NULL;
    m_state.winner=NULL;
    m_state.twos=4;
    m_state.aces=4;

    m_players=new Role *[n];
    player_t *data;
    for(int i=0; i<n; i++)
    {
        data=readData(i+1);
        m_players[i]=new NoRole(data);
    }
}

/** Card destructor.
 * Delete all the players and their datas.
 */
Game::~Game()
{
    for(int i=0; i<m_n; i++)
        if(m_players[i])
        {
            delete m_players[i]->getData();
            delete m_players[i];
        }
}

/** Return the number of players.
 * @return number of players
 */
int Game::getN(){ return m_n; }

/** Save players'datas in FILE_NAME..
 * @see Game::Game()
 */
void Game::save()
{
    player_t *data;
    ofstream saveFile(FILE_NAME);
    saveFile <<m_n <<endl;
    for(int i=0; i<m_n; i++)
    {
        data=m_players[i]->getData();
        saveFile <<data->name <<endl <<data->brainLevel <<endl;
        saveFile <<m_players[i]->getRoleNumber() <<"-" <<data->time <<"-" <<data->advancement <<endl;
    }
    saveFile.close();
}

/** Save in an array the couples playerNum-roleNum, for quick researches.
 * @return number of roles found
 */
int Game::roleRemapping()
{
    int i,j,assigned=0;
    for(i=0; i<7; i++)
        roleMap[i]=NO_ROLE;

    for(i=0; i<7; i++)
        for(j=0; j<m_n; j++)
            if(m_players[j]->getRoleNumber()==i)
            {
                roleMap[i]=j;
                assigned++;
                break;
            }

    if(assigned>0  &&  assigned!=m_n)
        cerr <<"Error while mapping roles. Assigned: " <<assigned;
    return assigned;
}

/** Create players'Decks, distributing Cards among them.
 * @see Deck::Deck(int players)
 * @see Deck:extractRandom()
 */
void Game::setDecks()
{
    Deck basic(m_n);
    int tot=basic.getTotCards() / m_n;
    for(int i=0; i<m_n; i++)
        for(int j=0; j<tot; j++)
            m_players[i]->giveCard(basic.extractRandom());
}

/** Manage the exchange of Cards between the players, according to their Roles.
 * This method is used at the beginning of every Game when players
 * already have a Role.
 * @see Role::exchange();
 * @see Role::offer(Choice *sale);
 * @see Merchant::valueOffers(option_t **offers,int lenght,Choice *sale);
 */
void Game::exchanges()
{
    int i,k;
    Choice *passed[7] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    Role *tmp;

    msg_startExchanges();

    for(i=0; i<m_n; i++)
    {
        k=m_players[i]->getRoleNumber();
        passed[k]=m_players[i]->exchange();
    }

    for(i=0; i<m_n; i++)
    {
        int hisRole=m_players[i]->getRoleNumber();
        switch(hisRole)
        {
            case R_KING:        k=R_SLAVE;      break;
            case R_KNIGHT:      k=R_BITCH;      break;
            case R_THIEF:       k=R_CHICKEN;    break;
            case R_MERCHANT:    k=NO_ROLE;      break;
            case R_CHICKEN:     k=R_THIEF;      break;
            case R_BITCH:       k=R_KNIGHT;     break;
            case R_SLAVE:       k=R_KING;       break;
            default: cerr <<"Error: invalid role number: " <<hisRole <<endl;
        };

        if(k!=NO_ROLE)
        {
            tmp=getThisRole(k);
            tmp->giveCard(passed[hisRole]->popCard());
            tmp->giveCard(passed[hisRole]->popCard());   // This could be NULL, but it is not a problem
            delete passed[hisRole];
        }
    }

    if(getPlayerIndex(R_MERCHANT)!=0)
    {
        m_players[0]->printDeck();
        waitForEnter();
    }

    if(getThisRole(R_MERCHANT))
    {
        if(passed[R_MERCHANT] && passed[R_MERCHANT]->getCount())
        {
            msg_merchOffer(true);
            passed[R_MERCHANT]->print(false);

            option_t *offers[7];
            // Ask everybody, from King to Slave, what they offer for Merchant's Cards
            for(i=R_KING; i<=R_SLAVE; i++)
            {
                offers[i]=NULL;
                if(getThisRole(i))
                    offers[i]=getThisRole(i)->offer(passed[R_MERCHANT]);
            }

            // Let the Merchant pick the best offer
            Card *accepted=( (Merchant *) getThisRole(R_MERCHANT))->valueOffers(offers,m_n,passed[R_MERCHANT]);

            // Give the Cards back
            for(i=R_KING; i<=R_SLAVE; i++)
            {
                if(offers[i] && offers[i]->chosen)
                {
                    if(offers[i]->chosen==accepted)
                    {
                        // Give chosen card to the Merchant
                        getThisRole(R_MERCHANT)->giveCard(offers[i]->chosen);
                        offers[i]->chosen=NULL;

                        // Give Merchant's Card to the player who made the selected offer
                        for(int j=0; j<passed[R_MERCHANT]->getCount(); j++)
                        {
                            if(j==offers[i]->grade)
                            {
                                Card *chosen=getThisRole(R_MERCHANT)->extractCard(passed[R_MERCHANT]->getThis(j)->getValue(),1);
                                getThisRole(i)->giveCard(chosen);
                            }
                        }
                    }else
                        // Offer refused, give back the Card to his owner
                        getThisRole(i)->giveCard(offers[i]->chosen);
                }
            }

            // Delete every other Merchant's offers
            while(passed[R_MERCHANT]->getCount())
                delete passed[R_MERCHANT]->popCard();

            // Give back the refused offers to their owners
            for(i=R_KING; i<=R_SLAVE; i++)
            {
                if(offers[i])
                {
                    getThisRole(i)->giveCard(offers[i]->chosen);
                    delete offers[i];
                }
            }
        }else
            msg_merchOffer(false);
    }
}

/** Return new roleNum of a player who just finished his Cards.
 * @param nextRole    number of people who already finished their Cards
 * @return won role number
 */
int Game::wonThisRole(int nextRole)
{
    // roleMatrix[people who already finished][number if players]= won role number
    int roleMatrix[5][7] = {    {0,3,6,-1,-1,-1,-1},
                                {0,1,5, 6,-1,-1,-1},
                                {0,1,3, 5, 6,-1,-1},
                                {0,1,2, 4, 5, 6,-1},
                                {0,1,2, 3, 4, 5, 6}     };
    return roleMatrix[m_n-3][nextRole];
}

/** Get the index of the player with role=roleNum.
 * @param roleNum    number of the requested Role
 * @return index of the player, -1 if that Role is not present
 * @see Game::getThisRole(int roleNum)
 * @see Game::getThisPlayer(int playerNum)
 */
int Game::getPlayerIndex(int roleNum){   return roleMap[roleNum];    }

/** Get the player with role=roleNum.
 * @param roleNum    number of the requested Role
 * @return player requested, NULL if that Role is not present
 * @see Game::getPlayerIndex(int roleNum)
 * @see Game::getThisPlayer(int playerNum)
 */
Role *Game::getThisRole(int roleNum)
{
    if(roleMap[roleNum]!=NO_ROLE)
        return m_players[roleMap[roleNum]];
    return NULL;
}

/** Get the player with index playerNum.
 * @param playerNum    index of the requested player
 * @return player requested, NULL if playerNum is invalid
 * @see Game::getPlayerIndex(int roleNum)
 * @see Game::getThisRole(int roleNum)
 */
Role *Game::getThisPlayer(int playerNum)
{
    if(playerNum<0  || playerNum>=m_n)
        return NULL;
    return m_players[playerNum];
}
