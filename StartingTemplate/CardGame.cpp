#include "CardGame.h"
#include <iostream>

bool gameOver = false;

CardGame::CardGame() : lifes(3), currentCard(0), guessCard(0), minCard(0), maxCard(0) {

}

void header() {
	std::cout << ""
		" _______  _______  ______   ______     _______  _______  __   __  _______    __  \n"
		"|       ||   _   ||    _ | |      |   |       ||   _   ||  |_|  ||       |  |  | \n"
		"|       ||  |_|  ||   | || |  _    |  |    ___||  |_|  ||       ||    ___|  |  | \n"
		"|      _||       ||   |_||_| | |   |  |   | __ |       ||       ||   |___   |  | \n"
		"|     |_ |       ||    __  | |_|   |  |   ||  ||       ||       ||    ___|  |__| \n"
		"|       ||   _   ||   |  | |       |  |   |_| ||   _   || ||_|| ||   |___    __  \n"
		"|_______||__| |__||___|  |_|______|   |_______||__| |__||_|   |_||_______|  |__| \n"
		"";
}

void howToPlay() {
	std::cout << "\n\nHow to play:\n"
		"Press SPACEBAR to shuffle de deck\n"
		"Press N to Start a New Game\n"
		"The program will select one card and show it.\n"
		"The program will select other card and you need to guess the if the number is higher or lower...\n"
		"You have 3 chances to guess the card  \n"
		"										\n"
		"Press the command now:																			\n"
		""
		"";
}

void CardGame::PlayerLossLifes() {
	std::cout << "Wrong guess!!! You have " << lifes << " more attempts!\n";
}

CardGame::~CardGame() {

}

float MyRandom(int min, int max) {
	if (max == min)
		return 0;

	int diff = (max - min) * 1000;
	return min + (rand() % diff) / 1000;
}

void CardGame::RandomizeCards() {
	currentCard = MyRandom(minCard, maxCard);
	do {
		guessCard = MyRandom(minCard, maxCard);
	} while (guessCard == currentCard);
}

void CardGame::ShowCards() {
	std::cout << "First Card:\n";
	std::cout << currentCard << "\n";
	std::cout << "Second Card:\n";
	std::cout << "???\n\n";
	std::cout << "Now guess if the second card is higher or lower than the first card\n";
}

void CardGame::Victory() {
	std::cout << "CONGRATULATIONS, you win!!!\n";
}

void CardGame::GameOver() {
	std::cout << "GAME OVER!!!\n";
}

bool CardGame::HigherGuess() {
	if (currentCard < guessCard) {
		Victory();
		return true;
	}	
	else {
		lifes -= 1;
		PlayerLossLifes();
		if (lifes <= 0)
			GameOver();

		return false;
	}
}

bool CardGame::LowerGuess() {
	if (currentCard > guessCard) {
		Victory();
		return true;
	}
	else {
		lifes -= 1;
		PlayerLossLifes();
		if (lifes <= 0)
			GameOver();

		return false;
	}
		
}

void CardGame::Initialize(int minCard, int maxCard) {
	header();
	howToPlay();

	this->minCard = minCard;
	this->maxCard = maxCard;
}

void CardGame::Destroy() {

}

void CardGame::StartNewGame(int minCard, int maxCard) {
	header();
	howToPlay();

	this->minCard = minCard;
	this->maxCard = maxCard;
}

void CardGame::Shuffle() {
	//Show one card and ask for guess

	RandomizeCards();
	ShowCards();
}

void CardGame::DisplayTextToUser(const std::string& text) {

}
