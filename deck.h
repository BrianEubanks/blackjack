#include <stdint.h>

#include "rules.h"

//
// External Card Data and APIs
//
extern const uint8_t cards[DECK_SIZE];
extern const char* cardstr[DECK_SIZE];

void clearDeck();   

void burnCard();
    
uint8_t drawCard();


