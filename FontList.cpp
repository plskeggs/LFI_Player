//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: FontList.cpp                               *
// * Description: Implementation of the FontList           *
// * class.                                                *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#include <stdlib.h>
#include <stdio.h>

#include "FontList.h"
#include "Routines.h"

#include <iostream>
#include <fstream>

using namespace std; 

extern ofstream logFile;

FontCharacterPtr_T FontList::fontList;
FontCharacter_T    FontList::defaultFontList[NUMBER_OF_FONT_CHARACTERS];
FontCharacter_T    FontList::alt1FontList[NUMBER_OF_FONT_CHARACTERS];
FontCharacter_T    FontList::alt2FontList[NUMBER_OF_FONT_CHARACTERS];
FontCharacter_T    FontList::alt3FontList[NUMBER_OF_FONT_CHARACTERS];
M_UINT8             FontList::firstPoint;
FontType_T         FontList::currentFontListValue;
M_UINT16            FontList::okFlag;

//////////////////////////////////////////////////////////
//
// Function: FontList::FontList
//
//////////////////////////////////////////////////////////
FontList::FontList(void)
//    okFlag(M_FALSE)
{
    return;
}

//////////////////////////////////////////////////////////
//
// Function: FontList::~FontList
//
//////////////////////////////////////////////////////////
FontList::~FontList(void)
{
    return;
}

//////////////////////////////////////////////////////////
//
// Function: FontList::InitFontList
//
//////////////////////////////////////////////////////////
void
FontList::InitFontList(void)
{
    M_UINT32 i;
	
	fontList = &alt1FontList[0];

	for(i=0 ; i<NUMBER_OF_FONT_CHARACTERS ; ++i)
	{
	    fontList[i].xMinValue   = (M_UINT16)TEXT_WORLD_MAX;
	    fontList[i].xMaxValue   = (M_UINT16)TEXT_WORLD_MIN;
	    fontList[i].headPtr     = 0;
	    fontList[i].tailPtr     = 0;
	    fontList[i].currentPtr  = 0;
	    fontList[i].numOfPoints = 0;
	}

	fontList = &alt2FontList[0];

	for(i=0 ; i<NUMBER_OF_FONT_CHARACTERS ; ++i)
	{
	    fontList[i].xMinValue   = (M_UINT16)TEXT_WORLD_MAX;
	    fontList[i].xMaxValue   = (M_UINT16)TEXT_WORLD_MIN;
	    fontList[i].headPtr     = 0;
	    fontList[i].tailPtr     = 0;
	    fontList[i].currentPtr  = 0;
	    fontList[i].numOfPoints = 0;
	}

	fontList = &alt3FontList[0];

	for(i=0 ; i<NUMBER_OF_FONT_CHARACTERS ; ++i)
	{
	    fontList[i].xMinValue   = (M_UINT16)TEXT_WORLD_MAX;
	    fontList[i].xMaxValue   = (M_UINT16)TEXT_WORLD_MIN;
	    fontList[i].headPtr     = 0;
	    fontList[i].tailPtr     = 0;
	    fontList[i].currentPtr  = 0;
	    fontList[i].numOfPoints = 0;
	}

	fontList = &defaultFontList[0];

	for(i=0 ; i<NUMBER_OF_FONT_CHARACTERS ; ++i)
	{
	    fontList[i].xMinValue   = (M_UINT16)TEXT_WORLD_MAX;
	    fontList[i].xMaxValue   = (M_UINT16)TEXT_WORLD_MIN;
	    fontList[i].headPtr     = 0;
	    fontList[i].tailPtr     = 0;
	    fontList[i].currentPtr  = 0;
	    fontList[i].numOfPoints = 0;
	}

	okFlag = M_FALSE;
}

//////////////////////////////////////////////////////////
//
// Function: FontList::DeleteFontList
//
//////////////////////////////////////////////////////////
void
FontList::DeleteFontList(void)
{
    M_UINT32 i;
	
	for(i=0 ; i<NUMBER_OF_FONT_CHARACTERS ; ++i)
	{
        if (!(fontList[i].headPtr)) // character is already empty
        {
            if (fontList[i].tailPtr)
            {
//			    printf("Character %ld is empty, but tail pointer not NULL.\r\n", i);
                exit(0);
            }
        }
        else // list is not empty
        {
            FontPointPtr_T tempPtr = 0;
            fontList[i].currentPtr = fontList[i].headPtr;
            M_UINT16 j = 1;

            while (fontList[i].currentPtr && (j <= fontList[i].numOfPoints))
            {
                tempPtr = fontList[i].currentPtr->next;
                free(fontList[i].currentPtr);
                fontList[i].currentPtr = tempPtr;
                ++j;
            }

            fontList[i].currentPtr = 0;
            fontList[i].headPtr    = 0;
            fontList[i].tailPtr    = 0;
			fontList[i].xMinValue  = (M_UINT16)TEXT_WORLD_MAX;
			fontList[i].xMaxValue  = (M_UINT16)TEXT_WORLD_MIN;
        }

	    fontList[i].numOfPoints = 0;
		
	} // loop through all of the characters
}
	
//////////////////////////////////////////////////////////
//
// Function: FontList::AddPoint
//
//////////////////////////////////////////////////////////
M_UINT8
FontList::AddPoint(FontPointPtr_T newPoint,
                   M_UINT16        characterNumber)
{
    if (!newPoint)
    {
        return(M_FAILURE);
    }

	if (!fontList[characterNumber].headPtr) // first in list
    {
        fontList[characterNumber].headPtr = newPoint;
        fontList[characterNumber].tailPtr = newPoint;
//        framePtr->previous = headPtr;
        newPoint->previous = 0;
        newPoint->next = 0;
    }
    else
    {
        fontList[characterNumber].tailPtr->next = newPoint;
        newPoint->previous = fontList[characterNumber].tailPtr;
        newPoint->next = 0;
        fontList[characterNumber].tailPtr = newPoint;
    }
	
	if(newPoint->xValue < fontList[characterNumber].xMinValue)
	{
	    fontList[characterNumber].xMinValue = newPoint->xValue;
	}

	if(newPoint->xValue > fontList[characterNumber].xMaxValue)
	{
	    fontList[characterNumber].xMaxValue = newPoint->xValue;
	}

// logFile << "Adding Point (" << newPoint->xValue
//        << ", " << newPoint->yValue
//		<< ", Current X Max/Min: ("
//		<< fontList[characterNumber].xMaxValue
//		<< ", " << fontList[characterNumber].xMinValue << ")" << endl;
    
	fontList[characterNumber].currentPtr = newPoint;
	++fontList[characterNumber].numOfPoints;

    return(M_SUCCESS);

}
	
//////////////////////////////////////////////////////////
//
// Function: FontList::DeletePoint
//
//////////////////////////////////////////////////////////
M_UINT8
FontList::DeletePoint(FontPointPtr_T thePoint,
                      M_UINT16 characterNumber)
{
    if(fontList[characterNumber].headPtr == 0 && 
	   fontList[characterNumber].tailPtr == 0) // list is already empty
    {
        return(M_FAILURE);
    }

    if (thePoint == fontList[characterNumber].tailPtr) // if last in list
    {
        if (thePoint == fontList[characterNumber].headPtr) // is only point in list
        {
            fontList[characterNumber].headPtr = 0;
            fontList[characterNumber].tailPtr = 0;
        }
        else // is last but not only point in list
        {
            (thePoint->previous)->next = 0;
            fontList[characterNumber].tailPtr = thePoint->previous;
        }
    }
    else // else not last in list
    {
        if (thePoint == fontList[characterNumber].headPtr) // first but not only point
        {
            fontList[characterNumber].headPtr = thePoint->next;
            (thePoint->next)->previous = 0;
        }
        else // somewhere in the middle
        {
            (thePoint->previous)->next = thePoint->next;
            (thePoint->next)->previous = thePoint->previous;
        }
    }

    free(thePoint);
    thePoint = 0;
	--fontList[characterNumber].numOfPoints;
    return(M_SUCCESS);
}


//////////////////////////////////////////////////////////
//
// Function: FontList::ResetIterator
//
//////////////////////////////////////////////////////////
void
FontList::ResetIterator(M_UINT16 characterNumber)
{
	fontList[characterNumber].currentPtr = fontList[characterNumber].headPtr;
}

//////////////////////////////////////////////////////////
//
// Function: FontList::GetPoint
//
//////////////////////////////////////////////////////////
FontPointPtr_T
FontList::GetPoint(GetPoint_T direction,
                   M_UINT16 characterNumber)
{
	FontPointPtr_T tempPtr = 0;

	switch (direction)
	{
	    case PREVIOUS:
		{
            if (fontList[characterNumber].currentPtr->previous)
			{
                fontList[characterNumber].currentPtr = fontList[characterNumber].currentPtr->previous;
				tempPtr = fontList[characterNumber].currentPtr;
			}
		}
		break;

	    case NEXT:
		{
            if (fontList[characterNumber].currentPtr->next)
			{
                fontList[characterNumber].currentPtr = fontList[characterNumber].currentPtr->next;
				tempPtr = fontList[characterNumber].currentPtr;
			}
		}
		break;

	    case CURRENT:
		default:
		{
            if (fontList[characterNumber].currentPtr)
			{
				tempPtr = fontList[characterNumber].currentPtr;
			}
		}
		break;

	} // end switch

    return(tempPtr);
}

//////////////////////////////////////////////////////////
//
// Function: FontList::ReadFontFile
// 
// msvob 4/9/2022: Seems counterproductive to have a
// separate method to read in a font file when it seems
// could be combined with RadFile() in Routines.cpp
//
//////////////////////////////////////////////////////////
M_UINT16
FontList::ReadFontFile(char* fontFileName)
{
   M_INT16  frameIndex = 0;
   M_UINT16 fileDone   = M_FALSE;
   M_UINT8  tempBuff[6];

   M_INT16 totalFrames;
   int m, n, i;

   FILE* fontFilePtr;
   fopen_s(&fontFilePtr, fontFileName, "rb");

    if (!fontFilePtr)
	{
		logFile << "FontList::ReadFontFile(): Unable to open font file " << fontFileName << " for reading." << endl;
        return(M_FAILURE);
	}

   while (fileDone == M_FALSE)
   {
       totalFrames = 0;

       // BYTES 1 - 4: The ASCII characters "I" "L" "D" "A"
       if(!fread(&tempBuff,
                 sizeof(char),
                 4,
                 fontFilePtr))
       {
           fputs("Error reading ILDA preamble.\r\n",stderr);
           return(M_FAILURE);
       }

	   // BYTES 5 - 7: Not used, but must be 0, 0, 0
       if(fread(&tempBuff,
                 sizeof(char),
                 3,
                 fontFilePtr))
       {
           if (tempBuff[0]!=0 || tempBuff[1]!=0 || tempBuff[2]!=0)
           {
               fputs("One or more of first 3 unused bytes not zero.\r\n",stderr);
               return(M_FAILURE);
           }

       }
       else
       {
           fputs("Error reading first 3 unused bytes.\r\n",stderr);
           return(M_FAILURE);
       }

       // BYTE 8: Format Code; binary 0 for 3D and 1 for 2D
       M_UINT8 formatCode = 0x00;

       if (!fscanf(fontFilePtr,"%c", &formatCode))
       {
           fputs("Error reading format code.\r\n",stderr);
           return(M_FAILURE);
       }

       // BYTES 9 - 16: Frame Name; 8 ASCII characters
       char frameName[FRAME_NAME_LEN+1];
	   frameName[FRAME_NAME_LEN] = 0;

       if (!fread(&frameName[0],
                  sizeof(char),
                  FRAME_NAME_LEN,
                  fontFilePtr))
       {
           fputs("Error reading frame name.\r\n",stderr);
           return(M_FAILURE);
       }

	   for (m=0 ; m<FRAME_NAME_LEN ; ++m)
	   {
           if(frameName[m] == ' ')
			   frameName[m] = '_';
	   }

	   // BYTES 17 - 24: Company Name; 8 ASCII characters
       char companyName[COMPANY_NAME_LEN+1];
	   companyName[COMPANY_NAME_LEN] = 0;

       if (!fread(&companyName[0],
                  sizeof(char),
                  COMPANY_NAME_LEN,
                  fontFilePtr))
       {
           fputs("Error reading company name.\r\n",stderr);
           return(M_FAILURE);
       }

	   for (n=0 ; n<COMPANY_NAME_LEN ; ++n)
	   {
           if(companyName[n] == ' ')
			   companyName[n] = '_';
	   }

	   // BYTES 25 - 26: Total Points; Total # of points in image
       M_INT16  totalPoints = 0;

       if (GetWord(fontFilePtr, totalPoints) == M_SUCCESS)
       {
           if (totalPoints == 0)
           {
               fileDone = M_TRUE;
           }
       }
       else
       {
           fputs("Error reading total points.\r\n",stderr);
           return(M_FAILURE);
       }

       // BYTES 27 - 28: Frame Number; Frame number if part of
       //                sequence
       short frameNumber = 0;

       if (GetWord(fontFilePtr, frameNumber) != M_SUCCESS)
       {
           fputs("Error reading frame number.\r\n",stderr);
           return(M_FAILURE);
       }

       // BYTES 29 - 30: Total Frames; Total frames in sequence
       if (GetWord(fontFilePtr, totalFrames) != M_SUCCESS)
       {
           fputs("Error reading total frames.\r\n",stderr);
           return(M_FAILURE);
       }

       // BYTE 31: Scanner Head; Scanner head or projector number
       M_UINT8 scannerHead = 0;
       if (!fscanf(fontFilePtr, "%c", &scannerHead))
       {
           return(M_FAILURE);
       }

       // BYTE 32: Future; Must be set for binary zero
       M_UINT8 future = 0;
       if (fscanf(fontFilePtr, "%c", &future))
       {
           if (future != 0)
           {
               fputs("Non zero future value.\r\n",stderr);
               return(M_FAILURE);
           }
       }
       else
       {
           fputs("Error reading future value.\r\n",stderr);
           return(M_FAILURE);
       }

//       buffIndex = 0;
       
       for (i=0 ; i < totalPoints ; ++i)
       {
           M_INT16 xCoord = 0x0000;
           M_INT16 yCoord = 0x0000;
           M_INT16 zCoord = 0x0000;
           M_INT16 status = 0x0000;

           if (GetWord(fontFilePtr, xCoord) == M_FAILURE)
           {
               fputs("Error reading X coordinate.\r\n",stderr);
               return(M_FAILURE);
           }

           if (GetWord(fontFilePtr, yCoord) == M_FAILURE)
           {
               fputs("Error reading Y coordinate.\r\n",stderr);
               return(M_FAILURE);
           }

           if (GetWord(fontFilePtr, zCoord) == M_FAILURE)
           {
               fputs("Error reading Z coordinate.\r\n",stderr);
               return(M_FAILURE);
           }

           if (GetWord(fontFilePtr, status) == M_FAILURE)
           {
               fputs("Error reading point status.\r\n",stderr);
               return(M_FAILURE);
           }
		   
		   FontPointPtr_T newPoint = new FontPoint_T;

		   if(!newPoint)
		   {
               fputs("Cannot allocate memory for new point.\r\n",stderr);
               return(M_FAILURE);		   
		   }
		   
		   newPoint->xValue = xCoord + FONT_NORMALIZER;
		   newPoint->yValue = yCoord + FONT_NORMALIZER;

           if (status & BLANKING_MASK_WORD)
           {
               newPoint->isBlanked = M_TRUE;
           }
           else
           {
               newPoint->isBlanked = M_FALSE;
           }

		   AddPoint(newPoint, frameIndex);

       } // loop through all points
	   
       ++frameIndex;

       if (feof(fontFilePtr))
       {
           fileDone = M_TRUE;
       }

   } // end while fileDone == M_FALSE

   fprintf(stderr,"\n");

   if(fontFilePtr)
   {
	   fclose(fontFilePtr);
   }
    
   okFlag = M_TRUE;
   return(M_SUCCESS);

} // end function ReadFontFile


//////////////////////////////////////////////////////////
//
// Function: FontList::SetFontList
//
//////////////////////////////////////////////////////////
void
FontList::SetFontList(FontType_T fontType)
{
	switch(fontType)
	{
	    case ALTERNATE_1_FONT:
		{
            fontList = &alt1FontList[0];
			currentFontListValue = ALTERNATE_1_FONT;
		}
		break;

	    case ALTERNATE_2_FONT:
		{
            fontList = &alt2FontList[0];
			currentFontListValue = ALTERNATE_2_FONT;
		}
		break;

		case ALTERNATE_3_FONT:
		{
            fontList = &alt3FontList[0];
			currentFontListValue = ALTERNATE_3_FONT;
		}
		break;

		default:
		{
            fontList = &defaultFontList[0];
			currentFontListValue = DEFAULT_FONT;
		}
		break;
	}
}




