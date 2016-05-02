//--------------------------------------------------------------------
// C++ Source File - HighScorer.cpp
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

#include "Arduino.h"
#include "LocalScorer.h"

//--------------------------------------------------------------------
// Public methods
//--------------------------------------------------------------------

LocalScorer::LocalScorer()
{
    //Check if stored values are null
    if (EEPROM.read(UN1address) == EEPROM.read(40))
    {
        //If value stored for UN1, something that has to have something if value exists,
        // is equal to value in memory not used, then we must put values in? IDK, we'll just store dashes & 0's.
        Uname1 = "---";
        Uname2 = "---";
        Uname3 = "---";
        for(int i=0;i<3;i++)
        {
            EEPROM.update(UN1address + i, '-');
            EEPROM.update(UN2address + i, '-');
            EEPROM.update(UN3address + i, '-');
        }
        EEPROMWritelong(S1address,0);
        EEPROMWritelong(S2address,0);
        EEPROMWritelong(S3address,0);
    }
    
    setVolatiles();
}



void LocalScorer::setVolatiles()
{
    //Set volatile variables
    //Serial.println("Setting current EEPROM stored scores to volatile variables.");
    Uname1 = "";
    Uname2 = "";
    Uname3 = "";
    for(int i=0;i<3;i++)
    {
        name1[i] = EEPROM.read(UN1address + i);
        name2[i] = EEPROM.read(UN2address + i);
        name3[i] = EEPROM.read(UN3address + i);
        Uname1 += name1[i];
        Uname2 += name2[i];
        Uname3 += name3[i];
    }
    intScore1 = EEPROMReadlong(S1address);
    intScore2 = EEPROMReadlong(S2address);
    intScore3 = EEPROMReadlong(S3address);
}



void LocalScorer::UpdateScores(int score, String newUserName)
{
    //Visual confirmation of conversions
    /*Serial.println("Current High Scores:");
    Serial.print("First Username: ");
    Serial.println(Uname1);
    Serial.print("Second Username: ");
    Serial.println(Uname2);
    Serial.print("Third Username: ");
    Serial.println(Uname3);
    Serial.print("First Score: ");
    Serial.println(intScore1);
    Serial.print("Second Score: ");
    Serial.println(intScore2);
    Serial.print("Third Score: ");
    Serial.println(intScore3);
    delay(3000);*/
    Serial.println("Updating High Scores...");
    //Set Score position
    if (score > intScore1)
    {
        Serial.println("Score 1");
        //Move Score2 to Score3
        EEPROMWritelong(S3address, EEPROMReadlong(S2address));
        for(int i=0;i<3;i++)
        {
            EEPROM.update(UN3address + i, EEPROM.read(UN2address + i));
        }
        
        //Move Score1 to Score 2
        EEPROMWritelong(S2address, EEPROMReadlong(S1address));
        for(int i=0;i<3;i++)
        {
            EEPROM.update(UN2address + i, EEPROM.read(UN1address + i));
        }
        
        //Set new first place
        EEPROMWritelong(S1address, score);
        for(int i=0;i<3;i++)
        {
            EEPROM.update(UN1address + i, newUserName[i]);
        }
    }
    else if (score > intScore2)
    {
        Serial.println("Score 2");
        //Move Score2 to Score3
        EEPROMWritelong(S3address, EEPROMReadlong(S2address));
        for(int i=0;i<3;i++)
        {
            EEPROM.update(UN3address + i, EEPROM.read(UN2address + i));
        }
        //Set new second place
        EEPROMWritelong(S2address, score);
        for(int i=0;i<3;i++)
        {
            EEPROM.update(UN2address + i, newUserName[i]);
        }
    }
    else if (score > intScore3)
    {
        Serial.println("Score 3");
        //Set new third place
        EEPROMWritelong(S3address, score);
        for(int i=0;i<3;i++)
        {
            EEPROM.update(UN3address + i, newUserName[i]);
        }
    }
    
    setVolatiles();
}



void LocalScorer::EEPROMWritelong(int address, long value)
{
    //This function will write a 4 byte (32bit) long to the eeprom at
    //the specified address to address + 3.
    Serial.println("Writing scores to EEPROM");
    //Decomposition from a long to 4 bytes by using bitshift.
    //One = Most significant -> Four = Least significant byte
    byte four = (value & 0xFF);
    byte three = ((value >> 8) & 0xFF);
    byte two = ((value >> 16) & 0xFF);
    byte one = ((value >> 24) & 0xFF);
    
    //Write the 4 bytes into the eeprom memory.
    EEPROM.update(address, four);
    EEPROM.update(address + 1, three);
    EEPROM.update(address + 2, two);
    EEPROM.update(address + 3, one);
}



long LocalScorer::EEPROMReadlong(long address)
{
    //Read the 4 bytes from the eeprom memory.
    long four = EEPROM.read(address);
    long three = EEPROM.read(address + 1);
    long two = EEPROM.read(address + 2);
    long one = EEPROM.read(address + 3);
    
    //Return the recomposed long by using bitshift.
    return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}



bool LocalScorer::isNewHighScore(int score)
{
    setVolatiles();
    return score > intScore3;
}



//void LocalScorer::setUsername(String username) { this->username = username; }



String LocalScorer::CurrentScores()
{
    setVolatiles();
    String temp = Uname1 + " " + intScore1 + "\n" + Uname2 + " " + intScore2 + "\n" + Uname3 + " " + intScore3;
    Serial.println(temp);
    return temp;
}
