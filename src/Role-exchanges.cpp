/*###########################################################################
  # Copyright (c) 2010 Giovanni Pessiva <giovanni.pessiva@gmail.com>        #
  # This program is free software: you can redistribute it and/or modify    #
  # it under the terms of the GNU General Public License version 3          #
  # as published by the Free Software Foundation.                           #
  ###########################################################################*/

#include <iostream>
#include "option.h"
#include "Role.h"
#include "interface.h"
#include "loRe2.h"

/** Exchange method for players without any role.
 * This method should not ever be called, but is necessary
 * to correcty implement Role virtual class
 * @return NULL
 */
Choice *NoRole::exchange(){   return NULL;    }

/** Exchange method for players with the role of King.
 * @return new Choice with his two lowest cards.
 * @see Slave::exchange()
 * @see Game::exchanges()
 */
Choice *King::exchange()
{
    Choice *offer=new Choice;
    offer->addOption(m_hand.extract(m_hand.getFirst()->getValue(),1),1);
    offer->addOption(m_hand.extract(m_hand.getFirst()->getValue(),1),2);
    return offer;
}

/** Exchange method for players with the role of Slave.
 * @return new Choice with his two highest cards.
 * @see King::exchange()
 * @see Game::exchanges()
 */
Choice *Slave::exchange()
{
    Choice *offer=new Choice;
    offer->addOption(m_hand.extract(m_hand.getLast()->getValue(),1),1);
    offer->addOption(m_hand.extract(m_hand.getLast()->getValue(),1),2);
    return offer;
}

/** Exchange method for players with the role of Knight.
 * @return new Choice with his lowest card.
 * @see Bitch::exchange()
 * @see Game::exchanges()
 */
Choice *Knight::exchange()
{
    Choice *offer=new Choice;
    offer->addOption(m_hand.extract(m_hand.getFirst()->getValue(),1),1);
    return offer;
}

/** Exchange method for players with the role of Bitch.
 * @return new Choice with his higher card.
 * @see Knight::exchange()
 * @see Game::exchanges()
 */
Choice *Bitch::exchange()
{
    Choice *offer=new Choice;
    offer->addOption(m_hand.extract(m_hand.getLast()->getValue(),1),1);
    return offer;
}

/** Exchange method for players with the role of Thief.
 * @return new Choice with his lowest card.
 * @see Chicken::exchange()
 * @see Game::exchanges()
 */
Choice *Thief::exchange()
{
    Choice *offer=new Choice;
    offer->addOption(m_hand.extract(m_hand.getFirst()->getValue(),1),1);
    return offer;
}

/** Exchange method for players with the role of Chicken.
 * @return new Choice with a random card.
 * @see Thief::exchange()
 * @see Game::exchanges()
 */
Choice *Chicken::exchange()
{
    Choice *offer=new Choice;
    offer->addOption(m_hand.extractRandom(),1);
    return offer;
}

/** Exchange method for players with the role of Merchant.
 * @return new Choice with copies of the offered cards.
 * @see Role::offer()
 * @see Game::exchanges()
 */
Choice *Merchant::exchange()
{
    Choice *offer=NULL;

    if(m_data->brainLevel==HUMAN)
    {
        offer=selectCard(NULL,&m_hand,N_CARDS);
    }else{
        int i=0;
        offer=new Choice;

        for(Card *tmp=m_hand.getLast();  tmp!=NULL  &&  i<MAX_OFFERED_CARDS;    tmp=m_hand.getPrevious())
        {
            if(tmp->getCount()==1  &&  tmp->getValue()>2)
            {
                Card *cloned=new Card(tmp);
                offer->addOption(cloned,++i);
            }
        }
    }
    return offer;
}

/** Offer an exchange of Cards to the Merchant
 * @param sale    Cards offered by the Merchant
 * @return a new option_t with a pointer to the Card offered and the index of the Card requested
 * @see Merchant::exchange()
 * @see Merchant::valueOffers(option_t **offers,int length,Choice *sale)
 */
option_t *Role::offer(Choice *sale)
{
    option_t *fromMe=NULL;

    if(getRoleNumber()==R_MERCHANT)
        return NULL;

    if(m_data->brainLevel==HUMAN)
    {

        int n=selectCard(sale);
        if(n!=-1)
        {
            Choice *tmp=selectCard(NULL,&m_hand,ONE_CARD);
            fromMe=tmp->popOption();
            delete tmp;
            fromMe->grade=n;
        }
    }else{
        int earn,max_earn=0;
        Card *c=NULL;
        fromMe=new option_t;
        fromMe->chosen=NULL;

        for(int j=0; j<sale->getCount(); j++)
        {
            for(int i=0; i<m_hand.getTotGroups(); i++)
            {
                c=m_hand.getThisGroup(i);

                earn=evaluateExchange(c->getValue(),sale->getThis(j));

                if(earn>max_earn &&
                    (c->getValue()>=(sale->getThis(j)->getValue() -1) ||
                    (c->getValue()>=(sale->getThis(j)->getValue() -4) &&  sale->hasCard(c->getValue())) ))
                {
                    max_earn=earn;
                    fromMe->chosen=c;
                    fromMe->grade=j;
                }
            }
        }
    }
    if(fromMe  &&  fromMe->chosen)
    {
        fromMe->chosen=m_hand.extract(fromMe->chosen->getValue(),1);
        msg_offer(sale->getThis(fromMe->grade),fromMe->chosen,m_data->name);
    }else
        msg_offer(NULL,NULL,m_data->name);

    return fromMe;
}

/** Accept one exchange offered by a player.
 * @param offers array of pointers to players'offers, in order of Role
 * @param length number of offers
 * @param sale Cards offered by Merchant
 * @return pointer to the Card of the accepted offer
 * @see Role::offer(Choice *sale)
 */
Card *Merchant::valueOffers(option_t **offers,int length,Choice *sale)
{
    if(m_data->brainLevel==HUMAN)
    {
        if(countOffers(offers,length)==0)
        {
            msg_noOffers();
            return NULL;
        }else{
            selectCard(NULL,&m_hand,NO_CARDS);
            int n=selectOffer(offers,length,sale);
            if(n!=-1)
            {
                while(offers[n]==NULL || offers[n]->chosen==NULL)
                    n++;

                msg_accept(sale->getThis(offers[n]->grade),offers[n]->chosen);
                return offers[n]->chosen;
            }else{
                msg_accept(NULL,NULL);
                return NULL;
            }
        }
    }else{
        if(countOffers(offers,length)==0)
        {
            msg_noOffers(m_data->name);
            return NULL;
        }else{
            int i,best,earn,max_earn;

            best=-1;
            max_earn=0;
            for(i=0; i<length; i++)
            {
                if(offers[i] && offers[i]->chosen)
                {
                    earn=evaluateExchange(sale->getThis(offers[i]->grade)->getValue(),offers[i]->chosen);
                    // If two exchanges have the same value, he will keep the first
                    // since it has been offered by an higher Role
                    if(earn>max_earn)
                    {
                        max_earn=earn;
                        best=i;
                    }
                }
            }
            if(best!=-1)
            {
                msg_accept(sale->getThis(offers[best]->grade),offers[best]->chosen);
                return offers[best]->chosen;
            }else{
                msg_accept(NULL,NULL);
                return NULL;
            }
        }
    }
}

/** Return the gain from that exchange.
 * Create a new card usign given values.
 * @param given     value of the card that will be given away
 * @param received  Card that will be received
 * @return gain
 */
int Role::evaluateExchange(int given,Card *received)
{
    if(received==NULL)
        return -666;

    Card *giveAway=m_hand.extract(given,1);
    Card *takeForYou=new Card(received);
    m_hand.addCard(takeForYou);
    int lv=m_hand.getTotLevel(NULL) - 6*m_hand.getTotGroups();

    delete m_hand.extract(received->getValue(),1);
    m_hand.addCard(giveAway);

    lv-=m_hand.getTotLevel(NULL) - 6*m_hand.getTotGroups();

    return lv;
}

/** Count the valid offers.
 * @param offers array of pointers to players'offers
 * @param length number of offers
 * @return number of valid offers
 * @see Role::offer(Choice *sale)
 */
int Role::countOffers(option_t **offers,int length)
{
    if(offers==NULL) return 0;

    int n=length;
    for(int i=0; i<length; i++)
        if(offers[i]==NULL || offers[i]->chosen==NULL)
            n--;
    return n;
}
