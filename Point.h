#ifndef POINT_H
#define POINT_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Point.h                                    *
// * Description: This file contains the interface for the *
// *              Point class.                             *
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

#include <iostream>
using namespace std;
using std::cout;


// class ostream;
class Frame;
class Abstract;
class FontList;

class Point
{
public:
    // enum PointMaskEnums { BLANKING_MASK };

    friend Frame;
    friend Abstract;
	friend FontList;

    friend ostream&      operator<<(ostream&, const Point&);
//    M_INT16      Ok(void) const {return(okFlag);}

    Point(M_INT16  xCoord,
          M_INT16  yCoord,
          M_INT16  zCoord,
          M_UINT8  statusCode,
          M_UINT8  colorIndex,
          M_UINT8  redValue,
          M_UINT8  greenValue,
          M_UINT8  blueValue);
    Point();
    ~Point();

//  Point& operator=(const Point&);

    // accessor methods
    void  SetPointInfo(M_INT16  xCoord,
                       M_INT16  yCoord,
                       M_INT16  zCoord,
                       M_UINT8   statusCode,
                       M_UINT8   colorIndex,
                       M_UINT8   redValue,
                       M_UINT8   greenValue,
                       M_UINT8   blueValue);

    M_UINT16 GetPointInfo(M_INT16&  xCoord,
                        M_INT16&  yCoord,
                        M_INT16&  zCoord,
                        M_UINT8&  statusCode,
                        M_UINT8&  colorIndex,
                        M_UINT8&  redValue,
                        M_UINT8&  greenValue,
                        M_UINT8&  blueValue);

    M_UINT8 GetStatusCode()
    { 
        return statusCode; 
    }

	void SetStatusCode(const M_UINT8 newStatus)
	{ 
        statusCode = newStatus; 
    }

    M_UINT8 GetColorIndex()
    { 
        return colorIndex; 
    }

    void SetColorIndex(const M_UINT8 newColorIndex)
    {
        colorIndex = newColorIndex; 
    }

    void SetColorValue(const LaserColor color,
                       const M_UINT8 value)
    {
        switch (color)
        {
        case LaserColor::RED:
            redValue = value;
            break;

        case LaserColor::GREEN:
            greenValue = value;
            break;

        case LaserColor::BLUE:
            blueValue = value;
            break;

        default:
            break;
        }
    }

    M_UINT8 GetColorValue(const LaserColor color)
    {
        switch (color)
        {
            case LaserColor::RED:
                return redValue;
                break;

            case LaserColor::GREEN:
                return greenValue;
                break;

            case LaserColor::BLUE:
                return blueValue;
                break;

            default:
                return 0x00;
                break;
        }

    }

    M_UINT16 BlankingOn(void);

protected:
    Point*  next;
    Point*  previous;
private:
    M_INT16    xCoordinate;
    M_INT16    yCoordinate;
    M_INT16    zCoordinate;
    M_UINT8    statusCode;
    M_UINT8    colorIndex;
    M_UINT8    redValue;
    M_UINT8    greenValue;
    M_UINT8    blueValue;
    M_INT16    okFlag;
};
#endif
