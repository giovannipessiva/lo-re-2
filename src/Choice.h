#ifndef CHOICE_H_
#define CHOICE_H_

#include <list>
#include "Card.h"
#include "option.h"

/** \brief stack of option_t.
 *
 * A Choice object is a stack (LIFO list) of option_t.
 */
class Choice{
    std::list<option_t *> m_options;

    public:
    ~Choice();
    int getCount();
    bool hasCard(int n);
    void addOption(Card *chosen,int grade=-1);
    Card *popCard();
    option_t *popOption();
    Card *getThis(int num);
    Card *getBest();
    void reset();
    void print(bool showGrade);
};

#endif
