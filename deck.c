#include <stdint.h>
#include <stdlib.h>

#include "rules.h"

//
// Card Data
//
const uint8_t cards[] = {11,2,3,4,5,6,7,8,9,10,10,10,10,
                                11,2,3,4,5,6,7,8,9,10,10,10,10,
                                11,2,3,4,5,6,7,8,9,10,10,10,10,
                                11,2,3,4,5,6,7,8,9,10,10,10,10 };

const char* cardstr[] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K",
                                "A","2","3","4","5","6","7","8","9","10","J","Q","K",
                                "A","2","3","4","5","6","7","8","9","10","J","Q","K",
                                "A","2","3","4","5","6","7","8","9","10","J","Q","K" };

static uint8_t deck[DECK_SIZE];
static uint8_t cardsindeck;


void clearDeck() {
    for (int i = 0; i < DECK_SIZE; i++){
        deck[i] = 0; 
    }
    cardsindeck = SHOE_SIZE * DECK_SIZE;
}   

void burnCard(){
    uint8_t c = rand() % DECK_SIZE;
    while (deck[c] >=  SHOE_SIZE){
        c = rand() % DECK_SIZE;
    }
    deck[c] = SHOE_SIZE;
    cardsindeck--;
}       
    
uint8_t drawCard(){
    uint8_t c = rand() % DECK_SIZE;
    while (deck[c] >=  SHOE_SIZE){
        c = rand() % DECK_SIZE;
    }
    deck[c]++;
    cardsindeck--;
    return c;
}


