//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: TextScroll.cpp                             *
// * Description: Implementation of the TextScroll         *
// * class.                                                *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#include "stdafx.h"

#ifndef TEXTSCROLL_H
#include "TextScroll.h"
#endif

#ifndef FONTLIST_H
#include "FontList.h"
#endif

#include <iostream>
#include <fstream>

using namespace std; 

extern ofstream logFile;

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: TextScroll::TextScroll                             */
/*                                                                 */
/*******************************************************************/
TextScroll::TextScroll()
{
    textStringLength = 0;
    scrollQuickness  = 2000;
    currentXOffset   = 0.0;

    for(int i=0 ; i<MAX_TEXT_CHARACTERS ; ++i)
	{
        textString[i].startPosition  = 0;
        textString[i].endPosition    = 0;
        textString[i].characterIndex = 0;
	}
logFile << "TextScroll: " << currentXOffset << endl;
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: TextScroll::~TextScroll                            */
/*                                                                 */
/*******************************************************************/
TextScroll::~TextScroll()
{
    return;
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: TextScroll::SetNextXOffset                         */
/*                                                                 */
/*******************************************************************/
void
TextScroll::SetNextXOffset(void)
{
    double newXOffset = currentXOffset - scrollQuickness;

    if((textString[textStringLength-1].endPosition + newXOffset) >= (double)TEXT_WORLD_MIN)
	{
        currentXOffset = newXOffset;
	}
    else
	{
        currentXOffset = (double)TEXT_WORLD_MAX;
	}
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: TextScroll::ReadTextFile                           */
/*                                                                 */
/*******************************************************************/
M_UINT16
TextScroll::ReadTextFile(FILE* textFile)
{
    M_INT16 i, ch, charIndex;
	double currentStartPosition = 0;

	textStringLength = 0;
	currentXOffset = TEXT_WORLD_MAX;

    if(!FontList::Ok())
	{
	    printf("ReadTextFile(): Font info not available.");
	    return(M_FAILURE);
	}
	
    if (!textFile)
	{
	    printf("ReadTextFile(): NULL file pointer.");
	    return(M_FAILURE);
	}
	
	ch = fgetc(textFile);
	
    for (i=0 ; (i<MAX_TEXT_CHARACTERS) && feof(textFile)==0 ; ++i)
	{
	    if(ch>=FONT_CHAR_MIN && ch<=FONT_CHAR_MAX)
		{
	        charIndex = ch - FONT_CHAR_MIN;
		}
		else if(ch==0x0a || ch==0x0d)
	    {
	        charIndex = 0x20 - FONT_CHAR_MIN;
		}
		else
		{
		    printf("Character number %d has value 0x%x [%c] which is not included in font table.\r\n",
			        i, (M_INT16)ch, ch);
	        charIndex = 0x20 - FONT_CHAR_MIN;
		}
		
	    textString[i].characterIndex = charIndex;
	    textString[i].startPosition = currentStartPosition;
		textString[i].endPosition = textString[i].startPosition + (FontList::GetFontCharacter(charIndex)->xMaxValue - FontList::GetFontCharacter(charIndex)->xMinValue);

// logFile << "Character: " << i
//        << ", Index: " << textString[i].characterIndex
//		<< ", Character MIN/MAX: " << FontList::GetFontCharacter(charIndex)->xMinValue << "/" << FontList::GetFontCharacter(charIndex)->xMaxValue
//		<< ", Start: " << textString[i].startPosition
//		<< ", End: " << textString[i].endPosition << endl;

        currentStartPosition = textString[i].endPosition + 2500;
		ch = fgetc(textFile);		
	}
	
	textStringLength = i-1;
	return(M_SUCCESS);	

}
