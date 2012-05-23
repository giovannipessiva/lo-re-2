#ifndef DECK_H_
#define DECK_H_

#include <list>
#include "loRe2.h"
#include "Card.h"
#include "Choice.h"
#include "gameState.h"

/** \brief group of Card objects.
 *
 * A Deck object stores Card objects, and provide usefull methods to analyze,
 * organize and iterate through them.
 */
class Deck{
    std::list<Card *> m_cards;
    std::list<Card *>::iterator m_selected;
    void sort();
    void setLevels(gameState *state);

    public:
    Deck();
    Deck(int players);
    ~Deck();
    void addCard(Card *c);
    Card *extract(Card *c);
    Card *extract(int value,int n);
    Card *extractRandom();
    int getTotCards();
    int getTotGroups();
    int getTotLevel(gameState_t *state);
    int countCheapCards();
    Card *getFirst();
    Card *getLast();
    Card *getNext();
    Card *getPrevious();
    Card *getThisValue(int value);
    Card *getThisGroup(int index);
    Card *getFirstPlayable(Card *winning);
    Card *getNextPlayable(Card *winning);
    Card *getLowerPlayable(Card *winning,gameState_t *state);
    void print(bool shortName);

    friend Card *Choice::getBest();
    #ifdef _TEST
    friend void test_deck();
    #endif
};

#endif
