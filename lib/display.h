#ifndef display_h
#define display_h

#include <stdbool.h>
#include "rules.h"

void initializeDisplay();
void clearScreen();
void closeDisplay();


void showGameOverPrompt();
char getInput();


void showGamePrompt(bool canDouble, bool canSplit);
void showRunningCount(int count);


void printHands(hand* dealerhand, hand* playerhand);
void printHandsFinal(hand* dealerhand, hand* playerhand);

#endif
