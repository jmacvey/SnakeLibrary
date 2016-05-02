#ifndef LocalScorer_h
#define LocalScorer_h

/*---------------------------------------------------------------------
 Header Information
 ---------------------------------------------------------------------
 C++ Header file - HighScorer.h
 Author - J. MacVey, J. Johnson
 -------------------------------------
 This library was created as an easy way to manage the high scores
 for the snake game. It allows easy way to enter a new high score
 to the attached SD card (using the SD card shield "Data logging
 shield V1.0" made by Deek-Robot. This is unconfirmed to work with
 other Arduino SD card shields, but probably will just fine.
 -------------------------------------------------------------------*/

//--------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------

#include "Arduino.h"
#include <EEPROM.h>

//--------------------------------------------------------------------
// HighScorer Class Declaration
//--------------------------------------------------------------------

class LocalScorer {
public:
    //Startup Function
    LocalScorer();
    //Will return true if score is high score worthy
    bool isNewHighScore(int score);
    //Use to Check if High Score and Update
    void UpdateScores(int score, String newUserName);
    //Returns a string in the format UN1 Score1\n...
    String CurrentScores();
    //void setUsername(String username); //not needed
    
private:
    void setVolatiles();
    void EEPROMWritelong(int address, long value);
    long EEPROMReadlong(long address);
    
    //Variables
    const int UN1address = 1;
    const int UN2address = 4;
    const int UN3address = 7;
    const int S1address = 10;
    const int S2address = 15;
    const int S3address = 20;
    
    char name1[3];
    char name2[3];
    char name3[3];
    String Uname1;
    String Uname2;
    String Uname3;
    int intScore1;
    int intScore2;
    int intScore3;
};

#endif /* LocalScorer_h */
