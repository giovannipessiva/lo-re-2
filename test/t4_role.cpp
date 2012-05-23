#include "../Role.h"
#include "../loRe2.h"
#include "../gameState.h"
#include <iostream>
#include <string>
using namespace std;

void test_role()
{
    player_t *data=new player_t;
    data->name="Bobbemalle";
    data->brainLevel=1;
    data->time=1;
    data->advancement=2;
    Role *prova=new King(data);

    if(data!=prova->getData())
        cout <<"errore: dati non corretti\n";

    if(prova->getRoleNumber()!=0 || prova->getRoleName().compare("Lo Re")!=0 )
        cout <<"errore: dati ruolo non corretti\n";

    if(prova->getTotCards()!=0 )
        cout <<"errore: numero carte sbagliato 1\n";

    struct gameState stato;
    stato.winning=new Card(1,0,0,0,3);

    prova->giveCard(new Card(1,0,0,0,4));
    if(prova->getTotCards()!=1 )
        cout <<"errore: sbagliata aggiunta carta 2: -->" <<prova->getTotCards() <<"\n";

    cout <<"ora provo a giocare una carta...\n";
    Card *scelta=prova->play(&stato);
    if(scelta==NULL)
        cout <<"errore: nessuna carta giocata\n";
    else
        cout <<"carta giocata: " <<scelta->getLongName() <<"\n";

    if(scelta->getValue()!=4 )
        cout <<"errore: sbagliata aggiunta carta\n";

    delete scelta;


    prova->giveCard(new Card(1,0,0,0,3));
    if(prova->getTotCards()!=1 )
        cout <<"errore: sbagliata aggiunta carta 4: -->" <<prova->getTotCards() <<"\n";

    cout <<"ora provo a giocare un'altra carta...\n";
    scelta=prova->play(&stato);
    if(scelta!=NULL)
        cout <<"errore: carta giocata sbagliata: " <<scelta->getLongName() <<"\n";
    else
        cout <<"carta correttamente non giocata\n";

     if(prova->getTotCards()!=1 )
        cout <<"errore: numero carte sbagliato 5\n";


    cout <<"controlli sui ruoli fatti\n";
}
