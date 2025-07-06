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
    uint8_t 	softaces;
    uint8_t 	bet;
    handstatus 	status;
} hand;

//
// Hands
//
hand dealerhand;
hand playerhand;




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
    printf("\nDealer: %s *", cardstr[dealerhand.cards[0]]);
    
    printf("\nPlayer:");
    for (int i = 0; i < playerhand.num; i++){
	printf(" %s", cardstr[playerhand.cards[i]]);
    }   
    printf("\n");
}

void printHandsFinal(){
    printf("\nDealer:");
    for (int i = 0; i < dealerhand.num; i++){
	printf(" %s", cardstr[dealerhand.cards[i]]);
    }
    printf("\nPlayer:");
    for (int i = 0; i < playerhand.num; i++){
	printf(" %s", cardstr[playerhand.cards[i]]);
    }   
    printf("\n");
}

void clearHands(){
    for (int i = 0; i < HAND_SIZE; i++){
	dealerhand.cards[i] = 0;
	playerhand.cards[i] = 0;
    }
    dealerhand.num = 0;
    playerhand.num = 0;

    dealerhand.value = 0;
    playerhand.value = 0;
}


uint8_t dealerHit(){    
    uint8_t ret = 0;

    dealerhand.cards[dealerhand.num] = drawCard();
    if (dealerhand.cards[dealerhand.num] % 13 == 0){
	dealerhand.softaces++;
    }
    dealerhand.value += getCardValue(dealerhand.cards[dealerhand.num]);
    if (dealerhand.value > 21 && dealerhand.softaces > 0){
	dealerhand.value -= 10;
	dealerhand.softaces --;
    }
    dealerhand.num++;
    
    if (dealerhand.value == 21){
	ret = 1;
    } else if (dealerhand.value > 21){
	ret = 2;
    }

    return ret;    
}

uint8_t playerHit(){    
    uint8_t ret = 0;

    playerhand.cards[playerhand.num] = drawCard();
    if (playerhand.cards[playerhand.num] % 13 == 0){
	playerhand.softaces++;
    }
    playerhand.value += getCardValue(playerhand.cards[playerhand.num]);
    if (playerhand.value > 21 && playerhand.softaces > 0){
	playerhand.value -= 10;
	playerhand.softaces --;
    }
    playerhand.num++;
    
    if (playerhand.value == 21){
	ret = 1;
    } else if (playerhand.value > 21){
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
    if (playerhand.value == 21){
	handstate = 1;
    } 
}
    
void playerTurn(){
    uint8_t ret = 0;
    
    do {
	fflush(stdin);
	printf("h or s: ");

	char a1 = getchar();


    	if (a1 == 's'){
	    break;
	} else if (a1 == 'd' && playerhand.num == 2){
	    
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



    
    } while (true);

    
    handstate = ret;    
}

void dealerTurn(){
    uint8_t ret = 0;

    while (dealerhand.value < 17){
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
