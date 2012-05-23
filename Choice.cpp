/*###########################################################################
  # Copyright (c) 2010 Giovanni Pessiva <giovanni.pessiva@gmail.com>        #
  # This program is free software: you can redistribute it and/or modify    #
  # it under the terms of the GNU General Public License version 3          #
  # as published by the Free Software Foundation.                           #
  ###########################################################################*/

#include "Choice.h"
#include <iostream>
#include <list>
using namespace std;

/** Comparator of option_t for sorting operations.
 * @param o1    first option_t
 * @param o2    second option_t
 * @return true if grade of o1 is higher than grade of 02
 * @see Choice::getBest()
 */
bool compareByGrade(option_t *o1, option_t *o2)
{
    int a=o1->grade;
    int b=o2->grade;
    if(a>b)
        return true;
    return false;
}

/** Choice destructor
 * Dealloc all the option_t; it does not delete
 * the Card object inside them.
 */
Choice::~Choice(){  reset();    }

/** Return the number of option_t.
 * @return size of the stack
 */
int Choice::getCount(){     return m_options.size();    }

/** Look for a Card.
 * @param value    value of the Card
 * @return true if there is a Card with that value
 */
bool Choice::hasCard(int value)
{
    list<option_t *>::iterator it;
    for(it=m_options.begin(); it!=m_options.end(); it++)
        if((*it)->chosen->getValue() == value)
            return true;
    return false;
}

/** Store a new option_t.
 * @param chosen    Card to store
 * @param grade     value to assign to that Card
 */
void Choice::addOption(Card *chosen,int grade)
{
    option_t *tmp=new option_t;
    tmp->chosen=chosen;
    tmp->grade=grade;
    m_options.push_front(tmp);
}

/** Extract and return the Card of the last option_t added.
 * @return last Card added, NULL if there are no option_t
 * @see Choice::popOption()
 */
Card *Choice::popCard()
{
    if(m_options.size())
    {
        Card *tmp=m_options.front()->chosen;
        option_t *tmp2=m_options.front();
        m_options.pop_front();
        delete tmp2;
        return tmp;
    }else
        return NULL;
}

/** Extract and return the last option_t added.
 * @return last option_t created, NULL if there are no option_t
 * @see Choice::popCard()
 */
option_t *Choice::popOption()
{
    if(m_options.size())
    {
        option_t *tmp=m_options.front();
        m_options.pop_front();
        return tmp;
    }else
        return NULL;
}

/** Return a pointer to the numTH Card.
 * @param num number of option_t to iterate through
 * @return the numTH Card, NULL if num is not valid
 */
Card *Choice::getThis(int num)
{
    int i=0;
    list<option_t *>::iterator it;
    for(it=m_options.begin(); it!=m_options.end(); it++)
        if(i++ == num)
            return (*it)->chosen;
    return NULL;
}

/** Return a pointer to the Card associated to higher value.
 * @return pointer to Card with higher grade, NULL is the stack is empty
 */
Card *Choice::getBest()
{
    if(m_options.size())
    {
        option_t *tmp;
        m_options.sort(compareByGrade);
        tmp=m_options.front();
        return tmp->chosen;
    }else
        return NULL;
}

/** Empty the stack.
 * Dealloc all the option_t, without deleting the Card objects inside them
 */
void Choice::reset()
{
    while(!m_options.empty())
    {
        delete m_options.front();
        m_options.pop_front();
    }
}

/** Print all the option_t in a numbered list.
 * @param showGrade     if it is set to false, it will show only the cards
 * @see Card::getShortName()
 */
void Choice::print(bool showGrade)
{
    list<option_t *>::iterator it;

    if(m_options.empty())
        cout <<"(nessuna carta)\n";
    else{
        int i=1;
        for(it=m_options.begin(); it!=m_options.end(); it++)
        {
            cout <<i++ <<") " <<((*it)->chosen ? (*it)->chosen->getShortName() : "(passa)");
            if(showGrade)
                cout <<" (grado: " <<(*it)->grade <<")";
            cout <<"\n";
        }
    }
}
