#include <iostream>
#include "../Card.h"
using namespace std;

void test_card()
{
    Card *c=new Card(true,true,true,true,10);

    if( c->getValue()!=10 || \
        c->isHearts()!=true || \
        c->isDiamonds()!=true || \
        c->isClubs()!=true || \
        c->isSpades()!=true || \
        c->getCount()!=4 )
        cout <<"Errore nei dati di c\n";

    Card *copia=new Card(c);
    if( copia->getValue()!=10 || \
        copia->isHearts()!=true || \
        copia->isDiamonds()!=true || \
        copia->isClubs()!=true || \
        copia->isSpades()!=true || \
        copia->getCount()!=4 )
        cout <<"Errore nei dati di copia\n";

    cout <<"Carta iniziale: " <<c->getLongName() <<" ovvero: " <<c->getShortName() <<"\n";
    Card *div=c->divide(3);
    if( c->getValue()!=10 || \
        c->getCount()!=1 )
        cout <<"Errore nei dati di c\n";
    if( div->getValue()!=10 || \
        div->getCount()!=3 )
        cout <<"Errore nei dati di div\n";

    cout <<"Carte separate: " <<c->getLongName() <<" e " <<div->getLongName() <<"\n";

    c->absorb(div);
    if( c->getValue()!=10 || \
        c->isHearts()!=true || \
        c->isDiamonds()!=true || \
        c->isClubs()!=true || \
        c->isSpades()!=true || \
        c->getCount()!=4 )
        cout <<"Errore nei dati di c\n";
    cout <<"Carta finale: " <<c->getLongName() <<" ovvero: " <<c->getShortName() <<"\n";


    c->setLevel(6);
    if(c->getLevel()!=6)
        cout <<"Errore nel livello ci c\n";



    if( !((new Card(1,0,0,0,2))->isPlayable(new Card(1,0,0,0,1))) )
        cout <<"1 : Errore\n";

    if( (new Card(1,0,0,0,1))->isPlayable(new Card(1,0,0,0,2)) )
        cout <<"2 : Errore\n";

    if( (new Card(1,0,0,0,4))->isPlayable(new Card(1,0,0,0,5)) )
        cout <<"3 : Errore\n";

    if( (new Card(1,0,0,0,3))->isPlayable(new Card(1,0,0,0,1)) )
        cout <<"4 : Errore\n";

    if( (new Card(1,0,0,0,6))->isPlayable(new Card(1,1,1,1,5)) )
        cout <<"5 : Errore\n";

    if( (new Card(1,1,1,0,4))->isPlayable(new Card(1,0,0,0,2)) )
        cout <<"6 : Errore\n";

    if( !((new Card(1,0,0,1,6))->isPlayable(new Card(1,0,1,0,5))) )
        cout <<"7 : Errore\n";

    if( (new Card(1,1,0,0,1))->isPlayable(new Card(1,1,1,0,5)) )
        cout <<"8 : Errore\n";

    if( !((new Card(1,1,0,0,2))->isPlayable(new Card(1,0,0,0,1))) )
        cout <<"9 : Errore\n";

    cout <<"Controlli sulle Card fatti\n";
}
