#ifndef BLACKJACK_SERVER_H
#define BLACKJACK_SERVER_H 1

#define HAND_CARD_MAX  7
#define HAND_MAX       4
#define DECK_MAX       6
#define SEAT_MAX       7
#define TABLE_MAX      10

// Function prototypes
void blackjack_init(void);
void blackjack_shuffle_table(int);
int blackjack_deal_next_card_id(int);
int blackjack_get_table_id(int);
int blackjack_get_seat_id(int, int);
int blackjack_get_seat_socket(int, int);
int blackjack_get_seat_hand_card_id(int, int, int, int);
int blackjack_get_dealer_card_id(int, int);
void blackjack_set_seat_socket(int, int, int);
void blackjack_set_seat_hand_card_id(int, int, int, int, int);
void blackjack_set_seat_hand_stay(int, int, int, int);
void blackjack_set_dealer_card_id(int, int, int);

#endif
