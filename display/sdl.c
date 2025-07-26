//#include <SDL2/SDL.h>
#include <SDL.h>

#include <stdbool.h>
#include <pthread.h>
#include "display.h"



static pthread_t displaythread;
static char inputchar;

static SDL_Window* window; 
static SDL_Renderer *renderer;




void displayloop (){


    SDL_Event windowEvent;
    bool running;

    while (running) {

	// Poll Input
        if (SDL_PollEvent(&windowEvent)){
            
	    switch (windowEvent.type){
	    case SDL_QUIT:
               running = false;
               break; 
	    case SDL_KEYDOWN:
		printf( "Scancode: 0x%02X\n", windowEvent.key.keysym.scancode );
		switch (windowEvent.key.keysym.scancode){
		case 0x1A: // w up
		    inputchar = 'w';
		    break;
		case 0x16: // s down
		    inputchar = 's';
		    break;
		case 0x04: // a left
		    inputchar = 'a';
		    break;
		case 0x07: // d right
		    inputchar = 'd';
		    break;
		case 0x0B: // h hit
		    inputchar = 'h';
		    break;
		default:
		    break;
		}
	    
		break;
	    }
	}


        // Show the change on the screen
        SDL_RenderPresent(renderer);


    }



}



char getInput(){
    
    bool done = false;
    displayloop();
    while(!done){
	switch (inputchar){
	case 'h':
	case 's':
	case 'd':
	case 'x':
	case 'q':
	case 'c':
	    done = true;
	    break;
	default:
	    break;
	}
    }
    return inputchar;
}
	    

void initializeDisplay(){

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
	exit(-1);
    }
    window = SDL_CreateWindow("gamblin",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 500,500,0);

    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);      // Pointer for the renderer


    



}

void printHands(hand dealerhand, hand playerhand){

    int d_card_x = 350;
    int d_card_y = 50;
    int p_card_x = 50;
    int p_card_y = 50;

    int card_w = 75;
    int card_h = 100;

    
    int card_x = d_card_x;
    int card_y = d_card_y;    
    int cards = dealerhand.num;

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
    }


    //
    // Draw player 
    //
    card_x = p_card_x;
    card_y = p_card_y;
    cards = playerhand.num;
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
    }

}
    

void printHandsFinal(hand dealerhand, hand playerhand){
    printHands(dealerhand,playerhand);
}
