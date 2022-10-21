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

	bool HigherGuess();
	bool LowerGuess();

	int currentCard;
	int guessCard;
private:

	int minCard;
	int maxCard;

	int lifes;
};