#include <stdlib.h>
#include <stdio.h>

#include "display.h"

const char* cardstr[] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K",
                         "A","2","3","4","5","6","7","8","9","10","J","Q","K",
                         "A","2","3","4","5","6","7","8","9","10","J","Q","K",
                         "A","2","3","4","5","6","7","8","9","10","J","Q","K" };

void initializeDisplay(){
    return;
}

char getInput(){
    fflush(stdin);
    printf("\n");
    char a1 = getchar();
    return a1;
}

void printHands(hand dealerhand, hand playerhand){
    printf("\nDealer: %s *", cardstr[dealerhand.cards[0]]);

    printf("\nPlayer:");
    for (int i = 0; i < playerhand.num; i++){
        printf(" %s", cardstr[playerhand.cards[i]]);
    }
    printf("\n");
}

void printHandsFinal(hand dealerhand, hand playerhand){
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



