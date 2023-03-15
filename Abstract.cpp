//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Abstract.cpp                               *
// * Description: This file contains definitions of the    *
// *              abstract generation methods of the       *
// *              abstract class.                          *
// *                                                       *
// *    NOTE: Adapted from Laser Illusions GWS code,       *
// *          Copyright 1998.  Used and distributed        *
// *          with the kind permission of Master Rick      *
// *          Gebhardt.                                    *
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
#include <cmath>
#include <iostream>
#include <fstream>
#include <direct.h>
#include <time.h>

#include "stdafx.h"

#ifndef ABSTRACT_H
#include "abstract.h"
#endif

#ifndef POINT_H
#include "point.h"
#endif

#ifndef GLOBALS_H
#include "globals.h"
#endif

#ifndef VIEW_H
#include "view.h"
#endif

#ifndef ROUTINES_H
#include "routines.h"
#endif

using namespace std; 

extern ofstream logFile;
extern char appDirName[_MAX_PATH];


///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Function: Abstract::Abstract
//
// Input Parameters:
//  None
//
// Returns: N/A
//
// Description:
// This is the constructor for the Abstract class.  It will initialize
// the current sine radian angle and cosine radian angles to zero.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

Abstract::Abstract(const char* absFileName) :
  Track(),
  Z(0),
  Zz(0),
  Zzz(0),
  Zr(0),
  Zzr(0),
  Zzzr(0),
  numIntVals(0),
  numInt2Vals(0),
  numRealVals(0),
  G(0),
  numFillPoints(0),
  xDiff(0),
  XReal(0),
  YReal(0),
  Formula(1),
  GainZ(0),
  GainZz(0),
  GainZzz(0),
  OffsetZ(0),
  OffsetZz(0),
  OffsetZzz(0),
  ZChngNum(0),
  ZzChngNum(0),
  ZzzChngNum(0),
  redOffset(0),
  greenOffset(45),
  blueOffset(90),
  currentPattern(1),
  Blankerizer(0.0),
  Colorizer(0.0),
  X_Scan(-30000.0),
  firstSegment(M_TRUE),
  envelopeShape(SINE),
  envelopeDepth(0),
  lastZSlope(0.0),
// following moved to members
// from static local in GetNextSegment()
  incAmount(0.0),
  pointCount(0)
{
#ifdef ABSTRACT_LOG_ENTRY
   logFile << "Entering Abstract::Abstract()" << endl;
#endif

    char currentDirName[_MAX_PATH];
    M_UINT16 i=0;

    // Get the current working directory:
    if( _getcwd( currentDirName, _MAX_PATH ) == NULL )
    {
#ifdef ABSTRACT_LOG_ERROR
           logFile << "Abstract::Abstract(): Unable to get current directory name." 
                   << endl;
#endif
           okFlag = M_FALSE;
           return;
    }

    // Change to the "app" directory...
    if (_chdir(appDirName) == -1)
    {
#ifdef ABSTRACT_LOG_ERROR
           logFile << "Abstract::Abstract(): Unable to change to app directory\n" 
                   << appDirName << endl;
#endif
           okFlag = M_FALSE;
           return;
    }

    FILE*    absFilePtr;
    fopen_s(&absFilePtr, &absFileName[0], "rb");

    if (absFilePtr) // file was successfully opened
    {
       if (!fscanf(absFilePtr,"%d", (int*)&numIntVals))
       {
#ifdef ABSTRACT_LOG_ERROR
           logFile << "Abstract::Abstract(): Unable to read INT header info." 
                   << endl;
#endif
           fclose(absFilePtr);
           okFlag = M_FALSE;
           return;
       }

       for (i=0 ; i<numIntVals ; ++i)
       {
           if (!fscanf(absFilePtr,"%d", &(intArray[i])))
           {
#ifdef ABSTRACT_LOG_ERROR
               logFile << "Abstract::Abstract(): Can't read next entry "
                << "from file " << &absFileName[0] << endl;
#endif
               fclose(absFilePtr);
               okFlag = M_FALSE;
               return;
           }

#ifdef ABSTRACT_LOG_INFO
           logFile << "Abstract::Abstract(): intArray[" << dec << i
                   << "] = " << intArray[i] << endl;
#endif

       }

       if (!fscanf(absFilePtr,"%d", (int*)&numRealVals))
       {
#ifdef ABSTRACT_LOG_ERROR
           logFile << "Abstract::Abstract(): Unable to read REAL header info." 
                   << endl;
#endif
           fclose(absFilePtr);
           okFlag = M_FALSE;
           return;
       }

       for (i=0 ; i<numRealVals ; ++i)
       {
           if (!fscanf(absFilePtr,"%lf", &(realArray[i])))
           {
#ifdef ABSTRACT_LOG_ERROR
               logFile << "Abstract::Abstract(): Can't read next entry "
                << "from file " << &absFileName[0] << endl;
#endif
               fclose(absFilePtr);
               okFlag = M_FALSE;
               return;
           }

#ifdef ABSTRACT_LOG_INFO
           logFile << "Abstract::Abstract(): realArray[" << dec << i
                   << "] = " << realArray[i] << endl;
#endif

       }

       if (!fscanf(absFilePtr,"%d", (int*)&numInt2Vals))
       {
#ifdef ABSTRACT_LOG_ERROR
           logFile << "Abstract::Abstract(): Unable to read INT2 header info." 
                   << endl;
#endif
       }
       else
       {
           for (i=0 ; i<numInt2Vals ; ++i)
           {
               if (!fscanf(absFilePtr,"%d", &(int2Array[i])))
               {
#ifdef ABSTRACT_LOG_ERROR
                   logFile << "Abstract::Abstract(): Can't read next entry "
                           << "from file " << &absFileName[0] << endl;
#endif
                   fclose(absFilePtr);
                   okFlag = M_FALSE;
                   return;
               }

#ifdef ABSTRACT_LOG_INFO
               logFile << "Abstract::Abstract(): int2Array[" << dec << i
                       << "] = " << int2Array[i] << endl;
#endif

           } // end loop through file entries

       } // end else file header read okay

       fclose(absFilePtr);

    } // else file opened okay
    else
    {
#ifdef ABSTRACT_LOG_ERROR
        logFile << "Abstract::Abstract(): Unable to open file " 
                << &absFileName[0] << endl;
#endif
        okFlag = M_FALSE;
    }

    // initialize list of last displayed points
    next.x = 0x0000;
    next.y = 0x0000;
    next.z = 0x0000;
    next.status = 0;
    curr.x = 0x0000;
    curr.y = 0x0000;
    curr.z = 0x0000;
    curr.status = 0x0000;

    // Change to the "root" directory...
    if (_chdir(currentDirName) == -1)
    {
#ifdef ABSTRACT_LOG_ERROR
           logFile << "Abstract::Abstract(): Unable to return to working directory\n" 
                   << currentDirName << endl;
#endif
           okFlag = M_FALSE;
           return;
    }


} // end Abstract::Abstract


///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Function: Abstract::~Abstract
//
// Input Parameters:
//  None
//
// Returns: N/A
//
// Description:
// This is the destructor for the Abstract class.  Nothing to delete,
// so just return.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

Abstract::~Abstract()
{
#ifdef ABSTRACT_LOG_ENTRY
   logFile << "Entering Abstract::~Abstract()" << endl;
#endif

    return;

} // end Abstract::~Abstract


///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Function: Abstract::SetPattern
//
// Input Parameters:
//  const M_UINT16 newPattern :  The number of the new pattern.
//
// Returns: M_UINT16 - M_SUCCESS if values set successfully
//                   M_FAILURE otherwise
//          
//
// Description:
// This method will set the member data related to the current
// abstract pattern with data obtained from the integer and
// real pattern arrays.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

M_UINT16
Abstract::SetPattern(const M_UINT16 newPattern)
{
#ifdef ABSTRACT_LOG_ENTRY
   logFile << "Entering Abstract::SetPattern()" << endl;
#endif

    // Initialize those variables not read from the arrays.
    Z   = 0.0;
    Zz  = 0.0;
    Zzz = 0.0;
    G   = 0.0;
    Blankerizer = 0.0;
    Colorizer   = 0.0;

    int theIndex = 0;

    Formula    = intArray[theIndex++];

    // msvob 4/8/2022: Following looks wrong.  Investigate.
    // If anything, seems like should be...
    // if (Formula & 0x8000)
    // {
    //     usePalette = M_TRUE;
    // }
    usePalette = Formula & 0x8000;
    if (usePalette)
    {
        usePalette = M_TRUE;
    }

    Formula &= 0x000F;

    ZChngNum   = intArray[theIndex++];
    ZzChngNum  = intArray[theIndex++];
    ZzzChngNum = intArray[theIndex++];
    GChngNum   = intArray[theIndex++];
    ColorPtn   = intArray[theIndex++];
    // Set to default if old .ABS file
    if (ColorPtn == 0)
    {
        ColorPtn = 1;
    }
    ColorSpd   = intArray[theIndex++];
    BlankSpd   = intArray[theIndex++];
    BlankSpot  = intArray[theIndex++];
    ZRed       = intArray[theIndex++];
    ZBlue      = intArray[theIndex++];
    ZGreen     = intArray[theIndex++];
    envelopeDepth = intArray[theIndex++];
    envelopeShape = (enum EnvelopeShape)(((int)envelopeDepth & (int)0xF000) >> 12);
    envelopeDepth = envelopeDepth & 0x0FFF;

    theIndex = 0;

    Zr    = realArray[theIndex++];
    Zzr   = realArray[theIndex++];
    Zzzr  = realArray[theIndex++];
    GReal = realArray[theIndex++];

    if(numRealVals == 12)
    {
        Z   = realArray[theIndex++];
        Zz  = realArray[theIndex++];
        Zzz = realArray[theIndex++];
        G   = realArray[theIndex++];
        Blankerizer = realArray[theIndex++];
        Colorizer   = realArray[theIndex++];
    }

    theIndex = 0;

    redOffset    = int2Array[theIndex++];	
    greenOffset  = int2Array[theIndex++];	
    blueOffset   = int2Array[theIndex++];	
    GainZ        = int2Array[theIndex++];	
    GainZz       = int2Array[theIndex++];	
    GainZzz      = int2Array[theIndex++];	
    OffsetZ      = int2Array[theIndex++];	
    OffsetZz     = int2Array[theIndex++];	
    OffsetZzz    = int2Array[theIndex++];	
    ZAxisFormula = int2Array[theIndex++];
    ZAxisGain    = int2Array[theIndex++];
    AbsPtsPerFrame = int2Array[theIndex++];
    // set the number of abstract points per frame to
    // a reasonable default if the '.abs' file read is
    // an older version
    if (AbsPtsPerFrame == 0)
    {
        AbsPtsPerFrame = 400;
    }

    ZAxisOffset = int2Array[theIndex++];	

    return(M_SUCCESS);

} // end Abstract::SetPattern


///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Function: Abstract::GetNextSegment
//
// Input Parameters:
//  Point* currPointPtr
//  Point* nextPointPtr
//
// Returns: M_UINT16 - M_SUCCESS if values set successfully
//                   M_FAILURE otherwise
//          
//
// Description:
// This method will set the member data related to the current
// abstract pattern with data obtained from the integer and
// real pattern arrays.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

/* static double 
ResetCycle(double cycleValue, double cycleThreshold)
{
    double tempValue = cycleValue;

    if (cycleValue >= cycleThreshold)
    {
        tempValue = cycleValue / (2*PI);
        tempValue = (tempValue - (int)tempValue) * (2*PI);
//        tempValue = 0;
    }
    return tempValue;
}

static double 
ResetCycle(double cycleValue, double cycleThreshold, M_UINT16 &blnkThis)
{
    // Note that blnkThis only goes from M_FALSE to M_TRUE.  If
    // already M_TRUE, will not be affected.
    double tempValue = cycleValue;

    if (cycleValue >= cycleThreshold)
    {
        blnkThis = M_TRUE;
        tempValue = cycleValue / (2*PI);
        tempValue = (tempValue - (int)tempValue) * (2*PI);
//        tempValue = 0;
    }
    return tempValue;
}

static double
ResetCycleNew(double cycleValue,
              double incrementValue,
              double cycleThreshold)
{
    double tempValue = cycleValue + incrementValue;

    if (tempValue >= cycleThreshold)
    {
        return (tempValue - cycleThreshold);
    }

    return tempValue;
}
            
static double
ResetCycleNew(double cycleValue,
              double incrementValue,
              double cycleThreshold,
              M_UINT16 &blnkThis)
{
    double tempValue = cycleValue + incrementValue;

    if (tempValue >= cycleThreshold)
    {
        blnkThis = M_TRUE;
        return (tempValue - cycleThreshold);
    }

    return tempValue;
}
*/
            
////////////////////////////////////////////////////////////////////////
//
// Method Abstract::GetNextSegment
//
////////////////////////////////////////////////////////////////////////

M_UINT16
Abstract::GetNextSegment(Point*  const currPointPtr,
                         Point*  const nextPointPtr,
                         M_UINT8& redValue,
                         M_UINT8& greenValue,
                         M_UINT8& blueValue,
                         M_UINT16& useDiscreet)
{
#ifdef ABSTRACT_LOG_ENTRY
   logFile << "Entering Abstract::GetNextSegment()" << endl;
#endif

    double Gain;
    double X2a;
    double Y2a;
    double X2b;
    double Y2b;
    M_UINT16 blankThis;

    static M_UINT16 fullGain;

    useDiscreet = usePalette;
        
    if (firstSegment == M_TRUE)
    {
        X_Scan = -30000;
        blankThis = M_TRUE;
        fullGain  = M_TRUE;
        firstSegment = M_FALSE;
    }
    else
    {
        blankThis = M_FALSE;
    }

    curr = next;

    // msvob 4/8/2022: Assuming RGB values aren't needed here,
    // but make a note in case you need to revisit...
    currPointPtr->SetPointInfo(curr.x,
                               curr.y,
                               curr.z,
                               GetMsbFromUshort(curr.status),
                               GetLsbFromUshort(curr.status),
                               0x00, 0x00, 0x00);

//  ****************************************************************
    // Here we're in the process of calculating the fill points.
//  ****************************************************************
    if (numFillPoints > 0 && xDiff != 0)
    {
#ifdef ABSTRACT_LOG_INFO
        logFile << "Calculating fill points..." << endl;
#endif
        next.x = curr.x + xDiff;
        next.z = curr.z + zDiff;
        next.y = (M_INT16)((mX * (double)next.x) + bX);
        next.status = savedNext.status;
        redValue   = savedRed;
        greenValue = savedGreen;
        blueValue  = savedBlue;
        --numFillPoints;
    } // end if calculating fill points

//  ****************************************************************
    // Here we're finishing up the process of filling in
    // extra points.  We need to display the "next" point
    // we calculated and saved prior to starting the fill
    // process.
//  ****************************************************************
    else if(numFillPoints == 0 && xDiff != 0)
    {
#ifdef ABSTRACT_LOG_INFO
        logFile << "Finishing the fill points (original next)..." << endl;
#endif
        next = savedNext;
        redValue   = savedRed;
        greenValue = savedGreen;
        blueValue  = savedBlue;

        xDiff = 0;
        mX = 0.0;
        bX = 0.0;

    } // end else if we're finishing the fill points
        
//  ****************************************************************
    // In this scenario, we're finding the next point normally,
    // and will check to see if we need to add additional points
    // in between after we've made the next X and Y calculations.
//  ****************************************************************
    else
    {
        Z  = ResetCycle(Z + Zr + (((double)OffsetZ/10.0)*(PI/720.0)), 100000.0*PI, blankThis);
        Zz = ResetCycle(Zz + Zzr + (((double)OffsetZz/10.0)*(PI/720.0)), 100000.0*PI, blankThis);
        
        if (Zzz == 0.0)
        {
            Zzz  = 0.5;
            Zzzr = 0.0;
//            Zzz  = ::sin(rand());
//		 	  Zzzr = (Z + Zz) / rand();

        }
        else
        {
            Zzz = ResetCycle(Zzz + Zzzr + (((double)OffsetZzz/10.0)*(PI/720.0)), 100000.0*PI, blankThis);
        }

#ifdef ABSTRACT_LOG_INFO
        logFile << "Normal current/next calculation scenario..." << "\n"
                << "Abstract::GetNextSegment(): Z = " << dec << Z << "\n"
                << "                           Zz = " << Zz << endl;
#endif

        Gain  = 0.0;
        X2a   = 0.0;
        Y2a   = 0.0;
        X2b   = 0.0;
        Y2b   = 0.0;

        XReal = 0.0;
        YReal = 0.0;

        double tempVal = ::sin(GReal);
        Gain = tempVal;

        if (tempVal < 0)
        {
            Gain = -Gain;
        }

        Gain = 1.0 - (Gain * (envelopeDepth/100.0));

        if (envelopeShape == SQUARE)
        {
            if (tempVal <= ((100-envelopeDepth)/100.0))
            {
                Gain = ((100-envelopeDepth)/100.0);
                if (fullGain == M_TRUE)
                {
                    fullGain  = M_FALSE;
                    blankThis = M_TRUE;
                }
            }
            else
            {
                Gain = 1.0;
                if (fullGain == M_FALSE)
                {
                    fullGain  = M_TRUE;
                    blankThis = M_TRUE;
                }
            }

        } // end if envelopeShape is SQUARE

/*
        if (Formula != 2)
        {
            if (tempVal < 0)
            {
                Gain = -Gain;
            }

            Gain = 1.0 - (Gain * (envelopeDepth/100.0));
        }
        else
        {
            if (tempVal < 0)
            {
                Gain = (-Gain * (envelopeDepth/100.0)) - 1.0;
            }
            else
            {
                Gain = 1.0 - (Gain * (envelopeDepth/100.0));   
            }
        }

        if (envelopeShape == SQUARE)
        {
            double depthLevel;

            if (Formula == 2 && tempVal < 0)
            {
                depthLevel = (envelopeDepth - 100) / 100.0;
            }
            else
            {
                depthLevel = (100 - envelopeDepth) / 100.0;
            }

            if(tempVal <= depthLevel)
            {
                Gain = depthLevel;
                if (fullGain == M_TRUE)
                {
                    fullGain  = M_FALSE;
                    blankThis = M_TRUE;
                }
            }
            else
            {
                if (Formula == 2 && tempVal < 0)
                    Gain = -1.0;
                else
                    Gain = 1.0;

                if (fullGain == M_FALSE)
                {
                    fullGain  = M_TRUE;
                    blankThis = M_TRUE;
                }
            }

        } // end if envelopeShape is SQUARE
*/

// Keep the following to use in future if formatted output ever needed.  Note,
// however, that firstSegment is NEVER TRUE at this point.
/*	if (firstSegment == M_TRUE)
    {
    logFile << "Abstract initial values...\n"
            << "current point is: (" << curr.x << "," << curr.y << "," << curr.z << "," << curr.status << ")\n"
            << "   next point is: (" << next.x << "," << next.y << "," << next.z << "," << next.status << ")\n"
            << "        redValue: " << redValue << "\n"
            << "      greenValue: " << greenValue << "\n"
            << "       blueValue: " << blueValue << "\n"
            << "           xDiff: " << xDiff << "\n"
            << "           zDiff: " << zDiff << "\n"
            << "   numFillPoints: " << numFillPoints << "\n"
            << "              mX: " << mX << "\n"
            << "              bX: " << bX << "\n"
            << "       blankThis: " << blankThis << "\n"
            << "          X_Scan: " << X_Scan << "\n"
            << "        fullGain: " << fullGain << "\n"
            << "    firstSegment: " << firstSegment << "\n"
            << "               Z: " << Z << "\n"
            << "              Zz: " << Zz << "\n"
            << "             Zzz: " << Zzz << "\n"
            << "======================================\n"
            << "      usePalette: " << usePalette << "\n"
            << "         Formula: " << Formula << "\n"
            << "        ZChngNum: " << ZChngNum << "\n"
            << "       ZzChngNum: " << ZzChngNum << "\n"
            << "      ZzzChngNum: " << ZzzChngNum << "\n"
            << "        GChngNum: " << GChngNum << "\n"
            << "        ColorPtn: " << ColorPtn << "\n"
            << "        ColorSpd: " << ColorSpd << "\n"
            << "        BlankSpd: " << BlankSpd << "\n"
            << "       BlankSpot: " << BlankSpot << "\n"
            << "            ZRed: " << ZRed << "\n"
            << "          ZGreen: " << ZGreen << "\n"
            << "           ZBlue: " << ZBlue << "\n"
            << "   envelopeDepth: " << envelopeDepth << "\n"
            << "   envelopeShape: " << envelopeShape << "\n"
            << "======================================\n"
            << "              Zr: " << Zr << "\n"
            << "             Zzr: " << Zzr << "\n"
            << "            Zzzr: " << Zzzr << "\n"
            << "           GReal: " << GReal << "\n"
            << "      ExtraReal1: " << ExtraReal1 << "\n"
            << "======================================\n"
            << "       redOffset: " << redOffset << "\n"
            << "     greenOffset: " << greenOffset << "\n"
            << "      blueOffset: " << blueOffset << "\n"
            << "           GainZ: " << GainZ << "\n"
            << "          GainZz: " << GainZz << "\n"
            << "         GainZzz: " << GainZzz << "\n"
            << "         OffsetZ: " << OffsetZ << "\n"
            << "        OffsetZz: " << OffsetZz << "\n"
            << "       OffsetZzz: " << OffsetZzz << "\n"
            << "    ZAxisFormula: " << ZAxisFormula << "\n"
            << "       ZAxisGain: " << ZAxisGain << "\n"
            << "  AbsPtsPerFrame: " << AbsPtsPerFrame << "\n"
            << "     ZAxisOffset: " << ZAxisOffset << endl;

            firstSegment = M_FALSE;
    } */

//  ****************************************************************
        // Calculate the next X & Y values based on the
        // selected formula.
//  ****************************************************************
        switch(Formula)
        {
            case 1:
                XReal = (GainZ / 100.0) * ::sin(Z);
                YReal = (GainZ / 100.0) * ::cos(Z);
                X2a   = (GainZz / 100.0) * ::sin(Zz) * Gain;
                Y2a   = (GainZz / 100.0) * ::cos(Zz) * Gain;
                X2b   = (GainZzz / 100.0) * ::sin(Zzz);
                Y2b   = (GainZzz / 100.0) * ::cos(Zzz);
                next.x = (M_INT16)(((XReal+X2a+X2b+2.0) * 10000) - 20000);
                next.y = (M_INT16)(((YReal+Y2a+Y2b+2.0) * 10000) - 20000);
            break;

            case 2:
                XReal = (GainZ / 100.0) * ::sin(Z);
                YReal = (GainZ / 100.0) * ::cos(Z);

                X2a   = (GainZz / 100.0) * ::sin(Zz) * ::sin(Zz);
                if (::sin(Zz) < 0)
                {
                    X2a = -X2a * Gain;
                }
                else
                {
                    X2a = X2a * Gain;
                }

                Y2a   = (GainZz / 100.0) * ::cos(Zz) * ::cos(Zz);
                if (::cos(Zz) < 0)
                {
                    Y2a = -Y2a * Gain;
                }
                else
                {
                    Y2a = Y2a * Gain;
                }
                
                X2b   = (GainZzz / 100.0) * ::sin(Zzz);
                Y2b   = (GainZzz / 100.0) * ::cos(Zzz) * ::cos(Zzz) * Gain;
                next.x = (M_INT16)(((XReal+X2a+X2b+2.0) * 10000) - 20000);
                next.y = (M_INT16)(((YReal+Y2a+Y2b+2.0) * 10000) - 20000);
            break;

            case 3:
                XReal = (GainZ / 100.0) * ::sin(Z) * ::sin(Z) * ::sin(Z);
                YReal = (GainZ / 100.0) * ::cos(Z) * ::cos(Z) * ::cos(Z);
                X2a   = (GainZz / 100.0) * ::sin(Zz) * ::sin(Zz) * ::sin(Zz) * Gain;
                Y2a   = (GainZz / 100.0) * ::cos(Zz) * ::cos(Zz) * ::cos(Zz) * Gain;
                X2b   = (GainZzz / 100.0) * ::sin(Zzz);
                Y2b   = (GainZzz / 100.0) * ::cos(Zzz);
                next.x = (M_INT16)(((XReal+X2a+X2b+2) * 10000) - 20000);
                next.y = (M_INT16)(((YReal+Y2a+Y2b+2) * 10000) - 20000);
            break;
        
            case 4:
                XReal = (GainZ / 100.0) * ::sin(Z)  * Gain;
                YReal = (GainZ / 100.0) * ::cos(Z)  * ::cos(((double)OffsetZzz/10.0)*(PI/180.0)) * Gain;
                X2a   = (GainZz / 100.0) * ::sin(Zz) * Gain;
                Y2a   = (GainZz / 100.0) * ::cos(Zz) * ::cos(((double)OffsetZzz/10.0)*(PI/180.0)) * Gain;
                X2b   = (GainZzz / 100.0) * ::sin(Zzz);
                Y2b   = (GainZzz / 100.0) * ::cos(Zzz);

                next.x = (M_INT16)(((XReal+X2a+X2b+2) * 10000) - 20000);
                next.y = (M_INT16)(((YReal+Y2a+Y2b+2) * 10000) - 20000);
            break;

            case 5:
                {
                XReal = (GainZ / 100.0) * ::sin(Z)  * Gain;
                YReal = (GainZ / 100.0) * ::cos(Z)  * Gain;
                X2a   = (GainZz / 100.0) * ::sin(Zz) * Gain;
                Y2a   = (GainZz / 100.0) * ::cos(Zz) * Gain;

                static M_INT16 X2b_Positive;
                static M_INT16 Y2b_Positive;

                if (::sin(Zzz) > 0)
                {
                    X2b   = (GainZzz / 100.0) * 1.0;
                }
                else if (::sin(Zzz) < 0)
                {
                    X2b   = (GainZzz / 100.0) * -1.0;
                }
                else
                {
                    X2b = 0;
                }

                if (X2b >= 0)
                {
                    if (X2b_Positive == M_FALSE)
                    {
                        blankThis = M_TRUE;
                    }

                    X2b_Positive = M_TRUE;
                }
                else
                {
                    if (X2b_Positive == M_TRUE)
                    {
                        blankThis = M_TRUE;
                    }

                    X2b_Positive = M_FALSE;
                }

                if (::cos(Zzz) > 0)
                {
                    Y2b   = (GainZzz / 100.0) * 1.0;
                }
                else if (::cos(Zzz) < 0)
                {
                    Y2b   = (GainZzz / 100.0) * -1.0;
                }
                else
                {
                    Y2b = 0;
                }

                if (Y2b >= 0)
                {
                    if (Y2b_Positive == M_FALSE)
                    {
                        blankThis = M_TRUE;
                    }

                    Y2b_Positive = M_TRUE;
                }
                else
                {
                    if (Y2b_Positive == M_TRUE)
                    {
                        blankThis = M_TRUE;
                    }

                    Y2b_Positive = M_FALSE;
                }

                next.x = (M_INT16)(((XReal+X2a+X2b+2) * 10000) - 20000);
                next.y = (M_INT16)(((YReal+Y2a+Y2b+2) * 10000) - 20000);

                if (abs(curr.x - next.x) > 10000 ||
                    abs(curr.y - next.y) > 10000)
                {
                    blankThis = M_TRUE;
                }

                }
            break;

            case 6:
                {
                XReal = (GainZ / 100.0) * ::sin(Z);
                YReal = (GainZ / 100.0) * ::cos(Z);
                X2a   = (GainZz / 100.0) * ::sin(Zz) * ::sin(Zz) * ::sin(Zz) * Gain;
                Y2a   = (GainZz / 100.0) * ::cos(Zz) * ::cos(Zz) * ::cos(Zz) * Gain;
                static M_INT16 X2b_Positive;
                static M_INT16 Y2b_Positive;

                if (::sin(Zzz) > 0)
                {
                    X2b   = (GainZzz / 100.0) * 1.0;
                }
                else if (::sin(Zzz) < 0)
                {
                    X2b   = (GainZzz / 100.0) * -1.0;
                }
                else
                {
                    X2b = 0;
                }

                if (X2b >= 0)
                {
                    if (X2b_Positive == M_FALSE)
                    {
                        blankThis = M_TRUE;
                    }

                    X2b_Positive = M_TRUE;
                }
                else
                {
                    if (X2b_Positive == M_TRUE)
                    {
                        blankThis = M_TRUE;
                    }

                    X2b_Positive = M_FALSE;
                }

                if (::cos(Zzz) > 0)
                {
                    Y2b   = (GainZzz / 100.0) * 1.0;
                }
                else if (::cos(Zzz) < 0)
                {
                    Y2b   = (GainZzz / 100.0) * -1.0;
                }
                else
                {
                    Y2b = 0;
                }

                if (Y2b >= 0)
                {
                    if (Y2b_Positive == M_FALSE)
                    {
                        blankThis = M_TRUE;
                    }

                    Y2b_Positive = M_TRUE;
                }
                else
                {
                    if (Y2b_Positive == M_TRUE)
                    {
                        blankThis = M_TRUE;
                    }

                    Y2b_Positive = M_FALSE;
                }

                next.x = (M_INT16)(((XReal+X2a+X2b+2) * 10000) - 20000);
                next.y = (M_INT16)(((YReal+Y2a+Y2b+2) * 10000) - 20000);

                if (abs(curr.x - next.x) > 10000 ||
                    abs(curr.y - next.y) > 10000)
                {
                    blankThis = M_TRUE;
                }

                }
            break;

            case 7:
                YReal  = (GainZz / 100.0) * ::sin(Zz);
//				X_Scan += (30*GainZzz)+(Zzzr*100000.0)+DegreeToRadian(OffsetZzz/10);
                X_Scan += (30.0*((double)GainZzz+((double)OffsetZzz/3600.0)))+(Zzzr*100000.0);
                if (X_Scan > 30000)
                {
                    X_Scan = -30000;
                    blankThis = M_TRUE;
                }
                next.x = (M_INT16)X_Scan;
                Y2a = (GainZ / 100.0) * ::sin(X_Scan+(Zr*100.0)+DegreeToRadian(OffsetZ/10));
                next.y = (M_INT16)(Gain * (((YReal+Y2a+2.0) * 10000) - 20000));
            break;

            case 8:
            {
                YReal = (GainZ / 100.0) * ::sin(Z);
                Y2a   = (GainZz / 100.0) * ::sin(Zz);

//				X_Scan += (30*GainZzz)+(Zzzr*100000.0)+DegreeToRadian(OffsetZzz/10);
                X_Scan += (30.0*((double)GainZzz+((double)OffsetZzz/3600.0)))+(Zzzr*100000.0);
                if (X_Scan > 30000)
                {
                    X_Scan = -30000;
                    blankThis = M_TRUE;
                }
                next.x = (M_INT16)X_Scan;
                next.y = (M_INT16)(Gain * (((YReal+Y2a+2.0) * 10000) - 20000));
            }
            break;

            default:
            {
#ifdef ABSTRACT_LOG_ERROR
                logFile << "Abstract::GetNextSegment(): Invalid formula number "
                        << dec << Formula << " specified." << endl;
#endif
                return(M_FAILURE);
            }
          break;

        } // end switch on Formula

#ifdef ABSTRACT_LOG_INFO
       logFile << "Abstract::GetNextSegment(): Gain = " << dec << Gain << "\n"
               << "                         Formula = " << Formula  << "\n"
               << "                           XReal = " << XReal    << "\n"
               << "                           YReal = " << YReal    << "\n"
               << "                             X2a = " << X2a      << "\n"
               << "                             Y2a = " << Y2a      << endl;
#endif

    double xNDC, yNDC;
    double xRadian, yRadian;
    double ZReal;
    double Z2a, Z2b;

    MapAbsToNDC(next.x,
                next.y,
                &xNDC,
                &yNDC);

    xRadian = xNDC * 2.0 * PI;
    yRadian = yNDC * 2.0 * PI;

//  ****************************************************************
    // Calculate next Z value based on selected formula.
//  ****************************************************************
    if (++pointCount > AbsPtsPerFrame)
    {
//        IncrementValue = ResetCycleNew(IncrementValue, ((double)ZAxisOffset/10.0), 360, blankThis);
        IncrementValue = ResetCycle(IncrementValue+((double)ZAxisOffset/10.0), 360, blankThis);

        pointCount = 0;
    }

    switch(ZAxisFormula)
    {
        case 1:
        {
            if (1)
                next.z = (M_INT16)(::sin( (::sqrt(::pow((double)(next.x),2) + ::pow((double)(next.y),2)) / 6000.0) + DegreeToRadian(IncrementValue)) * 10000.0);
            else
                next.z = curr.z;
//              next.z = ::sin( sqrt(pow(next.x,2) + pow(next.y,2)) / (3600.0 + (IncrementValue*10.0))) * 10000.0;
//              next.z = ::sin( sqrt(pow(next.x,2) + pow(next.y,2)) / (2000.0 + ((double)ZAxisOffset * 10.33333))) * 10000.0;
//            next.z = 0.0;
//            next.z = ((double)(abs(next.x) / WORLD_MAX)) * abs(next.x);
        }
            break;
        case 2:
            {
                ZReal = ::cos(xRadian+DegreeToRadian((M_UINT16)(ZAxisOffset/24))) * ::cos(yRadian);
                next.z = (M_INT16)((ZReal * 50000) - 100000);
            }
            break;
        case 3:
            {
//                GenericMapAtoB(next.x,
//				               next.y,
//							   (double)WORLD_MIN,
//							   (double)WORLD_MAX,
//							   (double)0,
//							   (1.0 * PI),
//		                       &xRadian,
//				               &yRadian);
//
//			    ZReal = ::sin(xRadian+DegreeToRadian((M_UINT16)ZAxisOffset));
//			    next.z = (ZReal * (double)WORLD_MAX);
                
                ZReal = -::cos(xRadian+DegreeToRadian((M_UINT16)(ZAxisOffset/24)));
                next.z = (M_INT16)((ZReal * 50000) - 100000);
            }
            break;
        case 4:
            {
                ZReal = (GainZ / 100.0) * ::cos(Z+yRadian+DegreeToRadian((M_UINT16)(ZAxisOffset/4)));
                Z2a   = (GainZz / 100.0) * ::cos(Zz+yRadian+DegreeToRadian((M_UINT16)(ZAxisOffset/4)+90));
                Z2b   = (GainZzz / 100.0) * ::cos(Zzz+yRadian+DegreeToRadian((M_UINT16)(ZAxisOffset/4)+180));
                next.z = (M_INT16)(((ZReal+Z2a+Z2b+2.0) * 10000) - 20000);
            }
            break;
        case 5:
            {
                ZReal = (GainZ / 100.0) * ::sin(Z+xRadian+DegreeToRadian((M_UINT16)(ZAxisOffset  )));
                Z2a   = (GainZz / 100.0) * ::sin(Zz+yRadian+DegreeToRadian((M_UINT16)(ZAxisOffset  )  ));
                Z2b   = (GainZzz / 100.0) * ::sin(Zzz+xRadian+DegreeToRadian((M_UINT16)(ZAxisOffset  )  ));
                next.z = (M_INT16)(((ZReal+Z2a+Z2b+2.0) * 10000) - 20000);
            }
            break;
        case 6:
            {
                next.z =  (M_INT16)((- sqrt(pow((double)(next.x),2) + pow((double)(next.y),2)) * 1.5) + (ZAxisOffset * 100));
//              ZReal = (GainZ / 100.0) * ::sin(Z+xRadian+DegreeToRadian((M_UINT16)(ZAxisOffset/4)));
//				Z2a   = (GainZz / 100.0) * ::sin(Zz+xRadian+DegreeToRadian((M_UINT16)(ZAxisOffset/4)+90));
//				Z2b   = (GainZzz / 100.0) * ::sin(Zzz+xRadian+DegreeToRadian((M_UINT16)(ZAxisOffset/4)+180));
//              next.z = ((ZReal+Z2a+Z2b+2.0) * 10000) - 20000;
            }
            break;
        case 7:
        {
            if (! X2b)
            {
//                ZReal = (GainZ / 100.0) * ::cos(Z+DegreeToRadian(ZAxisOffset)) * Gain;
                ZReal = (GainZ / 100.0) * ::cos(Z+DegreeToRadian(ZAxisOffset));
                Z2b   = (GainZz / 100.0) * ::cos(Zz+DegreeToRadian(ZAxisOffset));
                next.z = (M_INT16)(((ZReal+Z2b+2) * 10000) - 20000);
            }
            else
            {
                if (!GainZ)
                {
//                    ZReal = (GainZzz / 100.0) * ::cos(Zzz+DegreeToRadian(ZAxisOffset)) * Gain;
                    ZReal = (GainZzz / 100.0) * ::cos(Zzz+DegreeToRadian(ZAxisOffset));
                }
                else
                {
//                    ZReal = (GainZ / 100.0) * ::cos(Z+DegreeToRadian(ZAxisOffset)) * Gain;
                    ZReal = (GainZ / 100.0) * ::cos(Z+DegreeToRadian(ZAxisOffset));
                }
                next.z = (M_INT16)(((ZReal+1.0) * 20000) - 20000);
            }
        }
            break;
        case 8:
        {
            if (! X2a)
            {
//                ZReal = (GainZ / 100.0) * ::sin(Z+DegreeToRadian(ZAxisOffset)) * Gain;
                ZReal = (GainZ / 100.0) * ::sin(Z+DegreeToRadian(ZAxisOffset));
                Z2a   = (GainZz / 100.0) * ::sin(Zz+DegreeToRadian(ZAxisOffset));
                next.z = (M_INT16)((((ZReal+Z2a+2) * 10000) - 20000) * Gain);

            }
            else
            {
                if (!GainZ)
                {
//                    ZReal = (GainZzz / 100.0) * ::sin(Zzz+DegreeToRadian(ZAxisOffset)) * Gain;
                    ZReal = (GainZzz / 100.0) * ::sin(Zzz+DegreeToRadian(ZAxisOffset));
                }
                else
                {
//                    ZReal = (GainZ / 100.0) * ::sin(Z+DegreeToRadian(ZAxisOffset)) * Gain;
                    ZReal = (GainZ / 100.0) * ::sin(Z+DegreeToRadian(ZAxisOffset));
                }
                next.z = (M_INT16)((((ZReal+1.0) * 20000) - 20000) * Gain);
            }
        }
            break;
        default:
            next.z  = 0;
            break;

    } // end switch on Z Axis Formula

/*    M_INT16 currZDelta = abs(next.z-curr.z);

    if(abs(currZDelta - lastZDelta) > 5000)
    {
// logFile << "(" << curr.x << "," << next.x <<") "
//         << "(" << curr.y << "," << next.y <<") "
//         << "(" << curr.z << "," << next.z <<")" << endl;
        blankThis = M_TRUE;
    }

    lastZDelta = currZDelta; */

    next.z = (M_INT16)((double)next.z*((double)ZAxisGain / 100.0));

    double deltaX = (double)(abs(next.x - curr.x));
    double deltaY = (double)(abs(next.y - curr.y));
    double deltaZ = (double)(abs(next.z - curr.z));

    if (ZAxisFormula == 1)
    {
        // need the following to prevent wierd lines from appearing
        // between segments when the slope in the Z axis is too great...
        // still needs work... 
        double tempDelta = (deltaX > deltaY) ? deltaX : deltaY;

        if (tempDelta == 0)
        {
            tempDelta = 0.0000001;
        }

        double tempZSlope = deltaZ / tempDelta;
        double slopeDelta = tempZSlope - lastZSlope;
        slopeDelta = (slopeDelta < 0) ? -slopeDelta : slopeDelta;

        if (slopeDelta > 0.3)
            blankThis = M_TRUE;

        lastZSlope = tempZSlope;
    }


//  ****************************************************************
        // Calculate color and blanking for the next segment.
//  ****************************************************************
        Blankerizer = ResetCycle(Blankerizer+((double)BlankSpd/1800.0), DegreeToRadian(3600));
//        Colorizer   = ResetCycle(Colorizer+((double)ColorSpd/1800.0), DegreeToRadian(3600));

        static double redHold;
        static double greenHold;
        static double blueHold;

        if (ColorPtn != 3)
        Colorizer   = ResetCycle(Colorizer+((double)ColorSpd/1800.0), DegreeToRadian(3600));

        if (ColorSpd == 0)
        {
            redOffset<=255   ? ZRed=redOffset     : ZRed=255; 
            greenOffset<=255 ? ZGreen=greenOffset : ZGreen=255; 
            blueOffset<=255  ? ZBlue=blueOffset   : ZBlue=255; 
        }
        else
        {
            switch(ColorPtn)
            {
                case 1:
                {
                    ZRed   = (M_INT16)((::sin(Colorizer+DegreeToRadian(redOffset)) + 1.0) * 127.5);
                    ZGreen = (M_INT16)((::sin(Colorizer+DegreeToRadian(greenOffset)) + 1.0) * 127.5);
                    ZBlue  = (M_INT16)((::sin(Colorizer+DegreeToRadian(blueOffset)) + 1.0) * 127.5);
                }
                break;

                case 2:
                {
                    ZRed   = (M_INT16)((::sin(Colorizer+DegreeToRadian(redOffset)+Z) + 1.0) * 127.5);
                    ZGreen = (M_INT16)((::sin(Colorizer+DegreeToRadian(greenOffset)+Zz) + 1.0) * 127.5);
                    ZBlue  = (M_INT16)((::sin(Colorizer+DegreeToRadian(blueOffset)+Zzz) + 1.0) * 127.5);
                }
                break;

                case 3:
                {
                    ZRed   = (M_INT16)((::sin(Colorizer+DegreeToRadian(redOffset)+OffsetZ) + 1.0) * 127.5);
                    ZGreen = (M_INT16)((::sin(Colorizer+DegreeToRadian(greenOffset)+OffsetZz) + 1.0) * 127.5);
                    ZBlue  = (M_INT16)((::sin(Colorizer+DegreeToRadian(blueOffset)+OffsetZzz) + 1.0) * 127.5);

// following are two failed (so far) attempts at coming
// up with an interesting ramp color algorithm...
//                      redHold = (redHold + (((double)ColorSpd/1800.0)*((double)redOffset/360.0)));
//                      greenHold = (greenHold + (((double)ColorSpd/1800.0)*((double)greenOffset/360.0)));
//                      blueHold = (blueHold + (((double)ColorSpd/1800.0)*((double)blueOffset/360.0)));
//
//                    if (redHold > 255)
//                        redHold = 0.0;
//                    if (greenHold > 255)
//                        greenHold = 0.0;
//                    if (blueHold > 255)
//                        blueHold = 0.0;
//
//                    ZRed = redHold;
//                    ZGreen = greenHold;
//                    ZBlue = blueHold;
//
//                    ZRed = ZRed + ((Colorizer+((double)redOffset / 10.0))*4.125);
//                    if (ZRed > 255)
//                        ZRed = 0.0;
//                    if (ZGreen > 255)
//                        ZGreen = 0.0;
//                    if (ZBlue > 255)
//                        ZBlue = 0.0;

// logFile << "(" << Colorizer << ", " << ZRed << ")" << endl;
                }
                break;

                case 4:
                {
                    ZRed   = (M_INT16)((::sin(Colorizer+DegreeToRadian(redOffset)+Z+OffsetZ) + 1.0) * 127.5);
                    ZGreen = (M_INT16)((::sin(Colorizer+DegreeToRadian(greenOffset)+Zz+OffsetZz) + 1.0) * 127.5);
                    ZBlue  = (M_INT16)((::sin(Colorizer+DegreeToRadian(blueOffset)+Zzz+OffsetZzz) + 1.0) * 127.5);
                }
                break;

                case 5:
                {
                    double tempRed, tempGreen, tempBlue;

                    tempRed = (::sin(Colorizer+DegreeToRadian(0)) + 1.0) / 2.0;
                    if(tempRed <= ((double)(360-redOffset)/360.0))
                        ZRed = 0;
                    else
                        ZRed = 255;
                    
                    tempGreen = (::sin(Colorizer+DegreeToRadian(90)) + 1.0) / 2.0;
                    if(tempGreen <= ((double)(360-greenOffset)/360.0))
                        ZGreen = 0;
                    else
                        ZGreen = 255;
                    
                    tempBlue = (::sin(Colorizer+DegreeToRadian(180)) + 1.0) / 2.0;
                    if(tempBlue <= ((double)(360-blueOffset)/360.0))
                        ZBlue = 0;
                    else
                        ZBlue = 255;
                    
                }
                break;

                case 6:
                {
                    double tempRed, tempGreen, tempBlue;

                    tempRed = (::sin(Colorizer+DegreeToRadian(0)+Z) + 1.0) / 2.0;
                    if(tempRed <= ((double)(360-redOffset)/360.0))
                        ZRed = 0;
                    else
                        ZRed = 255;
                    
                    tempGreen = (::sin(Colorizer+DegreeToRadian(90)+Zz) + 1.0) / 2.0;
                    if(tempGreen <= ((double)(360-greenOffset)/360.0))
                        ZGreen = 0;
                    else
                        ZGreen = 255;
                    
                    tempBlue = (::sin(Colorizer+DegreeToRadian(180)+Zzz) + 1.0) / 2.0;
                    if(tempBlue <= ((double)(360-blueOffset)/360.0))
                        ZBlue = 0;
                    else
                        ZBlue = 255;
                    
                }
                break;

                default:
                {
                    ZRed   = (M_INT16)((::sin(Colorizer+DegreeToRadian(redOffset)) + 1.0) * 127.5);
                    ZGreen = (M_INT16)((::sin(Colorizer+DegreeToRadian(greenOffset)) + 1.0) * 127.5);
                    ZBlue  = (M_INT16)((::sin(Colorizer+DegreeToRadian(blueOffset)) + 1.0) * 127.5);
                }
                break;
            }
        }

        next.status = 0x0000;
        if (((::sin(Blankerizer) < ((BlankSpot-100.0)/100.0)) && ColorPtn != 0))
        {
            blankThis = M_TRUE;
        }

        if ( blankThis == M_TRUE)
        {
            ZRed   = 0x0000;
            ZGreen = 0x0000;
            ZBlue  = 0x0000;
            next.status = next.status | BLANKING_MASK_WORD;
        }

#ifdef ABSTRACT_LOG_INFO
   logFile << "Abstract::GetNextSegment(): "
           << "          Blankerizer = " << dec << Blankerizer << "\n"
           << "            Colorizer = " << Colorizer << "\n" 
           << "             BlankSpd = " << BlankSpd  << "\n" 
           << "             ColorSpd = " << ColorSpd  << "\n" 
           << "            BlankSpot = " << BlankSpot << "\n" 
           << "             ColorPtn = " << ColorPtn  << endl;
#endif

        Zr    = ResetCycle(Zr   + (DegreeToRadian(ZChngNum)   / 100000000.0), DegreeToRadian(360), blankThis);
        Zzr   = ResetCycle(Zzr  + (DegreeToRadian(ZzChngNum)  / 100000000.0), DegreeToRadian(360), blankThis);
        Zzzr  = ResetCycle(Zzzr + (DegreeToRadian(ZzzChngNum) / 100000000.0), DegreeToRadian(360), blankThis);

        if (GChngNum)
        {
            if (Formula == 7 || Formula == 8)
            {
                GReal = ResetCycle((GReal + (GChngNum/10000.0)), DegreeToRadian(360), blankThis);
            }
            else
            {
                GReal = ResetCycle((GReal + (GChngNum/200000.0)), DegreeToRadian(360), blankThis);
            }
        }
        else
        {
            GReal = 1.0;
        }

#ifdef ABSTRACT_LOG_INFO
       logFile << "Abstract::GetNextSegment(): Zr = " << dec << Zr    << "\n"
               << "                           Zzr = " << Zzr       << "\n" 
               << "                         GReal = " << GReal     << "\n" 
               << "                      ZChngNum = " << ZChngNum  << "\n" 
               << "                     ZzChngNum = " << ZzChngNum << "\n" 
               << "                      GChngNum = " << GChngNum  << "\n" 
               << "                             G = " << G         << endl;
#endif

        const M_INT16 DELTA_THRESHOLD = 2000;
   
//  ****************************************************************
        // Check here to see if we want to add some points in between
        // the current and next points.
//  ****************************************************************
//        int deltaX = abs(next.x - curr.x);
//        int deltaY = abs(next.y - curr.y);
//        int deltaZ = abs(next.z - curr.z);

        if (deltaX >= DELTA_THRESHOLD)
        {
            numFillPoints = (M_UINT16)((deltaX / DELTA_THRESHOLD) - 1);
        }

        if ( (deltaY >= DELTA_THRESHOLD) && (deltaY > deltaX) && (deltaX != 0))
        {
            numFillPoints = (M_UINT16)((deltaY / DELTA_THRESHOLD) - 1);
        }

        if (numFillPoints > 0)
        {
            xDiff = (M_INT16)(deltaX / (numFillPoints + 1));
            zDiff = (M_INT16)(deltaZ / (numFillPoints + 1));
            mX = ((double)(next.y - curr.y) / (double)(next.x - curr.x));
            bX = ( ((double)next.y) - (mX * ((double)next.x)) );
            if (curr.x > next.x)
            {
                xDiff = - xDiff;
            }
            if (curr.z > next.z)
            {
                zDiff = - zDiff;
            }


#ifdef ABSTRACT_LOG_INFO
    logFile << dec << "curr.x = "   << dec << curr.x
            << ", next.x = " << next.x << "\n"
            << "curr.y = "   << curr.y
            << ", next.y = " << next.y << "\n"
            << "deltaX, deltaY: " << deltaX << ", " << deltaY
            << "\n adding " << numFillPoints << " points" << "\n"
            << " with " << xDiff << " distance between X" << "\n"
            << " with " << zDiff << " distance between Z" << "\n"
            << " and mX = " << mX << " and bX = " << bX << "\n" << endl;
#endif

//  ****************************************************************
            // Call ourselves to get the first fill point.  Since
            // we have not yet displayed anything, we don't want
            // the "current" point to change yet, so take care of
            // that here...
//  ****************************************************************
            savedNext  = next;
            savedRed   = redValue;
            savedGreen = greenValue;
            savedBlue  = blueValue;

            next = curr;
            M_UINT16 retVal = GetNextSegment(currPointPtr,
                                           nextPointPtr,
                                           redValue,
                                           greenValue,
                                           blueValue,
                                           useDiscreet);
            return(retVal);
        
        }

    } // end else currently not filling in points

    redValue = (M_UINT8)ZRed;
    greenValue = (M_UINT8)ZGreen;
    blueValue = (M_UINT8)ZBlue;

    nextPointPtr->SetPointInfo(next.x,
                               next.y,
                               next.z,
                               GetMsbFromUshort(next.status),
                               GetLsbFromUshort(next.status),
                               redValue,
                               greenValue,
                               blueValue);

#ifdef ABSTRACT_LOG_INFO
   logFile << "Abstract::GetNextSegment(): curr.x = "   << dec << curr.x
           << ", next.x = " << next.x << "\n"
           << "                            curr.y = "   << curr.y
           << ", next.y = " << next.y << "\n"
           << "                            curr.z = "   << curr.z
           << ", next.z = " << next.z << "\n"
           << "                       next.status = 0x" << hex   << next.status << "\n" 
           << "                         redValue = "   << dec << (M_UINT16) redValue << "\n"
           << "                       greenValue = "   << (M_UINT16)greenValue << "\n"
           << "                        blueValue = "   << (M_UINT16)blueValue  << endl;
#endif

    return(M_SUCCESS);

} // end Abstract::GetNextSegment


