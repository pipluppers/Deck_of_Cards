#include "Deck.h"
#include <iostream>
#include <ctime>

int main() {

	srand( (unsigned) time(0));
	std::string action;


	std::cout << "What do you want to play?\n1. Solitaire\n2. BlackJack\n3. Quit";
	std::cin >> action;
	while (action != "1" && action != "2") {
		std::cout << "Invalid response. Please enter \"1\" for Solitaire or \"2\" for BlackJack or \"3\" for Quit: ";
		std::cin >> action;
		std::cout << std::endl;
	}
	
	if (action == "1") Solitaire();
	else if (action == "2") BlackJack();

	return 0;
}
