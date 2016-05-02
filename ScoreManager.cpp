//-------------------------------------------------------------------------------
// C++ Source File - ScoreManager.cpp
// Authors: J. Macvey, J. Johnson
//-------------------------------------------------------------------------------

/* This file is part of the Snake library developed for UMD's LED
Grid snake implementation

The Snake Library is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The Snake Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with the Snake Library.If not, see <http://www.gnu.org/licenses/>. */

#include "ScoreManager.h"

ScoreManager::ScoreManager(RGBmatrixPanel& matrix, Adafruit_BluefruitLE_SPI& ble, LocalScorer* localScorer) :
	difficulty(INITIAL_DIFFICULTY),
	applesLeft(INITIAL_APPLES), applesEaten(0),
	nextColorChange(INITIAL_COLOR_CHANGE), multiplier(INITIAL_MULTIPLIER),
	currentLevel(1), totalApples(0), levelDelay(INITIAL_DELAY), matrix(matrix), ble(ble),
    localScorer(localScorer)
{
	snakeColor = matrix.Color333(7, 7, 7);
	appleColor = matrix.Color333(0, 7, 0);
	matrixColor = matrix.Color333(0, 0, 0);
    
	displayManager = new LevelDisplay(matrix); // starts the display
};

ScoreManager::~ScoreManager() {
	delete displayManager;
    
	displayManager = NULL;
}

void ScoreManager::AddApple() {
	applesEaten++;
	difficulty += 10; // default difficulty for speed increase on the level
	applesLeft--;

	if (applesLeft == 0)
	{
		totalApples += applesEaten;
		applesEaten = 0;
		currentLevel++;
		displayManager->DisplayLevel(currentLevel); // informs the user
		UpdateColorChange();
	}
}

void ScoreManager::AccumulateFinalApples() {
	totalApples += applesEaten;
	applesEaten = 0;
}

int ScoreManager::getDifficulty() const
{
	return max(levelDelay - difficulty, MIN_DELAY);
}

uint16_t ScoreManager::getSnakeColor() const {
	return snakeColor;
}

uint16_t ScoreManager::getAppleColor() const {
	return appleColor;
}

uint16_t ScoreManager::getMatrixColor() const {
	return matrixColor;
}

void ScoreManager::UpdateColorChange() {
	switch (currentLevel)
	{
	case 1: // white
		snakeColor = matrix.Color333(7, 7, 7);
		appleColor = matrix.Color333(0, 7, 0);
		break;
	case 2: // purple
		snakeColor = matrix.Color333(4, 0, 7);
		appleColor = matrix.Color333(7, 7, 7);
		applesLeft = 10;
		levelDelay = 180;
		difficulty = 0;
		break;
	case 3: // blue
		snakeColor = matrix.Color333(0, 7, 7);
		appleColor = matrix.Color333(4, 0, 7);
		applesLeft = 10;
		levelDelay = 160;
		difficulty = 0;
		break;
	case 4: // green
		snakeColor = matrix.Color333(0, 7, 0);
		appleColor = matrix.Color333(0, 7, 7);
		applesLeft = 10;
		levelDelay = 150;
		difficulty = 0;
		break;
	case 5: // yellow
		snakeColor = matrix.Color333(7, 7, 0);
		appleColor = matrix.Color333(0, 7, 0);
		levelDelay = 130;
		applesLeft = 10;
		difficulty = 0;
		break;
	case 6: // orange
		snakeColor = matrix.Color333(7, 4, 0);
		appleColor = matrix.Color333(7, 7, 0);
		levelDelay = 120;
		difficulty = 0;
		break;
	case 7: // red
		snakeColor = matrix.Color333(7, 7, 0);
		appleColor = matrix.Color333(0, 7, 0);
		applesLeft = 10;
		levelDelay = 110;
		difficulty = 0;
		break;
	case 8: // inverse final level
		snakeColor = matrix.Color333(0, 0, 0);
		appleColor = matrix.Color333(0, 0, 0);
		matrixColor = matrix.Color333(2, 2, 2); // brighter white
		applesLeft = 40;
		levelDelay = 100;
		difficulty = 0;
		break;
	}
}

void ScoreManager::CheckHighScoreAndUpdate() {
	AccumulateFinalApples();

	displayManager->ClearScreen();
	matrix.setCursor(10, 5);
	matrix.print("Score: ");
	
	matrix.println(totalApples);
	delay(5000);
	String str = ConvertScoreToString(totalApples);
	String extra = "EN" + str;
	displayManager->ClearScreen();
	matrix.setCursor(0, 0);
	if (ble.isConnected())
    {
        //If high score, then do the following line
        if (localScorer->isNewHighScore(totalApples))
        {
            sendRequestForInitsAndWait(extra);
        }
    }
}

void ScoreManager::sendRequestForInitsAndWait(const String& endString) {
	ble.waitForOK();
	ble.print("AT+BLEUARTTX=");
	ble.println(endString);
	int timeDelayed = 0;
	ble.println("AT+BLEUARTRX");
	ble.readline();
	ble.waitForOK();
	char* sample = "#";
	// take 20 seconds to wait for user response
	while (timeDelayed < 20 && strstr(ble.buffer, sample) == NULL) // while no data
	{
		ble.waitForOK();
		ble.println("AT+BLEUARTRX");
		ble.readline();
		delay(1000);
		timeDelayed++;
	}
	if (strstr(ble.buffer, sample) != NULL)
	{
		String initials = "";
		for (int i = 0; i < 3; i++)
			initials += ble.buffer[i];
        localScorer->UpdateScores(totalApples, initials);
	}
	else {
		displayManager->ClearScreen();
		matrix.setCursor(0, 0);
		matrix.println("Limit Reached");
        delay(2000);
	}
}

String ScoreManager::ConvertScoreToString(int score) {

	String strBuffer = "";
	while (score != 0)
	{
		char ch = '0';
		int tens = score % 10;
		score /= 10;
		ch += (char)tens;
		strBuffer += ch;
	}
	String reverseBuffer = "";
	for (int i = strBuffer.length() - 1; i >= 0; i--)
	{
		reverseBuffer += strBuffer[i];
	}
	// final formatting of score string
	while (reverseBuffer.length() < 3)
	{
		reverseBuffer += "x";
	}
	return reverseBuffer;
}

void ScoreManager::setBle(Adafruit_BluefruitLE_SPI& ble) {
	this->ble = ble;
}

int ScoreManager::getApplesLeft() const {
	return applesLeft;
}

int ScoreManager::getApplesEaten() const {
	return applesEaten;
}



