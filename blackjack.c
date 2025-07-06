#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "rules.h"

#include "deck.h"

typedef enum {
    InPlay,
    Done,
    Bust
} handstatus;

typedef struct _hand {
    uint8_t 	cards[HAND_SIZE];
    uint8_t 	num;
    uint8_t 	value;
    uint8_t 	bet;
    handstatus 	status;
} hand;

//
// Hands
//
static uint8_t dealerhand[HAND_SIZE];
static uint8_t playerhand[HAND_SIZE];

static uint8_t dealercards;
static uint8_t playercards;

static uint8_t dealertotal;
static uint8_t playertotal;

static uint8_t dealersoft;
static uint8_t playersoft;



//
// Game Stats
//
static uint8_t dealerwincount;
static uint8_t playerwincount;
static uint8_t tiecount;

//
// State Note
//
// 0  < 21 InPlay
// 1  = 21 Win
// 2  > 21 Bust
//
static uint8_t handstate;

void printHands(){
    printf("\nDealer: %s *", cardstr[dealerhand[0]]);
    //for (int i = 0; i < dealercards; i++){
    //printf(" %s", cardstr[dealerhand[i]]);
    //}
    printf("\nPlayer:");
    for (int i = 0; i < playercards; i++){
	printf(" %s", cardstr[playerhand[i]]);
    }   
    printf("\n");
}

void printHandsFinal(){
    printf("\nDealer:");
    for (int i = 0; i < dealercards; i++){
	printf(" %s", cardstr[dealerhand[i]]);
    }
    printf("\nPlayer:");
    for (int i = 0; i < playercards; i++){
	printf(" %s", cardstr[playerhand[i]]);
    }   
    printf("\n");
}

void clearHands(){
    for (int i = 0; i < HAND_SIZE; i++){
	dealerhand[i] = 0;
	playerhand[i] = 0;
    }
    dealercards = 0;
    playercards = 0;

    dealertotal = 0;
    playertotal = 0;
}


uint8_t dealerHit(){    
    uint8_t ret = 0;

    dealerhand[dealercards] = drawCard();
    if (dealerhand[dealercards] % 13 == 0){
	dealersoft++;
    }
    dealertotal += cards[dealerhand[dealercards]];
    if (dealertotal > 21 && dealersoft > 0){
	dealertotal -= 10;
	dealersoft --;
    }
    dealercards++;
    
    if (dealertotal == 21){
	ret = 1;
    } else if (dealertotal > 21){
	ret = 2;
    }

    return ret;    
}

uint8_t playerHit(){    
    uint8_t ret = 0;

    playerhand[playercards] = drawCard();
    if (playerhand[playercards] % 13 == 0){
	playersoft++;
    }
    playertotal += cards[playerhand[playercards]];
    if (playertotal > 21 && playersoft > 0){
	playertotal -= 10;
	playersoft --;
    }
    playercards++;
    
    if (playertotal == 21){
	ret = 1;
    } else if (playertotal > 21){
	ret = 2;
    }

    return ret;
}

void dealHands(){
    burnCard();

    playerHit();
    playerHit();
    dealerHit();
    dealerHit();

    printHands();

}

bool isStateInPlay(uint8_t* state){
    if (*state == 0){
	return true;
    }
    return false;
}

void checkPlayerBlackJack(){
    if (playertotal == 21){
	handstate = 1;
    } 
}
    
void playerTurn(){
    uint8_t ret = 0;
    

    printf("h or s: ");
    char a1 = getchar();

    while (true){
	if (a1 == 's'){
	    break;
	} else if (a1 == 'd' && playercards == 2){
	    
	    // only on 1st one
	    // bet
	    ret = playerHit();
	    printHands();
	    break;
	} else if (a1 == 'x'){
	    
	} else if (a1 =='h'){
	    ret = playerHit();
	    printHands(); 
	    if (ret == 2){
		break;
	    }
	}
	
	a1 = getchar();
    }
    
    handstate = ret;    
}

void dealerTurn(){
    uint8_t ret = 0;

    while (dealertotal < 17){
	ret = dealerHit();
	if (ret == 2){
	    break;
	}
    }
    //printHandsFinal();
    handstate = ret;    
}


bool playHand(){

    handstate = 0;

    clearHands();

    // Bet

    dealHands();

    // Player Blackjack
    checkPlayerBlackJack();
    
    // Insurance
    
    // Split?

    // Dbl?

    playerTurn();

    dealerTurn();

    return false;

}
    

void playGame(){

    char p = '\0';

    clearDeck();

    while(true){
	playHand();
	
	printHandsFinal();
    
	
	printf("deal or quit:");
	fflush(0);
	p = getchar();
	
	if (p != 'd'){
	    printf("\n*d%c\n",p);	
	    break;
	}

    }
}


int main(int argc, char** argv){

    srand(time(NULL));   // Initialization, should only be called once.

    playGame();   

}
