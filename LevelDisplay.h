#ifndef LEVEL_DISPLAY_H
#define LEVEL_DISPLAY_H

//--------------------------------------------------------------------
// C++ Header File - LevelDisplay.
// Author - J. MacVey, J. Johnson
//--------------------------------------------------------------------

#include <RGBmatrixPanel.h> // needs access to matrix

// Class is responsible for displaying useful information to the user
// between levels in the game.
class LevelDisplay {

public:
	LevelDisplay(RGBmatrixPanel& matrix);
	void DisplayLevel(int level);
	void ClearScreen();
	void PromptInitials(int score);
private:
	String getNextLevelString();
	void InformStart(int level);
	void InformStringOneSecondDelay(String str, int multipler = 1);
	const String congratsString = "GREAT!";
	const String goString = "GO!";
	const int SCREEN_DELAY = 3000;
	const int ONE_SECOND = 1000; // 1000 ms = 1 sec delay
	int currentLevel;
	int centerHeight;
	RGBmatrixPanel matrix;
};

#endif