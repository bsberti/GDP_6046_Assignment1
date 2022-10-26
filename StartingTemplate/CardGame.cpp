#include "CardGame.h"
#include <iostream>

bool gameOver = false;

CardGame::CardGame() : lifes(3), 
currentCard(0), 
guessCard(0), 
minCard(0), 
maxCard(0), 
playerGuessing(false), 
playerPoints(0) {
	//loadGameState();
	//loadGameLanguage();
}

//void CardGame::loadGameState() {}

//void CardGame::loadGameLanguage() {}

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
	std::cout << "First Card:\t";
	std::cout << currentCard << "\n";
	std::cout << "Second Card:\t ? ? ? \n\n";
	std::cout << "Now guess if the second card is higher or lower than the first card\n";
}

void CardGame::Victory() {
	std::cout << "CONGRATULATIONS, right guess!!!\n";
}

void CardGame::GameOver() {
	std::cout << "GAME OVER!!!\n";
}

int CardGame::HigherGuess() {
	playerGuessing = false;
	if (currentCard < guessCard) {
		Victory();
		return 0;
	}	
	else {
		playerGuessing = true;
		lifes -= 1;
		PlayerLossLifes();
		if (lifes <= 0) {
			GameOver();
			return 2;
		}

		return 1;
	}
}

int CardGame::LowerGuess() {
	playerGuessing = false;
	if (currentCard > guessCard) {
		Victory();
		return 0;
	}
	else {
		playerGuessing = true;
		lifes -= 1;
		PlayerLossLifes();
		if (lifes <= 0) {
			GameOver();
			return 2;
		}

		return 1;
	}
		
}

void CardGame::Initialize(int minCard, int maxCard) {
	header();
	howToPlay();

	this->minCard = minCard;
	this->maxCard = maxCard;

	RandomizeCards();
	ShowCards();
	playerGuessing = true;
}

void CardGame::Destroy() {

}

void CardGame::StartNewGame(int minCard, int maxCard) {
	header();
	howToPlay();

	playerGuessing = false;

	this->minCard = minCard;
	this->maxCard = maxCard;
}

void CardGame::Shuffle() {
	//Show one card and ask for guess

	if (!playerGuessing) {
		RandomizeCards();
		ShowCards();
		playerGuessing = true;
	}
	else {
		std::cout << "Now guess if the second card is higher or lower than the first card\n";
	}
}

void CardGame::DisplayTextToUser(const std::string& text) {

}
