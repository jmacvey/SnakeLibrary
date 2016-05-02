//-------------------------------------------------------------------------------
// C++ Source File - LevelDisplay.cpp
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

#include "LevelDisplay.h"

LevelDisplay::LevelDisplay(RGBmatrixPanel& matrix) : currentLevel(1), matrix(matrix)
{
	matrix.setTextSize(1);
	matrix.setTextColor(matrix.Color333(7, 7, 7));
	centerHeight = matrix.height() / 2 - 4;
	matrix.setCursor(10, centerHeight);
	matrix.println("Welcome!");
	delay(ONE_SECOND * 3);
	ClearScreen();
	InformStart(currentLevel);
}

void LevelDisplay::DisplayLevel(int level) {
	ClearScreen();
	matrix.setTextColor(matrix.Color333(7, 7, 7));
	matrix.setCursor(16, centerHeight);
	matrix.println(congratsString);
	delay(SCREEN_DELAY);
	ClearScreen();
	currentLevel++;
	delay(SCREEN_DELAY);
	InformStart(currentLevel);
}

void LevelDisplay::ClearScreen() {
	matrix.fillRect(0, 0, matrix.width(),
		matrix.height(), matrix.Color333(0, 0, 0));
}

void LevelDisplay::PromptInitials(int score) {
	ClearScreen();
	delay(ONE_SECOND);
	matrix.print("Score: ");
	//matrix.println(score);
	delay(ONE_SECOND * 4);
	ClearScreen();
	matrix.println("Enter Initials");
}

void LevelDisplay::InformStart(int level) {
	if (level != 1)
	{
		String str = getNextLevelString();
		if (str.length() == 11)
		{
			matrix.setCursor(18, centerHeight - 5);
			matrix.println("FINAL");
			matrix.setCursor(18, centerHeight + 5);
			matrix.println("LEVEL");
			delay(ONE_SECOND * 2);
			ClearScreen();
		}
		else
			InformStringOneSecondDelay(str, 2);
	}
	InformStringOneSecondDelay("3");
	InformStringOneSecondDelay("2");
	InformStringOneSecondDelay("1");
	InformStringOneSecondDelay(goString);
}

void LevelDisplay::InformStringOneSecondDelay(String str, int multiplier) {
	int startX = (matrix.width()) / 2 - 3 * (str.length());
	int startY = (matrix.height() - 8) / 2;
	matrix.setCursor(startX, startY);
	matrix.println(str);
	delay(ONE_SECOND * multiplier);
	ClearScreen();
}

String LevelDisplay::getNextLevelString() {
	String result;
	switch (currentLevel)
	{
	case 1:
		result = "Level 1";
		break;
	case 2:
		result = "Level 2";
		break;
	case 3:
		result = "Level 3";
		break;
	case 4:
		result = "Level 4";
		break;
	case 5:
		result = "Level 5";
		break;
	case 6:
		result = "Level 6";
		break;
	case 7:
		result = "Level 7";
		break;
	case 8:
		result = "FINAL LEVEL";
		break;
	}
	return result;
}