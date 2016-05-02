#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

//--------------------------------------------------------------------
// C++ Header File - ScoreManager.h
// Author - J. MacVey, J. Johnson
//--------------------------------------------------------------------

#include <RGBmatrixPanel.h> // Score needs access to matrix
#include "LevelDisplay.h" // for displaying level differences to user
#include "LocalScorer.h" // for managing and updating high scores in EEPROM
#include "Adafruit_BluefruitLE_SPI.h" // for writing end game

class ScoreManager {

public:
	ScoreManager(RGBmatrixPanel& matrix, Adafruit_BluefruitLE_SPI& ble, LocalScorer* localScorer);

	~ScoreManager();

	void AddApple();

	void UpdateColorChange();

	void CheckHighScoreAndUpdate();

	void AccumulateFinalApples();

	void setBle(Adafruit_BluefruitLE_SPI& ble);

	String ConvertScoreToString(int score);

	int getDifficulty() const; 

	int getApplesLeft() const;

	int getApplesEaten() const;

	uint16_t getSnakeColor() const;

	uint16_t getAppleColor() const;

	uint16_t getMatrixColor() const;

private:

	void sendRequestForInitsAndWait(const String& endString);
	int difficulty;
	int applesLeft;
	int applesEaten;
	int totalApples; //current score for end game
	int nextColorChange;
	int multiplier;
	int currentLevel;
	int levelDelay;

	
	RGBmatrixPanel matrix;
	uint16_t snakeColor;
	uint16_t appleColor;
	uint16_t matrixColor;

	
    LocalScorer* localScorer;
	LevelDisplay* displayManager;
	Adafruit_BluefruitLE_SPI& ble; // reference 

	const static int INITIAL_DIFFICULTY = 0;
	const static int INITIAL_APPLES = 10;
	const static int INITIAL_COLOR_CHANGE = 10;
	const static int INITIAL_MULTIPLIER = 2;
	const static int INITIAL_DELAY = 200;
	const static int MIN_DELAY = 35;

};

#endif