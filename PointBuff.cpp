//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: PointBuff.cpp                              *
// * Description: Implementation of the PointBuff class.   *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#ifndef POINTBUFF_H
#include "PointBuff.h"
#endif

#include <string.h>

PointBuff::PointBuff()
{
    memset (&pointBuffer[0],
            0,
            sizeof(OutputPoint_T)*MAX_BUFFER_POINTS);

    // since we always increment the currentIndex,
    // start at the end...
    currentIndex = LAST_POINT_BUFFER_INDEX;

    return;
}

PointBuff::~PointBuff()
{
    return;
}

void
PointBuff::AddPoint(OutputPoint_T newBuffPoint)
{

    if (currentIndex >= LAST_POINT_BUFFER_INDEX)
    {
        currentIndex = 0;
    }
    else
    {
        ++currentIndex;
    }

    pointBuffer[currentIndex] = newBuffPoint;

    return;
}

OutputPoint_T
PointBuff::GetPoint(M_UINT8 offset)
{
    M_INT16  tempIndex = currentIndex - offset;

    if (tempIndex < 0)
    {
        tempIndex += MAX_BUFFER_POINTS;
    }

    return pointBuffer[(M_UINT8)tempIndex];
}