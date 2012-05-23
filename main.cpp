/*###########################################################################
  # Copyright (c) 2010 Giovanni Pessiva <giovanni.pessiva@gmail.com>        #
  # This program is free software: you can redistribute it and/or modify    #
  # it under the terms of the GNU General Public License version 3          #
  # as published by the Free Software Foundation.                           #
  ###########################################################################*/

#include <iostream>
#include "interface.h"
#include "Game.h"
#include "loRe2.h"
using namespace std;

void test_all();

/** Main function of Lo Re 2 */
int main(int argc,char** argv)
{
#ifdef _TEST
    test_all();
    return 0;

#else
    bool notFirst=false;
    int menuOption;
    Game *g=NULL;

    msg_title();

    do{
        menuOption=selectMenuOption(notFirst);

        switch(menuOption)
        {
            case -1: // Continua la partita
                    g->start();
                    notFirst=true;
                    break;

            case 0: // Salva la parita
                    g->save();
                    break;

            case 1: // Nuova partita
                    if(g) delete g;
                    g=new Game(selectPlayers());
                    g->start();
                    notFirst=true;
                    break;

            case 2: // Carica partita salvata
                    if(g) delete g;
                    g=new Game();
                    if(g->getN())
                    {
                        g->start();
                        notFirst=true;
                    }
                     break;

            case 3: // Leggi le regole
                    msg_rules();
                    break;

            case 4: if(g) delete g;
                    return 0;

            default: cerr <<"Invalid value from selectMenuOption(): " <<menuOption;
                    return -1;
        };
    }while(true);
#endif
}
