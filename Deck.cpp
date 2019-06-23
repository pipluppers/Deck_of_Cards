#include "Deck.h"

//	Does NOT output a newline at the end
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
	else out << c.num + 1;

	return out;
}

//	Does output a newline at the end
std::ostream& operator<<(std::ostream& out, const Deck& d) {
	int i;
	for (i = 0; i < d.current_size; ++i) {
		out << d.deck[i] << std::endl;
	}
	return out;
}

//	Default constructor
Deck::Deck() {
	int i = 0; Suit j = spade;
	this->size = 52, this->current_size = 52;
	deck = new Card[52];
	for (; i < 13; ++i) {
		for (j = 0; j < 4; ++j) {
			deck[i * 4 + j] = Card(i, j);
		}
	}
}

//	Integer Constructor
Deck::Deck(int size) {
	this->size = size;
	this->current_size = size;
	deck = new Card[size];
	int i = 0, j = 0;
	while (i * 4 + j < size) {							//	While loop is for sizes above 52
		for (; i < 13 && i*4 + j < size; ++i) {			//	This loop is for the numbers
			for (j = 0; j < 4 && i*4+j < size; ++j) {	//	This loop is for the suits
				deck[i * 4 + j] = Card(i, j);
			}
		}
	}
}

//	Copy Constructor
Deck::Deck(Deck& d) {
	this->size = d.size;
	this->current_size = d.current_size;
	this->deck = new Card[this->size];
	int i = 0;
	for (; i < d.current_size; ++i) {
		this->deck[i] = d.deck[i];
	}
}

//	Destructor
Deck::~Deck() {
	if (this->deck != 0) delete[] this->deck;
}

//	Copy Assignment Operator
Deck& Deck::operator=(const Deck& d) {
	this->size = d.size;
	this->current_size = d.current_size;
	if (this->deck != 0) delete[] this->deck;
	deck = new Card[this->size];
	int i = 0;
	for (i = 0; i < d.current_size; ++i) {
		this->deck[i] = d.deck[i];
	}
	return *this;
}





//	---------------	Functions	--------------------------------

int Deck::currentSize() const {
	return this->current_size;
}

void Deck::displayDeck() const {
	std::cout << "----------" << std::endl <<  "Deck" << std::endl;
	int i = 0;
	for (; i < this->current_size; ++i) {
		std::cout << i + 1 << ". " << this->deck[i] << std::endl;
	}
	std::cout << "----------" << std::endl << std::endl;
}

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
	if (index < 0 || index >= this->current_size) std::cout << "Index is out-of-range\n";
	assert(index >= 0 && index < this->current_size);
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
	assert(index >= 0 && index < this->current_size);
	Card temp = this->deck[index];
	for (; index < this->current_size - 1; ++index) {
		this->deck[index] = this->deck[index + 1];
	}
	--this->current_size;
	return temp;
}

Card Deck::takeFrontCard() {
	assert(this->current_size != 0);
	return this->takeCard(0);
}

Card Deck::takeEndCard() {
	assert(this->current_size != 0);
	return this->takeCard(this->current_size - 1);
}

/*
	Input:
		- Card to add
		- Location to add it before (e.g. if index = 0, add to front. if index = 2, add before the third card i.e. at index 2)
	Process:
		- Assert that the index is valid
		- Check if deck is already full
			- If yes, allocate space for the card and add it. Increment both current_size and size
			- If no, add card. Increment only current_size
*/
bool Deck::addCard(Card c, int index) {
	if (index < 0 || index > this->current_size) return false;
	if (this->current_size == this->size) {
		Card* tempDeck = new Card[this->size + 1];
		int i = 0;
		while (i < index) {
			tempDeck[i] = this->deck[i];
			++i;
		}
		tempDeck[i] = c;		// Shallow copy. Probably doesn't matter
		++i;
		while (i < this->size + 1) {
			tempDeck[i] = this->deck[i - 1];
			++i;
		}
		if (this->deck != 0) delete[] this->deck;
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
	return true;
}

bool Deck::addCardtoEnd(Card c) {
	return addCard(c, this->current_size);
}

bool Deck::addCardtoFront(Card c) {
	return addCard(c, 0);
}

void Deck::fullempty() {
	this->size = 0;
	this->current_size = 0;
}

void Deck::empty() {
	this->current_size = 0;
}

void Deck::reset() {
	std::cout << "Starting with a fresh 52-card deck\n";
	this->size = 52;
	this->current_size = 52;
	int i = 0, j = 0;
	if (this->deck != 0) delete[] deck;
	deck = new Card[52];
	for (; i < 13; ++i) {
		for (j = 0; j < 4; ++j) {
			deck[i * 4 + j] = Card(i, j);
		}
	}
}
