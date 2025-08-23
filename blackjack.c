#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "rules.h"

#include "deck.h"
#include "display.h"


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


uint64_t stackbase;

#define MAX_SPLIT 10

//
// Hands
//
hand dealerhand;
hand playerhands[MAX_SPLIT];
uint8_t handcount = 0;

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

void checkBasicStrategy(char m, int h, bool canDouble, bool canSplit){
    uint8_t upcard;
    uint8_t value;
    uint8_t soft;
    uint8_t index;
    move playermove = 0xFF;
    move bookmove = 0xFF;
    hand playerhand = playerhands[h];

    upcard = dealerhand.cards[1] % 13;
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
	    bookmove = Split;
	}
	
    } else {

	if (soft > 0 && bookmove == 0xFF){
	    index = value-13;
	    bookmove = soft_table[index][upcard];
	} else {
	    index = value - 5;
	    bookmove = hard_table[index][upcard];
	}
    }



    printf("playermove: %d\n",playermove);
    printf("bookmove: %d\n",bookmove);

    if (!canDouble && bookmove == Double){
	bookmove = Hit;
	printf("degradedouble: %d\n",bookmove);
    }

    printBasicStrategy(bookmove, playermove);

    return;

}

void clearHands(){

    // DealerHand
    for (int i = 0; i < HAND_SIZE; i++){
	dealerhand.cards[i] = 0;
    }
    dealerhand.num = 0;
    dealerhand.value = 0;
    dealerhand.softaces = 0;
    dealerhand.bet = 0;
    dealerhand.status = 0;

    //
    // PlayerHands
    //
    for (int h = 0; h < MAX_SPLIT; h++){
	for (int i = 0; i < HAND_SIZE; i++){
	    playerhands[h].cards[i] = 0;   
	}
	playerhands[h].num = 0;
	playerhands[h].value = 0;
	playerhands[h].softaces = 0;
	playerhands[h].bet = 0;
	playerhands[h].status = 0;
    }
    handcount = 1;

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

//
// Split hand[h] to hand[handcount]
//   Appends split to the first empty slot in playerhands array
// Increment handcount
//
void splitHands(int h){
    int a = h;
    int b = handcount;

    playerhands[b].cards[0] = playerhands[a].cards[1];
    playerhands[a].cards[1] = 0;

    playerhands[a].num = 1;
    playerhands[b].num = 1;

    playerhands[a].value /= 2;
    playerhands[b].value = playerhands[a].value;

    if (playerhands[a].softaces > 0){
	playerhands[a].softaces = 1;
	playerhands[b].softaces = 1;
    } else {	
	playerhands[a].softaces = 0;
	playerhands[b].softaces = 0;
    }
    
    playerhands[b].bet = playerhands[a].bet;

    playerhands[b].status = playerhands[a].status;

    handcount++;
}

void dealHands(){
    //burnCard();

    hitHand(&dealerhand);
    hitHand(&playerhands[0]);
    hitHand(&dealerhand);
    hitHand(&playerhands[0]);

    printHands(&dealerhand,&playerhands[0]);

}
    
bool isPair(int h){
    if (playerhands[h].cards[0] % 13 == playerhands[h].cards[1] % 13){
	return true;
    }
    return false;
}

void playHand(int h){
    
    bool canDouble = true;
    bool canSplit = isPair(h); // Rules

    if (playerhands[h].value == 21){
	playerhands[h].status = Done;
	return;
    }
    
    do {

	showGamePrompt(canDouble, canSplit);	
	char a1 = getInput();

	checkBasicStrategy (a1,h,canDouble,canSplit);

    	if (a1 == 's'){
	    playerhands[h].status = Done;
	    break;
	} else if (a1 == 'd' && canDouble){
	    
	    hitHand(&playerhands[h]);
	    playerhands[h].bet *= 2;
	    if (playerhands[h].status == InPlay){
		playerhands[h].status = Done;
	    }	
	    printHands(&dealerhand,&playerhands[h]);
	    break;
	} else if (a1 == 'x' && canSplit){
	    if (handcount >= MAX_SPLIT){
		printf("Too many splits\n");
		canSplit = false;
		continue;
	    } else {
		splitHands(h);
		continue;
	    }
	    
	} else if (a1 == 'h'){
	    hitHand(&playerhands[h]);
	    printHands(&dealerhand,&playerhands[h]); 
	    if (playerhands[h].status != InPlay){
		break;
	    }
	} else {
	    continue;
	}
	canDouble = false;
	canSplit = false;
    
    } while (true);
}

void playerTurn(){
    for (int h = 0; h < handcount; h++){
	playHand(h);
    }
}

void dealerTurn(){
    bool skip = true;

    for (int h = 0; h < handcount; h++){
	if (playerhands[h].status != Bust){
	    skip = false;
	    return;
	}
    }

    if (skip){
	return;
    }

    while (dealerhand.value < 17){ // S17
	hitHand(&dealerhand);	
    }
    
}


void scoreHand(int h){
    if (playerhands[h].status == Bust){
	dealerwincount++;
	return;
    }

    if (dealerhand.status == Bust){
	playerwincount++;
	return;
    }

    if (playerhands[h].value > dealerhand.value){
	playerwincount++;
    } else if (dealerhand.value > playerhands[h].value){
	dealerwincount++;
    } else {
	tiecount++;
    }

    return;
}

void scoreRound(){
    for (int h = 0; h < handcount; h++){
	scoreHand(h);
    }
}

void playRound(){ 

    clearHands();

    // Bet

    dealHands();

    playerTurn();

    dealerTurn();

    scoreRound();

}
    

void playGame(){

    char p = '\0';

    clearDeck();

    
    while (true){

	playRound();
	
	printHandsFinal(&dealerhand, (void*) &playerhands, handcount);	
 
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
    uint8_t stack1 = 0;
    stackbase = (uint64_t) &stack1;
    printf("Stackbase: 0x%llx\n",stackbase);

    srand(time(NULL));   // Initialization, should only be called once.

    //printTable();

    initializeDisplay();
    playGame();   

}
