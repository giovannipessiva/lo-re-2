/*###########################################################################
  # Copyright (c) 2010 Giovanni Pessiva <giovanni.pessiva@gmail.com>        #
  # This program is free software: you can redistribute it and/or modify    #
  # it under the terms of the GNU General Public License version 3          #
  # as published by the Free Software Foundation.                           #
  ###########################################################################*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Game.h"
#include "loRe2.h"
#include "interface.h"

/** Main cycle of the game.
 * Start a new Game, manage the Card exchanges and players'choices.
 * At the end of the game, new Roles are assigned.
 */
void Game::start()
{
    int i,direction=1;
    Role *playing;
    Card *chosen;
    bool firstGame=!roleRemapping();


    msg_startGame();
    setDecks();

    getThisPlayer(0)->printDeck();
    waitForEnter();

    // se ci sono dei ruoli
    if(!firstGame)
    {
        exchanges();   //fa gli scambi
        waitForEnter();

        //imposta la direzione
        i=getPlayerIndex(R_KING);
        if(m_players[i>0? i-1 :  m_n-1]->getRoleNumber() > m_players[i<m_n-1? i+1 : 0]->getRoleNumber())
            direction=1;
        else
            direction=-1;

        i=getPlayerIndex(R_KING);
        msg_startFirst(false, i, getThisPlayer(i)->getData()->name);
    }else{
        direction=1;
        srand((unsigned) time(NULL));
        i=rand()%m_n;
        msg_startFirst(true, i, getThisPlayer(i)->getData()->name);
    }

    // Inizializza lo stato del gioco
    m_state.n=0;
    m_state.stillPlaying=m_n;
    m_state.twos=4;
    m_state.aces=4;
    msg_line();

    do{
        m_state.n++;
        m_state.winning=NULL;
        m_state.winner=NULL;

        do{
            playing=m_players[i];

            if(playing->hasCards())
            {
                msg_hisTurn(playing->getData()->name,playing->getTotCards());

                chosen=playing->play(&m_state);
                if(chosen)
                {
                    if(chosen->getValue()==2) m_state.twos--;
                    if(chosen->getValue()==1) m_state.aces--;

                    m_state.winning=chosen;
                    m_state.winner= (void *) m_players[i];
                    msg_playThis(chosen);

                    // Controlla chi ha finito
                    if(!playing->hasCards())
                    {
                        player_t *data=playing->getData();
                        int nextRole=wonThisRole(m_n - m_state.stillPlaying);

                        if(playing->getRoleNumber() == nextRole)
                            data->time++;
                        else
                            data->time=0;

                        if(firstGame)
                            playing->getData()->advancement=0;
                        else
                            data->advancement+=playing->getRoleNumber() - nextRole;

                        msg_newRole(playing->getData()->name,nextRole,!firstGame && !playing->getData()->time);

                        switch(nextRole)
                        {
                            case R_KING:        m_players[i]=new King(data);        break;
                            case R_KNIGHT:      m_players[i]=new Knight(data);      break;
                            case R_THIEF:       m_players[i]=new Thief(data);       break;
                            case R_MERCHANT:    m_players[i]=new Merchant(data);    break;
                            case R_CHICKEN:     m_players[i]=new Chicken(data);     break;
                            case R_BITCH:       m_players[i]=new Bitch(data);       break;
                            default: cout <<"Error - invalid next role number: " <<nextRole;
                        };
                        delete playing;
                        m_state.winner=(void *) m_players[i];

                        m_state.stillPlaying--;
                    }

                }else
                    msg_playNothing(playing->getData()->name);
            }

            // Find next player
            i+=direction;
            if(i >= m_n) i=0;
            if(i < 0)    i=m_n-1;

        }while( ((void *) m_players[i] != m_state.winner)  &&  m_state.stillPlaying>1);

        if(m_state.stillPlaying>1)
        {
            // Se il vincitore ha finito, cerco quello dopo
            if(!m_players[i]->hasCards())
            {
                //nessuno ha superato, quindi comincia il rango più alto
                //ma se siamo alla prima mano (tutti con ruolo 0) va quello dopo!
                if(!firstGame)
                {
                    int nextPlayerRole=R_KING;
                    while(getPlayerIndex(nextPlayerRole)==NO_ROLE  ||
                            getThisRole(nextPlayerRole)->hasCards()==false)
                    {
                        if(nextPlayerRole>=NO_ROLE)
                        {
                            cerr <<"Error while finding next player\n";
                            i=0;
                            break;
                        }
                        nextPlayerRole++;
                    }
                    i=getPlayerIndex(nextPlayerRole);
                }else{
                    do{
                        i+=direction;
                        if(i >= m_n)  i=0;
                        if(i < 0)    i=m_n-1;
                    }while(!m_players[i]->hasCards());
                }
            }

            msg_CardWin( (i? getThisPlayer(i)->getData()->name : "") );
            msg_line();
            waitForEnter();
        }

    }while(m_state.stillPlaying>1);

    for(i=0; !m_players[i]->hasCards()  &&  i<m_n; i++);

    playing=m_players[i];

    if(playing->getRoleNumber() == R_SLAVE)
        playing->getData()->time++;
    else
        playing->getData()->time=0;

    if(firstGame)
        playing->getData()->advancement=0;
    else
        playing->getData()->advancement+=playing->getRoleNumber() - R_SLAVE;

    msg_newRole(playing->getData()->name,R_SLAVE,!playing->getData()->time);

    m_players[i]=new Slave(playing->getData());

    delete playing;
    playing=m_players[i];
}
