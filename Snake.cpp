//--------------------------------------------------------------------
// C++ Source File - Snake.cpp
// Author - J. MacVey, J. Johnson
//--------------------------------------------------------------------

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

//--------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------

#include "Snake.h"

//--------------------------------------------------------------------
// Constructors
//--------------------------------------------------------------------

Snake::Snake(RGBmatrixPanel& matrix, Adafruit_BluefruitLE_SPI& ble, int rows, int cols) :
	matrix(matrix), ble(ble), ROWS(rows), COLS(cols),
	difficulty(0), score(0), direction("RIGHT"),
    localScorer(NULL)
{
    if(localScorer==NULL) {
        localScorer = new LocalScorer();
        Serial.println("Initialized localScorer");
    }
}

Snake::~Snake() {
	while (head != NULL)
	{
		BodyPart* tmp = head;
		head = head->next;
		delete tmp;
	}
	head = NULL;
	tail = NULL;

    if(localScorer!=NULL)
        delete localScorer;
    
	delete scoreManager;
	scoreManager = NULL;
}


//--------------------------------------------------------------------
// Public methods
//--------------------------------------------------------------------

void Snake::ChangeDirection(const String& newDirection)
{
	direction = newDirection;
}


bool Snake::MoveOptimized() {
	if (GameOver())
	{
		matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 0));
		matrix.setCursor(3, 12);
		matrix.println("Game Over!");
		delay(1000);
		scoreManager->CheckHighScoreAndUpdate();
		return false;
	}
	if (isGrowing)
	{
		GrowAfterEating();
		isGrowing = false;
	}
	WrapTail();
	if (apple.x == head->col && apple.y == head->row)
		isGrowing = true;
	//delay(max(1000 / (numApples + 1), MIN_DELAY));
	delay(scoreManager->getDifficulty());
	return true;
}


void Snake::AddNewBodyPart() {
	BodyPart* newPart = CreateNewHead(apple.y, apple.x);
	// if it's the first part
	if (tail->col == head->col && tail->row == head->row) {
		tail->next = newPart;
		head = newPart;
	}
	// if it's the body
	else {
		head->next = newPart;
		head = newPart;
	}
}

//--------------------------------------------------------------------
// Private Utility Methods
//--------------------------------------------------------------------
BodyPart* Snake::CreateNewHead(int oldHeadRow, int oldHeadCol)
{
	int newHeadRow = oldHeadRow;
	int newHeadCol = oldHeadCol;
	BodyPart* newHead = new BodyPart(newHeadRow, newHeadCol, NULL);
	return newHead;
}

bool Snake::SnakeContainsPoint(int x, int y)
{
	BodyPart* part = tail;
	while (part != NULL)
	{
		if (part->col == x && part->row == y)
			return true;
		else
			part = part->next;
	}
	return false;
}

bool Snake::GameOver() {
	return (CollidesWithBoundary() || CollidesWithItself());
}

void Snake::Clear() {
	BodyPart* tmp;
	while (tail != NULL)
	{
		tmp = tail;
		tail = tail->next;
		delete tmp;
	}
}

void Snake::WrapTail() {
	// black out the tail and set the body flag
	matrix.drawPixel(tail->col, tail->row, scoreManager->getMatrixColor());

	int newHeadCol = head->col;
	int newHeadRow = head->row;
	// pre-empt the next location
	if (direction == "LEFT")
		newHeadCol--;
	else if (direction == "RIGHT")
		newHeadCol++;
	else if (direction == "UP")
		newHeadRow--;
	else
		newHeadRow++;

	BodyPart* oldTail = tail;
	// convert old tail to new head
	oldTail->col = newHeadCol;
	oldTail->row = newHeadRow;
	head->next = oldTail;
	head = oldTail;
	// assign the new tail value
	tail = tail->next;
	// do other things for symmetry in the list
	head->next = NULL;
	// light up the new head
	matrix.drawPixel(head->col, head->row, scoreManager->getSnakeColor());
}

void Snake::GrowAfterEating()
{
	scoreManager->AddApple();
	AddNewBodyPart();
	int newX = random(0, COLS - 1);
	int newY = random(0, ROWS - 1);

	while (SnakeContainsPoint(newX, newY))
	{
		newX = random(0, COLS - 1);
		newY = random(0, ROWS - 1);
	}
	apple.x = newX;
	apple.y = newY;

	matrix.drawPixel(apple.x, apple.y, scoreManager->getAppleColor());
}

bool Snake::CollidesWithBoundary() {
	return head->col < 0 || head->col > COLS ||
		head->row < 0 || head->row > ROWS;
}

bool Snake::CollidesWithItself()
{
	BodyPart* part = tail;
	while (part != head)
	{
		if (part->col == head->col && part->row == head->row)
			return true;
		part = part->next;
	}
	return false;
}

void Snake::Splash() {
	for (int iii = 0; iii < 4; iii++)
	{
		if (ble.isConnected())
			return;
		SplashPartOne();
	}
	if (ble.isConnected())
		return;
	SplashPartTwo();
}

void Snake::SplashPartOne() {
    
	matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 0));
	matrix.setTextSize(1);     // size 1 == 8 pixels high
	matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves
							   //Create Array of column names
	int ColumnNameArray[64];
	for (int i = 0; i < 64; i++)
		ColumnNameArray[i] = i;
	//Grab size of array
	const size_t n = sizeof(ColumnNameArray) / sizeof(ColumnNameArray[0]);
	//Shuffle array contents
	for (size_t i = 0; i < n - 1; i++)
	{
		// check for bluetooth connection
		if (ble.isConnected())
			return;
		size_t j = random(1, n - i);
		int t = ColumnNameArray[i];
		ColumnNameArray[i] = ColumnNameArray[j];
		ColumnNameArray[j] = t;
	}

	//Rotate through the random column selection array
	for (int temp1 = 0; temp1 < 64; temp1++)
	{
		if (ble.isConnected())
			return;
		//Traverse down the column
		for (int temp2 = 0; temp2 < 32; temp2++)
		{
			delay(3);
			matrix.drawPixel(ColumnNameArray[temp1], temp2, matrix.Color333(0, 0, 1));
		}
	}
	matrix.setCursor(2, 4);
	matrix.setTextColor(matrix.Color333(7, 7, 7));
	matrix.println("Play Snake");
	for (int i = 0; i < 5; i++)
	{
		if (ble.isConnected())
			return;
		delay(100);
	}
	for (int textmove = 4; textmove < 32; textmove++)
	{

		matrix.setTextColor(matrix.Color333(7, 7, 7));
		if (ble.isConnected())
			return;
		matrix.setCursor(2, textmove);
		matrix.println("Play Snake");
		delay(130);

		matrix.setTextColor(matrix.Color333(1, 0, 0));
		matrix.setCursor(2, textmove);
		matrix.println("Play Snake");
		matrix.setTextColor(matrix.Color333(7, 7, 7));

	}
	for (int i = 0; i < 10; i++)
	{
		if (ble.isConnected())
			return;
	}
	for (int temp1 = 63; temp1 >= 0; temp1--)
	{
		if (ble.isConnected())
			return;
		//Traverse down the column
		for (int temp2 = 31; temp2 >= 0; temp2--)
		{
			//matrix.drawPixel(ColumnNameArray[temp1], temp2, matrix.Color333(random(0,7), random(0,7), random(0,7)));
			delay(1);
			matrix.drawPixel(ColumnNameArray[temp1], temp2, matrix.Color333(0, 0, 7));
		}
	}
	for (int i = 0; i < 10; i++) {
		if (ble.isConnected())
			return;
	}
	matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 6));
	delay(50);
	matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 5));
	delay(50);
	matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 4));
	delay(50);
	matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 3));
	delay(50);
	matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 2));
	delay(50);
	matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 1));
	delay(500);
	for (int temp1 = 0; temp1 < 64; temp1++)
	{
		if (ble.isConnected())
			return;
		//Traverse down the column
		for (int temp2 = 0; temp2 < 32; temp2++)
		{
			//matrix.drawPixel(ColumnNameArray[temp1], temp2, matrix.Color333(random(0,7), random(0,7), random(0,7)));
			//delay(3);
			matrix.drawPixel(ColumnNameArray[temp1], temp2, matrix.Color333(0, 0, 0));
		}
		delay(10);
	}
	for (int i = 0; i < 20; i++)
	{
		if (ble.isConnected())
			return;
		delay(100);
	}
    
    //Display High Scores
    matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 0));
    matrix.setTextColor(matrix.Color333(7, 7, 7));
    matrix.fillRect(0, 9, 64, 2, matrix.Color333(1, 0, 2));
    matrix.setCursor(0, 12);
    matrix.println("High Scores");
    matrix.fillRect(0, 21, 64, 2, matrix.Color333(1, 0, 2));
    delay(1000);
    matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 0));
    printHighScores(localScorer->CurrentScores());
    matrix.setCursor(2, 23);
    //matrix.println("J. Johnson");
    
    for (int i = 0; i < 75; i++) {
        if (ble.isConnected())
            return;
        delay(100);
    }
    matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 0));
}

void Snake::printHighScores(const String& highScoreString) {
    int strLength = highScoreString.length();
    int beginString = 0, currentWord = 0;
    matrix.fillRect(2, 2, 60, 1, matrix.Color333(1, 0, 2));

    for (int i = 0; i < strLength; i++) {
        if ((highScoreString[i] == '\n') || (i+1 == strLength))
        {
            matrix.setCursor(14, 5 + 8*currentWord);
            if (i+1 == strLength)
                matrix.println(highScoreString.substring(beginString, i + 1));
            else
                matrix.println(highScoreString.substring(beginString, i));

            beginString = i+1;
            currentWord++;
        }
    }
        matrix.fillRect(2, 6 + 8*(currentWord), 60, 1, matrix.Color333(1, 0, 2));
}

void Snake::SplashPartTwo() {


	matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 0));
	matrix.setTextColor(matrix.Color333(7, 7, 7));
	matrix.setCursor(2, 2);
	matrix.println("Created By");
	matrix.fillRect(2, 10, 60, 1, matrix.Color333(1, 0, 2));
	matrix.setCursor(5, 14);
	matrix.println("J. MacVey");
	matrix.setCursor(2, 23);
	matrix.println("J. Johnson");

	for (int i = 0; i < 75; i++) {
		if (ble.isConnected())
			return;
		delay(100);
	}
	matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 0));
    
    //Credit to Bret for making stand
    matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 0));
    matrix.setTextColor(matrix.Color333(7, 7, 7));
    matrix.setCursor(2, 2);
    matrix.println("Stand From");
    matrix.fillRect(2, 10, 60, 1, matrix.Color333(1, 0, 2));
    matrix.setCursor(12, 14);
    matrix.println("B. Cuda");
    matrix.setCursor(2, 23);
    
    for (int i = 0; i < 30; i++) {
        if (ble.isConnected())
            return;
        delay(100);
    }
    matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 0));
    
    //playDemoGame()
	//Cycle through Levels and around 15 seconds per level
        //This can be something people can add in the future
}

void Snake::Start() {
	tail = new BodyPart(16, 16, NULL);
	head = tail;
	scoreManager = new ScoreManager(matrix, ble, localScorer);
	scoreManager->setBle(ble);
	matrix.fillRect(0, 0, matrix.width(), matrix.height(), scoreManager->getMatrixColor());
	matrix.drawPixel(apple.x, apple.y, scoreManager->getAppleColor());
}