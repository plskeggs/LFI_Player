//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Point.cpp                                  *
// * Description: Implementation of the Point class.       *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#include <iostream>

#ifndef POINT_H
#include "point.h"
#endif

using namespace std; 

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Point::Point
//
// Input Parameters:
//  M_INT16 xCoord     - the new X point coordinate
//  M_INT16 yCoord     - the new Y point coordinate
//  M_INT16 zCoord     - the new Z point coordinate
//  M_UINT8  statusCode - the new status code
//  M_UINT8  colorIndex - the new color index 
//  M_UINT8  redValue   - the new red color value
//  M_UINT8  greenValue - the new green color value
//  M_UINT8  blueValue  - the new blue color value
//
// Returns: N/A
//
// Description:
// This is the constructor for the Point class.  It will populate
// the class data members with those arguments passed in.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

Point::Point(M_INT16   xCoord,
             M_INT16   yCoord,
             M_INT16   zCoord,
             M_UINT8   status,
             M_UINT8   index,
             M_UINT8   redVal,
             M_UINT8   greenVal,
             M_UINT8   blueVal):
    xCoordinate(xCoord),
    yCoordinate(yCoord),
    zCoordinate(zCoord),
    statusCode(status),
    colorIndex(index),
    redValue(redVal),
    greenValue(greenVal),
    blueValue(blueVal),
    next(0),
    previous(0),
    okFlag(M_TRUE)
{
    return;

} // end Point::Point()

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Point::Point
//
// Input Parameters:
//  void
//
// Returns: N/A
//
// Description:
//  Initializes the whole tamale to zero values.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

Point::Point() :
    xCoordinate(0),
    yCoordinate(0),
    zCoordinate(0),
    statusCode(0),
    colorIndex(0),
    redValue(0),
    greenValue(0),
    blueValue(0),
    next(0),
    previous(0),
    okFlag(M_TRUE)
{
    return;

} // end Point::Point()

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Point::~Point
//
// Input Parameters:
//  N/A
//
// Returns: N/A
//
// Description:
//  Does nothin but keep the linker from bitching.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

Point::~Point()

{
    return;

} // end Point::~Point()

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Point::operator<<
//
// Input Parameters:
//  ostream& stream   - the output stream
//  const point&      - the point to be output
//
// Returns: N/A
//
// Description:
//  This is the definition for the output operator for the point
//  class.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

ostream&
operator<<(ostream& stream, const Point& thePoint)
{
    stream << "(" << dec << thePoint.xCoordinate << ", "
                         << thePoint.yCoordinate << ", "
                         << thePoint.zCoordinate << ", 0x"
                  << hex << thePoint.statusCode  << ", "
                         << thePoint.colorIndex << ", "
                         << thePoint.redValue << ", "
                         << thePoint.greenValue << ", "
                         << thePoint.blueValue << ", "
                         << thePoint.previous    << ", "
                         << thePoint.next << ")" << endl;
    return stream;

} // end Point output operator

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Point::operator=
//
// Input Parameters:
//  const Point&      - the point to be assigned
//
// Returns: N/A
//
// Description:
//  This is the definition for the assignment operator for the Point
//  class.
//
///////////////////////////PROLOGUE_END/////////////////////////////////
//
//Point&
//operator=(const Point& thePoint)
//{
//  next        = thePoint.next;
//  previous    = thePoint.previous;
//  xCoordinate = thePoint.xCoordinate;
//  yCoordinate = thePoint.yCoordinate;
//  zCoordinate = thePoint.zCoordinate;
//  statusCode  = thePoint.statusCode;
//  colorIndex  = thePoint.colorIndex;
//  redValue    = thePoint.redValue;
//  greenValue  = thePoint.greenValue;
//  blueValue   = thePoint.blueValue;
//  okFlag      = thePoint.okFlag;
//
//    return *this;
//
//} // end Point assignment operator

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Point::BlankingOn
//
// Input Parameters:
//  None
//
// Returns: M_UINT16
//
// Description:
//  This function returns M_TRUE if the blanking bit in the status code
//  is set, and returns M_FALSE otherwise.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

M_UINT16
Point::BlankingOn()
{
    if (statusCode & BLANKING_MASK)
    {
        return(M_TRUE);
    }
    else
    {
        return(M_FALSE);
    }
} // end Point::BlankingOn

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Point::SetPointInfo
//
// Input Parameters:
//  M_INT16   xCoord   - The new X coordinate.
//  M_INT16   yCoord   - The new Y coordinate.
//  M_INT16   zCoord   - The new Z coordinate.
//  M_UINT8   status   - The new status code.
//  M_UINT8   index    - The new color index.
//  M_UINT8   redVal   - The new red value.
//  M_UINT8   greenVal - The new red value.
//  M_UINT8   blueVal  - The new red value.
//
// Returns: void
//
// Description:
//  This function sets the point's attribute values equal to those
//  passed in as arguments.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

void
Point::SetPointInfo(M_INT16  xCoord,
                    M_INT16  yCoord,
                    M_INT16  zCoord,
                    M_UINT8   status,
                    M_UINT8   index,
                    M_UINT8   redVal,
                    M_UINT8   greenVal,
                    M_UINT8   blueVal)
{
    xCoordinate = xCoord;
    yCoordinate = yCoord;
    zCoordinate = zCoord;
    statusCode  = status;
    colorIndex  = index;
    redValue    = redVal;
    greenValue  = greenVal;
    blueValue   = blueVal;

} // end Point::SetPointInfo()

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Point::GetPointInfo
//
// Input Parameters:
//  M_INT16&   xCoord   - The X coordinate.
//  M_INT16&   yCoord   - The Y coordinate.
//  M_INT16&   zCoord   - The Z coordinate.
//  M_UINT8&   status   - The status code.
//  M_UINT8&   index    - The color index.
//  M_UINT8&   redVal   - The red value.
//  M_UINT8&   greenVal - The green value.
//  M_UINT8&   blueVal  - The blue value.
//
// Returns: M_UINT16
//
// Description:
//  This function retrieves the point values.  Right now, this is a
//  simple assignment, so it will always return success.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

M_UINT16
Point::GetPointInfo(M_INT16&  xCoord,
                    M_INT16&  yCoord,
                    M_INT16&  zCoord,
                    M_UINT8&  status,
                    M_UINT8&  index,
                    M_UINT8&  redVal,
                    M_UINT8&  greenVal,
                    M_UINT8&  blueVal)
{
    xCoord   = xCoordinate;
    yCoord   = yCoordinate;
    zCoord   = zCoordinate;
    status   = statusCode;
    index    = colorIndex;
    redVal   = redValue;
    greenVal = greenValue;
    blueVal  = blueValue;


    return(M_SUCCESS);

} // end Point::GetPointInfo()
