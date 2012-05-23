#include <iostream>
#include "../interface.h"
using namespace std;

void test_card();
void test_deck();
void test_choice();
void test_role();
void test_interface();
void test_game();

/* This set of functions has been useful for testing every method.
 * Now they are quite useless.
 * To activate them you need to de-comment "#define _TEST" in loRe2.h
 */
void test_all()
{
    cout <<"\n-----------------------{ T1 }-----------------------\n";
    test_card();
    cout <<"\n-----------------------{ T2 }-----------------------\n";
    test_deck();
    /*
    // UNTESTED:
    int getTotLevel(gameState_t *state);
    Card *getLowerPlayable(Card *winning);
    int countCheapCards();
    void setLevels(gameState *state);
    */
    cout <<"\n-----------------------{ T3 }-----------------------\n";
    test_choice();
    cout <<"\n-----------------------{ T4 }-----------------------\n";
    test_role();
    /*
    // UNTESTED:
    option_t *offer(Card **sale,int num);
    int exchange(Card **toGive);

    PRIVATE:
    int evaluateExchange(Card *given,Card *received);

    ONLY MERCHANT:
    option_t *offer(Card **sale,int num);
    Card *valueOffers(option_t **offers,int num,Card **sale);
    */
    cout <<"\n-----------------------{ T5 }-----------------------\n";
    test_interface();
    /*
    // UNTESTED:
    int selectCard(Card **offers,int num,bool shortName);
    int selectOffer(option_t **offers,int num,Card **requests,bool shortName);
    int strToInt(std::string s,int min,int max);
    player_t *readData(int n);
    */
    cout <<"\n-----------------------{ T6 }-----------------------\n";
    test_game();
    /*
    // UNTESTED:

    */
    waitForEnter();
}
