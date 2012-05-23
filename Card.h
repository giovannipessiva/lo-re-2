#ifndef CARD_H_
#define CARD_H_

#include <string>

/** \brief cards of different suits with the same value.
 *
 * A Card object can represent a single card, a couple, a tris, a poker.
 * A "level" can be assigned to this Card, to indicate how much it is worth.
 */
class Card
{
    bool m_hearts;
    bool m_diamonds;
    bool m_clubs;
    bool m_spades;
    int m_value;
    int m_level;
    std::string convertInt(int number);

    public:
    Card(bool hearts,bool diamonds,bool clubs,bool spades,int value);
    Card(Card *clone);
    int getValue();
    int getLevel();
    void setLevel(int level);
    bool isHearts();
    bool isDiamonds();
    bool isClubs();
    bool isSpades();
    int getCount();
    std::string getShortName();
    std::string getLongName();
    bool isPlayable(Card *winning);
    void absorb(Card *old);
    Card *divide(int n);
};

#endif
