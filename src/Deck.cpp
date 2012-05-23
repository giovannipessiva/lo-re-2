/*###########################################################################
  # Copyright (c) 2010 Giovanni Pessiva <giovanni.pessiva@gmail.com>        #
  # This program is free software: you can redistribute it and/or modify    #
  # it under the terms of the GNU General Public License version 3          #
  # as published by the Free Software Foundation.                           #
  ###########################################################################*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "loRe2.h"
#include "Deck.h"
#include "gameState.h"
using namespace std;

/** Comparator of Card objects for sorting operations.
 * @param c1    first Card
 * @param c2    second Card
 * @return true if value of c1 is higher than value of c2
 * @see Deck::sort()
 */
bool compare(Card *c1, Card *c2)
{
    int a=c1->getValue();
    int b=c2->getValue();
    if(a>2){
        if(b>a || b==1 || b==2)
            return true;
        else
            return false;
    }else{
        if(b>2)
            return false;
        else
            if(a>b){
                return false;
            }else{
                return true;
            }
    }
}

/** Deck constructor 1.
 * Create an empty Deck
 * @see Deck::Deck(int players)
 */
Deck::Deck()
{
    m_selected=m_cards.end();
    srand((unsigned) time(0));
}

/** Deck constructor 2.
 * Create a Deck with Card objects for a given number of players,
 * so that (number of cards)%(players)=0
 * @see Deck::Deck()
 */
Deck::Deck(int players)
{
    int i;
    m_selected=m_cards.end();
    srand((unsigned) time(0));

    for(i=1; i<=2; i++)  // assi e due
        m_cards.push_back(new Card(true,true,true,true,i));

    switch(players)
    {
        case 3: m_cards.push_back(new Card(false,true,true,true,3)); break;   // 1 carta in meno
        case 4: m_cards.push_back(new Card(true,true,true,true,3)); break;   // tutto il mazzo
        case 5: m_cards.push_back(new Card(false,false,true,true,3)); break;   // 2 carte in meno
        case 6: break;   // 4 carte in meno
        case 7: m_cards.push_back(new Card(false,false,false,true,3)); break;   // 3 carte in meno
        default: cerr <<"Invalid argument for Deck::Deck(int players) /n 3<=players<=7";
    }

    for(i=4; i<=13; i++)  // dal 4 al Re
        m_cards.push_back(new Card(true,true,true,true,i));

}

/** Deck destructor.
 * Delete all the Card objects.
 */
Deck::~Deck()
{
    while(!m_cards.empty())
    {
        delete *(m_cards.begin());
        m_cards.pop_front();
    }
}

/** Add a Card object to this Deck.
 * Add a Card object; if there is already a Card
 * with the same value, they are merged.
 * After the insertion, the Deck is sorted.
 * @param c    pointer to the Card to add
 */
void Deck::addCard(Card *c)
{
    if(c)
    {
        Card *tmp=getThisValue(c->getValue());
        if(tmp)
            tmp->absorb(c);
        else{
            m_cards.push_front(c);
            sort();
        }
    }
}

/** Extract and return a specific Card.
 * Remove the Card from the Deck, and return a copy of its.
 * There are no checks on the Deck to see if the given Card
 * actually is inside the Deck.
 * @param c     pointer to the Card to remove
 * @return a clone of c
 * @see Deck::extract(int value,int n)
 * @see Deck::extractRandom()
 */
Card *Deck::extract(Card *c)
{
    Card *tmp=new Card(c);
    m_cards.remove(c);
    return tmp;
}

/** Extract and return a specific Card.
 * Remove or split the Card in the Deck, and return a new Card.
 * If there is no Card with that value, NULL is returned.
 * If n is bigger than [Card in the Deck]->getCount(), NULL is returned.
 * @param value     value of the Card
 * @param n         number of different suits (cards inside the Card)
 * @return a new Card
 * @see Deck::extract(Card *c)
 * @see Deck::extractRandom()
 */
Card *Deck::extract(int value,int n)
{
    Card *requested;
    list<Card *>::iterator it;
    for(it=m_cards.begin(); it!=m_cards.end(); it++)
    {
         if((*it)->getValue()==value)
         {
            requested=(*it);
            if(requested->getCount()<n)
            {
                cerr <<"Error: invalid n value. requested is: " <<requested->getShortName() <<endl;
                return NULL;
            }

            if(requested->getCount()==n)
                requested=extract(requested);
            else
                requested=requested->divide(n);

            return requested;
         }
    }
    cerr <<"Cannot find card: " <<value <<" x" <<n <<" in this Deck:" <<endl;
    print(true);
    return NULL;
}

/** Extract and return a single random Card.
 * @return a new Card with a single suit (getCount()==1), NULL is the Deck is empty
 * @see Deck::extract(Card *c)
 * @see Deck::extract(int value,int n)
 */
Card *Deck::extractRandom()
{
    if(getTotGroups()==0)
        return NULL;

    Card *tmp=getThisGroup(rand()%getTotGroups());
    if(tmp->getCount()==1)
        return extract(tmp);
    else
        return tmp->divide(1);
}

/** Count the cards in the Deck.
 * @return total number of single cards
 * @see Deck::getTotGroups()
 */
int Deck::getTotCards()
{
    int n=0;
    list<Card *>::iterator it;
    for(it=m_cards.begin(); it!=m_cards.end(); it++)
        if(*it)
            n+=(*it)->getCount();
        else
            cerr <<"Error getting total number of cards\n";
    return n;
}

/** Count the Card objects in the Deck.
 * @return total number of Card objects
 * @see Deck::getTotCards()
 */
int Deck::getTotGroups(){    return m_cards.size();    }

/** Return the total level of this Deck.
 * @param state    state of the current Game, or NULL
 * @return sum of the levels of all the Cards
 * @see Deck::setLevels(gameState_t *state)
 */
int Deck::getTotLevel(gameState_t *state)
{
    setLevels(state);

    int n=0;
    list<Card *>::iterator it;
    for(it=m_cards.begin(); it!=m_cards.end(); it++)
        if(*it)
            n+=(*it)->getLevel();
        else
            cerr <<"Error getting total level - NULL Card\n";
    return n;
}

/** Count the Card with level < TWO_LEV
 * @return number of Card objects with low level
 * @see Card::Card(Card *clone)
 */
int Deck::countCheapCards()
{
    setLevels(NULL);

    int n=0;
    list<Card *>::iterator it;
    for(it=m_cards.begin(); it!=m_cards.end(); it++)
        if((*it)->getLevel() < TWO_LEV  ||  (*it)->getValue() < 7)
            n++;
    return n;
}

/** Return a pointer to the Card of the Deck with lower value.
 * @return pointer to lower Card
 * @see Deck::getLast()
 */
Card *Deck::getFirst()
{
    m_selected=m_cards.begin();
    return m_cards.front();
}

/** Return a pointer to the Card of the Deck with higher value.
 * @return pointer to higher Card
 * @see Deck::getFirst()
 */
Card *Deck::getLast()
{
    //sposto l'iteratore sull'ultima carta
    for(m_selected=m_cards.begin(); m_selected!=m_cards.end(); m_selected++);
    m_selected--;

    return m_cards.back();
}

/** Return a pointer to the next Card in the internal iterator.
 * @return pointer to next Card
 * @see Deck::getFirst()
 * @see Deck::getLast()
 * @see Deck::getPrevious();
 * @see Deck::getNextPlayable(Card *winning)
 */
Card *Deck::getNext()
{
    if(m_selected!=m_cards.end())
    {
        m_selected++;
        if(m_selected!=m_cards.end()){
            return (Card *) *m_selected;
        }else{
            return NULL;
        }
    }else{
        return getFirst();
    }
}

/** Return a pointer to the next playable Card in the internal iterator.
 * @return pointer to next playable Card
 * @see Card::isPlayable(Card *winning)
 * @see Deck::getNext()
 * @see Deck::getFirst()
 * @see Deck::getNext()
 * @see Deck::getPrevious();
 */
Card *Deck::getNextPlayable(Card *winning)
{
    Card *tmp;
    do{
        tmp=getNext();
    }while(tmp && !tmp->isPlayable(winning));
    return tmp;
}

/** Return a pointer to the previous Card in the internal iterator.
 * @return pointer to previous Card
 * @see Deck::getFirst()
 * @see Deck::getLast()
 * @see Deck::getNext()
 * @see Deck::getNextPlayable(Card *winning)
 */
Card *Deck::getPrevious()
{
    if(m_selected!=m_cards.begin())
    {
        m_selected--;
        return (Card *) *m_selected;
    }else{
        return NULL;
    }
}

/** Return a specific Card.
 * @param index     number of the Card
 * @return pointer to the indexTH Card, NULL if index is invalid
 * @see Deck::getThisValue(int value)
 */
Card *Deck::getThisGroup(int index)
{
    if(index>getTotGroups())
        return NULL;

    m_selected=m_cards.begin();
    for(int i=0; i<index; i++)
        m_selected++;

    return (Card *) *m_selected;
}

/** Return a specific Card.
 * @param value     value of the Card
 * @return pointer to the Card with that value, NULL if it is not in the Deck
 * @see Deck::getThisGroup(int value)
 */
Card *Deck::getThisValue(int value)
{
    for(m_selected=m_cards.begin(); m_selected!=m_cards.end(); m_selected++)
        if((*m_selected)->getValue()==value)
            return *m_selected;
    return NULL;
}

/** Return the lowest-value Card playable.
 * @param winning    last played Card
 * @return pointer to the lowest Card playable, NULL if there are no playable Cards
 * @see Card::isPlayable(Card *winning)
 * @see Deck::getLowerPlayable(Card *winning,gameState_t *state)
 */
Card *Deck::getFirstPlayable(Card *winning)
{
    for(m_selected=m_cards.begin(); m_selected!=m_cards.end(); m_selected++)
        if((*m_selected)->isPlayable(winning))
            return *m_selected;
    return NULL;
}

/** Return the lowest-level Card playable.
 * @param winning   last played Card
 * @param state     state of current Game, or NULL
 * @return pointer to the lowest-level Card playable, NULL if there are no playable Cards
 * @see Card::isPlayable(Card *winning)
 * @see Deck::setLevels(gameState *state)
 * @see Deck::getLowerPlayable(Card *winning,gameState_t *state)
 */
Card *Deck::getLowerPlayable(Card *winning,gameState_t *state)
{
    setLevels(state);

    Card *tmp=NULL;
    for(m_selected=m_cards.begin(); m_selected!=m_cards.end(); m_selected++)
        if((*m_selected)->isPlayable(winning) \
         && (tmp==NULL  ||  tmp->getLevel()>=(*m_selected)->getLevel()) )
                tmp=*m_selected;
    return tmp;
}

/** Print a numbered list of all the Card objects in this Deck.
 * @param shortName    if it is set to true, Card::getShortName() will be used
 * @see Card::getShortName()
 * @see Card::getLongName()
 */
void Deck::print(bool shortName)
{
    int i=1;
    list<Card *>::iterator it;
    for(it=m_cards.begin(); it!=m_cards.end(); it++)
        cout <<i++ <<") " <<(shortName? (*it)->getShortName() : (*it)->getLongName()) <<endl;
}

/** Sort the Deck, from lower value to higher value.
 * @see compare(Card *c1, Card *c2)
 */
void Deck::sort(){ m_cards.sort(compare); }

/** Assign a level to every Card in the Deck.
 * @param state     state of current Game, or NULL
 */
void Deck::setLevels(gameState *state)
{
    int level=0;
    list<Card *>::iterator it;
    for(it=m_cards.begin(); it!=m_cards.end(); it++)
    {
         switch((*it)->getValue())
        {
            case 1:  level = ACE_LEV;           break;   //conventional value for ace
            case 2:  level = TWO_LEV;           break;   //conventional value for two
            default: level = (*it)->getValue(); break;
        }

        switch((*it)->getCount())
        {
            case 1: break;
            case 2: level *= 1.8;   break;   //couple
            case 3: level *= 2.5;   break;   //tris
            case 4: level *= 3.2;     break;   //poker
            default: cerr <<"Error! " <<(*it)->getShortName() <<" is not valid\n";
            exit(1);
        }

        if(state && state->n > 0)
        {
            switch((*it)->getValue())
            {
                case 1: switch(state->aces)
                        {
                            case 3: level +=1;  break;
                            case 2: level +=2;  break;
                            case 1: level +=3;  break;
                        }
                        switch(state->twos)
                        {
                            case 0: level +=6;  break;
                            case 1: level +=2;  break;
                            default:  break;
                        }
                case 2: switch(state->twos)
                        {
                            case 3: level +=1;  break;
                            case 2: level +=2;  break;
                            case 1: level +=4;  break;
                        }
            }
        }
        (*it)->setLevel(level);
    }
}
