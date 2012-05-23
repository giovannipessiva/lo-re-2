/*###########################################################################
  # Copyright (c) 2010 Giovanni Pessiva <giovanni.pessiva@gmail.com>        #
  # This program is free software: you can redistribute it and/or modify    #
  # it under the terms of the GNU General Public License version 3          #
  # as published by the Free Software Foundation.                           #
  ###########################################################################*/

#include <iostream>
#include <string>
#include <sstream>
#include "Card.h"
using namespace std;

/** Card constructor 1.
 * Create a new card usign given values.
 * @param hearts    suit: hearts
 * @param diamonds  suit: diamonds
 * @param clubs     suit: clubs
 * @param spades    suit: spades
 * @param value     value of these cards (1-13)
 * @see Card::Card(Card *clone)
 */
Card::Card(bool hearts,bool diamonds,bool clubs,bool spades,int value)
{
    if(value<1 || value>13 || (hearts || diamonds || clubs || spades)==false )
        cerr <<"Error: invalid card value: " <<value <<" (" <<hearts <<diamonds <<clubs <<spades <<")\n";

    m_hearts=hearts;
    m_diamonds=diamonds;
    m_clubs=clubs;
    m_spades=spades;

    m_value=value;
    m_level=0;
}

/** Card constructor 2.
 * Create a new card usign given values.
 * @param clone     card whose values will be copied
 * @see Card::Card(bool hearts,bool diamonds,bool clubs,bool spades,int value)
 */
Card::Card(Card *clone)
{
    m_hearts=clone->isHearts();
    m_diamonds=clone->isDiamonds();
    m_clubs=clone->isClubs();
    m_spades=clone->isSpades();

    m_value=clone->getValue();
    m_level=clone->getLevel();
}

/** Getter method for the value.
 * @return value of the card
 */
int Card::getValue(){ return m_value; }

/** Getter method for the level.
 * @return level of the card
 * @see Card::setLevel(int level)
 * @see Deck::setLevels(gameState *state)
 */
int Card::getLevel(){ return m_level; }

/** Setter method for the level.
 * @param level level of the card
 * @see Card::getLevel()
 * @see Deck::setLevels(gameState *state)
 */
void Card::setLevel(int level){ m_level=level; }

/** @return true if this Card contains suit of hearts */
bool Card::isHearts(){ return m_hearts; }
/** @return true if this Card contains suit of diamonds */
bool Card::isDiamonds(){ return m_diamonds; }
/** @return true if this Card contains suit of clubs */
bool Card::isClubs(){ return m_clubs; }
/** @return true if this Card contains suit of spades */
bool Card::isSpades(){ return m_spades; }

/** Card counter.
 * @return number of cards in this Card object (1-4)
 */
int Card::getCount()
{
    int n=0;
    if(m_hearts)    n++;
    if(m_diamonds)  n++;
    if(m_clubs)     n++;
    if(m_spades)    n++;
    return n;
}

/** Name of the card for shortening.
 * @return name of this Card (eg: "5 ♥", "Re x2")
 * @see Card::getLongName()
 */
string Card::getShortName()
{
    string name="";

    switch(m_value)
    {
        case 1:     name+="Asso";   break;
        case 11:    name+="Fante";  break;
        case 12:    name+="Donna";  break;
        case 13:    name+="Re";     break;
        default:    name+=convertInt(m_value);
    }

    int n=getCount();
    if(n==1)
    {
        #ifdef _WIN32
        if(m_hearts)    name+= (char) 3; //WINDOWS
        if(m_diamonds)  name+= (char) 4; //WINDOWS
        if(m_clubs)     name+= (char) 5; //WINDOWS
        if(m_spades)    name+= (char) 6; //WINDOWS
        #else
        if(m_hearts)    name+=" ♥"; //LINUX
        if(m_diamonds)  name+=" ♦"; //LINUX
        if(m_clubs)     name+=" ♣"; //LINUX
        if(m_spades)    name+=" ♠"; //LINUX
        #endif
    }else{
        name+=" x";
        name+=convertInt(n);
    }
    return name;
}

/** Name of the card
 * @return name of this Card (eg: "5 di cuori", "coppia di Re")
 * @see Card::getShortName()
 */
string Card::getLongName()
{
    int n=getCount();
    string name="";

    if(n==1)
    {
        switch(m_value)
        {
            case 1:     name+="Asso";   break;
            case 11:    name+="Fante";  break;
            case 12:    name+="Donna";  break;
            case 13:    name+="Re";     break;
            default:    name+=convertInt(m_value);
        }
        name+=" di ";
        if(m_hearts)    name+="cuori";
        if(m_diamonds)  name+="quadri";
        if(m_clubs)     name+="fiori";
        if(m_spades)    name+="picche";

    }else{

        switch(n)
        {
            case 2: name+="coppia"; break;
            case 3: name+="tris";   break;
            case 4: name+="poker";  break;
        }
        name+=" di ";
        switch(m_value)
        {
            case 1:     name+="Assi";   break;
            case 11:    name+="Fanti";  break;
            case 12:    name+="Donne";  break;
            case 13:    name+="Re";     break;
            default:    name+=convertInt(m_value);
        }
    }
    return name;
}

/** Compare this card with another.
 * @param winning   Card to beat
 * @return true if this Card can win over Card *winning
 */
bool Card::isPlayable(Card *winning)
{
    if(winning==NULL)
        return true;

    Card *chosen=this;
    if(chosen->getCount() >= winning->getCount())
    {
        int a=winning->getValue();
        int b=chosen->getValue();
        if(a>2){
            if(b>a || b==1 || b==2)
                return true;
            else
                return false;
        }else{
            if(b>2)
                return false;
            else
                if(b>a){
                    return true;
                }else{
                    return false;
                }
        }
    }else
        return false;
}

/** "Sum" another Card.
 * eg: C1=3♦, C2=3♣♠ -> C1.absorb(C2) -> C1=3♦♣♠
 * (C1 and C2 must have the same value)
 * @param old   Card to be absorbed
 */
void Card::absorb(Card *old)
{
    if(old->getValue()==this->getValue())
    {
        m_hearts=   m_hearts    || old->isHearts();
        m_diamonds= m_diamonds  || old->isDiamonds();
        m_clubs=    m_clubs     || old->isClubs();
        m_spades=   m_spades    || old->isSpades();
    }else{
        cerr <<"Error - attempt to sum two Cards with different values: ";
        cerr <<old->getShortName() <<" absorbed by " <<this->getShortName() <<endl;
    }
}

/** "Split" this Card.
 * eg: C1=3♦♣♠, C2=C1.divide(2) -> C1=3♠, C2=3♦♣
 * @param n   number of cards to be extracted, must be < getCount()
 * @return new Card
 */
Card *Card::divide(int n)
{
    if(n>=getCount())
    {
        cerr <<"Card-divide error - requested: " <<n <<", having: " <<getCount() <<endl;
        return this;
    }

    bool h,d,c,s;
    if(m_hearts && n>0)
    {
        n--;
        h=true;
        m_hearts=false;
    }else
        h=false;

    if(m_diamonds && n>0)
    {
        n--;
        d=true;
        m_diamonds=false;
    }else
        d=false;

    if(m_clubs && n>0)
    {
        n--;
        c=true;
        m_clubs=false;
    }else
        c=false;

    if(m_spades && n>0)
    {
        s=true;
        m_spades=false;
    }else
        s=false;

    return new Card(h,d,c,s,m_value);
}

/** Integer to string.
 * @param number   number to convert
 * @return string with the number
 */
string Card::convertInt(int number)
{
   stringstream ss; //create a stringstream
   ss << number;    //add number to the stream
   return ss.str(); //return a string with the contents of the stream
}
