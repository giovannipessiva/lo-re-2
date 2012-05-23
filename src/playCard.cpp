/*###########################################################################
  # Copyright (c) 2010 Giovanni "Raptor" Pessiva <gio_rpt@yahoo.it>         #
  # This program is free software: you can redistribute it and/or modify    #
  # it under the terms of the GNU General Public License version 3          #
  # as published by the Free Software Foundation.                           #
  ###########################################################################*/

#include <iostream>
#include "Card.h"
#include "Deck.h"
#include "Choice.h"
#include "interface.h"
#include "loRe2.h"
using namespace std;

/** Choose which Card to play, using different algorithms.
 * @param state         state of current game
 * @param hand          player's Deck
 * @param brainLevel    player's cleverness
 *                      HUMAN: player is the user
 *                      ...
 *                      AI_MAX: player is AI with full capability
 * @return copy of the Card chosen, NULL if he decided not to play
 */
Card *playCard(gameState_t *state,Deck *hand,int brainLevel)
{
    int algorithmLevel;
    Choice possibilities;
    Choice *cards;
    Card *chosen,*max1,*max2,*tmp;
    bool first=false;

    if(state->winning==NULL)
        first=true;

    if(state==NULL)
    {
        cerr <<"Error: argument \"state\" can't be NULL\n";
        return NULL;
    }

    if(!first && (state->winning->getValue()==2  ||  (state->winning->getValue()==1 && state->aces==0)) )
        return NULL;  //se non si può superare
    else{
        if(brainLevel==HUMAN)
        {
            cards=selectCard(state,hand,FOUR_CARDS);
            if(cards==0) return NULL;
            chosen=cards->popCard();
            delete cards;
            while(chosen->isPlayable(state->winning)==false)
            {
                msg_rejectCard();
                msg_printState(state);
                cards=selectCard(state,hand,FOUR_CARDS);
                if(!cards) return NULL;
                chosen=cards->popCard();
                delete cards;
            }
            return chosen;
        }else{
            // in base al loro livello BRAIN, i giocatori hanno più o meno opzioni
            // fra le quali scegliere. Ogni opzione è data da un algoritmo, che
            // può essere più o meno vantaggioso in base alla situazione delle carte.

            //____________________________ALGORITMO:"Base"____________________________
            //gioco le carte di valore più basso possibile
            chosen=hand->getFirstPlayable(state->winning);

            if(chosen != NULL)
            {
                //...............calcolo del livello dell'opzione.............
                algorithmLevel = 10;   //livello di base dell'algoritmo

                switch(chosen->getCount() - (first? 1 : state->winning->getCount()))   //se aumento il numero (es. da coppia a tris)
                {
                    case 0: algorithmLevel += 5;    break;   //buona
                    case 1: algorithmLevel -= 2;    break;
                    case 2: algorithmLevel -= 5;    break;
                    case 3: algorithmLevel -= 10;   break;   //molto svantaggiosa
                }

                if(chosen == hand->getLast())   //se la carta è alta
                {
                    if(state->n < 4)  algorithmLevel -= 5;   //se sono alle prime giocate è svantaggioso
                    if(state->n < 2)  algorithmLevel -= 5;   //giocare subito una carta alta!
                }
                //............................................................

                possibilities.addOption(new Card(chosen),algorithmLevel);   //mi salvo l'opzione nella lista
            }else
                return NULL;   //non può giocare nulla, deve per forza passare
            //_________________________________________________________________________

            //____________________________ALGORITMO:"Base++"____________________________
            //gioco le carte di livello più basso possibile

            if(brainLevel > 1)
            {
                chosen=hand->getLowerPlayable(state->winning,state);   //parto dalla carta più bassa giocabile, trovata prima

                //...............calcolo del livello dell'opzione.............
                algorithmLevel = 16;   //livello di base dell'algoritmo

                switch(chosen->getCount() - (first? 1 : state->winning->getCount()))   //se aumento il numero (es. da coppia a tris)
                {
                    case 0: algorithmLevel += 5;  break;   //buona
                    case 1: algorithmLevel -= 2;  break;
                    case 2: algorithmLevel -= 5;  break;
                    case 3: algorithmLevel -= 10;  break;   //molto svantaggiosa
                }

                if(chosen == hand->getLast())   //se la carta è alta
                {
                    if(state->n < 4)  algorithmLevel -= 5;   //se sono alle prime giocate è svantaggioso
                    if(state->n < 2)  algorithmLevel -= 5;   //      giocare subito una carta alta!
                }
                //............................................................

                possibilities.addOption(new Card(chosen),algorithmLevel);   //mi salvo l'opzione nella lista
            }
            //_________________________________________________________________________

            //____________________________ALGORITMO:"Final Rush"____________________________
            //se ho una carta bassa, e una che prende sicuramente, gioco prima la seconda

            if(brainLevel > 1)
            {
                if(hand->getTotGroups()==2  &&  hand->getFirst()==hand->getFirstPlayable(state->winning))   //se ci sono carte di 2 valori
                {
                    chosen=hand->getLast();   //passo alla più alta

                    if(chosen->getValue()==2)   //se è un 2
                    {
                        if(!chosen->isPlayable(state->winning))
                            chosen=NULL;
                    }else{
                        //se è un asso            e    i 2 sono finiti
                        if(chosen->getValue()==1  &&  state->twos==0)
                            if(!chosen->isPlayable(state->winning))
                                chosen=NULL;
                    }

                    if(chosen!=NULL)   //se le condizioni sono soddisfatte
                    {
                        //...............calcolo del livello dell'opzione.............
                        algorithmLevel = 40;   //livello di base dell'algoritmo
                        //............................................................
                        possibilities.addOption(new Card(chosen),algorithmLevel);   //mi salvo l'opzione nella lista
                    }
                }
            }
            //_________________________________________________________________________

            //____________________________ALGORITMO:"Final Rush++"____________________________
            //se ho solo una carta di livello basso, gioco prima le alte

            if(brainLevel > 2)
            {
                if(hand->countCheapCards()<2)   //se c'è al massimo una carta bassa
                {
                    chosen=hand->getFirstPlayable(state->winning);
                    while(chosen)
                    {
                        if(chosen->getLevel() >= TWO_LEV)   //se la carta è alta
                        {
                            if(chosen->isPlayable(state->winning))   //se posso giocarla
                            {
                                //...............calcolo del livello dell'opzione.............
                                algorithmLevel = 30;   //livello di base dell'algoritmo
                                //............................................................
                                possibilities.addOption(new Card(chosen),algorithmLevel);   //mi salvo l'opzione nella lista
                                chosen=NULL;
                            }
                        }
                        if(chosen)
                            chosen=hand->getNext();
                    }
                }
            }
            //_________________________________________________________________________


            //____________________________ALGORITMO:"Save Last"____________________________
            //se posso giocare solo la mia carta più alta e ho ancora tante carte, passo

            if(brainLevel > 2)
            {
                if(hand->getFirstPlayable(state->winning) == hand->getLast())   //se posso giocare solo la mia carta più alta
                {
                    if(hand->countCheapCards()>3)   //se ho ancora tante carte
                    {
                        //...............calcolo del livello dell'opzione.............
                        algorithmLevel = 20 + hand->getTotGroups();   //più carte mi restano, più conviene passare
                        //............................................................
                        possibilities.addOption(NULL,algorithmLevel);   //mi salvo l'opzione nella lista
                    }
                }
            }
            //_________________________________________________________________________



            //____________________________ALGORITMO:"Save More"____________________________

            if(brainLevel > 2)
            {

                //__________________________Save Pre____________________________
                //se posso giocare solo la seconda carta più alta, passo

                hand->getFirstPlayable(state->winning);
                if(hand->getNext() == hand->getLast())   //se dovrei giocare la penultima carta più alta
                {
                    if(hand->countCheapCards()>4)   //se ho ancora tante carte
                    {
                        //...............calcolo del livello dell'opzione.............
                        algorithmLevel = 15 + hand->getTotGroups();   //più carte mi restano, più conviene passare
                        //............................................................
                        possibilities.addOption(NULL,algorithmLevel);   //mi salvo l'opzione nella lista
                    }
                }


                max1=hand->getFirst();
                max2=hand->getFirst();

                for(tmp=hand->getNext(); tmp!=NULL; tmp=hand->getNext())
                {
                    if(tmp->getLevel() >= max1->getLevel())   //se il livello è più alto del massimo
                    {
                        max2=max1;   //faccio scalare il secondo massimo
                        max1=tmp;
                    }else{
                        if(tmp->getLevel() >= max2->getLevel())   //se il livello è più alto del secondo massimo
                            max2=tmp;
                    }
                }

                //__________________________Save Last++____________________________
                //se posso giocare solo la carta di livello più alto, passo
                if(hand->getFirstPlayable(state->winning)==max1)   //se dovrei giocare la carta di livello più alto
                {
                    if(hand->countCheapCards()>3)   //se ho ancora tante carte
                    {
                        //...............calcolo del livello dell'opzione.............
                        algorithmLevel = 18 + hand->getTotGroups();   //più carte mi restano, più conviene passare
                        //............................................................
                        possibilities.addOption(NULL,algorithmLevel);   //mi salvo l'opzione nella lista
                    }
                }else{

                    //__________________________Save Pre++____________________________
                    //se posso giocare solo la seconda carta di livello più alto, passo
                    if(hand->getFirstPlayable(state->winning)==max2)   //se dovrei giocare la seconda carta di livello più alto
                    {
                        if(hand->countCheapCards()>4)   //se ho ancora tante carte
                        {
                            //...............calcolo del livello dell'opzione.............
                            algorithmLevel = 15 + hand->getTotGroups();   //più carte mi restano, più conviene passare
                            //............................................................
                            possibilities.addOption(NULL,algorithmLevel);   //mi salvo l'opzione nella lista
                        }
                    }
                }

                //__________________________Save Last3____________________________
                //se giocherei la carta più alta o di livello più alto, passo
                if(possibilities.getBest()==max1  ||  possibilities.getBest()==hand->getLast())   //se dovrei giocare la carta di livello più alto
                {
                    if(hand->countCheapCards()>2)   //se ho ancora carte
                    {
                        //...............calcolo del livello dell'opzione.............
                        algorithmLevel = 20 + hand->getTotGroups();   //livello più basso di final rush
                        //............................................................
                        possibilities.addOption(NULL,algorithmLevel);   //mi salvo l'opzione nella lista
                    }
                }





            }


            /*
            cout <<"\nQuesto è il mazzo:\n";
            hand->print(true);
            cout <<"\nQueste sono le possibilità:\n";
            possibilities.print(true);
            //*/

            chosen=possibilities.getBest();

            /*
            if(chosen)
                cout <<"\nQuesta è la carta scelta: " <<chosen->getShortName() <<endl;
            else
                cout <<"\nPassa\n";
            //*/

            possibilities.reset();
            if(chosen)
            {
                if(state->winning  &&  chosen->getCount()>state->winning->getCount()
                    && ((chosen->getValue()==2) || (chosen->getValue()==1 && state->twos==0)))
                {
                    Card *tmp=new Card(chosen);
                    chosen=tmp->divide(state->winning->getCount());
                    delete tmp;
                }
                return new Card(chosen);
            }
            else
                return NULL;
        }
    }
}
