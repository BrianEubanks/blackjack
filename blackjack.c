#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <time.h>
#include <ncurses.h>


#define DEALER_ROW 2

#define PLAYER_ROW 5

#define CMD_ROW 8

#define SCORE_ROW 15

#define DECK_SIZE 52
#define HAND_SIZE 12
#define SHOE_SIZE 1

static uint8_t deck[DECK_SIZE];
static uint8_t cards[ ] = { 11,2,3,4,5,6,7,8,9,10,10,10,10, 
			    11,2,3,4,5,6,7,8,9,10,10,10,10,  
			    11,2,3,4,5,6,7,8,9,10,10,10,10,
			    11,2,3,4,5,6,7,8,9,10,10,10,10 };

static char* cardstr[] = {  "A","2","3","4","5","6","7","8","9","10","J","Q","K",   
			    "A","2","3","4","5","6","7","8","9","10","J","Q","K",   
			    "A","2","3","4","5","6","7","8","9","10","J","Q","K",
			    "A","2","3","4","5","6","7","8","9","10","J","Q","K" };

static uint8_t dealerhand[HAND_SIZE];
static uint8_t playerhand[HAND_SIZE];

static uint8_t dealerScore;
static uint8_t playerScore;
static uint8_t tie;



void clearDeck(){
    for (int i = 0; i < DECK_SIZE; i++){
        deck[i] = 0;
    }
}



void clearHands(){
    for (int i = 0; i < HAND_SIZE; i++){
        dealerhand[i]=0;
        playerhand[i]=0;
    }

    move(PLAYER_ROW+1,0);
    printw("                                        ");

    move(DEALER_ROW+1,0);
    printw("                                        ");

}

bool isAce(int card){
    return (card % 13 == 0);
}

int drawCard(){
    int c = rand() % DECK_SIZE;
    int counter = 0;
    while (deck[c] >= SHOE_SIZE){
	c = rand() % DECK_SIZE;
	counter ++;
	if (counter > SHOE_SIZE * DECK_SIZE){
	    move(SCORE_ROW+4,0);
	    printw("SHOE Empty\n done\n");
	    refresh();
	    exit(-1);
	}
    }
    deck[c] += 1;
    return c;
}

void playHand(){
    char a;
    char a1;
    bool play = true;
    bool bust = false;
    bool blackjack = false;
    int softp = 0;
    int softd = 0;
    bool deal = true;
    int total = 0;
    int totald = 0;
    int c;
    int h = 0;
    int hd = 0;


    dealerScore=0;
    playerScore=0;
    tie = 0;

    while(deal) {

	clearHands(); 
	h = 0;
	hd = 0;
	total = 0;
	totald = 0;
	softp = 0;
	softd = 0;
    
	play = true;
	bust = false;
	blackjack = false;

    //
    // Deal Player
    //
    move(PLAYER_ROW,0);
    printw("PLAYER:          \n");
    c = drawCard();
    playerhand[h] = c;
    total += cards[c];
    if (isAce(c)){
	softp ++;
    }
    if (softp > 0 && total > 21){
	total -= 10;
	softp --;
    }
    h++;

    c = drawCard();
    playerhand[h] = c;
    total += cards[c];
    if (isAce(c)){
	softp ++;
    }
    if (softp > 0 && total > 21){
	total -= 10;
	softp --;
    }
    h++;


    //
    // Print Player Hand
    //
    for (int i = 0; i < h; i++){
        printw(" %s ",cardstr[playerhand[i]]);
    }
    

    refresh();

    //
    // Deal Dealer
    //
    move(DEALER_ROW,0);
    printw("DEALER:          \n");
    c = drawCard();
    dealerhand[hd] = c;
    totald += cards[c];
    if (isAce(c)){
	softd ++;
    }
    if (softd > 0 && totald > 21){
	totald -= 10;
	softd --;
    }
    hd++;

    c = drawCard();
    dealerhand[hd] = c;
    totald += cards[c];
    if (isAce(c)){
	softd ++;
    }
    if (softd > 0 && totald > 21){
	totald -= 10;
	softd --;
    }
    hd++;


    //
    // Print Dealer Hand
    //
    printw(" %s ",cardstr[dealerhand[0]]);
    printw(" X ");
    printw("\n");

    refresh();


    //
    // Check Dealer BlackJack
    //
    if (totald == 21){
	move(DEALER_ROW+1,0);
	for (int i = 0; i < hd; i++){
	    printw(" %s ",cardstr[dealerhand[i]]);
	}
	if (total == 21){
	    move(DEALER_ROW,0);
	    printw("DEALER: PUSH");
	    move(PLAYER_ROW,0);
	    printw("PLAYER: PUSH");
	    play = false;
	} else {
	    move(DEALER_ROW,0);
	    printw("DEALER:    BLACKJACK");
	    play = false;
	}
	blackjack = true;
	refresh();
    }
	

    if (total == 21){
	move(PLAYER_ROW,0);
	printw("PLAYER:    BLACKJACK");
	blackjack = true;
	play = false;
	refresh();
    }

    while (play){



	while(true){
	    move(CMD_ROW+1,0);
	    printw("[h]it/[s]tand: ");
	    refresh();
	    a1 = getch();

	    if (a1 == 's'){
		play = false;
		break;
	    } else if (a1 =='h'){
		c = drawCard();
		playerhand[h] = c;
		total += cards[c];
		if (isAce(c)){
		    softp ++;
		}
		if (softp > 0 && total > 21){
		    total -= 10;
		    softp --;
		}
		h++;
		break;

	    } else {
		//printw("h or s\n");
		continue;
	    }
        }

        //
        // Print Hand
        //
	move(PLAYER_ROW+1,0);
        for (int i = 0; i < h; i++){
            printw(" %s ",cardstr[playerhand[i]]);
        }
        printw("\n");


        //
        // Check Score
        //
        if (total > 21){
            play = false;
	    bust = true;
	}

        refresh();

    }

    move(PLAYER_ROW,0);
    printw("PLAYER: %d ", total);
    if (bust){
	printw("bust");
	dealerScore++;
    }

    //
    // Play Dealer
    //
    //
    // Print Dealer Hand
    //
    if (!bust && !blackjack){
	move(DEALER_ROW+1,0);
	for (int i = 0; i < hd; i++){
	    printw(" %s ",cardstr[dealerhand[i]]);
	}
	printw("\n");

	while(totald < 17){
	    c = drawCard();
	    dealerhand[hd] = c;
	    totald += cards[c];
	    if (isAce(c)){
		softd ++;
	    }
	    if (softd > 0 && totald > 21){
		totald -= 10;
		softd --;
	    }
	    hd++;
	    move(DEALER_ROW+1,0);
	    for (int i = 0; i < hd; i++){
		printw(" %s ",cardstr[dealerhand[i]]);
	    }
	    refresh();
	}
    
	//
	// Check Score
	//
	move(DEALER_ROW,0);
	printw("DEALER: %d ", totald);
	if (totald > 21){
	    printw("bust");
	    playerScore++;
	    bust = true;
	}

	refresh();

    }

    if (!bust){
	if (totald > total){
	    dealerScore++;
	} else if (totald < total){
	    playerScore++;
	} else {
	    tie++;
	}
    }

    //
    // Print Score
    //
    move(SCORE_ROW,0);
    printw("DEALER: %d\nPLAYER: %d\nTIE: %d\n",dealerScore,playerScore,tie);



    while(true){
	move(CMD_ROW+1,0);
	printw("[d]eal/[q]uit: ");
	refresh();
	a1 = getch();

	if (a1 == 'q'){
	    deal = false;
	} else if (a1 =='d'){
	    deal = true;
	} else {
	    continue;
	}
	break;
    }


    } // deal

    move(SCORE_ROW+4,0);
    printw("\ndone\n");
    refresh();


    getch();
   

}


int main() {
    initscr();
    raw();
    printw("Blackjack");
    refresh();

    srand(time(NULL));   // Initialization, should only be called once.

    clearDeck();
    playHand();

    endwin();

}
