#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <string>
#include "Deck.h"
#include "Choice.h"

// interface.cpp prototypes
int strToInt(std::string s,int min,int max);
int strToInt(std::string s,int min,int max,int extra);
int scanInt(int min, int max);
int scanInt(int min, int max, int extra);
void waitForEnter();

int selectPlayers();
Choice *selectCard(gameState_t *winning,Deck *hand,int max);
int selectCard(Choice *offers);
int selectOffer(option_t **offers,int num,Choice *requests);
int selectMenuOption(bool continua);
player_t *readData(int playerN);

void msg_noFile();
void msg_rejectCard();
void msg_merchOffer(bool heDidSome);
void msg_exchange(Card *request,Card *offer);
void msg_offer(Card *request,Card *offer,std::string name);
void msg_accept(Card *request,Card *offer);
void msg_printState(gameState_t *state);
void msg_printData(player_t *data,std::string role);
void msg_noOffers();
void msg_noOffers(std::string name);
void msg_title();
void msg_rules();
void msg_startGame();
void msg_startExchanges();
void msg_startFirst(bool random, bool player,std::string name);
void msg_CardWin(std::string name);
void msg_playNothing(std::string name);
void msg_playThis(Card *c);
void msg_hisTurn(std::string name,int cards);
void msg_newRole(std::string name,int role,bool roleChanged);
void msg_line();
#endif
