#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "rules.h"

#include "deck.h"
#include "display.h"

typedef enum move {
    Hit = 0,
    Stand = 1,
    Double = 2,
    Split = 3,
    Surr = 4,
    max = 255,
} move;

//
// S17 Basic Strategy
//
static const move hard_table[17][10] = {
// Dealer Upcard
//A 2 3 4 5 6 7 8 9 T
 {0,0,0,0,0,0,0,0,0,0}, // 5 Player Hand 
 {0,0,0,0,0,0,0,0,0,0}, // 6
 {0,0,0,0,0,0,0,0,0,0}, // 7
 {0,0,0,0,0,0,0,0,0,0}, // 8
 {0,0,2,2,2,2,0,0,0,0}, // 9
 {0,2,2,2,2,2,2,2,2,0}, // 10
 {0,2,2,2,2,2,2,2,2,2}, // 11
 {0,0,0,1,1,1,0,0,0,0}, // 12
 {0,1,1,1,1,1,0,0,0,0}, // 13
 {0,1,1,1,1,1,0,0,0,0}, // 14
 {0,1,1,1,1,1,0,0,0,0}, // 15
 {0,1,1,1,1,1,0,0,0,0}, // 16
 {1,1,1,1,1,1,1,1,1,1}, // 17
 {1,1,1,1,1,1,1,1,1,1}, // 18
 {1,1,1,1,1,1,1,1,1,1}, // 19
 {1,1,1,1,1,1,1,1,1,1}, // 20
 {1,1,1,1,1,1,1,1,1,1}};// 21

static const move soft_table[8][10] = {
//A 2 3 4 5 6 7 8 9 T
 {0,0,0,0,2,2,0,0,0,0}, // A2 13
 {0,0,0,0,2,2,0,0,0,0}, // A3 14
 {0,0,0,2,2,2,0,0,0,0}, // A4 15
 {0,0,0,2,2,2,0,0,0,0}, // A5 16
 {0,0,2,2,2,2,0,0,0,0}, // A6 17
 {1,1,2,2,2,2,1,1,0,0}, // A7 18
 {1,1,1,1,1,1,1,1,1,1}, // A8 19
 {1,1,1,1,1,1,1,1,1,1}};// A9 20

static const bool split_table[10][10] = {
//A 2 3 4 5 6 7 8 9 T
 {0,1,1,1,1,1,1,0,0,0}, // 22 4
 {0,1,1,1,1,1,1,0,0,0}, // 33 6
 {0,0,0,0,1,1,0,0,0,0}, // 44 8
 {0,0,0,0,0,0,0,0,0,0}, // 55 10
 {0,1,1,1,1,1,0,0,0,0}, // 66 12
 {0,1,1,1,1,1,1,0,0,0}, // 77 14
 {1,1,1,1,1,1,1,1,1,1}, // 88 16
 {0,1,1,1,1,1,0,1,1,0}, // 99 18
 {0,0,0,0,0,0,0,0,0,0}, // TT 20
 {1,1,1,1,1,1,1,1,1,1}};// AA 13


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

void printTable(){
    printf("%ld\n",sizeof(hard_table));
    for (int i = 0; i < 17; i++){	
	for (int j = 0; j < 10; j++){
	    printf("%d ",hard_table[i][j]);
	}
	printf("\n");
    }
    printf("%ld\n",sizeof(soft_table));
    for (int i = 0; i < 8; i++){	
	for (int j = 0; j < 10; j++){
	    printf("%d ",soft_table[i][j]);
	}
	printf("\n");
    }
    printf("%ld\n",sizeof(split_table));
    for (int i = 0; i < 10; i++){	
	for (int j = 0; j < 10; j++){
	    printf("%d ",split_table[i][j]);
	}
	printf("\n");
    }

}

void checkBasicStrategy(char m, bool canDouble, bool canSplit){
    uint8_t upcard;
    uint8_t value;
    uint8_t soft;
    uint8_t index;
    move playermove;
    
    upcard = dealerhand.cards[0] % 13;
    if (upcard > 9){
	upcard = 9;
    }

    value = playerhand.value;
    soft = playerhand.softaces;

    switch (m){
	case 'h':
	    playermove = Hit;
	    break;
	case 's':
	    playermove = Stand;
	    break;
	case 'd':
	    playermove = Double;
	    break;
	case 'x':
	    playermove = Split;
	    break;
	default:
	    playermove = Surr;
    }   
 
    if (canSplit){

	index = value - 4;
	if (index != 9){
	    index /= 2;
	}

	if (split_table[index][upcard]){
	    if (playermove == Split){
		printf("correct\n");
	    } else {
		printf("incorrect\n");
	    }
	    return;
	} else {
	    if (playermove == 'x'){
		printf("incorrect\n");
	    }
	}
    }

    if (soft > 0){
	index = value-13;
	if (soft_table[index][upcard] == playermove){
	    printf("correct\n");    
	} else {
	    printf("incorrect\n");
	}
	return; 
    }

    index = value - 5;
    if (hard_table[index][upcard] == playermove){
	printf("correct\n");
    } else {
	printf("incorrect\n");
    }

    return;

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

    playerhand.softaces = 0;
    dealerhand.softaces = 0;

    playerhand.bet = 0;
    dealerhand.bet = 0;

    playerhand.status = 0;
    dealerhand.status = 0;

    clearScreen();
}


void hitHand(hand* h){    

    h->cards[h->num] = drawCard();
    if (h->cards[h->num] % 13 == 0){
	h->softaces++;
    }
    h->value += getCardValue(h->cards[h->num]);
    if (h->value > 21 && h->softaces > 0){
	h->value -= 10;
	h->softaces --;
    }
    h->num++;
    
    if (h->value == 21){
	h->status = Done;
    } else if (h->value > 21){
	h->status = Bust;
    }

}


void dealHands(){
    //burnCard();

    hitHand(&dealerhand);
    hitHand(&playerhand);
    hitHand(&dealerhand);
    hitHand(&playerhand);

    printHands(dealerhand,playerhand);

}
    
bool isPair(hand playerhand){
    if (playerhand.cards[0] % 13 == playerhand.cards[1] % 13){
	return true;
    }
    return false;
}

void playerTurn(){
    
    bool canDouble = true;
    bool canSplit = isPair(playerhand); // Rules

    if (playerhand.value == 21){
	playerhand.status = Done;
	return;
    }
    
    do {

	showGamePrompt(canDouble, canSplit);	
	char a1 = getInput();

	checkBasicStrategy (a1,canDouble,canSplit);

    	if (a1 == 's'){
	    playerhand.status = Done;
	    break;
	} else if (a1 == 'd' && canDouble){
	    
	    hitHand(&playerhand);
	    playerhand.bet *= 2;
	    if (playerhand.status == InPlay){
		playerhand.status = Done;
	    }	
	    printHands(dealerhand,playerhand);
	    break;
	} else if (a1 == 'x' && canSplit){
	    //
	    // Split Allocate another hand and do split routine
	    //
	    printf("split not implemented yet\n");
	} else if (a1 == 'h'){
	    hitHand(&playerhand);
	    printHands(dealerhand,playerhand); 
	    if (playerhand.status != InPlay){
		break;
	    }
	} else {
	    continue;
	}
	canDouble = false;
	canSplit = false;
    
    } while (true);

    
}

void dealerTurn(){

    if (playerhand.status == Bust){
	return;
    }

    while (dealerhand.value < 17){ // S17
	hitHand(&dealerhand);	
    }
    
}


void scoreHands(){
    if (playerhand.status == Bust){
	dealerwincount++;
	return;
    }

    if (dealerhand.status == Bust){
	playerwincount++;
	return;
    }

    if (playerhand.value > dealerhand.value){
	playerwincount++;
    } else if (dealerhand.value > playerhand.value){
	dealerwincount++;
    } else {
	tiecount++;
    }

    return;
}

void playHand(){ 

    clearHands();

    // Bet
    dealHands();

    playerTurn();

    dealerTurn();

    scoreHands();

}
    

void playGame(){

    char p = '\0';

    clearDeck();

    
    while (true){

	playHand();
	
	printHandsFinal(dealerhand, playerhand);	
 
	do {
	    
	    showGameOverPrompt();

	    p = getInput();

	    if (p == 'c'){
		showRunningCount(getRunningCount());
	    }

	} while (p != 'd' && p != 'q');

	if (p == 'q'){
	    break;
	}

    }

	
    
	    
    printf("Stats:\n");
    printf(" Wins: %d\n",playerwincount);
    printf(" Loss: %d\n",dealerwincount);
    printf(" Push: %d\n",tiecount);

}


int main(int argc, char** argv){

    srand(time(NULL));   // Initialization, should only be called once.

    //printTable();

    initializeDisplay();
    playGame();   

}
