#include "../Choice.h"
#include <iostream>
using namespace std;

void test_choice()
{
    Choice *c=new Choice();

    if( c->getCount()!=0 )
        cout <<"1 : Errore\n";

    Card *car=new Card(1,0,0,0,3);
    c->addOption(car,1);
    if( c->getCount()!=1 )
        cout <<"2 : Errore\n";
    if( c->getBest()!=car )
        cout <<"3 : Errore\n";
    c->print(true);

    Card *car2=new Card(1,0,0,0,4);
    c->addOption(car2,4);
    if( c->getCount()!=2 )
        cout <<"4 : Errore\n";
    if( c->getBest()!=car2 )
        cout <<"5 : Errore\n";
    c->print(false);

    if( c->popCard()!=car2 )
        cout <<"6 : Errore\n";
    if( c->getCount()!=1 )
        cout <<"7 : Errore\n";
    if( c->getBest()!=car )
        cout <<"8 : Errore\n";


    if( !c->hasCard(car->getValue()) )
        cout <<"9 : Errore\n";
    if( c->hasCard(car2->getValue()) )
        cout <<"10 : Errore\n";

    c->addOption(car2,21);
    if( c->getCount()!=2 )
        cout <<"11 : Errore\n";

    option_t *opt=c->popOption();
    if( opt->chosen!=car2 || opt->grade!=21 )
        cout <<"12 : Errore\n";
    if( c->getCount()!=1 )
        cout <<"13 : Errore\n";

    c->reset();
    if( c->getCount()!=0 )
        cout <<"6 : Errore\n";
    cout <<car->getShortName() <<" " <<car2->getShortName() <<"\n";

    cout <<"Controlli sulle Choice fatti\n";

    delete c;
    delete car;
    delete car2;
}
