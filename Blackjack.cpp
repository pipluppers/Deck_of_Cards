#include "Deck.h"

bool confirmPlay();
void play();
int sumHand(Deck);			// Sum of hand

void BlackJack() {
	if (!confirmPlay()) {
		std::cout << "Exiting blackjack\n";
		return;
	}

	play();

	std::cout << "Thank you for playing. Bye ^.^\n";
}

bool confirmPlay() {
	std::string ans;

	std::cout << "Ready to play Blackjack? (y/n)\n";
	std::cin >> ans;
	while (ans != "y" && ans != "Y" && ans != "n" && ans != "N") {
		std::cout << "Invalid response. Please enter y or n: ";
		std::cin >> ans;
	}
	return (ans == "y" || ans == "Y");
}


#define PLAYON 0
#define WIN 1
#define LOSE 2
#define DEALERTURN 3

void play() {
	Deck d, user, dealer;
	d.shuffle(2000);
	user.empty(); dealer.empty();
	std::string action;
	int sumUser, sumDealer;			// Current sum of the hands
	int i; bool dealerAce = false;	// True if dealer has ace
	int gameStatus = PLAYON;		// 0 for game on, 1 for lose, 2 for win
	bool keepPlaying = true;

	while (keepPlaying) {
		dealerAce = false;

		//	Setup for the first move
		dealer.addCardtoFront(d.takeCard(d.currentSize()));
		dealer.addCardtoFront(d.takeCard(d.currentSize()));
		std::cout << "\nTop of Dealer's deck\n";
		std::cout << dealer.checkCard(1);		// CheckCard and TakeCard start from 1 to current_size
		std::cout << "\n\n";
		user.addCardtoFront(d.takeCard(d.currentSize()));
		user.addCardtoFront(d.takeCard(d.currentSize()));
		std::cout << "Your hand:\n";
		std::cout << user << std::flush;
		sumUser = sumHand(user);
		std::cout << "Current sum: " << sumUser << "\n\n";

		//	Player Turn
		while (gameStatus == PLAYON) {
			action = "";
			while (action != "1" && action != "2") {
				std::cout << "Action:\n - 1. hit\n - 2. stay\n";
				std::cin >> action;
			}
			//	HIT
			if (action == "1") {
				user.addCardtoFront(d.takeCard(d.currentSize()));
				std::cout << "You drew " << user.checkCard(1) << "\n";
				sumUser = sumHand(user);
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
			else {
				gameStatus = DEALERTURN;
			}
		}

		//	Dealer Turn
		if (gameStatus == DEALERTURN) {
			sumDealer = 0;
			std::cout << "\n--------------\nDealer's initial hand\n";
			std::cout << dealer;

			i = 0;
			for (i = 0; i < dealer.currentSize(); ++i) {
				sumDealer += dealer.checkCard(i + 1).num + 1;		//	The +1's accounts for checkCard and nums starting at 0
				if (!dealerAce && dealer.checkCard(i + 1).num == 0) dealerAce = true;
			}

			while (sumDealer < sumUser && sumDealer < 21) {
				if (dealerAce && sumDealer == 11) {
					sumDealer = 21;	//	Dealer has won
					break;
				}
				sumDealer = 0;
				dealer.addCardtoFront(d.takeCard(d.currentSize()));
				std::cout << "Dealer drew " << dealer.checkCard(1) << "\n";
				for (i = 0; i < dealer.currentSize(); ++i) {
					if (dealer.checkCard(i + 1).num > 9) {
						sumDealer += 10;		// FACE CARD
					}
					else {
						sumDealer += dealer.checkCard(i + 1).num + 1;		//	The +1's accounts for checkCard and nums starting at 0
						if (!dealerAce && dealer.checkCard(i + 1).num == 0) dealerAce = true;
					}
				}
				std::cout << "Dealer's current sum: " << sumDealer << "\n";

			}
			if (sumDealer > 21) {
				gameStatus = WIN;	// Player wins
				std::cout << "You win\n";
			}
			else if (sumDealer >= sumUser) {
				gameStatus = LOSE;
				std::cout << "You lose\n";
			}
			else {
				std::cout << "This should never print. Inside play function in dealer's move\n";
			}
		}

		//	Player won or lost
		if (gameStatus == LOSE || gameStatus == WIN) {
			action = "";
			while (action != "y" && action != "Y" && action != "n" && action != "N") {
				std::cout << "Do you want to play again? (y/n)\n";
				std::cin >> action;
			}
			if (action == "N" || action == "n") {
				keepPlaying = false;	// not really necessary but whatever
				return;
			}
			else {
				action = "";
				while (action != "y" && action != "Y" && action != "n" && action != "N") {
					std::cout << "Do you want to reshuffle the cards back into the deck? (y/n)\n";
					std::cin >> action;
				}
				if (action == "y" || action == "Y") {
					d.reset();
					d.shuffle(5000);
				}
				user.empty(); dealer.empty();
			}
			gameStatus = PLAYON;
		}
		else {
			std::cout << "THIS SHOULD NEVER PRINT. AFTER CHECKING FOR WIN OR LOSS\n\n";
			return;
		}
	}


	return;
}

int sumHand(Deck d) {
	int i = 0, counter = 0;
	std::string ans;

	for (; i < d.currentSize(); ++i) {
		//	If ACE
		if (d.checkCard(i + 1).num == 0) {
			while (ans != "1" && ans != "2") {
				std::cout << "Do you want this ace to be worth 1 or 11? (Please enter 1 for 1 and 2 for 11)\n";
				std::cin >> ans;
			}
			if (ans == "1") counter += 1;
			else counter += 11;
		}
		else {
			if (d.checkCard(i + 1).num > 9) {
				counter += 10;
			}
			else {
				counter += d.checkCard(i + 1).num + 1;		//	The +1 accounts for cards starting at 0
			}
		}
	}
	return counter;
}