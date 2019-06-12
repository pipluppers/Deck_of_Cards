#include "Deck.h"

std::ostream& operator<<(std::ostream& out, const Card& c) {
	assert(c.num >= 0);			// No negative cards

	if (c.suit == 0) out << "Spade ";
	else if (c.suit == 1) out << "Club ";
	else if (c.suit == 2) out << "Diamond ";
	else if (c.suit == 3) out << "Heart ";
	else out << "Wild Card ";

	if (c.num == 0) out << "Ace\n";
	else if (c.num == 10) out << "Jack\n";
	else if (c.num == 11) out << "Queen\n";
	else if (c.num == 12) out << "King\n";
	else if (c.num == 13) out << "Joker\n";
	else out << c.num + 1 << "\n";

	return out;
}

Deck::Deck() {
	int i = 0, j = 0;
	this->size = 52, this->current_size = 52;
	deck = new Card[52];
	for (; i < 13; ++i) {
		for (j = 0; j < 4; ++j) {
			deck[i * 4 + j] = Card(i, j);
		}
	}
}

Deck::Deck(int size) {
	this->size = size;
	this->current_size = size;
	deck = new Card[size];
	int i = 0, j = 0;
	while (i * 4 + j < size) {
		for (; i < 13 && i*4 < size; ++i) {
			for (j = 0; j < 4 && i*4+j < size; ++j) {
				deck[i * 4 + j] = Card(i, j);
			}
		}
	}
}

int Deck::currentSize() const {
	return this->current_size;
}

void Deck::displayDeck() const {
	std::cout << "----------\nDisplaying Deck\n";
	int i = 0;
	for (; i < this->current_size; ++i) {
		std::cout << i + 1 << ". " << this->deck[i];
	}
	std::cout << "----------\n";
}

/*
	Input:
		- Number of times to shuffle
	Process:
		- Pull random numbers a and b
		- Move everything from a to b inclusive to the front
*/
void Deck::shuffle(int num) {
	int begin, begin_temp, end, stop, i, j;
	Card* tempDeck = new Card[this->current_size];
	while (num > 0) {
		i = 0, j = 0;		// Used for tempDeck and regular deck
		begin = rand() % this->current_size;
		begin_temp = begin;
		end = rand() % (this->current_size - begin);
		assert(begin + end < this->current_size);
		stop = begin + end;

		//	Actual Shuffling
		while (begin <= stop) {
			tempDeck[i] = this->deck[begin];
			++begin; ++i;
		}
		while (j < begin_temp) {
			tempDeck[i] = this->deck[j];
			++i; ++j;
		}
		stop++;
		while (stop < this->current_size) {
			tempDeck[i] = this->deck[stop];
			assert(i < this->current_size);			//	Blocks out-of-bounds access
			++i; ++stop;
		}

		// Copy over
		for (i = 0; i < this->current_size; ++i) {
			this->deck[i] = tempDeck[i];
		}
		--num;
	}
	delete[] tempDeck;
}

/*
	Input:
		- Index to check
	Process:
		- If index is out of range, exit
		- Calling function should have a check for this default card
	Output:
		- Card
*/
Card Deck::checkCard(int index) const {
	if (index < 1 || index >= this->current_size + 1) std::cout << "Index is out-of-range\n";
	assert(index >= 1 && index < this->current_size + 1);
	--index;
	return deck[index];
}

/*
	Input:
		- Index to check
	Process:
		- If index is out of range, exit
		- Store card in temp variable and move everything over
		- Decrease current size of deck
	Output:
		- Desired card
*/
Card Deck::takeCard(int index) {
	assert(index >= 1 && index < this->current_size + 1);
	--index;
	Card temp = this->deck[index];
	for (; index < this->current_size - 1; ++index) {
		this->deck[index] = this->deck[index + 1];
	}
	--this->current_size;

	return temp;
}

/*
	Input:
		- Card to add
	Process:
		- Check if the deck is already full
			- If yes: allocate space for the card and add it. Increment current_size and size
			- If not: add card. Increment current_size
*/
void Deck::addCardtoEnd(Card c) {
	if (this->current_size == this->size) {
		Card* tempDeck = new Card[this->size + 1];
		int i = 0;
		while (i < this->size) {
			tempDeck[i] = this->deck[i];
			++i;
		}
		tempDeck[i] = c;
		delete[] this->deck;
		this->deck = tempDeck;
		++this->size;
	}
	else {
		deck[this->current_size] = c;
	}
	++this->current_size;
}

void Deck::addCardtoFront(Card c) {
	if (this->current_size == this->size) {
		Card* tempDeck = new Card[this->size + 1];
		int i = 1;
		tempDeck[0] = c;
		while (i < this->size + 1) {
			tempDeck[i] = this->deck[i - 1];
			++i;
		}
		delete[] this->deck;
		this->deck = tempDeck;
		++this->size;
	}
	else {
		int i = this->current_size;
		for (; i > 0; --i) {
			this->deck[i] = this->deck[i - 1];
		}
		this->deck[i] = c;
	}
	++this->current_size;
}

void Deck::addCardtoLoc(Card c, int index) {
	assert(index >= 1 && index <= current_size + 1);
	--index;
	if (this->current_size == this->size) {
		Card* tempDeck = new Card[this->size + 1];
		int i = 0;
		while (i < index) {
			tempDeck[i] = this->deck[i];
			++i;
		}
		tempDeck[i] = c;
		++i;
		while (i < this->size + 1) {
			tempDeck[i] = this->deck[i - 1];
			++i;
		}
		delete[] this->deck;
		this->deck = tempDeck;
		++this->size;
	}
	else {
		int i = this->current_size;
		while (i > index) {
			this->deck[i] = this->deck[i - 1];
			--i;
		}
		this->deck[i] = c;
	}
	++this->current_size;
}

void Deck::fullempty() {
	this->size = 0;
	this->current_size = 0;
}

void Deck::empty() {
	this->current_size = 0;
}



Deck& Deck::operator=(const Deck& d) {
	this->size = d.size;
	this->current_size = d.current_size;
	delete[] this->deck;
	deck = new Card[this->size];
	int i = 0;
	for (i = 0; i < d.current_size; ++i) {
		this->deck[i] = d.deck[i];
	}
	return *this;
}