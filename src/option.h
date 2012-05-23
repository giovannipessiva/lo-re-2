#ifndef OPTION_H_
#define OPTION_H_

#include "Card.h"

/** \brief struct representing a Card and a value. */
typedef struct option
{
    Card *chosen;   ///< pointer to a Card object
    int grade;      ///< value assigned to this Card
} option_t;

#endif

