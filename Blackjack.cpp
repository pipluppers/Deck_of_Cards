//	Property of Alex Nguyen (nguyenalex24@gmail.com)

#include "Deck.h"

constexpr int PLAYON = 0, WIN = 1, LOSE = 2, DEALERTURN = 3, TIE = 4;
typedef int GameStatus;

bool confirmPlay();
void play();
int sumHand(Deck, bool);			// Sum of hand

void BlackJack() {
	if (!confirmPlay()) {
		std::cout << "Exiting BlackJack\n";
		return;
	}
	play();
	std::cout << "Thank you for playing BlackJack. Bye ^.^\n";
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

bool hit() {
	std::string action = "";
	while (action != "1" && action != "2") {
		std::cout << "Action:\n - 1. hit\n - 2. stay\n";
		std::cin >> action;
	}
	return action == "1";
}

bool playAgain() {
	std::string action = "";
	while (action != "y" && action != "Y" && action != "n" && action != "N") {
		std::cout << "Do you want to play again? (y/n)\n";
		std::cin >> action;
	}
	return (action == "y" || action == "Y");
}

bool reshuffle() {
	std::string action = "";
	while (action != "y" && action != "Y" && action != "n" && action != "N") {
		std::cout << "Do you want to reshuffle the cards back into the deck? (y/n)\n";
		std::cin >> action;
	}
	return (action == "y" || action == "Y");
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
	std::cout << "Your hand:\n";
	std::cout << user << std::flush;
	return true;
}

void play() {
	Deck d, user, dealer;
	d.shuffle(2000);
	user.empty(); dealer.empty();
	int sumUser, sumDealer, i;				// Current sum of the hands
	GameStatus gameStatus = PLAYON;

	while (1) {
		if (!openingMove(d, dealer, user)) std::cout << "Error drawing cards\n";

		sumUser = sumHand(user, true);
		std::cout << "Current sum: " << sumUser << "\n\n";
		
		//	If User has 21 already, check to see if dealer can match
		if (sumUser == 21) {
			std::cout << "\n--------------\nDealer's initial hand\n";
			std::cout << dealer;
			sumDealer = sumHand(dealer, false);
			while (sumDealer <= 21) {
				if (!dealer.addCardtoFront(d.takeEndCard()))
					std::cout << "Error occurred while drawing card\n";
				std::cout << "Dealer drew " << dealer.checkCard(0) << "\n";
				sumDealer = sumHand(dealer, false);
				std::cout << "Dealer's current sum: " << sumDealer << "\n";
			}
			if (sumDealer == 21) {
				std::cout << "Dealer also has 21. Tie game\n";
				gameStatus = TIE;
			}
			else {
				std::cout << "Dealer busts. You win\n";
				gameStatus = WIN;
			}
		}

		//	Player Turn
		while (gameStatus == PLAYON) {
			//	HIT
			if (hit()) {
				if (!user.addCardtoFront(d.takeEndCard()))
					std::cout << "Error occured while drawing card\n";
				std::cout << "You drew " << user.checkCard(0) << "\n";
				sumUser = sumHand(user, true);
				std::cout << "Current Sum: " << sumUser << "\n";

				if (sumUser > 21) {
					std::cout << "Bust. You lose\n";
					gameStatus = LOSE;
				}
				if (sumUser == 21) {
					std::cout << "Win\n";
					gameStatus = WIN;
				}
			}
			//	STAY
			else gameStatus = DEALERTURN;
		}

		//	Dealer Turn
		if (gameStatus == DEALERTURN) {
			sumDealer = 0;
			std::cout << "\n--------------\nDealer's initial hand\n";
			std::cout << dealer;

			i = 0;
			sumDealer = sumHand(dealer, false);
			std::cout << "Dealer's current sum: " << sumDealer << "\n";

			while (sumDealer < sumUser && sumDealer < 21) {
				if (!dealer.addCardtoFront(d.takeEndCard()))
					std::cout << "Error drawing card\n";
				std::cout << "Dealer drew " << dealer.checkCard(0) << "\n";
				sumDealer = sumHand(dealer, false);
				std::cout << "Dealer's current sum: " << sumDealer << "\n";
			}

			if (sumDealer > 21) {
				gameStatus = WIN;	// Player wins
				std::cout << "Dealer busts. You win\n";
			}
			else if (sumDealer >= sumUser) {
				gameStatus = LOSE;
				std::cout << "Dealer has equal or better hand. You lose\n";
			}
			else {
				std::cout << "This should never print. Inside play function in dealer's move\n";
			}
		}

		//	Player won or lost
		if (gameStatus == LOSE || gameStatus == WIN || gameStatus  == TIE) {
			if (playAgain()) {
				if (d.currentSize() < 20 || reshuffle()) {
					d.reset();
					d.shuffle(5000);
				}
				user.empty(); dealer.empty();
				gameStatus = PLAYON;
				std::cout << std::endl << "--------- New Game ---------" << std::endl;
			}
			else return;
		}
		else {
			std::cout << "THIS SHOULD NEVER PRINT. AFTER CHECKING FOR WIN OR LOSS\n\n";
			return;
		}
	}

	return;
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
