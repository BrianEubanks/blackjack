#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

//#include <ncurses.h>

#define DECK_SIZE 52
#define HAND_SIZE 10
#define SHOE_SIZE 1

static uint8_t deck[DECK_SIZE];
static uint8_t cards[ ] = { 11,2,3,4,5,6,7,8,9,10,10,10,10,
                            11,2,3,4,5,6,7,8,9,10,10,10,10,
                            11,2,3,4,5,6,7,8,9,10,10,10,10,
                            11,2,3,4,5,6,7,8,9,10,10,10,10 };

static uint8_t cardsindeck;

static char* cardstr[] = {  "A","2","3","4","5","6","7","8","9","10","J","Q","K",
                            "A","2","3","4","5","6","7","8","9","10","J","Q","K",
                            "A","2","3","4","5","6","7","8","9","10","J","Q","K",
                            "A","2","3","4","5","6","7","8","9","10","J","Q","K" };

static uint8_t dealerhand[HAND_SIZE];
static uint8_t playerhand[HAND_SIZE];

static uint8_t dealercards;
static uint8_t playercards;

static uint8_t dealertotal;
static uint8_t playertotal;

static uint8_t dealersoft;
static uint8_t playersoft;

static uint8_t dealerwincount;
static uint8_t playerwincount;
static uint8_t tiecount;


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

void clearDeck() {
    for (int i = 0; i < DECK_SIZE; i++){
	deck[i] = 0;
    }
    cardsindeck = SHOE_SIZE * DECK_SIZE;
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
    
void playerTurn(uint8_t* state){
    uint8_t ret = 0;
    printf("h or s: ");
    char a1 = getchar();

    while (true){
	if (a1 == 's'){
	    break;
	} else if (a1 =='h'){
	    ret = playerHit();
	    if (ret == 2){
		break;
	    }
	}
	printHands(); 
	a1 = getchar();
    }
    printHands();
    *state = ret;    
}

void dealerTurn(uint8_t* state){
    uint8_t ret = 0;

    while (dealertotal < 17){
	ret = dealerHit();
	if (ret == 2){
	    break;
	}
    }
    printHandsFinal();
    *state = ret;    
}


bool playHand(){

    uint8_t state = 0;

    clearHands();

    // Bet

    dealHands();
    
    // Insurance

    // Split?

    // Dbl?

    playerTurn(&state);

    dealerTurn(&state);

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
