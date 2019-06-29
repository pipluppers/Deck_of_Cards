//	Property of Alex Nguyen (nguyenalex24@gmail.com)

#include "Deck.h"

constexpr int PLAYON = 0, WIN = 1, LOSE = 2, DEALERTURN = 3, TIE = 4;
typedef int GameStatus;

//	Functions
void play();   bool confirmPlay();   bool playAgain();
bool openingMove(Deck&, Deck&, Deck&);
GameStatus firstTurn21(Deck&, Deck&);
GameStatus userTurn(Deck&, Deck&, Deck, int&);
GameStatus dealerTurn(Deck&, Deck&, int);
bool hit(Deck, Deck, int);
int sumHand(Deck, bool);
bool reshuffle();
bool postGameMenu(Deck&, Deck&, Deck&, GameStatus&);
//	End Functions

void Blackjack() {
	std::cout << "\n\n\n";
	std::cout << "----------------------------------------\n";
	std::cout << "          Welcome to Blackjack          \n";
	std::cout << "----------------------------------------\n\n\n";

	if (!confirmPlay()) {
		std::cout << "Exiting BlackJack\n";
		return;
	}
	play();
	std::cout << "Thank you for playing BlackJack. Bye ^.^\n";
}

void play() {
	Deck d, user, dealer;
	d.shuffle(2000);
	user.empty(); dealer.empty();
	int sumUser;
	GameStatus gameStatus = PLAYON;

	while (1) {
		if (!openingMove(d, dealer, user)) std::cout << "Error drawing cards\n";

		sumUser = sumHand(user, true);
		std::cout << "Current sum: " << sumUser << "\n";

		//	User draws 21 on the first turn
		if (sumUser == 21) gameStatus = firstTurn21(d, dealer);

		//	User turn (doesn't have 21 on the first turn)
		if (gameStatus == PLAYON) gameStatus = userTurn(d, user, dealer, sumUser);

		//	Dealer Turn
		if (gameStatus == DEALERTURN) gameStatus = dealerTurn(d, dealer, sumUser);

		//	Player won or lost
		assert(gameStatus == LOSE || gameStatus == WIN || gameStatus == TIE);
		if (!postGameMenu(d, user, dealer, gameStatus)) return;
	}
	return;
}

bool confirmPlay() {
	std::string ans;

	std::cout << "Ready to play BlackJack? (y/n)\n";
	std::cin >> ans;
	while (ans != "y" && ans != "Y" && ans != "n" && ans != "N") {
		std::cout << "Invalid response. Please enter y or n: ";
		std::cin >> ans;
	}
	return (ans == "y" || ans == "Y");
}

bool playAgain() {
	std::string action = "";
	while (action != "1" && action != "2") {
		std::cout << "\nDo you want to play again?\n - 1. Yes\n - 2. No\n";
		std::cin >> action;
	}
	return action == "1";
}

bool openingMove(Deck& d, Deck& dealer, Deck& user) {
	//	Add cards from deck to dealer's hand
	if (!dealer.addCardtoFront(d.takeEndCard()) || !dealer.addCardtoFront(d.takeEndCard()))
		return false;
	std::cout << "\nTop of Dealer's hand\n";
	std::cout << dealer.checkCard(0);
	std::cout << "\n\n";
	//	Add cards from deck to user's hand
	if (!user.addCardtoFront(d.takeEndCard()) || !user.addCardtoFront(d.takeEndCard()))
		return false;
	std::cout << "\tYour hand:\n";
	user.displayDeck();
	return true;
}

GameStatus firstTurn21(Deck& d, Deck& dealer) {
	int sumDealer;
	std::cout << "\n\tDealer's initial hand\n";
	dealer.displayDeck();
	sumDealer = sumHand(dealer, false);
	std::cout << "Dealer's current sum: " << sumDealer << "\n\n";
	while (sumDealer <= 21) {
		if (!dealer.addCardtoFront(d.takeEndCard()))
			std::cout << "Error occurred while drawing card\n";
		std::cout << "Dealer drew " << dealer.checkCard(0) << "\n";
		sumDealer = sumHand(dealer, false);
		std::cout << "Dealer's current sum: " << sumDealer << "\n\n";
	}
	if (sumDealer == 21) {
		std::cout << "Dealer also has 21. Tie game\n";
		return TIE;
	}
	else {
		std::cout << "Dealer busts. You win\n";
		return WIN;
	}
}

GameStatus userTurn(Deck& d, Deck& user, Deck dealer, int& sumUser) {
	while (1) {
		if (hit(user, dealer, sumUser)) {
			if (!user.addCardtoFront(d.takeEndCard()))
				std::cout << "Error occured while drawing card\n";
			std::cout << "You drew " << user.checkCard(0) << "\n";
			sumUser = sumHand(user, true);
			std::cout << "Current Sum: " << sumUser << "\n";

			if (sumUser > 21) {
				std::cout << "Bust. You lose\n";
				return LOSE;
			}
			if (sumUser == 21) {
				std::cout << "Win\n";
				return WIN;
			}
		}
		//	STAY
		else
			return DEALERTURN;
	}
}

GameStatus dealerTurn(Deck& d, Deck& dealer, int sumUser) {
	int sumDealer = 0;
	std::cout << "\n\tDealer's hand\n";
	dealer.displayDeck();
	sumDealer = sumHand(dealer, false);
	std::cout << "Dealer's current sum: " << sumDealer << "\n\n";

	while (sumDealer < sumUser && sumDealer < 21) {
		if (!dealer.addCardtoFront(d.takeEndCard()))
			std::cout << "Error drawing card\n";
		std::cout << "Dealer drew " << dealer.checkCard(0) << "\n";
		sumDealer = sumHand(dealer, false);
		std::cout << "Dealer's current sum: " << sumDealer << "\n\n";
	}

	if (sumDealer > 21) {
		std::cout << "Dealer busts. You win\n";
		return WIN;
	}
	else {
		std::cout << "Dealer has equal or better hand. You lose\n";
		return LOSE;
	}
}

bool hit(Deck user, Deck dealer, int current_sum) {
	std::string action = "";
	while (1) {
		action = "";
		while (action != "1" && action != "2" && action != "3") {
			std::cout << "\nAction:\n - 1. hit\n - 2. stay\n - 3. Display game state\n";
			std::cin >> action;
		}
		if (action == "3") {
			std::cout << "\n\tYour hand:\n";
			user.displayDeck();
			std::cout << "Current sum: " << current_sum << "\n";
			std::cout << "\nDealer's top card: " << dealer.checkCard(0) << "\n";
			continue;
		}
		return action == "1";
	}
}

int sumHand(Deck d, bool isUser) {
	int i = 0, counter = 0;
	std::string ans;
	bool dealerAce = false;

	for (; i < d.currentSize(); ++i) {
		//	If ACE and USER
		if (d.checkCard(i).num == 1 && isUser) {
			ans = "";
			while (ans != "1" && ans != "2") {
				std::cout << "Do you want this ace to be worth 1 or 11?\n - 1: 1\n - 2: 11\n";
				std::cin >> ans;
			}
			if (ans == "1") counter++;
			else counter += 11;
		}
		else if (d.checkCard(i).num == 1 && !isUser) {
			dealerAce = true;
			counter++;
		}
		else {
			if (d.checkCard(i).num > 10) counter += 10;
			else counter += d.checkCard(i).num;
		}
	}
	if (dealerAce && counter == 11) return 21;
	return counter;
}

bool reshuffle() {
	std::string action = "";
	while (action != "1" && action != "2") {
		std::cout << "Do you want to reshuffle the cards back into the deck?\n - 1. Yes\n - 2. No\n";
		std::cin >> action;
	}
	return action == "1";
}

bool postGameMenu(Deck& d, Deck& user, Deck& dealer, GameStatus &gameStatus) {
	if (playAgain()) {
		if (d.currentSize() < 20 || reshuffle()) {
			d.reset();
			d.shuffle(5000);
		}
		user.empty(); dealer.empty();
		gameStatus = PLAYON;
		std::cout << std::endl << "\n\n--------- New Game ---------" << std::endl;
		return true;
	}
	return false;
}
