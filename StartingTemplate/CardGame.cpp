#include "CardGame.h"
#include <iostream>

bool gameOver = false;

CardGame::CardGame() : lifes(0), 
currentCard(0), 
guessCard(0), 
minCard(0), 
maxCard(0), 
playerGuessing(false), 
playerPoints(0) {
	loadGameState();
}

void CardGame::loadGameState() {
	pugi::xml_parse_result result = gameSave.load_file("gameSave.xml");
	if (!result) {
		printf("Failed to load 'gameSave.xml'\n");
	}

	pugi::xml_node saveNode = gameSave.child("gameSave");
	pugi::xml_attribute aPlayerLifes = saveNode.attribute("playerLifes");
	pugi::xml_attribute aPlayerPoints = saveNode.attribute("playerPoints");

	lifes = std::stoi(aPlayerLifes.value());
	playerPoints = std::stoi(aPlayerPoints.value());
}

void CardGame::loadGameLanguage() {
	pugi::xml_parse_result result = gameLanguage.load_file("gameLanguage.xml");
	if (!result) {
		printf("Failed to load 'gameLanguage.xml'\n");
	}

	pugi::xml_object_range<pugi::xml_node_iterator> language = gameLanguage.child("gameLanguage").children();
	if (language.empty()) {
		printf("There is no sounds in the soundList\n");
	}

	pugi::xml_node_iterator languageIt;
	for (languageIt = language.begin(); languageIt != language.end(); languageIt++) {
		pugi::xml_node languageNode = *languageIt;

		pugi::xml_attribute imputLanguage = languageNode.attribute("language");
		if(imputLanguage.value() == choosenLanguage) {

			pugi::xml_node headerNode = languageNode.child("header");
			header = headerNode.child_value();

			pugi::xml_node howToPlayNode = languageNode.child("howToPlay");
			howToPlay = howToPlayNode.child_value();

			pugi::xml_node gameRulesNode = languageNode.child("gameRules");
			gameRules = gameRulesNode.child_value();

			pugi::xml_node showCardsNode = languageNode.child("showCards");
			showCards = showCardsNode.child_value();

			pugi::xml_node victoryNode = languageNode.child("victory");
			victory = victoryNode.child_value();

			pugi::xml_node gameOverNode = languageNode.child("gameOver");
			gameOver = gameOverNode.child_value();

			pugi::xml_node gameSavedNode = languageNode.child("gameSaved");
			gameSaved = gameSavedNode.child_value();

			pugi::xml_node guessNode = languageNode.child("guess");
			guess = guessNode.child_value();

			pugi::xml_node wrongGuessNode = languageNode.child("wrongGuess");
			wrongGuess = wrongGuessNode.child_value();
		}
	}
}

void CardGame::showHeader() {
	std::cout << header;
	//std::cout << ""
	//	" _______  _______  ______   ______     _______  _______  __   __  _______    __  \n"
	//	"|       ||   _   ||    _ | |      |   |       ||   _   ||  |_|  ||       |  |  | \n"
	//	"|       ||  |_|  ||   | || |  _    |  |    ___||  |_|  ||       ||    ___|  |  | \n"
	//	"|      _||       ||   |_||_| | |   |  |   | __ |       ||       ||   |___   |  | \n"
	//	"|     |_ |       ||    __  | |_|   |  |   ||  ||       ||       ||    ___|  |__| \n"
	//	"|       ||   _   ||   |  | |       |  |   |_| ||   _   || ||_|| ||   |___    __  \n"
	//	"|_______||__| |__||___|  |_|______|   |_______||__| |__||_|   |_||_______|  |__| \n"
	//	"";
}

void CardGame::showHowToPlay() {
	std::cout << howToPlay;
	//std::cout << "\n >>>> HOW TO PLAY <<<<\n"
	//	"Press SPACEBAR to shuffle de deck\n"
	//	"Press N to Start a New Game\n"
	//	"Press S to Save your Game\n"
	//	"Press UP ARROW to guess higher card\n"
	//	"Press DOWN ARROW to guess lower card\n"
	//	"Press I to see the game Rules and Commands again\n";
}

void CardGame::showGameRules() {
	std::cout << gameRules;
	//std::cout << "\n >>>> GAME RULES <<<<\n"
	//"Step 1 - The program will select one card and show it.\n"
	//	"Step 2 - The program will select other card and hide the other one\n"
	//	"Step 3 - You need to guess the if the number is higher or lower\n\n"
	//	"You have 3 lifes to guess the card\n"
	//	"Each right guess grants you 1 point\n";
}

void CardGame::PlayerLossLifes() {
	printf(wrongGuess.c_str(), lifes);
	//std::cout << "Wrong guess!!! You have " << lifes << " more attempts!\n";
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
	printf(showCards.c_str(), currentCard);
	//std::cout << "\nFirst Card:\t";
	//std::cout << currentCard << "\n";
	//std::cout << "Second Card:\t ? ? ? \n";
	//std::cout << "Now guess if the second card is higher or lower than the first card\n";
}

void CardGame::Victory() {
	playerPoints++;
	printf(victory.c_str(), playerPoints);
	//std::cout << "CONGRATULATIONS, right guess!!!\n";
	//std::cout << "You have " << playerPoints << " TOTAL POINTS!";
}

void CardGame::GameOver() {
	std::cout << "GAME OVER!!!\n";
}

void CardGame::SaveGame() {
	pugi::xml_parse_result result = gameSave.load_file("gameSave.xml");
	if (!result) {
		printf("Failed to load 'gameSave.xml'\n");
	}

	pugi::xml_node saveNode = gameSave.child("gameSave");
	pugi::xml_attribute aPlayerLifes = saveNode.attribute("playerLifes");
	pugi::xml_attribute aPlayerPoints = saveNode.attribute("playerPoints");

	aPlayerLifes.set_value(lifes);
	aPlayerPoints.set_value(playerPoints);
	
	gameSave.save_file("gameSave.xml");

	std::cout << gameSaved;
	//std::cout << "\n\nGame State saved sucessfully\n";
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

void CardGame::showGameInfo() {
	showHeader();
	showHowToPlay();
	showGameRules();
}

void CardGame::chooseGameLanguage() {
	std::cout << "Please select game language:\n";
	std::cout << "1 - English\n";
	std::cout << "2 - Portuguese\n";
	std::cout << "3 - Spanish\n";
	std::cout << "4 - Deutsch\n";
	std::cout << "5 - French\n";

	int playerChoice;
	std::cin >> playerChoice;

	switch (playerChoice) {
	case 1:
		choosenLanguage = "English";
		break;
	case 2:
		choosenLanguage = "Portuguese";
		break;
	case 3:
		choosenLanguage = "Spanish";
		break;
	case 4:
		choosenLanguage = "Deutsch";
		break;
	case 5:
		choosenLanguage = "French";
		break;
	default:
		break;
	}
	system("CLS");
	loadGameLanguage();
}

void CardGame::Initialize(int minCard, int maxCard) {
	chooseGameLanguage();
	showGameInfo();

	this->minCard = minCard;
	this->maxCard = maxCard;

	RandomizeCards();
	ShowCards();
	playerGuessing = true;
}

void CardGame::Destroy() {

}

void CardGame::StartNewGame(int minCard, int maxCard) {
	showGameInfo();

	this->minCard = minCard;
	this->maxCard = maxCard;

	RandomizeCards();
	ShowCards();
	playerGuessing = true;

	playerPoints = 0;
	lifes = 3;
	SaveGame();
}

void CardGame::Shuffle() {
	//Show one card and ask for guess

	if (!playerGuessing) {
		system("CLS");
		showHeader();
		RandomizeCards();
		ShowCards();
		playerGuessing = true;
	}
	else {
		std::cout << guess;
		//std::cout << "Now guess if the second card is higher or lower than the first card\n";
	}
}

void CardGame::DisplayTextToUser(const std::string& text) {

}
