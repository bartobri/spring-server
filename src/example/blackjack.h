#ifndef BLACKJACK_H
#define BLACKJACK_H 1

#include <string.h>
#include <stdlib.h>
#include <time.h>

#define HAND_CARD_MAX  7
#define DECK_CARD_MAX  32
#define HAND_MAX       4
#define DECK_MAX       6
#define SEAT_MAX       7
#define TABLE_MAX      10

// Declare function prototypes
void blackjack_shuffle_table(int);
void blackjack_init(void);

// Declare the entire blackjack game structure
struct card {
	int id;
	int highValue;
	int lowValue;
	char *display;
};

struct hand {
	struct card cards[HAND_CARD_MAX];
};

struct deck {
	struct card cards[DECK_CARD_MAX];
};

struct seat {
	int id;
	int socket;
	struct hand hands[HAND_MAX];
};

struct table {
	int id;
	struct seat seats[SEAT_MAX];
	struct deck decks[DECK_MAX];
	struct card dealerhand[HAND_CARD_MAX];
};

struct blackjack {
	struct table tables[TABLE_MAX];
};

// Function prototypes
void blackjack_init(void);
void blackjack_shuffle_table(int);
int blackjack_get_table_id(int);
int blackjack_get_seat_id(int, int);
int blackjack_get_seat_socket(int, int);

#endif
