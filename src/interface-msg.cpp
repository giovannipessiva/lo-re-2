/*###########################################################################
  # Copyright (c) 2010 Giovanni Pessiva <giovanni.pessiva@gmail.com>        #
  # This program is free software: you can redistribute it and/or modify    #
  # it under the terms of the GNU General Public License version 3          #
  # as published by the Free Software Foundation.                           #
  ###########################################################################*/

#include <iostream>
#include <string>
#include "loRe2.h"
#include "interface.h"
#include "gameState.h"
#include "player.h"
#include "Card.h"
#include "Role.h"
using namespace std;

/** Message: new game started */
void msg_startGame(){  cout <<endl <<"La mano comincia. Le carte vengono distribuite." <<endl; }

/** Message: no files found */
void msg_noFile(){  cout <<"Non ho trovato nessuna partita da caricare (nessun file \"" <<FILE_NAME <<"\")..." <<endl; }

/** Message: Card selected from user is not playable */
void msg_rejectCard(){  cout <<" Non puoi buttare queste carte!" <<endl; }

/** Message: no exchanges offered to user */
void msg_noOffers(){  cout <<" Nessuno e' interessato alle tua carte. Vieni sfottuto per questo." <<endl; }

/** Message: no exchanges offered to Merchant
 * @param name      Merchant's name
 */
void msg_noOffers(string name){  cout <<" Nessuno e' interessato alle carte di " <<name <<". Viene sfottuto per questo." <<endl; }

/** Message: exchanges started */
void msg_startExchanges(){  cout <<endl <<" Verranno ora effettuati gli scambi secondo i ruoli dei giocatori." <<endl; }

/** Message: now it is his turn
 * @param name      name of the player
 * @param cards     number of player's cards
 */
void msg_hisTurn(string name,int cards){ cout <<"Tocca a " <<name <<" (" <<cards <<" carte)" <<endl; }

/** Message: printed before Merchant's offers
 * @param heDidSome      true if the Merchant proposed any offer
 */
void msg_merchOffer(bool heDidSome)
{
    if(heDidSome)
        cout <<"\n Queste sono le carte offerte dal Mercante:" <<endl;
    else
        cout <<"\n Il Mercante non offre nessuna carta." <<endl;
}

/** Message: exchange of two Card
 * @param request       Card requested
 * @param offer         Card offered
 */
void msg_exchange(Card *request,Card *offer)
{
    if(offer && request)
        cout <<offer->getShortName() <<" in cambio di: " <<request->getShortName() <<endl;
}

/** Message: exchange of two Card by a player
 * @param name          name of the player
 * @param request       Card requested
 * @param offer         Card offered
 * @see msg_exchange();
 */
void msg_offer(Card *request,Card *offer,string name)
{
    cout <<" >" <<name;
    if(offer && request){
        cout <<" offre questo scambio:" <<endl;
        msg_exchange(request,offer);
    }else
        cout <<" non propone scambi." <<endl;
}

/** Message: response to an offer
 * @param request       Card requested
 * @param offer         Card offered
 */
void msg_accept(Card *request,Card *offer)
{
    if(offer && request){
        cout <<" Il mercante e' interessato allo scambio:" <<endl;
        msg_exchange(request,offer);
    }else
        cout <<" Il mercante non e' interessato a fare scambi." <<endl;
}

/** Message: print the information in a gameState_t
 * @param state      state to print
 */
void msg_printState(gameState_t *state)
{
    cout <<" Questa e' la situazione:" <<endl;
    cout <<" >Ci sono " <<state->stillPlaying;
    if(state->winner)
    {
        cout <<" giocatori, prende " <<( (Role *) state->winner)->getData()->name <<" con:" <<endl;
        if(state->winning)
            cout <<"    " <<state->winning->getShortName() <<endl;
        else
            cout <<" nessuna carta (tocca a lui)" <<endl;
    }else
        cout <<" giocatori, non prende nessuno." <<endl;
}

/** Message: print the information in a player_t
 * @param data      data to print
 * @param role      name of the player's role
 */
void msg_printData(player_t *data,string role)
{
    cout <<endl <<data->name;
    if(data->brainLevel)
        cout <<" (AI lv. " <<data->brainLevel <<")" <<endl;
    else
        cout <<" (giocatore umano)" <<endl;

    cout <<"   ruolo: " <<role <<", da " <<data->time <<" partite, e' salito di " <<data->advancement <<endl;
}

/** Message: player who can the first card
 * @param random    true if the player has been chosen randomly
 * @param player    true if the player is the user
 * @param name      player's name
 */
void msg_startFirst(bool random, bool player,string name)
{
    cout <<endl;
    if(random && player)    cout <<name <<" ha finito per primo di sistemarsi le carte, quindi comincia a giocare.";
    if(random && !player)   cout <<"Hai finito per primo di sistemarti le carte, quindi cominci tu.";
    if(!random && player)   cout <<"Essendo Lo Re, comincia " <<name;
    if(!random && !player)  cout <<"Essendo Lo Re, quindi cominci tu.";
    cout <<endl;
}

/** Message: nobody can play
 * @param name      player's name
 */
void msg_CardWin(string name)
{
    if(name.compare(""))
        cout <<"Nessuno puo' superare; tocca a " <<name <<endl;
    else
        cout <<"Nessuno puo' superare; tocca a te!" <<endl;
}

/** Message: player will not play
 * @param name      player's name
 */
void msg_playNothing(string name){  cout <<name <<" passa.\n";  }

/** Message: the players play this card
 * @param c         Card played
 */
void msg_playThis(Card *c){ cout <<" >" <<c->getShortName() <<endl; }

/** Message: player finished his cards and won a Role
 * @param name          player's name
 * @param role          Role number
 * @param roleChanged   true if he was not already this Role
 */
void msg_newRole(string name,int role,bool roleChanged)
{
    if(role==R_SLAVE)
        cout <<" -------->E' rimasto solo " <<name <<", che si becca il ruolo di schiavo!!" <<endl;
    else{
        cout <<" -------->" <<name <<" ha finito le carte!" <<endl;
        cout <<" -------->Ha vinto il ruolo di ";
        switch(role)
        {
            case R_KING:        cout <<"Lo Re!!\n";
                                if(roleChanged)
                                    cout <<" ---------->Lo Re e' stato spodestato!!";
                                                        break;
            case R_KNIGHT:      cout <<"Cavaliere!!\n"; break;
            case R_THIEF:       cout <<"Ladro!!\n";     break;
            case R_MERCHANT:    cout <<"Mercante!!\n";  break;
            case R_CHICKEN:     cout <<"Pollo!!\n";     break;
            case R_BITCH:       cout <<"Puttana!!\n";   break;
            default: cerr <<"Error - role unknown: " <<role <<endl;
        };
    }
}

/** Message: print the title */
void msg_title()
{
    cout <<" |\\  /\\  /|\n";
    cout <<" | \\/  \\/ |\n";
    #ifdef _WIN32
    char suit[4] = {3, 4,5,6}; //WINDOWS
    cout <<" |" <<suit[0] <<" " <<suit[1]  <<"  " <<suit[2] <<" " <<suit[3] <<"|\n"; //WINDOWS
    #else
    cout <<" |♥ ♦  ♣ ♠|\n"; //LINUX
    #endif
    cout <<" |________|\n";
    cout <<"Lo Re Project\n";
    cout <<" by Raptor\n";
}

/** Message: print the rules of Lo Re */
void msg_rules()
{
    cout <<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    cout <<"          SCOPO DEL GIUOCO\n";
    cout <<" Bisogna finire le carte; in base all'ordine con cui si finiscono le carte\n";
    cout <<"sono assegnati dei ruoli per la partita successiva.\n\n";
    waitForEnter();
    cout <<"------------------------------------------------------------------------------\n";
    cout <<"          LE CARTE\n";
    cout <<" Si gioca con un mazzo di carte normali (francesi) senza jolly;\n";
    cout <<"all'inizio della partita tutte le carte vengono distribuite fra i giocatori,\n";
    cout <<"quindi il numero di carte iniziali dipende dal numero totale di giocatori.\n";
    cout <<" L'ordine e': 3 - 4 - 5 - 6 - 7 - 8 - 9 - 10 - Fante - Donna - Re - asso - 2\n";
    cout <<"la carta piu' forte e' il 2.\n\n";
    waitForEnter();
    cout <<"------------------------------------------------------------------------------\n";
    cout <<"          I RUOLI\n";
    cout <<"I ruoli (assegnati dalla prima partita) sono:\n";
    cout <<" -Lo Re  (da' le sue 2 carte piu' basse allo Schiavo)\n";
    cout <<" -Il Cavaliere  (da' la sua carta piu' bassa alla Puttana)\n";
    cout <<" -Il Ladro  (da' la sua carta peggiore alla Puttana)\n";
    cout <<" -Il Mercante  (puo' scambiare una carta a scelta con un altro giocatore)\n";
    cout <<" -Il Pollo  (da' una carta a caso al Ladro)\n";
    cout <<" -La Puttana  (da' la sua carta piu' alta al cavaliere)\n";
    cout <<" -Lo Schiavo  (da' le sue 2 carte piu' alte a Lo Re)\n\n";

    cout <<" I ruoli determinano anche chi butta per primo e il senso del giro;\n";
    cout <<"comincia Lo Re, e si prosegue nel verso del piu' alto in grado\n";
    cout <<"fra i due giocatori a destra e a sinistra de Lo Re.\n\n";
    waitForEnter();
    cout <<"------------------------------------------------------------------------------\n";
    cout <<"          IL GIOCO\n";
    cout <<" Bisogna necessariamente superare il valore della/e carta/e che sono state\n";
    cout <<"buttate; bisogna mantenere o aumentare il numero di carte.\n";
    cout <<"Es:                Pippo butta Fante x2.\n";
    cout <<"        Chi viene dopo potrebbe giocare Donna x2, Asso x4, 2 x3...\n";
    cout <<"       se non ha almeno una coppia di carte di valore superiore al Fante,\n";
    cout <<"       o preferisce non buttare, allora puo' passare.\n";
    cout <<" Se nessuno puo' superare, chi ha buttato le carte vincenti puo' ricominciare\n";
    cout <<"il giro buttando le carte che preferisce; ma se il giocatore vincente\n";
    cout <<"ha finito le carte, tocca a chi ha il ruolo piu' alto.\n\n";
    waitForEnter();
    cout <<"------------------------------------------------------------------------------\n";
    cout <<"          FUNZIONALITA' DEL PROGRAMMA\n";
    cout <<" Ogni volta che tocca all'utente, si puo' scegliere quali carte buttare\n";
    cout <<"(eventualmente anche 'spezzando' coppie, tris e poker), se passare (scrivi 0)\n";
    cout <<"o chiedere un suggerimento (scrivi 666).\n";
    cout <<"In caso la carta selezionata non sia giocabile ne viene chiesta un'altra.\n";
    cout <<" Alla fine di ogni partita vengono visualizzati i ruoli, da quanto tempo\n";
    cout <<"i giocatori hanno quel ruolo, e di quante posizioni sono saliti/scesi\n";
    cout <<"rispetto alla prima partita.\n";
    cout <<" Vi e' la possibilità di salvare e caricare partite (contenenti i dati\n";
    cout <<"e i ruoli dei giocatori), o creare una nuova partita con n giocatori\n";
    cout <<"(n compreso fra 3 e 7); in questo caso vengono chiesti i nomi dei giocatori\n";
    cout <<"e il livello di intelligenza degli avversari dell'utente.\n";
    cout <<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
    waitForEnter();
}

/** Message: print a line */
void msg_line()
{
    cout <<"-------------------------------------------------\n";
}
