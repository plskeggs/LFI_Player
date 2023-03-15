#ifndef TEXTSCROLL_H
#define TEXTSCROLL_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: TextScroll.h                               *
// * Description: This file contains the interface for the *
// *              TextScroll class.                        *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#ifndef TRACK_H
#include "Track.h"
#endif

#ifndef FONTLIST_H
#include "FontList.h"
#endif

#ifndef GLOBALS_H
#include "globals.h"
#endif

#ifndef POINT_H
#include "Point.h"
#endif

#define MAX_TEXT_CHARACTERS 1000

typedef struct
{
    double  startPosition;
	double  endPosition;
	M_INT16  characterIndex;
} TextCharacter_T;

typedef TextCharacter_T* TextCharacterPtr_T;

class TextScroll	: public Track
{
public:
    M_UINT16 Ok(void) const {return(Track::okFlag);}

    void   ResetIterator();	

    TextScroll();
    ~TextScroll();
	M_UINT16 ReadTextFile(FILE* textFile);
    TextCharacterPtr_T GetTextCharacter(M_UINT16 characterNumber)
	{
        return &textString[characterNumber];
	}

	M_INT16  GetTextStringLength(void) {return textStringLength;}
	M_UINT16 GetScrollQuickness(void)  {return scrollQuickness;}
	double  GetCurrentXOffset(void)   {return currentXOffset;}
	void    SetScrollQuickness(M_UINT16 newQuickness)
	{
        scrollQuickness = newQuickness;
	}
	void    SetNextXOffset(void);

private:
	TextCharacter_T  textString[MAX_TEXT_CHARACTERS];
	M_INT16   textStringLength;
	M_UINT16	 scrollQuickness;
	double   currentXOffset;

};

#endif
