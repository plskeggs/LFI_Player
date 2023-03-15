#ifndef ABSTRACT_H
#define ABSTRACT_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Abstract.h                                 *
// * Description: This file contains the interface for the *
// *              Abstract class.                          *
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

#ifndef GLOBALS_H
#include "globals.h"
#endif

#ifndef TRACK_H
#include "track.h"
#endif

#ifndef POINT_H
#include "point.h"
#endif

class CAbstractControl;
class CCheap3DView;

// Macro to get a random integer with a specified range 
#define getrandom(min, max) \
    ((rand()%(int)(((max) + 1)-(min)))+ (min))

// const M_UINT16 MAX_Z_FORMULAS     = 8;
// const M_UINT16 INTS_PER_PATTERN   = 13;
// const M_UINT16 INT2S_PER_PATTERN  = 13;
// const M_UINT16 REALS_PER_PATTERN  = 5;
// const M_UINT16 MAX_PATTERNS       = 24;
// const M_UINT16 MAX_PATTERNS       = 1;
// const M_UINT16 MAX_INT_ARRAY      = INTS_PER_PATTERN * MAX_PATTERNS;
// const M_UINT16 MAX_INT2_ARRAY     = INT2S_PER_PATTERN * MAX_PATTERNS;
// const M_UINT16 MAX_REAL_ARRAY     = REALS_PER_PATTERN * MAX_PATTERNS;
const int MAX_INT_ARRAY      = 13;
const int MAX_INT2_ARRAY     = 13;
const int MAX_REAL_ARRAY     = 12;

const enum EnvelopeShape{SINE   = 0,
                         SQUARE = 1};

// const M_UINT16 NUM_POINTS_TO_SAVE  = 3;

class Abstract : public Track
{
friend CAbstractControl;
friend CCheap3DView;
public:
    M_UINT16 Ok(void) const {return(Track::okFlag);}

    Abstract(const char* absFileName);
    ~Abstract(void);

    M_UINT16  SetPattern(const M_UINT16 newPattern);
    M_UINT16  GetNextSegment(Point*  const currPointPtr,
                           Point*  const nextPointPtr,
                           M_UINT8&  redValue,
                           M_UINT8&  greenValue,
                           M_UINT8&  blueValue,
                           M_UINT16& useDiscreet);
    M_UINT16  GetAbsPointsPerFrame() {return AbsPtsPerFrame;}

private:
    double mX, bX;
    double XReal, YReal;

    typedef struct
    {
        M_INT16  x;
        M_INT16  y;
        M_INT16  z;
        M_UINT16  status;
    }AbsPoint;

    AbsPoint curr;
    AbsPoint next;
    AbsPoint savedNext;
    M_UINT8 savedRed;
    M_UINT8 savedGreen;
    M_UINT8 savedBlue;

    M_UINT16 numIntVals;
    M_UINT16 numRealVals;
    M_UINT16 numInt2Vals;
    
    M_UINT16 numFillPoints;
    M_INT16 xDiff;
    M_INT16 zDiff;
 
    M_UINT16 currentPattern;
    M_UINT16 usePalette;

    M_UINT16 Formula;
//    M_UINT16 DrawSpeed;
    M_UINT16 ColorPtn;
    M_INT16  ZRed;
    M_INT16  ZBlue;
    M_INT16  ZGreen;

    M_UINT16  GainZ;
    M_UINT16  GainZz;
    M_UINT16  GainZzz;

    M_UINT16  OffsetZ;
    M_UINT16  OffsetZz;
    M_UINT16  OffsetZzz;

//	M_UINT16  ZAxisGain;
//	M_UINT16  ZAxisOffset;
    M_UINT16  AbsPtsPerFrame;

    M_UINT16 redOffset;
    M_UINT16 greenOffset;
    M_UINT16 blueOffset;

    M_UINT16 ColorSpd;
    M_UINT16 BlankSpd;
    M_UINT16 BlankSpot;

    enum EnvelopeShape envelopeShape;
    M_UINT16 envelopeDepth;

    M_UINT16 firstSegment;

    double  Z;
    double  Zz;
    double  Zzz;
    double  G;
    double  Zr;
    double  Zzr;
    double  Zzzr;

    double  ExtraReal1;
    double  GReal;

    double  Blankerizer;
    double  Colorizer;

    double X_Scan;
    double lastZSlope;

// following moved to members
// from static local in GetNextSegment()
    double incAmount;
    M_UINT32 pointCount;



    int   intArray[MAX_INT_ARRAY];
    int   int2Array[MAX_INT2_ARRAY];
    double realArray[MAX_REAL_ARRAY];

// protected:
    M_UINT16 ZChngNum;
    M_UINT16 ZzChngNum;
    M_UINT16 ZzzChngNum;
    M_UINT16 GChngNum;

    M_UINT16 PatternNumber;

};

#endif