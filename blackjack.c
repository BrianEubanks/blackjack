#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "rules.h"

#include "deck.h"
#include "display.h"


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
}


void dealerHit(){    

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
	dealerhand.status = Done;
    } else if (dealerhand.value > 21){
	dealerhand.status = Bust;
    }

}

void playerHit(){    

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
	playerhand.status = Done;
    } else if (playerhand.value > 21){
	playerhand.status = Bust;
    }
    
}

void dealHands(){
    //burnCard();

    playerHit();
    playerHit();
    dealerHit();
    dealerHit();

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
    bool canSplit = isPair(playerhand);

    if (playerhand.value == 21){
	playerhand.status = Done;
	return;
    }
    
    do {
	fflush(stdin);
	printf("[h][s]");
	if (canDouble){
	    printf("[d]");
	    if (canSplit){
		printf("[x]");
	    }
	}
	printf("\n");
	char a1 = getchar();

    	if (a1 == 's'){
	    playerhand.status = Done;
	    break;
	} else if (a1 == 'd' && canDouble){
	    playerHit();
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
	} else if (a1 =='h'){
	    playerHit();
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

    while (dealerhand.value < 17){
	dealerHit();
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
	    fflush(stdin);
	    printf("[d][c][q]\n");	
	    p = getchar();

	    //if (p == 'c'){
		printf("running count: %d\n",getRunningCount());
	    //}
	} while (p != 'd' && p != 'q');

	if (p == 'q'){
	    break;
	}

    }

	
    printf("\ndone:%c\n",p);	
	    
    printf("Stats:\n");
    printf(" Wins: %d\n",playerwincount);
    printf(" Loss: %d\n",dealerwincount);
    printf(" Push: %d\n",tiecount);

}


int main(int argc, char** argv){

    srand(time(NULL));   // Initialization, should only be called once.

    playGame();   

}
