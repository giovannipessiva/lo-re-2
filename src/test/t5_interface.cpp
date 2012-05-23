#include <iostream>
#include "../gameState.h"
#include "../Role.h"
#include "../Choice.h"
#include "../loRe2.h"
#include "../interface.h"
using namespace std;

void test_interface()
{
    struct gameState stato;
    stato.n=1;
    stato.winning=new Card(1,0,0,0,3);
    stato.winner=new King(new player_t);
    ((Role *) stato.winner)->getData()->name="Vince";
    stato.stillPlaying=3;
    msg_printState(&stato);


    Deck mazzo;
    mazzo.addCard(new Card(1,0,0,0,3));
    mazzo.addCard(new Card(1,1,0,0,4));
    mazzo.addCard(new Card(1,0,0,0,10));
    mazzo.addCard(new Card(1,0,1,1,13));
    mazzo.addCard(new Card(0,0,1,0,1));
    mazzo.addCard(new Card(1,0,0,1,2));

    /*
    Choice *ch;
    cout <<"--------\ntest modalità NO CARDS\n";
    ch=selectCard(&stato,&mazzo,NO_CARDS);

    cout <<"----------\ntest modalità ONE CARD\n";
    ch=selectCard(&stato,&mazzo,ONE_CARD);
    ch=selectCard(&stato,&mazzo,NO_CARDS);

    cout <<"---------\ntest modalità FOUR CARDS\n";
    ch=selectCard(&stato,&mazzo,FOUR_CARDS);
    ch=selectCard(&stato,&mazzo,NO_CARDS);

    cout <<"---------\ntest modalità N CARDS\n";
    ch=selectCard(&stato,&mazzo,N_CARDS);
    ch=selectCard(&stato,&mazzo,NO_CARDS);
    */

    if(strToInt("-123",-999,999)!=-123)
        cout <<"Errore 1\n";

    /*
    player_t *pla=readData(1);
    cout <<"\nnome: " <<pla->name;
    cout <<"\nbrain: " <<pla->brainLevel;
    cout <<"\ntime: " <<pla->time;
    cout <<"\nadvancement: " <<pla->advancement;

    pla=readData(2);
    cout <<"\nnome: " <<pla->name;
    cout <<"\nbrain: " <<pla->brainLevel;
    cout <<"\ntime: " <<pla->time;
    cout <<"\nadvancement: " <<pla->advancement;
    */

    cout <<"controlli sull'interfaccia completati\n";
}
