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
	int i, s;
	
	// Initialize tables
	for (i = 0; i < TABLE_MAX; ++i) {
		
		// Assign table id
		myBlackjack.tables[i].id = i+1;
		
		// Assign seat ids and set socket asignments to zero
		for (s = 0; s < SEAT_MAX; ++s) {
			myBlackjack.tables[i].seats[s].id = s+1;
			myBlackjack.tables[i].seats[s].socket = 0;
		}
		
		// Shuffling cards
		blackjack_shuffle_table(i);

	}
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
}
