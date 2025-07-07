#ifndef rules_h
#define rules_h


#define DECK_SIZE 52
#define HAND_SIZE 10 // Charlie Win at 10 cards
#define SHOE_SIZE 1


//
// Hand Status
//   InPlay < 21
//   Done   = 21 or Stand
//   Bust   > 21
//
typedef enum {
    InPlay,
    Done,
    Bust
} handstatus;

//
// Hand Struct
//
// cards:       List of cards in the hand that have been dealt
// num:         Number of cards that have been dealt
// value:       Value of the hand
// softaces:    Number of softaces in the hand.
// bet:         Bet Value of the hand
// status:      Hand Status
//
typedef struct _hand {
    uint8_t     cards[HAND_SIZE];
    uint8_t     num;
    uint8_t     value;
    uint8_t     softaces;
    uint8_t     bet;
    handstatus  status;
} hand;

#endif
