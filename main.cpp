#include "Deck.h"
#include <iostream>
#include <ctime>

int main() {

	srand( (unsigned) time(0));
	std::string ans;

	std::cout << "There's a problem in Solitaire takeCard function\n\n\n\n";

	std::cout << "Do you want to play Solitaire? (y/n): ";
	std::cin >> ans;
	while (ans != "y" && ans != "Y" && ans != "n" && ans != "N" && ans != "yes" && ans != "Yes" && ans != "no" && ans != "No") {
		std::cout << "Invalid response. Please enter y or n: ";
		std::cin >> ans;
	}
	if (ans == "y" || ans == "Y" || ans == "yes" || ans == "Yes") Solitaire();


	return 0;
}