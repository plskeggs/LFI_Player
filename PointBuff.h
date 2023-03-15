#ifndef POINTBUFF_H
#define POINTBUFF_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: PointBuff.h                                *
// * Description: This file contains the interface for the *
// *              point buffer (PointBuff) class.          *
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

#define MAX_BUFFER_POINTS         50
#define LAST_POINT_BUFFER_INDEX   (MAX_BUFFER_POINTS-1)

class PointBuff
{
public:
    PointBuff();
    ~PointBuff();

void AddPoint(OutputPoint_T newBuffPoint);

OutputPoint_T GetPoint(M_UINT8 offset);


private:
    OutputPoint_T    pointBuffer[MAX_BUFFER_POINTS];
    M_UINT8          currentIndex;
};

#endif
