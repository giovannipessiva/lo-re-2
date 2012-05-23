#include <iostream>
#include "../loRe2.h"
#include "../Deck.h"
using namespace std;

void test_deck()
{
    Deck *d=new Deck();
    if( d->getTotCards()!=0 || \
        d->getTotGroups()!=0 )
        cout <<"1: Errore nel numero di carte (" <<d->getTotCards() <<"-" <<d->getTotGroups() <<")\n";
    delete d;

    d=new Deck(3);
    if( d->getTotCards()!=13*4 -1 || \
        d->getTotGroups()!=13 )
        cout <<"2: Errore nel numero di carte (" <<d->getTotCards() <<"-" <<d->getTotGroups() <<")\n";
    delete d;

    d=new Deck(4);
    if( d->getTotCards()!=13*4 || \
        d->getTotGroups()!=13 )
        cout <<"3: Errore nel numero di carte (" <<d->getTotCards() <<"-" <<d->getTotGroups() <<")\n";
    delete d;

    d=new Deck(5);
    if( d->getTotCards()!=13*4 -2 || \
        d->getTotGroups()!=13 )
        cout <<"4: Errore nel numero di carte (" <<d->getTotCards() <<"-" <<d->getTotGroups() <<")\n";
    delete d;

    d=new Deck(6);
    if( d->getTotCards()!=13*4 -4 || \
        d->getTotGroups()!=12 )
        cout <<"5: Errore nel numero di carte (" <<d->getTotCards() <<"-" <<d->getTotGroups() <<")\n";
    delete d;

    d=new Deck(7);
    if( d->getTotCards()!=13*4 -3 || \
        d->getTotGroups()!=13 )
        cout <<"6: Errore nel numero di carte (" <<d->getTotCards() <<"-" <<d->getTotGroups() <<")\n";
    delete d;

    d=new Deck();
    d->addCard(new Card(true,false,false,false,3));
    d->addCard(new Card(true,true,false,false,4));
    d->addCard(new Card(true,false,false,false,5));
    d->addCard(new Card(true,true,true,false,7));
    d->addCard(new Card(true,false,false,false,1));
    d->addCard(new Card(true,false,true,false,2));
    d->addCard(new Card(false,false,false,true,7));
    #ifdef _TEST
    d->sort();
    #endif

    d->extract(7,1);
    d->extract(2,2);

    if( d->getTotCards()!=8 || d->getTotGroups()!=5 )
        cout <<"7: Errore nel numero di carte (" <<d->getTotCards() <<"-" <<d->getTotGroups() <<")\n";

    if( d->getLast()->getValue()!=1 )
        cout <<"10: Errore\n";
    if( d->getNext()!=NULL )
        cout <<"11: Errore\n";


    if( d->getFirst()->getValue()!=3)
        cout <<"12: Errore\n";
    if( d->getNext()->getValue()!=4 )
        cout <<"13: Errore\n";
    if( d->getPrevious()->getValue()!=3)
        cout <<"15: Errore\n";

    if( d->getFirstPlayable(new Card(true,true,false,false,3))->getValue()!=4)
        cout <<"16: Errore\n";

    if( d->getNextPlayable(new Card(true,true,false,false,3))->getValue()!=7)
        cout <<"17: Errore\n";

    d->extract(d->getPrevious());
    if( d->getThisValue(5)!=NULL)
        cout <<"18: Errore\n";

    cout <<"Ne rimuovo 2 a caso:\n";
    d->extractRandom();
    d->extractRandom();
    d->addCard(new Card(true,true,false,false,9));
    cout <<"Trovo il secondo gruppo:\n";
    cout << d->getThisGroup(2)->getShortName() <<endl;

    cout <<"Controlli sui Deck fatti\n";
    delete d;
}
