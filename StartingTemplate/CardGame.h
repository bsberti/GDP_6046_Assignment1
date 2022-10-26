#pragma once

#include <string>

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
	void Victory();
	void GameOver();
	void PlayerLossLifes();

	int HigherGuess();
	int LowerGuess();

	int currentCard;
	int guessCard;
	bool playerGuessing;
	int playerPoints;
private:

	int minCard;
	int maxCard;

	int lifes;

	//void loadGameState();
	//void loadGameLanguage();
};