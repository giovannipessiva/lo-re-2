/*###########################################################################
  # Copyright (c) 2010 Giovanni Pessiva <giovanni.pessiva@gmail.com>        #
  # This program is free software: you can redistribute it and/or modify    #
  # it under the terms of the GNU General Public License version 3          #
  # as published by the Free Software Foundation.                           #
  ###########################################################################*/

#include <iostream>
#include <sstream>
#include <limits>
#include <string>
#include "interface.h"
#include "loRe2.h"
#include "gameState.h"
#include "player.h"
#include "Choice.h"
#include "Role.h"
using namespace std;

Card *playCard(gameState_t *state,Deck *hand,int brainLevel);

/** Conver a string to an int between min and max.
 * @param s         string to convert
 * @param min       smallest value acceptable
 * @param max       biggest value acceptable
 * @return value in the string, or (min-1) if it is not valid
 * @see strToInt(string s,int min,int max,int extra);
 */
int strToInt(string s,int min,int max)
{
    int n=min-1;
    stringstream ss(s);
    if( (ss >> n).fail() || n>max || n<min )
        return min-1;
    else
        return n;
}

/** Conver a string to an int between min and max.
 * @param s         string to convert
 * @param min       smallest acceptable value
 * @param max       biggest acceptable value
 * @param extra     acceptable value out of the rage min-max
 * @return value in the string, or (min-1) if it is not valid
 * @see strToInt(string s,int min,int max);
 */
int strToInt(string s,int min,int max,int extra)
{
    int n;
    stringstream ss(s);
    if( ((ss >> n).fail() || n>max || n<min ) && n!=extra)
        return min-1;
    else
        return n;
}

/** get an integer from the user, between min and max.
 * @param min       smallest acceptable value
 * @param max       biggest acceptable value
 * @return integer read
 * @see scanInt(int min,int max,int extra)
 */
int scanInt(int min,int max)
{
    string input;
    int n=0;
    do{
        getline(cin,input);
        n=strToInt(input,min,max);
    }while(n==min-1);
    return n;
}

/** get an integer from the user, between min and max or equal to extra.
 * @param min       smallest acceptable value
 * @param max       biggest acceptable value
* @param extra     acceptable value out of the rage min-max
 * @return integer read
 * @see scanInt(int min,int max)
 */
int scanInt(int min,int max,int extra)
{
    string input;
    int n=0;
    do{
        getline(cin,input);
        n=strToInt(input,min,max,extra);
    }while(n==min-1);
    return n;
}

/** Wait for the user to press Enter key */
void waitForEnter()
{
    cout <<"Premi INVIO per continuare...";
    string input;
    getline( cin, input );
}

/** Make the user select the number of players
 * @return number of players (from 3 to 7)
 */
int selectPlayers()
{
    cout <<"\nInserisci il numero di giocatori (da 3 a 7): ";
    return scanInt(3,7);
}

/** Make the user insert a player's datas
 * @param playerN      player's number
 * @return player's data
 */
player_t *readData(int playerN)
{
    player_t *tmp=new player_t;
    if(playerN==1)
    {
        do{
            cout <<" Scegli il tuo nome: ";
            cin >>tmp->name;
        }while( tmp->name.empty() );
        tmp->brainLevel=HUMAN;
    }else{
        do{
            cout <<"\n Scegli i nome del giocatore " <<playerN <<": ";
            cin >>tmp->name;
        }while( tmp->name.empty() );

        cout <<" > livello di " <<tmp->name <<" (3=bravo, 2=medio, 1=scarso):";
        tmp->brainLevel=scanInt(1,AI_MAX);
    }
    tmp->time=0;
    tmp->advancement=0;
    return tmp;
}

/** Show user's Deck and let him choose some cards
 * @param state     state of current game
 * @param hand      user's Deck
 * @param max       number of Card he can select:
 *                  NO_CARDS: no selection, just show the Deck
 *                  ONE_CARD: select one card (usage: offers to Merchant)
 *                  FOUR_CARDS: select one card, a couple, a tris or a poker (usage: card to play)
 *                  N_CARDS: select as many cards as you want (usage: offers of the Merchant)
 * @return copies of Cards chosen, NULL is no one had been chosen
 * @see selectCard(Choice *offers)
 */
Choice *selectCard(gameState_t *state,Deck *hand,int max)
{
    int tmp,i;
    Card *selected;
    Choice *cards=new Choice;

    cout <<" Queste sono le tue carte:\n";
    hand->print(true);

    switch(max)
    {
        case NO_CARDS: break;
        case ONE_CARD:   //fa selezionare una singola carta (per scambi col mercante)

            cout <<" Cosa offri in cambio? (0 = nessuna carta) ";
            tmp=scanInt(0,hand->getTotCards());

            if(tmp==0)  return NULL;   //l'utente passa

            selected=new Card(hand->getThisGroup(tmp-1));
            if(selected->getCount()>1)
            {
                Card *tmp=selected->divide(1);
                delete selected;
                selected=tmp;
            }
            cards->addOption(selected,1);  //restituisce una copia
            break;

        case FOUR_CARDS:   //fa selezionare carte in numero a piacere di un valore (max 4, poker)
            do{
                cout <<" Cosa vuoi buttare? (0 = nessuna carta) ";
                tmp=scanInt(0,hand->getTotCards(),666);

                if(tmp==0)  return NULL;   //l'utente non butta niente

                if(tmp == 666)   //mostro all'utente la carta che sceglierebbe un AI_MAX
                {
                    selected=Role::chooseCard(state,hand,AI_MAX);
                    if(selected)
                    {
                        switch(selected->getValue())
                        {
                            case 11:    cout <<" ...fossi in te giocherei "     <<selected->getLongName();          break;
                            case 12:    cout <<" ...che ne dici di giocare "    <<selected->getLongName() <<"?";    break;
                            case 13:    cout <<" ...vacci di "                  <<selected->getLongName();          break;
                            case 1:     cout <<" ...e buttagli 'sto "           <<selected->getLongName() <<"!";    break;
                            case 2:     cout <<" ...al mio segnale scatena l'inferno con " <<selected->getLongName() <<"!!"; break;
                            default:    cout <<" ...perche' non giochi "        <<selected->getLongName() <<"?";    break;
                        }
                        cout  <<endl;
                    }else
                        cout <<" ...forse dovresti passare! =(\n";   // NULL significa che passi
                }

            }while(tmp == 666);   //dopo aver dato il consiglio, chiedo di nuovo cosa fare

            selected=hand->getThisGroup(tmp-1);

            if(selected->getCount() > 1)   //se per quel valore ho più di una carta
            {
                cout <<" Quante (da 1 a " <<selected->getCount() <<")? ";
                tmp=scanInt(1,selected->getCount());
            }

            if(tmp<selected->getCount())
            {
                selected=new Card(selected);
                cards->addOption(selected->divide(tmp),1);
                delete selected;
            }else
                cards->addOption(new Card(selected),1);
            break;

        case N_CARDS:   //fa selezionare carte in numero a piacere (offerte del mercante)
            i=1;
            do{
                cout <<" Cosa vuoi offrire? (0 = fine) ";
                tmp=scanInt(0, hand->getTotGroups());

                if(tmp==0) break;

                selected=hand->getThisGroup(tmp-1);
                if( cards->hasCard(selected->getValue()) )
                    cout <<" Hai gia' messo sul tavolo questa carta.\n";
                else{
                    selected=new Card(selected);
                    if(selected->getCount()>1)
                    {
                        Card *tmp=selected->divide(1);
                        delete selected;
                        selected=tmp;
                    }
                    cards->addOption(selected,i++);
                    hand->print(true);
                }

            }while(tmp!=0  &&  hand->getTotGroups() > 0);
            break;

        default: std::cerr <<"invalid value of cards as third argument of: \n" \
                            <<"   Choice *selectCard(Card *winning,Deck *hand,int max)" \
                            <<"in 'interface.cpp'" <<endl;
    }
    return cards;
}

/** Show Merchant's offert and let the user offer an exchange
 * @param offers    Cards offered by Merchant
 * @return index of the Card chosen, -1 if no one had been chosen
 * @see selectCard(gameState_t *state,Deck *hand,int max)
 */
int selectCard(Choice *offers)
{
    int n;
    //cout <<endl <<" Carte offerte:\n";
    //offers->print(false);
    cout <<" Quale carta  del mercante vorresti? (0 = nessuna carta) ";
    n=scanInt(0, offers->getCount());

    if(n)
        return n -1;
    else
        return -1;
}

/** Show offers to Merchant and let his choose one
 * @param offers        offers from players for Merchant's cards
 * @param num           number of offers
 * @param requests      Cards offered by Merchant
 * @return index of the offer chosen, -1 if no one had been chosen
 * @see Role::offer(Choice *sale)
 */
int selectOffer(option_t **offers,int num,Choice *requests)
{
    int i,n=0;
    cout <<" Queste sono le offerte:\n";
    for(i=0; i<num; i++)
    {
        if(offers[i] && offers[i]->chosen) // && offers[i]->grade>0 && offers[i]->grade<requests->getCount())
        {
            cout <<++n <<") ";
            cout<<offers[i]->chosen->getShortName();
            cout <<" per il ";
            cout<<requests->getThis(offers[i]->grade)->getShortName() <<endl;
        }
    }

    cout <<" Quale offerta scegli? (0 = nessuna offerta) ";
    n=scanInt(0, n);

    return n-1;
}

/** Let the user choose what he wants to do now
 * @param continua      true if this is not the first game
 * @return selected menu option
 * @see main(int argc,char** argv)
 */
int selectMenuOption(bool continua)
{
    int menuOption,i;

    do{
        if(continua)
            cout <<"\nLa mano e' terminata. ";
        cout <<"\nCosa vuoi fare?\n";
        i=1;
        if(continua)
        {
            cout <<i++ <<") Continua la partita\n";
            cout <<i++ <<") Salva la parita\n";
        }
        cout <<i++ <<") Nuova partita\n";
        cout <<i++ <<") Carica partita salvata\n";
        cout <<i++ <<") Leggi le regole\n";
        cout <<i <<") Esci\n";
        cout <<"scelta: ";
        menuOption=scanInt(1,7);

    }while(menuOption>4 && !continua);

    return (continua? menuOption-2 : menuOption);
}
