//	Property of Alex Nguyen (nguyenalex24@gmail.com)

#include "Deck.h"
#include <iostream>
#include <ctime>

void menu() {
	std::string action;
	std::cout << "What do you want to play?\n - 1. Solitaire\n - 2. Blackjack\n - 3. Quit\n";
	std::cin >> action;
	while (action != "1" && action != "2" && action != "3") {
		std::cout << "Invalid response. Please enter \"1\" for Solitaire or \"2\" for Blackjack or \"3\" for Quit: ";
		std::cin >> action;
		std::cout << std::endl;
	}

	if (action == "1") Solitaire();
	else if (action == "2") Blackjack();
}

int main() {

	srand( (unsigned) time(0));

	menu();

	return 0;
}
