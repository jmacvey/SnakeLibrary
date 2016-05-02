#ifndef SNAKE_H
#define SNAKE_H

//--------------------------------------------------------------------
// C++ Header File - Snake.h
// Author - J. MacVey, J. Johnson
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------

#include "ScoreManager.h"
#include "BodyPart.h"
#include "Arduino.h"
#include <RGBmatrixPanel.h> // Hardware-specific library
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h" // needed for splash screen interruptions
#include "Apple.h"
#include "LocalScorer.h"

//--------------------------------------------------------------------
// Snake Class Declaration
//--------------------------------------------------------------------

// This class represents the classic snake game with respective algorithms.  This class
// wraps around the RGBmatrixPanel to facilitate functionality with the Adafruit RGB LED
// Grid to produce the snake display. 

class Snake {
public:

	//----------------------------------------------------------------
	// Constructor/Destructor
	//----------------------------------------------------------------

	Snake(RGBmatrixPanel& matrix, Adafruit_BluefruitLE_SPI& ble, int rows, int cols);

	~Snake();

	void ChangeDirection(const String& newDirection); 

	bool MoveOptimized();

	void AddNewBodyPart();

	void Splash();

	void Start();

	void Clear();

private:

	//--------------------------------------------------------------------
	// Utility Methods
	//--------------------------------------------------------------------
	

	BodyPart* CreateNewHead(int oldHeadRow, int oldHeadCol);

	bool SnakeContainsPoint(int x, int y);

	bool GameOver();

	void WrapTail();

	void GrowAfterEating();

	bool CollidesWithBoundary();

	bool CollidesWithItself();

	void SplashPartOne();
	void SplashPartTwo();
    void printHighScores(const String& highScoreString);
	//--------------------------------------------------------------------
	// Data
	//--------------------------------------------------------------------

	// Initialized via constructor initialization list
	const int ROWS;
	const int COLS;
	String direction;
	int difficulty;
	int score;
	BodyPart* tail;
	BodyPart* head;

	RGBmatrixPanel matrix;
	Adafruit_BluefruitLE_SPI& ble;
	
	// Score manager
	ScoreManager* scoreManager;

	const int MIN_DELAY = 50;
	const int START_X = 16;
	const int START_Y = 16;
	Apple apple = Apple(24, 24);
	bool isGrowing = false;

    LocalScorer* localScorer;
};




#endif
