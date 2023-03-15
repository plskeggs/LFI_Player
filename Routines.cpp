//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Routines.cpp                               *
// * Description: This file contains a variety of          *
// * miscellaneous support functions.                      *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>

#include "stdafx.h"

#ifndef POINT_H
#include "POINT.h"
#endif

#ifndef FRAME_H
#include "FRAME.h"
#endif

#ifndef FRAMESEQUENCE_H
#include "FrameSeq.h"
#endif

#ifndef TRACK_H
#include "Track.h"
#endif

#ifndef TRACKLIST_H
#include "TrackLst.h"
#endif

#ifndef GLOBALS_H
#include "globals.h"
#endif

#ifndef FONTLIST_H
#include "FontList.h"
#endif

#ifndef ROUTINES_H
#include "routines.h"
#endif

using namespace std; 

extern ofstream logFile;

M_UINT16 GetWord(FILE*, short&);

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: ReadFile                                           */
/*                                                                 */
/*******************************************************************/

M_UINT16 ReadFile(FILE* ildaFilePtr, FrameSequence* frameSeqPtr)
{
   M_UINT8  tempBuff[6] = "\0\0\0\0\0";

#ifdef ROUTINE_LOG_INFO
   logFile << "Entering ReadFile()" << endl;
#endif

   if (!frameSeqPtr)
   {
#ifdef ROUTINE_LOG_ERROR
       logFile << "ReadFile(): Unexpected NULL FrameSequence pointer." 
               << endl;
#endif
       return(M_FAILURE);
   }

   if (!ildaFilePtr)
   {
#ifdef ROUTINE_LOG_ERROR
       logFile << "ReadFile(): Unexpected NULL FILE pointer." << endl;
#endif
       return(M_FAILURE);
   }

   frameSeqPtr->SetNumberOfFrames(0);

   short  frameIndex = 0;
   M_UINT16 fileDone   = M_FALSE;

   while (fileDone == M_FALSE)
   {
       Frame* newFrame = 0;
       newFrame = new Frame();

       if (!newFrame)
       {
#ifdef ROUTINE_LOG_ERROR
           logFile << "ReadFile(): Unable to new memory for newFrame."    
                   << endl;
#endif
           return(M_FAILURE);
       }

       // ***************************************************************
       // BYTES 1 - 4: The ASCII characters "I" "L" "D" "A"
       // ***************************************************************
#ifdef ROUTINE_LOG_INFO
       logFile << "Bytes 1 - 4 (ILDA Header): ";
#endif
       if(!fread(&tempBuff,
                 sizeof(char),
                 4,
                 ildaFilePtr))
       {
           if(feof(ildaFilePtr))
           {
#ifdef ROUTINE_LOG_INFO
       logFile << "EOF" << endl;
#endif
               delete(newFrame);

               newFrame = frameSeqPtr->GetCurrentFrame();

               if (newFrame && !(newFrame->GetNumberOfPoints()))
               {
                   frameSeqPtr->DeleteFrame(newFrame);
               } 
               break;

           }
           else
           {
#ifdef ROUTINE_LOG_ERROR
               logFile << "ReadFile(): Unable to read ILDA character marker."
                       << endl;
#endif
               return(M_FAILURE);
           }
       }

#ifdef ROUTINE_LOG_INFO
           logFile << "\'" << tempBuff[0] << "\' "
                   << "\'" << tempBuff[1] << "\' "
                   << "\'" << tempBuff[2] << "\' "
                   << "\'" << tempBuff[3] << "\' " << endl;
#endif
       // ***************************************************************
       // BYTES 5 - 7: Not used, but must be 0, 0, 0
       // ***************************************************************
       if(fread(&tempBuff,
                 sizeof(char),
                 3,
                 ildaFilePtr))
       {
           if (tempBuff[0]!=0 || tempBuff[1]!=0 || tempBuff[2]!=0)
           {
#ifdef ROUTINE_LOG_ERROR
               logFile << "ReadFile(): Initial unused bytes not all zero."    
                       << endl;
#endif
//               return(M_FAILURE);
           }
#ifdef ROUTINE_LOG_INFO
           logFile << "Bytes 5 - 7 (Unused): 0x" << hex
                   << (M_UINT16)tempBuff[0] << " 0x"
                   << (M_UINT16)tempBuff[1] << " 0x"
                   << (M_UINT16)tempBuff[2] << dec << endl; 
#endif
       }
       else
       {
#ifdef ROUTINE_LOG_ERROR
           logFile << "ReadFile(): Unable to read initial unused bytes."
                   << endl;
#endif
           return(M_FAILURE);
       }

       // ***************************************************************
       // BYTE 8: Format Code
       // 0 - 3D Coordinates with Indexed Color
       // 1 - 2D Coordinates with Indexed Color
       // 2 - Color Palette for Indexed Color Frames
       // 3 - UNUSED
       // 4 - 3D Coordinates with True Color
       // 5 - 2D Coordinates with True Color
       // ***************************************************************
       M_UINT8 tempFormatCode = 0x00;

       if (fscanf(ildaFilePtr,"%c", &tempFormatCode))
       {
           newFrame->SetFormatCode((ILDAFormatCode)tempFormatCode);
#ifdef ROUTINE_LOG_INFO
           logFile << "Byte 8 (Format Code): 0x" << hex
                   << (M_UINT16)formatCode << dec << endl; 
#endif
       }
       else
       {
#ifdef ROUTINE_LOG_ERROR
           logFile << "ReadFile(): Unable to read frame format code."    
                   << endl;
#endif
           return(M_FAILURE);
       }

       // ***************************************************************
       // BYTES 9 - 16: Frame Name; 8 ASCII characters
       // ***************************************************************
       char frameName[FRAME_NAME_LEN];

       if (!fread(&frameName[0],
                  sizeof(char),
                  FRAME_NAME_LEN,
                  ildaFilePtr))
       {
#ifdef ROUTINE_LOG_ERROR
           logFile << "ReadFile(): Unable to read frame name."    
                   << endl;
#endif
           return(M_FAILURE);
       }
            
#ifdef ROUTINE_LOG_INFO
           logFile << "Bytes 9 - 16 (Frame Name): \"";
           for (int l=0 ; l<FRAME_NAME_LEN ; ++l)
           {
               logFile << frameName[l];
           }
           logFile << "\"" << endl;
#endif
       // ***************************************************************
       // BYTES 17 - 24: Company Name; 8 ASCII characters
       // ***************************************************************
       char companyName[COMPANY_NAME_LEN];

       if (!fread(&companyName[0],
                  sizeof(char),
                  COMPANY_NAME_LEN,
                  ildaFilePtr))
       {
#ifdef ROUTINE_LOG_ERROR
           logFile << "ReadFile(): Unable to read company name."    
                   << endl;
#endif
           return(M_FAILURE);
       }

#ifdef ROUTINE_LOG_INFO
           logFile << "Bytes 17 - 24 (Company Name): \"";
           for (int m=0 ; m<COMPANY_NAME_LEN ; ++m)
           {
               logFile << companyName[m];
           }
           logFile << "\"" << endl;
#endif

       // ***************************************************************
       // BYTES 25 - 26: Total Points; Total # of points in image
       // ***************************************************************
       short totalPoints = 0;

       if (GetWord(ildaFilePtr, totalPoints) == M_SUCCESS)
       {
           if (totalPoints == 0)
           {
#ifdef ROUTINE_LOG_INFO
               logFile << "ReadFile(): Zero points in image."    
                       << endl;
#endif
//               return(M_FAILURE);
           }
           else
           {
               newFrame->SetNumberOfPoints(totalPoints);
#ifdef ROUTINE_LOG_INFO
               logFile << "Bytes 25 - 26 (Number of Points): " << totalPoints << endl; 
#endif
           }
       }
       else
       {
#ifdef ROUTINE_LOG_ERROR
           logFile << "ReadFile(): Unable to read total image points."    
                   << endl;
#endif
           return(M_FAILURE);
       }

       // ***************************************************************
       // BYTES 27 - 28: Frame Number; Frame number if part of sequence
       // ***************************************************************
       short frameNumber = 0;

       if (GetWord(ildaFilePtr, frameNumber) == M_SUCCESS)
       {
           newFrame->SetFrameNumber(frameNumber);
#ifdef ROUTINE_LOG_INFO
           logFile << "Bytes 27 - 28 (Frame Number): " << frameNumber << endl; 
#endif
       }
       else
       {
#ifdef ROUTINE_LOG_ERROR
           logFile << "ReadFile(): Unable to read frame number."    
                   << endl;
#endif
           return(M_FAILURE);
       }

       // ***************************************************************
       // BYTES 29 - 30: Total Frames; Total frames in sequence
       // ***************************************************************
       short totalFrames = 0;
           
       if (GetWord(ildaFilePtr, totalFrames) == M_SUCCESS)
       {
           frameSeqPtr->SetNumberOfFrames(totalFrames);
#ifdef ROUTINE_LOG_INFO
           logFile << "Bytes 29 - 30 (Total Frames): " << totalFrames << endl; 
#endif
       }
       else
       {
#ifdef ROUTINE_LOG_ERROR
           logFile << "ReadFile(): Unable to read number of frames in sequence."    
                   << endl;
#endif
           return(M_FAILURE);
       }

       // ***************************************************************
       // BYTE 31: Scanner Head; Scanner head or projector number
       // ***************************************************************
       M_UINT8 scannerHead = 0;
       if (fscanf(ildaFilePtr, "%c", &scannerHead))
       {
           newFrame->SetScannerHead(scannerHead);
#ifdef ROUTINE_LOG_INFO
           logFile << "Byte 31 (Scanner Head): 0x" << hex
                   << (M_UINT16)scannerHead << dec << endl; 
#endif
       }
       else
       {
#ifdef ROUTINE_LOG_ERROR
           logFile << "ReadFile(): Unable to read scanner head number."    
                   << endl;
#endif
           return(M_FAILURE);
       }

       // ***************************************************************
       // BYTE 32: Reserved; Must be set for binary zero
       // ***************************************************************
       M_UINT8 future = 0;
       if (fscanf(ildaFilePtr, "%c", &future))
       {
           if (future != 0)
           {
#ifdef ROUTINE_LOG_ERROR
               logFile << "ReadFile(): Byte reserved for future use is not zero."    
                       << endl;
#endif
//               return(M_FAILURE);
           }
#ifdef ROUTINE_LOG_INFO
               logFile << "Byte 32 (Future): 0x" << hex
                       << (M_UINT16)future << dec << endl; 
#endif
       }
       else
       {
#ifdef ROUTINE_LOG_ERROR
           logFile << "ReadFile(): Unable to read byte reserved for future use."    
                   << endl;
#endif
           return(M_FAILURE);
       }

       short xCoord   = 0x0000;
       short yCoord   = 0x0000;
       short zCoord   = 0x0000;
       M_UINT8 status   = 0x00;
       M_UINT8 index    = 0x00;
       M_UINT8 redVal   = 0x00;
       M_UINT8 greenVal = 0x00;
       M_UINT8 blueVal  = 0x00;

       // ***************************************************************
       // Begin loop to retrieve all the points in the frame
       // ***************************************************************
       for (int i=0 ; i < totalPoints ; ++i)
       {
           Point* newPoint = 0;
           newPoint = new Point();

           if (!newPoint)
           {
#ifdef ROUTINE_LOG_ERROR
               logFile << "ReadFile(): Unable to new memory for newPoint."    
                       << endl;
#endif
               return(M_FAILURE);
           }

           // ***************************************************************
           // Get the X coordinate
           // ***************************************************************
           if (GetWord(ildaFilePtr, xCoord) == M_FAILURE)
           {
#ifdef ROUTINE_LOG_ERROR
               logFile << "ReadFile(): Unable to read X coordinate of point "
                       << dec << i << " of " << totalPoints << endl;    
#endif
               return(M_FAILURE);
           }

           // ***************************************************************
           // Get the Y coordinate
           // ***************************************************************
           if (GetWord(ildaFilePtr, yCoord) == M_FAILURE)
           {
#ifdef ROUTINE_LOG_ERROR
               logFile << "ReadFile(): Unable to read Y coordinate of point "
                       << dec << i << " of " << totalPoints << endl;    
#endif
               return(M_FAILURE);
           }

           // ***************************************************************
           // Get the Z coordinate only if format code indicates 3D
           // ***************************************************************
           if (newFrame->GetFormatCode() == ILDAFormatCode::INDEXED_COLOR_3D ||
               newFrame->GetFormatCode() == ILDAFormatCode::TRUE_COLOR_3D)
           {
               if (GetWord(ildaFilePtr, zCoord) == M_FAILURE)
               {
#ifdef ROUTINE_LOG_ERROR
                   logFile << "ReadFile(): Unable to read Z coordinate of point "
                           << dec << i << " of " << totalPoints << endl;    
#endif
                   return(M_FAILURE);
               }
           }
           else
           {
               zCoord = 0x0000;
           }

           // ***************************************************************
           // Get the Status Code
           // ***************************************************************
           if (!fscanf(ildaFilePtr, "%c", &status))
           {
#ifdef ROUTINE_LOG_ERROR
               logFile << "ReadFile(): Unable to read the Status Code of point "
                       << dec << i << " of " << totalPoints << endl;    
#endif
               return(M_FAILURE);
           }

           if (newFrame->GetFormatCode() == ILDAFormatCode::INDEXED_COLOR_2D ||
               newFrame->GetFormatCode() == ILDAFormatCode::INDEXED_COLOR_3D)
           {
               // ***************************************************************
               // Indexed Color so retrieve the Color Index
               // ***************************************************************
               if (!fscanf(ildaFilePtr, "%c", &index))
               {
#ifdef ROUTINE_LOG_ERROR
                   logFile << "ReadFile(): Unable to read the Color Index of point "
                       << dec << i << " of " << totalPoints << endl;
#endif
                   return(M_FAILURE);
               }
           }
           else
           {
               // ***************************************************************
               // True Color so retrieve the Blue, Green, and Red values
               // in that order
               // ***************************************************************
               if (!fscanf(ildaFilePtr, "%c", &blueVal))
               {
#ifdef ROUTINE_LOG_ERROR
                   logFile << "ReadFile(): Unable to read the Blue Color value of point "
                       << dec << i << " of " << totalPoints << endl;
#endif
                   return(M_FAILURE);
               }

               if (!fscanf(ildaFilePtr, "%c", &greenVal))
               {
#ifdef ROUTINE_LOG_ERROR
                   logFile << "ReadFile(): Unable to read the Green Color value of point "
                       << dec << i << " of " << totalPoints << endl;
#endif
                   return(M_FAILURE);
               }

               if (!fscanf(ildaFilePtr, "%c", &redVal))
               {
#ifdef ROUTINE_LOG_ERROR
                   logFile << "ReadFile(): Unable to read the Red Color value of point "
                       << dec << i << " of " << totalPoints << endl;
#endif
                   return(M_FAILURE);
               }
           }

           // ***************************************************************
           // Now save the retrieved Point data
           // ***************************************************************
           newPoint->SetPointInfo(xCoord,
                                  yCoord,
                                  zCoord,
                                  status,
                                  index,
                                  redVal,
                                  greenVal,
                                  blueVal);

           if ((newFrame->AddPoint(newPoint)) != M_SUCCESS)
           {
#ifdef ROUTINE_LOG_ERROR
               logFile << "ReadFile(): Could not add new point."
                       << "\nNumber of points is " << dec
                       << newFrame->GetNumberOfPoints() << endl;
#endif
               return(M_FAILURE);
           }

#ifdef POINT_INFO
           logFile << "ReadFile() - SetPointInfo(): X[" << xCoord << "], "
                                                << "X[" << yCoord << "], "
                                                << "Z[" << yCoord << "], "
                                                << "S[" << (M_UINT16)status << "], "
                                                << "I[" << (M_UINT16)index << "], "
                                                << "R[" << (M_UINT16)redVal << "], "
                                                << "G[" << (M_UINT16)greenVal << "], "
                                                << "B[" << (M_UINT16)blueVal << "]" << endl;
#endif

       } // end loop through all points in frame

       if(totalPoints == 0 && frameNumber == totalFrames)
       {
           delete(newFrame);
       }
       else if ((frameSeqPtr->AddFrame(newFrame)) != M_SUCCESS)
       {
#ifdef ROUTINE_LOG_ERROR
           logFile << "ReadFile(): Could not add new frame."
                   << "\nNumber of frames is " << dec
                   << frameSeqPtr->GetNumberOfFrames() << endl;
#endif
           return(M_FAILURE);
       }

// Pangolin doesn't set the total number of frames value when
// outputting ILDA formatted files, so have to just read to
// EOF and hope for the best...
//       if (++frameIndex >= frameSeqPtr->GetNumberOfFrames())
//       {
//           fileDone = M_TRUE;
//       }

       ++frameIndex;

       if (feof(ildaFilePtr))
       {
           fileDone = M_TRUE;
       }


   } // end while fileDone == M_FALSE

   frameSeqPtr->SetNumberOfFrames(frameIndex);


//   logFile << "ReadFile(): Successfully read file "
//           << "into the following track..." << endl;
//   frameSeqPtr->PrintFrameSequence(logFile, M_TRUE);

   return(M_SUCCESS);

} // end function ReadFile


/*******************************************************************/
/*                                                                 */
/*    INTERNAL FUNCTION: GetWord                                   */
/*                                                                 */
/*******************************************************************/

M_UINT16
GetWord(FILE* ildaFilePtr, short& theWord)
{
    BYTE tempByte = 0x00;
    M_UINT16 error = M_FALSE;

    theWord = 0x0000;

    if (fread(&tempByte,
              sizeof(char),
              CHAR_LEN,
              ildaFilePtr))
    {
        theWord = (theWord | tempByte) << 8;

        if (fread(&tempByte,
              sizeof(char),
              CHAR_LEN,
              ildaFilePtr))
        {
            theWord = (theWord | ((M_INT16)tempByte & 0x00FF));
        }
        else
        {
#ifdef ROUTINE_LOG_ERROR
            logFile << "GetWord(): fread failed for LS byte." << endl;
#endif
            error = M_TRUE;
        } 
    }
    else
    {
#ifdef ROUTINE_LOG_ERROR
        logFile << "GetWord(): fread failed for MS byte." << endl;
#endif
        error = M_TRUE;
    }

    if (error == M_FALSE)
    {
        return(M_SUCCESS);
    }
    else
    {
        return(M_FAILURE);
    }

} // end GetWord


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: PutWord                                            */
/*                                                                 */
/*******************************************************************/

void
PutWord(FILE* outFile, M_INT16 theWord)
{
    M_INT16 tempWord = 0x0000;

    tempWord = theWord;
    tempWord = (theWord & 0x00ff) << 8;
    tempWord = ((theWord & 0xff00) >> 8) | tempWord;
    fwrite(&tempWord,sizeof(M_INT16),1,outFile);
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: PutByte                                            */
/*                                                                 */
/*******************************************************************/

void
PutByte(FILE* outFile, M_UINT8 theByte)
{
    // This function provided for consistancy with PutWord()
    fwrite(&theByte, sizeof(M_UINT8), 1, outFile);
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: GetMsbFromUshort                                     */
/*                                                                 */
/*******************************************************************/

M_UINT8
GetMsbFromUshort(M_UINT16 word)
{
    return (M_UINT8)((word >> 8) & 0x00ff);
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: GetLsbFromUshort                                     */
/*                                                                 */
/*******************************************************************/

M_UINT8
GetLsbFromUshort(M_UINT16 word)
{
    return (M_UINT8)(word & 0x00ff);
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DoReadILDAFile                                     */
/*                                                                 */
/*******************************************************************/

M_UINT16
DoReadILDAFile(const char* const fileName,
               FrameSequence**  frameSeqPtr)
{
#ifdef ROUTINE_LOG_INFO
    logFile << "Entering DoReadILDAFile()" << endl;
#endif
                    
    delete *frameSeqPtr;
    *frameSeqPtr = new FrameSequence();

    if (!(*frameSeqPtr))
    {
#ifdef ROUTINE_LOG_ERROR
        logFile << "DoReadILDAFile(): Unable to new memory for " 
                << "frameSeqPtr." << endl;
#endif
        return(M_FAILURE);
    }

    FILE*    ildaFilePtr;
    fopen_s(&ildaFilePtr, fileName, "rb");

    if (ildaFilePtr) // file was successfully opened
    {
        if (ReadFile(ildaFilePtr, *frameSeqPtr) != M_SUCCESS)
        {
#ifdef ROUTINE_LOG_ERROR
            logFile << "DoReadILDAFile(): Call to ReadFile() failed for "
                    << "file \n" << fileName << endl;
#endif
            fclose(ildaFilePtr);
            return(M_FAILURE);
        }
        else
        {
            fclose(ildaFilePtr);
        }

    } // end if file was successfully opened
    else
    {
#ifdef ROUTINE_LOG_ERROR
        logFile << "DoReadILDAFile(): Unable to open file \n"
                << fileName << endl;
#endif
        return(M_FAILURE);

    } // end else unable to open the file

    if (!(*frameSeqPtr))
    {
#ifdef ROUTINE_LOG_ERROR
        logFile << "NULL frameSeqPtr after reading file \n"
                << fileName << endl;
#endif
        return(M_FAILURE);
    }
    else
    {
        (*frameSeqPtr)->ResetIterator();
    }

    return(M_SUCCESS);

} // end function DoReadILDAFile

////////////////////////////////////////////////////////////////////////
//
// Function ResetCycle
//
////////////////////////////////////////////////////////////////////////
double 
ResetCycle(double cycleValue, double cycleThreshold)
{
    double tempValue = cycleValue;

    if (cycleValue >= cycleThreshold)
    {
//        tempValue -= cycleThreshold;
    }

    return tempValue;
}

////////////////////////////////////////////////////////////////////////
//
// Function ResetCycle
//
////////////////////////////////////////////////////////////////////////
double 
ResetCycle(double cycleValue, double cycleThreshold, M_UINT16 &blnkThis)
{
    // Note that blnkThis only goes from M_FALSE to M_TRUE.  If
    // already M_TRUE, will not be affected.
    double tempValue = cycleValue;

    if (cycleValue >= cycleThreshold)
    {
//		blnkThis = M_TRUE;
//        tempValue -= cycleThreshold;
    }

    return tempValue;
}

////////////////////////////////////////////////////////////////////////
//
// Function MapAbsToNDC
//
////////////////////////////////////////////////////////////////////////
void 
MapAbsToNDC(const M_INT16 x_abstract,
            const M_INT16 y_abstract,
            double* x_NDC,
            double* y_NDC)
{
    double   a, b, c, d;
    double   x_min_d, x_max_d, y_min_d, y_max_d;
    double   xd_temp, yd_temp;

// Note that these values represent the MIN and MAX
// values for the abstract generator, not the actual
// "device" (i.e. screen or laser) values.
    x_min_d = -70000;
    x_max_d = 70000;
    y_min_d = -70000;
    y_max_d = 70000;

    xd_temp = x_abstract;
    yd_temp = y_abstract;

    a = 1/(x_max_d-x_min_d);
    b = 0-(a*x_min_d);
    c = 1/(y_max_d-y_min_d);
    d = 0-(c*y_min_d);

    *x_NDC = (a*xd_temp)+b;
    *y_NDC = (c*yd_temp)+d;

} /* MapAbsToNDC */

////////////////////////////////////////////////////////////////////////
//
// Function MapTextToNDC
//
////////////////////////////////////////////////////////////////////////
void 
MapTextToNDC(const M_INT16 x_abstract,
             const M_INT16 y_abstract,
             double* x_NDC,
             double* y_NDC)
{
    double   a, b, c, d;
    double   x_min_d, x_max_d, y_min_d, y_max_d;
    double   xd_temp, yd_temp;

// Note that these values represent the MIN and MAX
// values for the abstract generator, not the actual
// "device" (i.e. screen or laser) values.
    x_min_d = TEXT_WORLD_MIN;
    x_max_d = TEXT_WORLD_MAX;
    y_min_d = TEXT_WORLD_MIN;
    y_max_d = TEXT_WORLD_MAX;

    xd_temp = x_abstract;
    yd_temp = y_abstract;

    a = 1/(x_max_d-x_min_d);
    b = 0-(a*x_min_d);
    c = 1/(y_max_d-y_min_d);
    d = 0-(c*y_min_d);

    *x_NDC = (a*xd_temp)+b;
    *y_NDC = (c*yd_temp)+d;

} /* MapTextToNDC */

////////////////////////////////////////////////////////////////////////
//
// Function MapNDCToRadian
//
////////////////////////////////////////////////////////////////////////
void 
MapNDCToRadian(const double x_NDC,
               const double y_NDC,
               double* x_radian,
               double* y_radian)

{
    double   a, b, c, d;
    double   temp_x, temp_y;
    double   x_min_d, x_max_d, y_min_d, y_max_d;

//    x_min_d = xmin_device;
//    x_max_d = xmax_device;
//    y_min_d = ymin_device;
//    y_max_d = ymax_device;

    x_min_d = 0.0;
    x_max_d = 2.0 * PI;
    y_min_d = 0.0;
    y_max_d = 2.0 * PI;

    a = (x_max_d-x_min_d);
    b = x_min_d;
    c = (y_max_d-y_min_d);
    d = y_min_d;

    temp_x = (a*x_NDC)+b;
    temp_y = (c*y_NDC)+d;

    *x_radian = temp_x;
    *y_radian = temp_y;

} /* MapNDCToRadians */


////////////////////////////////////////////////////////////////////////
//
// Function GenericMapAtoB
//
////////////////////////////////////////////////////////////////////////
void 
GenericMapAtoB(const double x_A,
               const double y_A,
               const double A_min,
               const double A_max,
               const double B_min,
               const double B_max,
               double* x_B,
               double* y_B)

{
    double   a, b, c, d;
    double   x_min_d, x_max_d, y_min_d, y_max_d;
    double   xd_temp, yd_temp;

    x_min_d = A_min;
    x_max_d = A_max;
    y_min_d = A_min;
    y_max_d = A_max;

    xd_temp = x_A;
    yd_temp = y_A;

    a = B_max/(x_max_d-x_min_d);
    b = B_min-(a*x_min_d);
    c = B_max/(y_max_d-y_min_d);
    d = B_min-(c*y_min_d);

    *x_B = (a*xd_temp)+b;
    *y_B = (c*yd_temp)+d;

} // GenericMapAtoB

////////////////////////////////////////////////////////////////////////
//
// Function AdjustColor
//
////////////////////////////////////////////////////////////////////////
void
AdjustColor(enum DisplayColorMode  colorMode,
                 UINT              colorIntensity,
                 double            redThreshold,
                 double            greenThreshold,
                 double            blueThreshold,
                 M_UINT8*            redValue,
                 M_UINT8*            greenValue,
                 M_UINT8*            blueValue)
{
    switch(colorMode)
    {
        case DisplayColorMode::COLOR_GRAYSCALE:
        case DisplayColorMode::COLOR_MONOCHROME:
        {
            M_UINT16 divisor = 0;
            if (*redValue > 0)
                ++divisor;
            if (*greenValue > 0)
                ++divisor;
            if (*blueValue > 0)
                ++divisor;

            if (divisor)
            {
                if (colorMode == DisplayColorMode::COLOR_GRAYSCALE)
                {
                    M_UINT8 avgValue = (M_UINT8)((((*redValue)*0.3)+((*greenValue)*0.59)+((*blueValue)*0.11)) / (double)divisor);
                    *redValue   = avgValue;
                    *greenValue = avgValue;
                    *blueValue  = avgValue;
                }
                else
                {
                    *redValue   = 0xff;
                    *greenValue = 0xff;
                    *blueValue  = 0xff;
                }
            }
        }
        break;

        case DisplayColorMode::COLOR_INVERT:
        {
            // Hmmmm...I wonder what effect, if any,
            // this has on blanking???  We shall see...
            *redValue = ~(*redValue);
            *greenValue = ~(*greenValue);
            *blueValue = ~(*blueValue);
        }
        break;

        case DisplayColorMode::COLOR_CVL:
        {
            if(*blueValue)
            {
                *redValue   = (M_UINT8)(((double)(*redValue)   + (double)(*blueValue)) / 2.0);
                *greenValue = (M_UINT8)(((double)(*greenValue) + (double)(*blueValue)) / 2.0);
                *blueValue  = 0;
            }
        }
        break;

        case DisplayColorMode::COLOR_TTL:
        {
            if (*redValue > (redThreshold*0xff))
                *redValue = 0xff;
            else
                *redValue = 0x00;

            if (*greenValue > (greenThreshold*0xff))
                *greenValue = 0xff;
            else
                *greenValue = 0x00;

            if (*blueValue > (blueThreshold*0xff))
                *blueValue = 0xff;
            else
                *blueValue = 0x00;
        }
        break;

        case DisplayColorMode::COLOR_RG_INV_TTL:
        {
            if (*redValue > (redThreshold*0xff))
                *redValue = 0xff;
            else
                *redValue = 0x00;

            if (*greenValue > (greenThreshold*0xff))
                *greenValue = 0xff;
            else
                *greenValue = 0x00;

            if (*blueValue > (blueThreshold*0xff))
                *redValue = 0xff;
            else
                *greenValue = 0xff;

            *blueValue = 0x00;

        }
        break;

        default:
        break;

    } // end switch on colorMode

    double intensity = (double)colorIntensity / (double)MAX_LIVE_COLOR_INTENSITY;

    *redValue = (M_UINT8)(((double)*redValue) * intensity);
    *greenValue = (M_UINT8)(((double)*greenValue) * intensity);
    *blueValue = (M_UINT8)(((double)*blueValue) * intensity);

} // AdjustColor

////////////////////////////////////////////////////////////////////////
//
// Function comStringToIndex
//
////////////////////////////////////////////////////////////////////////
int comStringToIndex(CString comString)
{
    if (strcmp((LPCTSTR)comString, "COM1") == 0)
        return(0);
    else if (strcmp((LPCTSTR)comString, "COM2") == 0)
        return(1);
    else if (strcmp((LPCTSTR)comString, "COM3") == 0)
        return(2);
    else if (strcmp((LPCTSTR)comString, "COM4") == 0)
        return(3);
    else
        return(-1);

}

int baudrateToIndex (CSerial::EBaudrate eBaudrate)
{
    int retVal;

    switch (eBaudrate)
    {
    case CSerial::EBaud1200:   retVal = 0; break;
    case CSerial::EBaud2400:   retVal = 1; break;
    case CSerial::EBaud9600:   retVal = 2; break;
    case CSerial::EBaud14400:  retVal = 3; break;
    case CSerial::EBaud19200:  retVal = 4; break;
    case CSerial::EBaud38400:  retVal = 5; break;
    case CSerial::EBaud56000:  retVal = 6; break;
    case CSerial::EBaud57600:  retVal = 7; break;
    case CSerial::EBaud115200: retVal = 8; break;
    default:                   retVal = 2; break;
    }

    return retVal;
}

int databitsToIndex (CSerial::EDataBits eDataBits)
{
    int retVal;

    switch (eDataBits)
    {
    case CSerial::EData5: retVal = 0; break;
    case CSerial::EData6: retVal = 1; break;
    case CSerial::EData7: retVal = 2; break;
    case CSerial::EData8: retVal = 3; break;
    default:              retVal = 3; break;
    }

    return retVal;
}

int parityToIndex (CSerial::EParity eParity)
{
    int retVal;

    switch (eParity)
    {
    case CSerial::EParNone:  retVal = 0; break;
    case CSerial::EParOdd:   retVal = 1; break;
    case CSerial::EParEven:  retVal = 2; break;
    case CSerial::EParMark:  retVal = 3; break;
    case CSerial::EParSpace: retVal = 4; break;
    default:                 retVal = 0; break;
    }

    return retVal;
}

int stopbitsToIndex (CSerial::EStopBits  eStopBits)
{
    int retVal;

    switch (eStopBits)
    {
    case CSerial::EStop1:   retVal = 0; break;
    case CSerial::EStop1_5: retVal = 1; break;
    case CSerial::EStop2:   retVal = 2; break;
    default:                retVal = 0; break;
    }

    return retVal;
}

int handshakeToIndex(CSerial::EHandshake eHandshake)
{
    int retVal;

    switch (eHandshake)
    {
    case CSerial::EHandshakeOff:      retVal = 0; break;
    case CSerial::EHandshakeSoftware: retVal = 1; break;
    case CSerial::EHandshakeHardware: retVal = 2; break;
    default:                          retVal = 2; break;
    }

    return retVal;
}

CSerial::EBaudrate  indexToBaudrate(int index)
{
    // Determine baudrate
    CSerial::EBaudrate eBaudrate = CSerial::EBaudUnknown;
    switch (index)
    {
    case 0:  eBaudrate = CSerial::EBaud1200;   break;
    case 1:  eBaudrate = CSerial::EBaud2400;   break;
    case 2:  eBaudrate = CSerial::EBaud9600;   break;
    case 3:  eBaudrate = CSerial::EBaud14400;  break;
    case 4:  eBaudrate = CSerial::EBaud19200;  break;
    case 5:  eBaudrate = CSerial::EBaud38400;  break;
    case 6:  eBaudrate = CSerial::EBaud56000;  break;
    case 7:  eBaudrate = CSerial::EBaud57600;  break;
    case 8:  eBaudrate = CSerial::EBaud115200; break;
    default:
        break;
    }

    return eBaudrate;
}

CSerial::EDataBits indexToDatabits(int index)
{
    CSerial::EDataBits eDataBits = CSerial::EDataUnknown;
    switch (index)
    {
    case 0:  eDataBits = CSerial::EData5; break;
    case 1:  eDataBits = CSerial::EData6; break;
    case 2:  eDataBits = CSerial::EData7; break;
    case 3:  eDataBits = CSerial::EData8; break;
    default:
        break;
    }

    return eDataBits;
}

CSerial::EParity indexToParity(int index)
{
    CSerial::EParity eParity = CSerial::EParUnknown;
    switch (index)
    {
    case 0: eParity = CSerial::EParNone;  break;
    case 1: eParity = CSerial::EParOdd;   break;
    case 2: eParity = CSerial::EParEven;  break;
    case 3: eParity = CSerial::EParMark;  break;
    case 4: eParity = CSerial::EParSpace; break;
    default:
        break;
    }

    return eParity;
}

CSerial::EStopBits indexToStopbits(int index)
{
    CSerial::EStopBits eStopBits = CSerial::EStopUnknown;
    switch (index)
    {
    case 0: eStopBits = CSerial::EStop1;   break;
    case 1: eStopBits = CSerial::EStop1_5; break;
    case 2: eStopBits = CSerial::EStop2;   break;
    default:
        break;
    }

    return eStopBits;
}

CSerial::EHandshake indexToHandshake(int index)
{
    CSerial::EHandshake eHandshake = CSerial::EHandshakeUnknown;
    switch (index)
    {
    case 0: eHandshake = CSerial::EHandshakeOff;      break;
    case 1: eHandshake = CSerial::EHandshakeSoftware; break;
    case 2: eHandshake = CSerial::EHandshakeHardware; break;
    default:
        break;
    }

    return eHandshake;
}

SerialCommandT ProcessSerialString(char* serialString,
                                   M_UINT16* commandValue)
{
    SerialCommandT retVal = SERIAL_INVALID;

    // Display the fetched string
//    logFile << "Received string [" << serialString << "]" << endl;

    *commandValue = 0;

    // Search forward
    char* pdest  = strchr( serialString, (int)'=');
    int result = pdest - serialString + 1;

    if (pdest != NULL) // found equal sign
    {
        char commandBuff[20];
        char valueBuff[20];

        for (int v=0; v<20; ++v)
        {
            commandBuff[v] = 0;
            valueBuff[v] = 0;
        }

        int j = 0;

        for (int i=0 ; i<result ; ++i)
        {
            if(IsCharAlpha(serialString[i]) == TRUE)
            {
                commandBuff[j++] = serialString[i];
            }
        }

        commandBuff[j] = '\0';

//        strncpy(commandBuff, serialString, result-1);
//        logFile << "Command is [" << commandBuff << "]" << endl;

        if (_strnicmp(commandBuff, "PAGE", 4) == 0)
        {
            retVal = SERIAL_PAGE;
        }
        else if (_strnicmp(commandBuff, "CUE", 3) == 0)
        {
            retVal = SERIAL_CUE;
        }
        else
        {
            return SERIAL_INVALID;
        }

//        logFile << "commandBuff [" << commandBuff << "], commandType [" << retVal << "]"<< endl;

        j = 0;
        int k = result; // result is where '=' is

        while (serialString[k] != '\r' &&
               serialString[k] != '\n' &&
               serialString[k] != EOF)
        {
            if (isdigit((unsigned char)(serialString[k])))
            {
                valueBuff[j++] = serialString[k];
            }

            ++k;
        }

        int tempVal = atoi(valueBuff);

        switch(retVal)
        {
            case SERIAL_PAGE:
            {
                if(tempVal < 1 || tempVal > 12)
                {
                    return SERIAL_INVALID;
                }
            }
            break;

            case SERIAL_CUE:
            {
                if(tempVal < 1 || tempVal > 48)
                {
                    return SERIAL_INVALID;
                }
            }
            break;

            default:
                break;
        }

        *commandValue = (M_UINT16)tempVal;
        return retVal;

/*		do
        {
            valueBuff[j++] = serialString[k++];
            valueBuff[j] = '\0';

        } while (serialString[k] != '\0' &&
                 serialString[k] != '\r' &&
                 serialString[k] != '\n' &&
                 serialString[k] != EOF); */


//        logFile << "valueBuff [" << valueBuff << "], commandValue [" << *commandValue << "]"<< endl;
    }

    return SERIAL_INVALID;

}
