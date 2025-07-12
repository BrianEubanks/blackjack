#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "rules.h"

//
// Card Data
//
static const uint8_t cards[] = {11,2,3,4,5,6,7,8,9,10,10,10,10,
                                11,2,3,4,5,6,7,8,9,10,10,10,10,
                                11,2,3,4,5,6,7,8,9,10,10,10,10,
                                11,2,3,4,5,6,7,8,9,10,10,10,10 };

                              //A,2,3,4,5,6,7,8,9,10
static const int8_t hiopt1[] = {0,0,1,1,1,1,0,0,0,-1,-1,-1,-1};

			      //A,2,3,4,5,6,7,8,9,10
static const int8_t hiopt2[] = {0,1,1,2,2,1,1,0,0,-2,-2,-2,-2};


static int8_t runningcount;
static int8_t hiopt1count;
static int8_t hiopt2count;
static int8_t acecount;

static int8_t burncount;

static uint8_t deck[DECK_SIZE];
static uint8_t cardsindeck;
static uint8_t cardsdrawn;

void clearDeck() {
    for (int i = 0; i < DECK_SIZE; i++){
        deck[i] = 0; 
    }
    cardsindeck = SHOE_SIZE * DECK_SIZE;
    runningcount = 0;
}   

uint8_t getCardValue(uint8_t cardindex){
    return cards[cardindex];
}

void updateBurnCount(uint8_t c){
    uint8_t cardvalue = getCardValue(c);
    
    if (cardvalue >= 10){
	burncount--;
    } else if (cardvalue <= 6){
	burncount++;
    }
}
void burnCard(){
    uint8_t c = rand() % DECK_SIZE;
    while (deck[c] >=  SHOE_SIZE){
        c = rand() % DECK_SIZE;
    }
    deck[c] = SHOE_SIZE;
    cardsindeck--;
}

int8_t getRunningCount(){
    return runningcount;
}
   
void updateRunningCount(uint8_t c){
    uint8_t cardvalue = getCardValue(c);
    
    // hi lo running count
    if (cardvalue >= 10){
	runningcount--;
    } else if (cardvalue <= 6){
	runningcount++;
    }

    // aces
    if (cardvalue == 11){
	acecount++;
    }

    // hi opt running count
    hiopt1count += hiopt1[c];
    hiopt2count += hiopt2[c];
 
}

uint8_t drawCard(){
    uint8_t c = rand() % DECK_SIZE;
    uint8_t counter = 0;
    while (deck[c] >=  SHOE_SIZE){
        c = rand() % DECK_SIZE;
	counter++;
	if (counter >= DECK_SIZE * SHOE_SIZE){
	    printf("shoe_empty: %d %d",runningcount, burncount);
	    clearDeck();
	}
    }
    deck[c]++;
    cardsindeck--;
    updateRunningCount(c); // Counts the Dealer hole card
    return c;
}


