//	Property of Alex Nguyen (nguyenalex24@gmail.com)

/*
	TODO
		Hideous UI
		Refactor move function
*/

#include "Deck.h"

typedef int UserAction;
constexpr UserAction DRAW = 0, MOVE = 1, QUIT = 2;

enum MoveChoice {ONE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, 
	FOUNDATION1, FOUNDATION2, FOUNDATION3, FOUNDATION4, MENU, QUIT_MC, INVALID};

bool playAgame();
std::string fetchplaystyle();		//	Draw 1 or 3 cards at a time
bool gameOver(Deck&, Deck&, Deck&, Deck&);
void draw(Deck&, Deck&, std::string); //	Returns the current hand (which is essentially a stack)
void play(std::string);
void prepGame(Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&);
UserAction userAction();

bool move(Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&, Deck&);
MoveChoice move_firstChoice(Deck,Deck,Deck,Deck,Deck,Deck,Deck,Deck,Deck,Deck,Deck);
MoveChoice move_secondChoice(Deck,Deck,Deck,Deck,Deck,Deck,Deck,Deck,Deck,Deck,Deck,MoveChoice);
MoveChoice is_equalMoveChoices(MoveChoice, std::string);

bool validMove(Deck*, Deck*, int);	//	The integer is used to determine dest (foundation or not)	

//	This UI is currently super ugly
void displayGameState(Deck, Deck, Deck, Deck, Deck, Deck, Deck, Deck, Deck, Deck, Deck, Deck, Deck);



void Solitaire() {
	while (1) {
		if (!playAgame()) {
			std::cout << "Exiting Solitaire. Bye friend\n";
			return;
		}
		std::string playstyle = fetchplaystyle();

		play(playstyle);

		std::cout << "Good game! Want to play another?\n";
	}
}

bool playAgame() {
	std::string ch;
	std::cout << "\nReady to play Solitaire?\n - 1. Yes\n - 2. No\n";
	std::cin >> ch;
	while (ch != "1" && ch != "2") {
		std::cout << "Invalid response. Please enter 1 or 2: ";
		std::cin >> ch;
	}
	return ch == "1";
}

std::string fetchplaystyle() {
	std::string ps;
	std::cout << "How many cards do you want to draw at a time?\n - 1. Single\n - 2. Triple\n";
	std::cin >> ps;
	while (ps != "1" && ps != "2") {
		std::cout << "Invalid response. Please enter 1 or 2: ";
		std::cin >> ps;
	}
	return ps;
}

bool gameOver(Deck& spades, Deck& clubs, Deck& diamonds, Deck& hearts) {
	return !(spades.currentSize() < 13 && clubs.currentSize() < 13 && 
		diamonds.currentSize() < 13 && hearts.currentSize() < 13);
}

void draw(Deck &d, Deck &hand, std::string playstyle) {
	if (playstyle == "1") {
		if (d.currentSize() + hand.currentSize() <= 10) {
			std::cout << "Cannot draw or reset anymore\n";
		}
		else if (d.isEmpty()) {
			std::cout << "Resetting hand and deck\n";
			d = hand;
			hand.empty();
		}
		//	Reset
		else {
			if (!hand.addCardtoFront(d.takeEndCard()))
				std::cout << "Error drawing card\n";
		}
		std::cout << "\tYour hand:\n";
		hand.displayDeck();
	}
	else {
		int count = 0;
		//	Reset
		if (d.isEmpty()) {
			std::cout << "Resetting hand and deck\n";
			d = hand;
			hand.empty();
		}
		else {
			std::cout << "Drawing 3\n";
			while (count < 3 && !d.isEmpty()) {
				if (!hand.addCardtoFront(d.takeEndCard()))
					std::cout << "Error drawing card\n";
				count++;
			}
		}
		std::cout << "\tYour hand\n";
		hand.displayDeck();
		std::cout << "\n";
	}
}

void play(std::string ps) {
	Deck spades(13), clubs(13), diamonds(13), hearts(13);
	Deck one(15), two(17), three(20), four(20), five(20), six(20), seven(52);
	Deck hand, d;
	hand.empty(); d.shuffle(2000);
	int i = 0;
	UserAction action;

	prepGame(d, one, two, three, four, five, six, seven, spades, clubs, diamonds, hearts);
	displayGameState(d, hand, one, two, three, four, five, six, seven, spades, clubs, diamonds, hearts);

	while (!gameOver(spades, clubs, diamonds, hearts)) {
		
		action = userAction();
		if (action == DRAW) draw(d, hand, ps);
		else if (action == MOVE) {
			if (!move(d, hand, one, two, three, four, five, six, seven, spades, clubs, diamonds, hearts)) {
				std::cout << "Leaving game\n";
				return;
			}
		}
		else if (action == QUIT) { std::cout << "Leaving game\n"; return; }
		else std::cout << "This should NOT print. Inside while loop in play function\n";

		displayGameState(d, hand, one, two, three, four, five, six, seven, spades, clubs, diamonds, hearts);

		std::cout << "\nNext iteration inside play function\n";
	}
}

UserAction userAction() {
	std::string action;

	std::cout << "What would you like to do?\n - 1: Draw from deck (d)\n - 2: Move (m)\n - 3: Quit (q)\n";
	std::cin >> action;
	while (action != "1" && action != "2" && action != "3") {
		std::cout << "Invalid option. Please enter 1, 2, or 3:\n";
		std::cin >> action;
	}
	if (action == "1") return DRAW;
	else if (action == "2") return MOVE;
	else return QUIT;
}

//	d stands for deck, 1-7 stands for bottom decks, 10-13 stands for goal decks, q for quit
bool move(Deck &d, Deck &hand, Deck &one, Deck &two, Deck &three, Deck &four, Deck &five, Deck &six, Deck &seven,
	Deck &spades, Deck &clubs, Deck &diamonds, Deck &hearts) {
	std::cout << "Still have to check\n\n";

	MoveChoice mc1, mc2;
	std::string playerChoice1, playerChoice2;
	Deck *choice1 = 0, *choice2 = 0;

	bool madeamove = false;
	while (!madeamove) {

		std::cout << "Making first choice\n";

		//	FirstChoice + Setup
		mc1 = move_firstChoice(one, two, three, four, five, six, seven, spades, clubs, diamonds, hearts);
		if (mc1 == QUIT_MC) return false;
		if (mc1 == MENU) return true;
		if (mc1 == ONE) choice1 = &one; if (mc1 == TWO) choice1 = &two; if (mc1 == THREE) choice1 = &three;
		if (mc1 == FOUR) choice1 = &four; if (mc1 == FIVE) choice1 = &five; if (mc1 == SIX) choice1 = &six;
		if (mc1 == SEVEN) choice1 = &seven; 
		if (mc1 == FOUNDATION1) choice1 = &spades; if (mc1 == FOUNDATION2) choice1 = &clubs; 
		if (mc1 == FOUNDATION3) choice1 = &diamonds; if (mc1 == FOUNDATION4) choice1 = &hearts;

		std::cout << "Making second choice\n";

		//	Second Choice + Setup (Duplicate already checked for)
		int foundation = 0;		// 0 for num, 1 for spade, 2 for club, 3 diamond, 4 heart
		mc2 = move_secondChoice(one, two, three, four, five, six, seven, spades, clubs, diamonds, hearts, mc1);
		if (mc2 == QUIT_MC) return false;
		if (mc2 == MENU) return true;
		if (mc2 == ONE) choice2 = &one; if (mc2 == TWO) choice2 = &two; if (mc2 == THREE) choice2 = &three;
		if (mc2 == FOUR) choice2 = &four; if (mc2 == FIVE) choice2 = &five; if (mc2 == SIX) choice2 = &six;
		if (mc2 == SEVEN) choice2 = &seven;
		if (mc2 == FOUNDATION1) {
			choice2 = &spades;
			foundation = 1;
		}
		if (mc2 == FOUNDATION2) {
			choice2 = &clubs;
			foundation = 2;
		}
		if (mc2 == FOUNDATION3) {
			choice2 = &diamonds;
			foundation = 3;
		}
		if (mc2 == FOUNDATION4) {
			choice2 = &hearts;
			foundation = 4;
		}

		if (validMove(choice1, choice2, foundation)) {
			std::cout << "Move is valid\n";
			madeamove = true;
		}
		return true;
	}
	return true;
}

MoveChoice move_firstChoice(Deck one, Deck two, Deck three, Deck four, Deck five, Deck six, Deck seven, 
	Deck spades, Deck clubs, Deck diamonds, Deck hearts) {
	std::string response = "";
	bool empty_deck = false;
	while (1) {
		empty_deck = false;

		std::cout << "\nFrom which deck would you like to move:\n - 1. Bottom decks\n - 2. Foundation decks\n - 3. Return to menu\n";
		std::cout << " - 4. Quit\n";
		std::cin >> response;
		while (response != "1" && response != "2" && response != "3" && response != "4") {
			std::cout << "Invalid response. Please enter 1, 2, 3, or 4:\n";
			std::cin >> response;
		}
		if (response == "3") return MENU;
		if (response == "4") return QUIT_MC;
		if (response == "1") {
			std::cout << "\nFrom which of the bottom decks would you like to move:\n - 1. One\n - 2. Two\n - 3. Three\n - 4. Four\n";
			std::cout << " - 5. Five\n - 6. Six\n - 7. Seven\n - 8. Go back to previous menu\n";
			std::cin >> response;
			while (response != "1" && response != "2" && response != "3" && response != "4" && response != "5" &&
				response != "6" && response != "7" && response != "8") {
				std::cout << "Invalid response. Please enter a number between 1 and 8:\n";
				std::cin >> response;
			}
			if (response == "8") continue;
			if (response == "1" && one.isEmpty()) empty_deck = true;
			if (response == "2" && two.isEmpty()) empty_deck = true;
			if (response == "3" && three.isEmpty()) empty_deck = true;
			if (response == "4" && four.isEmpty()) empty_deck = true;
			if (response == "5" && five.isEmpty()) empty_deck = true;
			if (response == "6" && six.isEmpty()) empty_deck = true;
			if (response == "7" && seven.isEmpty()) empty_deck = true;
			if (empty_deck) {
				std::cout << "Invalid move: deck is empty. Returning to previous menu\n";
				continue;
			}
			if (response == "1") return ONE;
			if (response == "2") return TWO;
			if (response == "3") return THREE;
			if (response == "4") return FOUR;
			if (response == "5") return FIVE;
			if (response == "6") return SIX;
			if (response == "7") return SEVEN;
		}
		if (response == "2") {
			std::cout << "\nFrom which of the foundation decks would you like to move:\n - 1. Spades\n - 2. Clubs\n";
			std::cout << " - 3. Diamonds\n - 4. Hearts\n - 5. Go back to previous menu\n";
			std::cin >> response;
			while (response != "1" && response != "2" && response != "3" && response != "4" && response != "5") {
				std::cout << "Invalid response. Please enter a number between 1 and 5:\n";
				std::cin >> response;
			}
			if (response == "5") continue;
			if (response == "1" && spades.isEmpty()) empty_deck = true;
			if (response == "2" && clubs.isEmpty()) empty_deck = true;
			if (response == "3" && diamonds.isEmpty()) empty_deck = true;
			if (response == "4" && hearts.isEmpty()) empty_deck = true;
			if (empty_deck) {
				std::cout << "Invalid move: deck is empty. Returning to previous menu\n";
				continue;
			}
			if (response == "1") return FOUNDATION1;
			if (response == "2") return FOUNDATION2;
			if (response == "3") return FOUNDATION3;
			if (response == "4") return FOUNDATION4;
		}
	}
}

MoveChoice move_secondChoice(Deck one, Deck two, Deck three, Deck four, Deck five, Deck six, Deck seven,
	Deck spades, Deck clubs, Deck diamonds, Deck hearts, MoveChoice mc1) {
	std::string response = "";
	MoveChoice temp;
	while (1) {
		std::cout << "From which deck would you like to move:\n - 1. Bottom decks\n - 2. Foundation decks\n - 3. Return to menu\n";
		std::cout << " - 4. Quit\n";
		std::cin >> response;
		while (response != "1" && response != "2" && response != "3" && response != "4") {
			std::cout << "Invalid response. Please enter 1, 2, 3, or 4:\n";
			std::cin >> response;
		}
		if (response == "3") return MENU;
		if (response == "4") return QUIT_MC;
		if (response == "1") {
			std::cout << "From which of the bottom decks would you like to move:\n - 1. One\n - 2. Two\n - 3. Three\n - 4. Four\n";
			std::cout << " - 5. Five\n - 6. Six\n - 7. Seven\n - 8. Go back to previous menu\n";
			std::cin >> response;
			while (response != "1" && response != "2" && response != "3" && response != "4" && response != "5" &&
				response != "6" && response != "7" && response != "8") {
				std::cout << "Invalid response. Please enter a number between 1 and 8:\n";
				std::cin >> response;
			}
			if (response == "8") continue;
			temp = is_equalMoveChoices(mc1, response);
			if (temp == INVALID) continue;
			return temp;
		}
		if (response == "2") {
			std::cout << "From which of the foundation decks would you like to move:\n - 1. Spades\n - 2. Clubs\n";
			std::cout << " - 3. Diamonds\n - 4. Hearts\n - 5. Go back to previous menu\n";
			std::cin >> response;
			while (response != "1" && response != "2" && response != "3" && response != "4" && response != "5") {
				std::cout << "Invalid response. Please enter a number between 1 and 5:\n";
				std::cin >> response;
			}
			if (response == "5") continue;
			temp = is_equalMoveChoices(mc1, "1" + response);
			if (temp == INVALID) continue;
			return temp;
		}
	}
}

MoveChoice is_equalMoveChoices(MoveChoice mc1, std::string response) {
	if (response == "1") {
		if (mc1 == ONE) return INVALID;
		else return ONE;
	}
	if (response == "2") {
		if (mc1 == TWO) return INVALID;
		else return TWO;
	}
	if (response == "3") {
		if (mc1 == THREE) return INVALID;
		else return THREE;
	}
	if (response == "4") {
		if (mc1 == FOUR) return INVALID;
		else return FOUR;
	}
	if (response == "5") {
		if (mc1 == FIVE) return INVALID;
		else return FIVE;
	}
	if (response == "6") {
		if (mc1 == SIX) return INVALID;
		else return SIX;
	}
	if (response == "7") {
		if (mc1 == SEVEN) return INVALID;
		else return SEVEN;
	}
	if (response == "11") {
		if (mc1 == FOUNDATION1) return INVALID;
		else return FOUNDATION1;
	}
	if (response == "12") {
		if (mc1 == FOUNDATION2) return INVALID;
		else return FOUNDATION2;
	}
	if (response == "13") {
		if (mc1 == FOUNDATION3) return INVALID;
		else return FOUNDATION3;
	}
	if (response == "14") {
		if (mc1 == FOUNDATION4) return INVALID;
		else return FOUNDATION4;
	}
	std::cout << "This should not be reached in is_equalMoveChoices function\n"; return INVALID;
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
			choice2->addCardtoFront(choice1->takeFrontCard());
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
			choice2->addCardtoFront(choice1->takeFrontCard());
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
			choice2->addCardtoFront(choice1->takeFrontCard());
			return true;
		}
		if (choice2->currentSize() == 0 && choice1->checkCard(0).num != 0) return false;
		if (choice1->checkCard(0).num == choice2->checkCard(0).num + 1) {
			choice2->addCardtoFront(choice1->takeFrontCard());
			return true;
		}
		else if (choice1->checkCard(0).num != choice2->checkCard(0).num + 1) return false;
	}
	std::cout << "This should not happen. In ValidMove function\n\n";
	return false;
}

void displayGameState(Deck d, Deck hand, Deck one, Deck two, Deck three, Deck four, Deck five, Deck six, Deck seven,
	Deck spades, Deck clubs, Deck diamonds, Deck hearts) {

	std::cout << "Deck\tHand\t\t\t\tSpades\tClubs\tDiamonds\tHearts\n";
	std::cout << d.currentSize() << "\t";

	if (!hand.isEmpty()) hand.checkCard(0).shortDisplay();
	
	std::cout << "\t\t\t\t";
	if (!spades.isEmpty()) spades.checkCard(0).shortDisplay();

	std::cout << "\t";
	if (!clubs.isEmpty()) clubs.checkCard(0).shortDisplay();

	std::cout << "\t";
	if (!diamonds.isEmpty()) diamonds.checkCard(0).shortDisplay();

	std::cout << "\t";
	if (!hearts.isEmpty()) hearts.checkCard(0).shortDisplay();


	std::cout << "\n\n";
	std::cout << "One\tTwo\tThree\tFour\tFive\tSix\tSeven\n";
	
	if (!one.isEmpty()) one.checkCard(0).shortDisplay();

	std::cout << "\t";
	if (!two.isEmpty()) two.checkCard(0).shortDisplay();

	std::cout << "\t";
	if (!three.isEmpty()) three.checkCard(0).shortDisplay();

	std::cout << "\t";
	if (!four.isEmpty()) four.checkCard(0).shortDisplay();
	
	std::cout << "\t";
	if (!five.isEmpty()) five.checkCard(0).shortDisplay();

	std::cout << "\t";
	if (!six.isEmpty()) six.checkCard(0).shortDisplay();

	std::cout << "\t";
	if (!seven.isEmpty()) seven.checkCard(0).shortDisplay();
	
	std::cout << "\n\n";
}

void prepGame(Deck& d, Deck& one, Deck& two, Deck& three, Deck& four, Deck& five, Deck& six, Deck& seven,
	Deck& spades, Deck& clubs, Deck& diamonds, Deck& hearts) {
	int i = 0;
	one.empty(); two.empty(); three.empty(); four.empty(); five.empty(); six.empty(); seven.empty();
	one.addCardtoFront(d.takeEndCard());
	two.addCardtoFront(d.takeEndCard()); two.addCardtoFront(d.takeEndCard());
	for (i = 0; i < 3; ++i) three.addCardtoFront(d.takeEndCard());
	for (i = 0; i < 4; ++i) four.addCardtoFront(d.takeEndCard());
	for (i = 0; i < 5; ++i) five.addCardtoFront(d.takeEndCard());
	for (i = 0; i < 6; ++i) six.addCardtoFront(d.takeEndCard());
	for (i = 0; i < 7; ++i) seven.addCardtoFront(d.takeEndCard());

	spades.empty(); clubs.empty(); diamonds.empty(); hearts.empty();
}
