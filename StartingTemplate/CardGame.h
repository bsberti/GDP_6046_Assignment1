#pragma once

#include <string>
#include <pugixml/pugixml.hpp>

class CardGame {
public:
	CardGame();
	~CardGame();

	void Initialize(int minCard, int maxCard);
	void Destroy();

	void StartNewGame(int minCard, int maxCard);
	void Shuffle();
	void DisplayTextToUser(const std::string& text);
	void RandomizeCards();
	void ShowCards();
	void SaveGame();
	void Victory();
	void GameOver();
	void PlayerLossLifes();
	void showGameInfo();

	void loadGameLanguage();

	int HigherGuess();
	int LowerGuess();

	int currentCard;
	int guessCard;
	bool playerGuessing;
	int playerPoints;

	std::string choosenLanguage;

private:

	int minCard;
	int maxCard;

	int lifes;

	void showHeader();
	void showHowToPlay();
	void showGameRules();

	pugi::xml_document gameSave;
	pugi::xml_document gameLanguage;

	std::string header;
	std::string howToPlay;
	std::string gameRules;
	std::string showCards;
	std::string victory;
	std::string gameOver;
	std::string gameSaved;
	std::string guess;
	std::string wrongGuess;

	void loadGameState();
};