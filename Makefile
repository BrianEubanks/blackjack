stdout:
	clang -Ilib blackjack.c deck.c display/stdout.c

sdl:
	clang -Ilib blackjack.c deck.c display/sdl.c -I/opt/homebrew/include/SDL2 -L/opt/homebrew/lib -lSDL2

ncurses:
	rm ./a.out

clean:
	rm ./a.out
