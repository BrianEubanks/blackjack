#ifndef display_h
#define display_h

#include "rules.h"

void initializeDisplay();
void clearScreen();
void closeDisplay();

char getInput();

void printHands(hand dealerhand, hand playerhand);
void printHandsFinal(hand dealerhand, hand playerhand);

#endif
