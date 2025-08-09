//#include <SDL2/SDL.h>
#include <SDL.h>

#include <stdbool.h>

#include "display.h"




static char inputchar;

static SDL_Window* window; 
static SDL_Renderer *renderer;

const uint8_t sprites[13][8][8] = {
    {{0,0,0,1,1,0,0,0},
     {0,0,1,1,1,1,0,0},
     {0,0,1,1,0,1,1,0},
     {0,1,1,1,1,1,1,0},
     {0,1,1,0,0,0,1,0},
     {0,1,1,0,0,0,1,0},
     {0,1,1,0,0,0,1,0},
     {1,1,0,0,0,0,1,1}},
    {{0,0,0,1,1,0,0,0},
     {0,0,1,1,1,1,0,0},
     {0,0,1,0,0,1,1,0},
     {0,1,0,0,1,1,1,0},
     {0,0,0,1,1,0,0,0},
     {0,0,0,1,0,0,0,0},
     {0,0,1,0,0,0,0,0},
     {0,1,1,1,1,1,1,1}},
    {{0,0,0,1,1,0,0,0},
     {0,0,1,1,1,1,1,0},
     {0,1,0,0,1,1,0,0},
     {0,0,0,1,1,1,0,0},
     {0,0,0,1,1,1,0,0},
     {0,1,0,0,0,1,1,0},
     {0,0,1,1,1,1,0,0},
     {0,0,1,1,1,1,0,0}},
    {{0,1,1,0,0,1,1,0},
     {0,1,1,0,0,1,1,0},
     {0,1,1,0,0,1,1,0},
     {0,1,1,1,1,1,1,0},
     {0,0,0,0,0,1,1,0},
     {0,0,0,0,0,1,1,0},
     {0,0,0,0,0,1,1,0},
     {0,0,0,0,0,1,1,0}},
    {{0,1,1,1,1,1,1,0},
     {0,1,1,1,1,1,1,0},
     {0,1,1,0,0,0,0,0},
     {0,1,1,1,1,0,0,0},
     {0,0,0,0,1,1,0,0},
     {0,0,0,0,0,1,0,0},
     {0,1,0,0,1,1,0,0},
     {0,1,1,1,1,0,0,0}},
    {{0,0,0,1,1,1,0,0},
     {0,0,1,1,1,0,0,0},
     {0,0,1,1,0,0,0,0},
     {0,1,1,1,1,1,1,0},
     {0,1,1,0,0,0,1,0},
     {0,1,1,0,0,0,1,0},
     {0,1,1,1,0,1,1,0},
     {0,0,1,1,1,1,0,0}},
    {{0,0,1,1,1,1,1,0},
     {0,1,1,1,1,1,1,0},
     {0,0,0,0,1,1,0,0},
     {0,0,0,0,1,1,0,0},
     {0,0,0,1,1,0,0,0},
     {0,0,0,1,1,0,0,0},
     {0,0,0,1,1,0,0,0},
     {0,0,0,1,1,0,0,0}},
    {{0,0,0,1,1,0,0,0},
     {0,0,1,0,0,1,0,0},
     {0,1,0,0,0,1,0,0},
     {0,0,1,1,1,1,0,0},
     {0,0,1,0,0,0,1,0},
     {0,1,1,0,0,0,1,0},
     {0,1,1,0,0,1,1,0},
     {0,0,1,1,1,1,0,0}},
    {{0,0,1,1,1,1,0,0},
     {0,0,1,0,0,1,1,0},
     {0,0,1,0,0,1,1,0},
     {0,0,1,0,0,1,1,0},
     {0,0,0,1,1,1,1,0},
     {0,0,0,0,0,1,1,0},
     {0,0,0,0,0,1,1,0},
     {0,0,0,0,0,1,1,0}},
    {{1,0,0,1,1,0,0,0},
     {1,1,0,0,0,1,1,0},
     {1,1,0,0,1,1,0,1},
     {1,1,0,0,1,0,0,1},
     {1,1,0,0,1,0,0,1},
     {1,1,0,0,1,0,0,1},
     {1,1,0,0,1,1,0,1},
     {1,1,1,0,0,1,1,0}},
    {{0,1,1,1,1,1,1,0},
     {0,1,1,1,1,1,1,0},
     {0,0,0,0,1,1,0,0},
     {0,0,0,0,1,1,0,0},
     {0,1,0,0,1,1,0,0},
     {0,1,1,0,1,1,0,0},
     {0,1,1,0,1,1,0,0},
     {0,1,1,1,1,1,0,0}},
    {{0,0,1,1,1,0,0,0},
     {0,1,1,1,1,1,0,0},
     {0,1,1,0,1,1,0,0},
     {1,1,0,0,0,1,1,0},
     {1,1,0,0,0,1,1,0},
     {1,1,0,0,1,1,0,0},
     {0,1,1,0,1,1,1,0},
     {0,0,1,1,0,0,1,1}},
    {{1,1,0,1,1,0,0,0},
     {1,1,0,1,1,0,0,0},
     {1,1,0,1,1,0,0,0},
     {1,1,1,1,0,0,0,0},
     {1,1,1,1,0,0,0,0},
     {1,1,0,1,1,0,0,0},
     {1,1,0,1,1,0,0,0},
     {1,1,0,0,1,1,0,0}}
    };

const uint8_t control[6][8][8] = {
    {{0,1,1,0,0,1,1,0},
     {0,1,1,0,0,1,1,0},
     {0,1,1,0,0,1,1,0},
     {0,1,1,1,1,1,1,0},
     {0,1,1,1,1,1,1,0},
     {0,1,1,0,0,1,1,0},
     {0,1,1,0,0,1,1,0},
     {0,1,1,0,0,1,1,0}},
    {{0,0,0,1,1,1,0,0},
     {0,0,1,1,0,0,1,0},
     {0,1,1,0,0,0,1,0},
     {0,0,1,0,0,0,0,0},
     {0,0,0,1,1,1,0,0},
     {0,0,0,0,0,1,1,0},
     {0,0,1,0,0,1,1,0},
     {0,0,1,1,1,1,1,0}},
    {{0,1,1,1,0,0,0,0},
     {0,1,1,0,1,1,0,0},
     {0,1,1,0,0,1,0,0},
     {0,1,1,0,0,1,0,0},
     {0,1,1,0,0,1,0,0},
     {0,1,1,0,0,1,0,0},
     {0,1,1,0,0,1,0,0},
     {0,1,1,1,1,0,0,0}},
    {{0,1,1,0,0,1,1,0},
     {0,1,1,0,0,1,1,0},
     {0,0,1,0,1,1,0,0},
     {0,0,0,1,1,0,0,0},
     {0,0,0,1,1,0,0,0},
     {0,0,1,1,1,1,0,0},
     {0,1,1,0,0,1,1,0},
     {0,1,1,0,0,1,1,0}},
    {{0,0,1,1,1,0,0,0},
     {0,1,1,1,1,1,0,0},
     {0,1,1,0,1,1,0,0},
     {1,1,0,0,0,1,1,0},
     {1,1,0,0,0,1,1,0},
     {1,1,0,0,1,1,0,0},
     {0,1,1,0,1,1,1,0},
     {0,0,1,1,0,0,1,1}},
    {{0,0,0,1,1,0,0,0},
     {0,0,1,1,1,1,1,0},
     {0,1,0,0,0,0,1,0},
     {0,1,0,0,0,0,0,0},
     {0,1,0,0,0,0,0,0},
     {0,1,0,0,0,0,1,0},
     {0,1,1,0,0,1,1,0},
     {0,0,0,1,1,1,0,0}}
    };

char getInput(){


    SDL_Event windowEvent;
    bool running =  true;

    while (running) {

	// Poll Input
        if (SDL_PollEvent(&windowEvent)){
            
	    switch (windowEvent.type){
	    case SDL_QUIT:
               exit(0);
               break; 
	    case SDL_KEYDOWN:
		printf( "Scancode: 0x%02X\n", windowEvent.key.keysym.scancode );
		switch (windowEvent.key.keysym.scancode){
		case 0x1A: // w up
		    inputchar = 'w';
		    break;
		case 0x16: // s down
		    inputchar = 's';
		    running = false;
		    break;
		case 0x04: // a left
		    inputchar = 'a';
		    break;
		case 0x07: // d right
		    inputchar = 'd';
		    running = false;
		    break;
		case 0x0B: // h hit
		    inputchar = 'h';
		    running  = false;
		    break;
		case 0x14: // q
		    inputchar = 'q';
		    running = false;
		    break;
		case 0x1B: // x
		    inputchar = 'x';
		    running = false;
		    break;
		default:
		    break;
		}
	    } // PollEvent
	}// Poll Event
    }// while running
    return inputchar;
}


void initializeDisplay(){

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
	exit(-1);
    }
    window = SDL_CreateWindow("gamblin",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 500,500,0);

    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);      // Pointer for the renderer


    // Show the change on the screen
    SDL_RenderPresent(renderer);    



}

void showGameOverPrompt(){
    printf("[d][c][q]\n");
}

void showGamePrompt(bool canDouble, bool canSplit){
    int numPrompts = 2;
    int control_x = 50;
    int control_y = 300;  
    
    printf("[h][s]");
    if (canDouble){
        printf("[d]");
	numPrompts++;
        if (canSplit){
            printf("[x]");
	    numPrompts++;
	}
    }
    printf("\n");


    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    
    for (int p = 0; p < numPrompts; p++){    

	for (int x = 0; x < 16; x++){ 
	    for (int y = 0; y < 16; y++){
		if (control[p][y/2][x/2] == 1){
		    SDL_RenderDrawPoint(renderer, x+control_x, y+control_y);
		}
	    }
	}

	control_x += 50;

    }


    SDL_RenderPresent(renderer);
}

void showRunningCount(int count){
    printf("running count: %d\n",count);
}


void clearScreen(){
    // Setting the color to be GREEN with 100% opaque (0% trasparent).
    SDL_SetRenderDrawColor(renderer, 0, 104, 1, 255);
    SDL_RenderClear(renderer);
}

void printHands(hand* dealerhand, hand* playerhand){

    int d_card_x = 300;
    int d_card_y = 50;
    int p_card_x = 50;
    int p_card_y = 50;

    int card_w = 75;
    int card_h = 100;

    
    int card_x = d_card_x;
    int card_y = d_card_y;    
    int cards = dealerhand->num;
    int card;
    int cardsp;

    //
    // Draw Dealer
    //
    for (int c = 0; c < cards; c++){
	card_x += 25;
	card_y += 25;
	for (int x = card_x; x <= card_x + card_w; x++){
	    for (int y = card_y; y <= card_y + card_h; y++){
		if (x == card_x || y == card_y || 
		    x == card_x + card_w || y == card_y + card_h) {
		    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		} else {
		    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
		SDL_RenderDrawPoint(renderer, x, y);
	    }
	}

	if (c == 0){
	    continue;
	}
        card = dealerhand->cards[c];
        cardsp = card%13;
        for (int x = 0; x < 16; x++){
            printf("\n");
            for (int y = 0; y < 16; y++){
                //printf("%d",sprites[cardsp][x/2][y/2]);
                if (sprites[cardsp][y/2][x/2] == 1){
                    if (card > 25){
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    } else {
                        SDL_SetRenderDrawColor(renderer, 185, 0, 0, 255);
                    }
                    SDL_RenderDrawPoint(renderer, x+card_x+5, y+card_y+5);
                }
            }
        }		
    }


    //
    // Draw player 
    //
    card_x = p_card_x;
    card_y = p_card_y;
    cards = playerhand->num;
    for (int c = 0; c < cards; c++){
	card_x += 25;
	card_y += 25;
	for (int x = card_x; x <= card_x + card_w; x++){
	    for (int y = card_y; y <= card_y + card_h; y++){
		if (x == card_x || y == card_y || 
		    x == card_x + card_w || y == card_y + card_h) {
		    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		} else {
		    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
		SDL_RenderDrawPoint(renderer, x, y);
	    }
	}
	card = playerhand->cards[c];
	cardsp = card%13;
	for (int x = 0; x < 16; x++){
	    printf("\n");
	    for (int y = 0; y < 16; y++){
		//printf("%d",sprites[cardsp][x/2][y/2]);
		if (sprites[cardsp][y/2][x/2] == 1){
		    if (card > 25){
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		    } else {
			SDL_SetRenderDrawColor(renderer, 185, 0, 0, 255);
		    }
		    SDL_RenderDrawPoint(renderer, x+card_x+5, y+card_y+5);
		}
	    }
	}
    }

    // Show the change on the screen
    SDL_RenderPresent(renderer);

}
    

void printHandsFinal(hand* dealerhand, hand* playerhand){
    printHands(dealerhand,playerhand);
    
    int card;
    int cardsp;
    int card_x = 300;
    int card_y = 50;

    card_x+=25;
    card_y+=25;

    //
    // Draw Dealer Hole Card
    //
    card = dealerhand->cards[0];
    cardsp = card%13;
    if (cardsp > 9){
        cardsp = 9;
    }
    for (int x = 0; x < 16; x++){
        printf("\n");
        for (int y = 0; y < 16; y++){
            //printf("%d",sprites[cardsp][x/2][y/2]);
            if (sprites[cardsp][y/2][x/2] == 1){
                if (card > 25){
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                } else {
                    SDL_SetRenderDrawColor(renderer, 185, 0, 0, 255);
                }
                SDL_RenderDrawPoint(renderer, x+card_x+5, y+card_y+5);
            }
	}
    }
    // Show the change on the screen
    SDL_RenderPresent(renderer);
}
