#include "Deck.h"

/*
	TODO

*/




std::string playornotplay();
std::string fetchplaystyle();		//	Draw 1 or 3 cards at a time
void draw(Deck&, Deck&, std::string); //	Returns the current hand (which is essentially a stack)
void play(std::string);
bool move(Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&);
bool validMove(Deck*, Deck*, int);	//	The integer is used to determine dest (foundation or not)	
void displayGameState(Deck, Deck, Deck, Deck, Deck, Deck, Deck, Deck, Deck, Deck, Deck, Deck, Deck);

void Solitaire() {
	std::string toplay = playornotplay();
	if (toplay == "n" || toplay == "N") {
		std::cout << "Exiting Solitaire\n";
		return;
	}
	
	std::string playstyle = fetchplaystyle();
	
	play(playstyle);

	std::cout << "Good game! Want to play another?\n";
	toplay = playornotplay();
	while (toplay == "y" || toplay == "Y") {
		playstyle = fetchplaystyle();
		play(playstyle);
	}
	std::cout << "Exiting Solitaire\n";
	return;
	
}

std::string playornotplay() {
	std::string ch;
	std::cout << "Ready to play? (y/n)\n";
	std::cin >> ch;
	while (ch != "y" && ch != "n" && ch != "Y" && ch != "N") {
		std::cout << "Invalid response. Please enter y or n: ";
		std::cin >> ch;
	}
	return ch;
}

std::string fetchplaystyle() {
	std::string ps;
	std::cout << "Do you want to play drawing 3 cards or 1 card?\n";
	std::cin >> ps;
	while (ps != "3" && ps != "1") {
		std::cout << "Invalid response. Please enter 3 or 1: ";
		std::cin >> ps;
	}
	return ps;
}

void draw(Deck &d, Deck &hand, std::string playstyle) {
	if (playstyle == "1") {
		if (d.currentSize() + hand.currentSize() <= 10) {
			std::cout << "Cannot draw or reset anymore\n";
		}
		else if (d.currentSize() == 0) {
			std::cout << "Resetting hand and deck\n";
			d = hand;
			hand.empty();
		}
		//	Reset
		else {
			hand.addCardtoFront(d.takeCard(d.currentSize()));
		}
		std::cout << "Hand:\n";
		hand.displayDeck();
		std::cout << "\n";
	}
	else {
		int count = 0;
		//	Reset
		if (d.currentSize() == 0) {
			std::cout << "Resetting hand and deck\n";
			d = hand;
			hand.empty();
		}
		else {
			std::cout << "Drawing 3\n";
			while (count < 3 && d.currentSize() > 0) {
				hand.addCardtoFront(d.takeCard(d.currentSize()));
				count++;
			}
		}
		std::cout << "Hand\n";
		hand.displayDeck();
		std::cout << "\n";
	}
}

void play(std::string ps) {
	Deck spades(13), clubs(13), diamonds(13), hearts(13);
	Deck one(15), two(17), three(20), four(20), five(20), six(20), seven(25);
	Deck hand;
	hand.empty();
	Deck d;
	d.shuffle(1000);
	int i = 0;

	spades.empty();	clubs.empty(); diamonds.empty(); hearts.empty();	// Change all current sizes to 0
	one.empty(); two.empty(); three.empty(); four.empty(); five.empty(); six.empty(); seven.empty();
	one.addCardtoFront(d.takeCard(d.currentSize()));
	two.addCardtoFront(d.takeCard(d.currentSize())); two.addCardtoFront(d.takeCard(d.currentSize()));
	for (i = 0; i < 3; ++i) three.addCardtoFront(d.takeCard(d.currentSize()));
	for (i = 0; i < 4; ++i) four.addCardtoFront(d.takeCard(d.currentSize()));
	for (i = 0; i < 5; ++i) five.addCardtoFront(d.takeCard(d.currentSize()));
	for (i = 0; i < 6; ++i) six.addCardtoFront(d.takeCard(d.currentSize()));
	for (i = 0; i < 7; ++i) seven.addCardtoFront(d.takeCard(d.currentSize()));

	bool playon = true;
	std::string action;

	displayGameState(d, hand, one, two, three, four, five, six, seven, spades, clubs, diamonds, hearts);
	return;

	while (spades.currentSize() < 13 && clubs.currentSize() < 13 &&
		diamonds.currentSize() < 13 && hearts.currentSize() < 13) {
		
		std::cout << "What would you like to do?\n - Draw from deck (d)\n - Move (m)\n - Quit (q)\n";
		std::cin >> action;
		while (action != "D" && action != "d" && action != "m" && action != "M" && action != "Q" && action != "q") {
			std::cout << "Invalid response. Please enter d or m: ";
			std::cin >> action;
		}
		if (action == "Q" || action == "q") {
			std::cout << "Leaving game\n";
			return;
		}
		else if (action == "d" || action == "D") {
			draw(d, hand, ps);
		}
		else {
			playon = move(d, hand, one, two, three, four, five, six, seven, spades, clubs, diamonds, hearts);
			if (playon == false) {
				std::cout << "Leaving game\n";
				return;
			}
		}

		std::cout << "Next ieration inside play function\n";
	}
}

//	d stands for deck, 1-7 stands for bottom decks, 10-13 stands for goal decks, q for quit
bool move(Deck &d, Deck &hand, Deck &one, Deck &two, Deck &three, Deck &four, Deck &five, Deck &six, Deck &seven,
	Deck &spades, Deck &clubs, Deck &diamonds, Deck &hearts) {
	std::cout << "Still have to check\n\n";

	std::string playerChoice1, playerChoice2;
	Deck *choice1 = 0, *choice2 = 0;

	bool madeamove = false;
	while (!madeamove) {

		//	FirstChoice
		while (playerChoice1 != "r" && playerChoice1 != "R" && playerChoice1 != "1" && playerChoice1 != "2" && playerChoice1 != "3" &&
			playerChoice1 != "4" && playerChoice1 != "5" && playerChoice1 != "6" && playerChoice1 != "7" && playerChoice1 != "11" &&
			playerChoice1 != "12" && playerChoice1 != "13" && playerChoice1 != "14" && playerChoice1 != "q" && playerChoice1 != "Q") {

			std::cout << "What card would you like to move?\n(Enter r to go back, or 1-7 for bottom decks, or 11-14 for foundation decks" <<
				", or q to quit)\n";
			std::cin >> playerChoice1;
			//	If the deck is empty, also continue
			{
				if (playerChoice1 == "1" && one.currentSize() == 0) playerChoice1 = "Z";
				if (playerChoice1 == "2" && two.currentSize() == 0) playerChoice1 = "Z";
				if (playerChoice1 == "3" && three.currentSize() == 0) playerChoice1 = "Z";
				if (playerChoice1 == "4" && four.currentSize() == 0) playerChoice1 = "Z";
				if (playerChoice1 == "5" && five.currentSize() == 0) playerChoice1 = "Z";
				if (playerChoice1 == "6" && six.currentSize() == 0) playerChoice1 = "Z";
				if (playerChoice1 == "7" && seven.currentSize() == 0) playerChoice1 = "Z";
				if (playerChoice1 == "11" && spades.currentSize() == 0) playerChoice1 = "Z";
				if (playerChoice1 == "12" && clubs.currentSize() == 0) playerChoice1 = "Z";
				if (playerChoice1 == "13" && diamonds.currentSize() == 0) playerChoice1 = "Z";
				if (playerChoice1 == "14" && hearts.currentSize() == 0) playerChoice1 = "Z";
			}
		}
		if (playerChoice1 == "q" || playerChoice1 == "Q") return false;			//	Quit
		if (playerChoice1 == "r" || playerChoice1 == "R") return true;			//	Return without moving

		while (playerChoice2 != "r" && playerChoice2 != "R" && playerChoice2 != "1" && playerChoice2 != "2" && playerChoice2 != "3" &&
			playerChoice2 != "4" && playerChoice2 != "5" && playerChoice2 != "6" && playerChoice2 != "7" && playerChoice2 != "10" &&
			playerChoice2 != "11" && playerChoice2 != "12" && playerChoice2 != "13" && playerChoice2 != "q" && playerChoice2 != "Q") {

			std::cout << "Where would you like to move this card?\nEnter (r to go back, or 1-7 for bottom decks, or 11-14 for foundation " <<
				"decks, or q to quit)\n";
			std::cin >> playerChoice2;
		}
		if (playerChoice2 == "q" || playerChoice2 == "Q") return false;
		if (playerChoice2 == "r" || playerChoice2 == "R") return true;

		/*
			Foundation is used for checking the destination of the move
			- 0 if not foundation
			- 1 if spades
			- 2 if clubs
			- 3 if diamonds
			- 4 if hearts
		*/
		int foundation = 0;
		//	Store the corresponding decks in choice1 and choice2
		{
			if (playerChoice1 == "1") choice1 = &one;
			else if (playerChoice1 == "2") choice1 = &two;
			else if (playerChoice1 == "3") choice1 = &three;
			else if (playerChoice1 == "4") choice1 = &four;
			else if (playerChoice1 == "5") choice1 = &five;
			else if (playerChoice1 == "6") choice1 = &six;
			else if (playerChoice1 == "7") choice1 = &seven;
			else if (playerChoice1 == "11") choice1 = &spades;
			else if (playerChoice1 == "12") choice1 = &clubs;
			else if (playerChoice1 == "13") choice1 = &diamonds;
			else if (playerChoice1 == "14") choice1 = &hearts;
			if (playerChoice2 == "1") choice2 = &one;
			else if (playerChoice2 == "2") choice2 = &two;
			else if (playerChoice2 == "3") choice2 = &three;
			else if (playerChoice2 == "4") choice2 = &four;
			else if (playerChoice2 == "5") choice2 = &five;
			else if (playerChoice2 == "6") choice2 = &six;
			else if (playerChoice2 == "7") choice2 = &seven;
			else if (playerChoice2 == "11") {
				choice2 = &spades;
				foundation = 1;
			}
			else if (playerChoice2 == "12") {
				choice2 = &clubs;
				foundation = 2;
			}
			else if (playerChoice2 == "13") {
				choice2 = &diamonds;
				foundation = 3;
			}
			else if (playerChoice2 == "14") {
				choice2 = &hearts;
				foundation = 4;
			}
		}

		if (validMove(choice1, choice2, foundation)) {
			madeamove = true;
		}
	}
	return true;
}

/*
	Input:
		- Deck pointers to the source and destination
		- integer to determine whether dest is foundation or not
	Output:
		- 0 Foundation
			- If choice1 is king and choice2 is empty, return true
			- If choice1 is 1 lower than choice2, return true
			- If choice1 is not king and choice2 is empty, return false;
			- If choice1 is not 1 lower than choice2, return false
			- If same color, return false
		- Nonzero Foundation
			- If choice1 is wrong suit, return false
			- If choice1 is ace and choice2 is empty, return true
			- If choice1 is not ace and choice2 is empty, return false
			- If choice1 is 1 higher than choice2, return true
*/
bool validMove(Deck* choice1, Deck* choice2, int foundation) {
	if (foundation == 0) {
		//	Choice1 is King and Choice2 is empty
		if (choice2->currentSize() == 0 && choice1->checkCard(0).num == 12) {
			choice2->addCardtoFront(choice1->takeCard(0));
			return true;
		}
		//	Choice1 is not King and Choice2 is empty
		if (choice2->currentSize() == 0 && choice1->checkCard(0).num != 12) {
			return false;
		}

		//	Same color
		if ((choice1->checkCard(0).suit == 0 || choice1->checkCard(0).suit == 1) &&
			(choice2->checkCard(0).suit == 0 || choice2->checkCard(0).suit == 1)) {
			return false;
		}
		if ((choice1->checkCard(0).suit == 2 || choice1->checkCard(0).suit == 3) &&
			(choice2->checkCard(0).suit == 2 || choice2->checkCard(0).suit == 3)) {
			return false;
		}

		//	Choice1 is 1 smaller than Choice2
		if (choice1->checkCard(0).num == choice2->checkCard(0).num - 1) {
			choice2->addCardtoFront(choice1->takeCard(0));
			return true;
		}
		//	Choice1 is not 1 smaller than Choice2
		if (choice1->checkCard(0).num != choice2->checkCard(0).num - 1) {
			return false;
		}
	}
	else {
		if (foundation == 1 && choice1->checkCard(0).suit != 0) return false;
		else if (foundation == 2 && choice1->checkCard(0).suit != 1) return false;
		else if (foundation == 3 && choice1->checkCard(0).suit != 2) return false;
		else if (foundation == 4 && choice1->checkCard(0).suit != 3) return false;

		if (choice2->currentSize() == 0 && choice1->checkCard(0).num == 0) {
			choice2->addCardtoFront(choice1->takeCard(0));
			return true;
		}
		if (choice2->currentSize() == 0 && choice1->checkCard(0).num != 0) return false;
		if (choice1->checkCard(0).num == choice2->checkCard(0).num + 1) {
			choice2->addCardtoFront(choice1->takeCard(0));
			return true;
		}
		else if (choice1->checkCard(0).num != choice2->checkCard(0).num + 1) return false;
	}
	std::cout << "This should not happen. In ValidMove function\n\n";
	return false;
}

void displayGameState(Deck d, Deck hand, Deck one, Deck two, Deck three, Deck four, Deck five, Deck six, Deck seven,
	Deck spades, Deck clubs, Deck diamonds, Deck hearts) {

	std::cout << "Deck\t\thand\t\t\tSpades\tClubs\tDiamonds\tHearts\n";
	if (d.currentSize() > 0) std::cout << d.checkCard(0) << "\t";
	else std::cout << "\t\t";
	if (hand.currentSize() > 0) std::cout << hand.checkCard(0) << "\t";
	else std::cout << "\t\t";
	if (spades.currentSize() > 0) std::cout << spades.checkCard(0) << "\t";
	else std::cout << "\t\t";
	if (clubs.currentSize() > 0) std::cout << clubs.checkCard(0) << "\t";
	else std::cout << "\t\t";
	if (diamonds.currentSize() > 0) std::cout << diamonds.checkCard(0) << "\t";
	else std::cout << "\t\t";
	if (hearts.currentSize() > 0) std::cout << hearts.checkCard(0) << "\t";
	else std::cout << "\t\t";

	std::cout << "\n\n";
	std::cout << "One\t\tTwo\t\tThree\t\tFour\t\tFive\t\tSix\t\tSeven\n";
	if (one.currentSize() > 0) std::cout << one.checkCard(0) << "\t";
	else std::cout << "\t\t";
	if (two.currentSize() > 0) std::cout << two.checkCard(0) << "\t";
	else std::cout << "\t\t";
	if (three.currentSize() > 0) std::cout << three.checkCard(0) << "\t";
	else std::cout << "\t\t";
	if (four.currentSize() > 0) std::cout << four.checkCard(0) << "\t";
	else std::cout << "\t\t";
	if (five.currentSize() > 0) std::cout << five.checkCard(0) << "\t";
	else std::cout << "\t\t";
	if (six.currentSize() > 0) std::cout << six.checkCard(0) << "\t";
	else std::cout << "\t\t";
	if (seven.currentSize() > 0) std::cout << seven.checkCard(0) << "\t";
	else std::cout << "\t\t";
}