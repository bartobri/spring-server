// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "example/blackjack.h"

// Define static structures
static struct blackjack myBlackjack;
static struct deck masterDeck = {
	{
		{ 1, 7,  7,  "7-H" },
		{ 2, 8,  8,  "8-H" },
		{ 3, 9,  9,  "9-H" },
		{ 4, 10, 10, "10-H" },
		{ 5, 10, 10, "J-H" },
		{ 6, 10, 10, "Q-H" },
		{ 7, 10, 10, "K-H" },
		{ 8, 11, 1,  "A-H" },
		{ 9, 7,  7,  "7-D" },
		{ 10, 8,  8,  "8-D" },
		{ 11, 9,  9,  "9-D" },
		{ 12, 10, 10, "10-D" },
		{ 13, 10, 10, "J-D" },
		{ 14, 10, 10, "Q-D" },
		{ 15, 10, 10, "K-D" },
		{ 16, 11, 1,  "A-D" },
		{ 17, 7,  7,  "7-C" },
		{ 18, 8,  8,  "8-C" },
		{ 19, 9,  9,  "9-C" },
		{ 20, 10, 10, "10-C" },
		{ 21, 10, 10, "J-C" },
		{ 22, 10, 10, "Q-C" },
		{ 23, 10, 10, "K-C" },
		{ 24, 11, 1,  "A-C" },
		{ 25, 7,  7,  "7-S" },
		{ 26, 8,  8,  "8-S" },
		{ 27, 9,  9,  "9-S" },
		{ 28, 10, 10, "10-S" },
		{ 29, 10, 10, "J-S" },
		{ 30, 10, 10, "Q-S" },
		{ 31, 10, 10, "K-S" },
		{ 32, 11, 1,  "A-S" },
	}
};

// Game functions

void blackjack_init(void) {
	int t, s, h, c;
	
	// Initialize tables
	for (t = 0; t < TABLE_MAX; ++t) {
		
		// Assign table id
		myBlackjack.tables[t].id = t+1;
		
		// Assign seat ids and set socket asignments to zero
		for (s = 0; s < SEAT_MAX; ++s) {
			myBlackjack.tables[t].seats[s].id = s+1;
			myBlackjack.tables[t].seats[s].socket = 0;
		}
		
		// Shuffling cards
		blackjack_shuffle_table(t);

	}
	
	// Initialize all player cards to zero (empty)
	for (t = 0; t < TABLE_MAX; ++t)
		for (s = 0; s < SEAT_MAX; ++s)
			for (h = 0; h < HAND_MAX; ++h)
				for (c = 0; c < HAND_CARD_MAX; ++c)
					blackjack_set_seat_hand_card_id(t, s, h, c, 0);
	
	// Initialize all player 'stay' state to zero
	for (t = 0; t < TABLE_MAX; ++t)
		for (s = 0; s < SEAT_MAX; ++s)
			for (h = 0; h < HAND_MAX; ++h)
				blackjack_set_seat_hand_stay(t, s, h, 0);

	// Initalize all dealer cards to zero
	for (t = 0; t < TABLE_MAX; ++t)
		for (c = 0; c < HAND_CARD_MAX; ++c)
			blackjack_set_dealer_card_id(t, c, 0);
}

void blackjack_shuffle_table(int tableIndex) {
	int d, c, r, i;
	struct card n;
	
	static int seedRand = 1;
	if (seedRand) {
		srand(time(NULL));
		seedRand = 0;
	}
	
	// TODO - this shuffle algorithm does not mix all decks together.
	//        change deck array to card array: struct card shoe[max_decks * cards_per_deck]
	//        then change dealDeckIndex and dealCardIndex to a single index
	for (d = 0; d < DECK_MAX; ++d) {
		for (c = 0; c < DECK_CARD_MAX; ++c) {

			// Generate random card number
			r = rand() % DECK_CARD_MAX;
			
			while (1) {
				
				// Get random card from master deck
				n = masterDeck.cards[r];
			
				// Check if we already have this card by looking at all
				// the previous assigned cards and comparing the id member
				for (i = c-1; i >= 0; --i)
					if (n.id == myBlackjack.tables[tableIndex].decks[d].cards[i].id)
						break;
				
				// If i was decremented below zero, card is unique
				if (i < 0) {
					myBlackjack.tables[tableIndex].decks[d].cards[c].id = n.id;
					myBlackjack.tables[tableIndex].decks[d].cards[c].highValue = n.highValue;
					myBlackjack.tables[tableIndex].decks[d].cards[c].lowValue = n.lowValue;
					myBlackjack.tables[tableIndex].decks[d].cards[c].display = n.display;
					break;
				}

				// Increment random number until we find a unique card.
				// Be sure to set it to zero if we reach DECK_CARD_MAX.
				if (++r == DECK_CARD_MAX)
					r = 0;
			}
		}
	}
	
	myBlackjack.tables[tableIndex].dealDeckIndex = 0;
	myBlackjack.tables[tableIndex].dealCardIndex = 0;
}

int blackjack_deal_next_card_id(int tableIndex) {
	int d = myBlackjack.tables[tableIndex].dealDeckIndex;
	int c = myBlackjack.tables[tableIndex].dealCardIndex;
	int r = myBlackjack.tables[tableIndex].decks[d].cards[c].id;
	
	// Advance deal position
	if (myBlackjack.tables[tableIndex].dealCardIndex + 1 < DECK_CARD_MAX) {
		++myBlackjack.tables[tableIndex].dealCardIndex;
	} else {
		if (myBlackjack.tables[tableIndex].dealDeckIndex + 1 < DECK_MAX) {
			myBlackjack.tables[tableIndex].dealCardIndex = 0;
			++myBlackjack.tables[tableIndex].dealDeckIndex;
		} else {
			// OOPS - We reached the end of the cards. This should never happen!
			// TODO - Make sure we reshuffle before we get here.
		}
	}
	
	return r;
}

int blackjack_get_table_id(int tableIndex) {
	return myBlackjack.tables[tableIndex].id;
}

int blackjack_get_seat_id(int tableIndex, int seatIndex) {
	return myBlackjack.tables[tableIndex].seats[seatIndex].id;
}

int blackjack_get_seat_socket(int tableIndex, int seatIndex) {
	return myBlackjack.tables[tableIndex].seats[seatIndex].socket;
}

int blackjack_get_seat_hand_card_id(int tableIndex, int seatIndex, int handIndex, int cardIndex) {
	return myBlackjack.tables[tableIndex].seats[seatIndex].hands[handIndex].cards[cardIndex].id;
}

int blackjack_get_dealer_card_id(int tableIndex, int cardIndex) {
	return myBlackjack.tables[tableIndex].dealerhand[cardIndex].id;
}

void blackjack_set_seat_socket(int tableIndex, int seatIndex, int socket) {
	myBlackjack.tables[tableIndex].seats[seatIndex].socket = socket;
}

void blackjack_set_seat_hand_card_id(int tableIndex, int seatIndex, int handIndex, int cardIndex, int value) {
	myBlackjack.tables[tableIndex].seats[seatIndex].hands[handIndex].cards[cardIndex].id = value;
}

void blackjack_set_seat_hand_stay(int tableIndex, int seatIndex, int handIndex, int value) {
	myBlackjack.tables[tableIndex].seats[seatIndex].hands[handIndex].stay = value;
}

void blackjack_set_dealer_card_id(int tableIndex, int cardIndex, int cardId) {
	myBlackjack.tables[tableIndex].dealerhand[cardIndex].id = cardId;
}
