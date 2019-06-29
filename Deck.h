//	Property of Alex Nguyen (nguyenalex24@gmail.com)

#pragma once
#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <ctime>

typedef int Suit;
constexpr int SPADE = 0, CLUB = 1, DIAMOND = 2, HEART = 3;

struct Card {
	int num;	// Ace is 1, King is 13
	Suit suit;	// Spade 0, Club 1, Diamond 2, Heart 3
	Card() : num(0), suit(SPADE) {}
	Card(int num, Suit suit) : num(num), suit(suit) {}
	friend std::ostream& operator<<(std::ostream&, const Card&);
};

class Deck {
public:
	Deck();
	Deck(int);		// Deck with variable size
	Deck(Deck&);
	Deck& operator=(const Deck&);	// Copy assignment
	~Deck();

	//	Data Members
	int currentSize() const;
	void incrementCurrentSize(int = 1);
	void incrementSize(int = 1);
	void displayDeck() const;

	//	Checking/Removing Card
	Card checkCard(int) const;
	Card takeCard(int);
	Card takeFrontCard();
	Card takeEndCard();

	//	Adding Card
	bool addCard(Card, int);
	bool addCardtoEnd(Card);
	bool addCardtoFront(Card);

	//	Deck Manipulation
	void shuffle(int = 1000);			// O(2 * size) space
	void fullempty();					// Change the size and current size to 0
	void empty();						// Change only the current size to 0
	void reset();						// Start with a fresh 52-card deck
private:
	Card* deck;		// Last index is the top of the deck. Index 0 is bottom of deck (head face up)
	int size;		// Can be used for current size (generally 52)
	int current_size;	// For games, magic tricks, etc.
};

//	Games
void Solitaire();
void Blackjack();
