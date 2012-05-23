#ifndef LORE_H_
#define LORE_H_

//#define _TEST


#define HUMAN   0       // brainLevel for human player
#define AI_MAX  3       // brainLevel for the best AI player

#define NO_ROLE     7   // roleNumber for NoRole
#define R_KING      0   // roleNumber for King
#define R_KNIGHT    1   // roleNumber for Knight
#define R_THIEF     2   // roleNumber for Thief
#define R_MERCHANT  3   // roleNumber for Merchant
#define R_CHICKEN   4   // roleNumber for Chicken
#define R_BITCH     5   // roleNumber for Bitch
#define R_SLAVE     6   // roleNumber for Slave

#define ACE_LEV 15      // base value assigned to aces
#define TWO_LEV 17      // base value assigned to twos

// number of cards selectable in selectCard()
#define NO_CARDS    0   // no selection, just show the Deck
#define ONE_CARD    1   // select one card (usage: offers to Merchant)
#define FOUR_CARDS  2   // select one card, a couple, a tris or a poker (usage: card to play)
#define N_CARDS     3   // select as many cards as you want (usage: offers of the Merchant)

#define MAX_OFFERED_CARDS 4     // max number of cards that AI Merchant will offer

#define FILE_NAME "kinga.rpt"   // name of the file to save in/load from

#endif
