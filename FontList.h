#ifndef FONTLIST_H
#define FONTLIST_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: FontList.h                                 *
// * Description: This file contains the interface for the *
// *              FontList class.                          *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#include "Globals.h"

#define TEXT_WORLD_MIN   0
#define TEXT_WORLD_MAX   65534

#define FONT_NORMALIZER  32767

typedef enum
{
    PREVIOUS = 0,
	CURRENT  = 1,
	NEXT     = 2,
} GetPoint_T;

typedef enum
{
    DEFAULT_FONT     = 0,
	ALTERNATE_1_FONT = 1,
	ALTERNATE_2_FONT = 2,
	ALTERNATE_3_FONT = 3,
	INVALID_FONT     = 4,
} FontType_T;

typedef struct
{
    M_UINT8   formatCode;
	char     frameName[FRAME_NAME_LEN];
	char     companyName[COMPANY_NAME_LEN];
	M_INT16   totalPoints;
	M_INT16   frameNumber;
	M_INT16   totalFrames;
    M_UINT8   scannerHead;
} Frame_Data_T;

typedef struct fontPoint
{
    M_UINT16    xValue;
	M_UINT16    yValue;
	M_UINT16    isBlanked;
	struct fontPoint*   next;
	struct fontPoint*   previous;  
} FontPoint_T;

typedef FontPoint_T* FontPointPtr_T;

typedef struct
{
    M_UINT16         xMinValue;
	M_UINT16         xMaxValue;
	FontPointPtr_T  headPtr;
	FontPointPtr_T  tailPtr;
	FontPointPtr_T  currentPtr;
	M_UINT32         numOfPoints;  
} FontCharacter_T;

typedef FontCharacter_T* FontCharacterPtr_T;

#define FONT_CHAR_MIN 0x20 // space
#define FONT_CHAR_MAX 0x7e // ~

#define NUMBER_OF_FONT_CHARACTERS (FONT_CHAR_MAX - FONT_CHAR_MIN + 1)

class FontList
{
public:

    FontList();
	~FontList();

    static void InitFontList(void);

	static void DeleteFontList(void);

	static void SetFontList(FontType_T fontType);

	static FontType_T GetFontListValue(void) {return currentFontListValue;}

    static M_UINT8 AddPoint(FontPointPtr_T newPoint,
                    M_UINT16        characterNumber);

    M_UINT8 DeletePoint(FontPointPtr_T thePoint,
                       M_UINT16        characterNumber);

    static FontPointPtr_T GetPoint(GetPoint_T direction,
                            M_UINT16    characterNumber);
    static void ResetIterator(M_UINT16 characterNumber);

    static M_UINT16 ReadFontFile(char* fontFileName);

    M_UINT8 DisplayCharacter(M_INT16  characterNumber,
                            M_INT32  xOffset,
							M_UINT8  redValue,
							M_UINT8  greenValue,
							M_UINT8  blueValue);

	static FontCharacterPtr_T GetFontCharacter(M_INT16 characterNumber)
	{
        return &fontList[characterNumber];
	}

	static M_UINT16 Ok(void)
	{
	    if(fontList[0].headPtr != 0)
		{
            return M_TRUE;
		}
		else
		{
			return M_FALSE;
		}
	}

private:
	static FontCharacterPtr_T fontList;
	static FontCharacter_T defaultFontList[NUMBER_OF_FONT_CHARACTERS];
	static FontCharacter_T alt1FontList[NUMBER_OF_FONT_CHARACTERS];
	static FontCharacter_T alt2FontList[NUMBER_OF_FONT_CHARACTERS];
	static FontCharacter_T alt3FontList[NUMBER_OF_FONT_CHARACTERS];
    static M_UINT8  firstPoint;
	static FontType_T currentFontListValue;
    static M_UINT16 okFlag;
};

#endif




