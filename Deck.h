#pragma once
#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <ctime>

//enum Suit {spade = 0, club, diamond, heart};

struct Card {
	int num;	// Ace is 0, King is 12
	int suit;	// Spade 0, Club 1, Diamond 2, Heart 3
	Card() : num(0), suit(0) {}
	Card(int num, int suit) : num(num), suit(suit) {}
	friend std::ostream& operator<<(std::ostream&, const Card&);
};

class Deck {
private:
	Card* deck;		// Last index is the top of the deck. Index 0 is bottom of deck (head face up)
	int size;		// Can be used for current size (generally 52)
	int current_size;	// For games, magic tricks, etc.
public:
	Deck();
	Deck(int);		// Deck with variable size

	void displayDeck() const;
	int currentSize() const;

	void shuffle(int = 100);		// If no number inputted, shuffle 100 times. O(2*size) space
	Card checkCard(int) const;
	Card takeCard(int);
	void addCardtoEnd(Card);
	void addCardtoFront(Card);
	void addCardtoLoc(Card, int);
	void fullempty();					// Change the size and current size to 0
	void empty();						// Change only the current size to 0

	Deck& operator=(const Deck&);
};

//	Games
void Solitaire();