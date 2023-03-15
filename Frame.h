#ifndef FRAME_H
#define FRAME_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Frame.h                                    *
// * Description: This file contains the interface for the *
// *              Frame class.                             *
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

#ifndef POINT_H
#include "point.h"
#endif

#include <iostream>

using namespace std;
using std::cout;

class FrameSequence;

class Frame
{
public:
    friend FrameSequence;
    short Ok(void) const {return(okFlag);}

    Frame(void);
    ~Frame(void);

//  Frame& operator=(const Frame&);

    // accessor methods
    void   ResetIterator()         {currentPtr = headPtr;}
    Point* GetNextPoint(); 
    Point* GetPrevPoint();

    Point* GetCurrentPoint() const {return(currentPtr);}

    void   SetScannerHead(const M_UINT8 headNum) {scannerHead = headNum;}
    M_UINT8  GetScannerHead()  const {return(scannerHead);}

    void            SetFormatCode(const ILDAFormatCode formCode) {formatCode = formCode;}
    ILDAFormatCode  GetFormatCode()   const {return(formatCode);}

    void   SetFrameNumber(const short frameNum) {frameNumber = frameNum;}
    short  GetFrameNumber()  const {return(frameNumber);}

    void   SetNumberOfPoints(const short numPoints) {numOfPoints = numPoints;}
    M_UINT16 GetNumberOfPoints()  const {return(numOfPoints);}

    M_UINT16 AddPoint(Point*);
    M_UINT16 DeletePoint(Point*);

    void   PrintFrame(ostream&, const M_UINT16);

protected:
    Frame*  next;
    Frame*  previous;
private:
    Point*           headPtr;
    Point*           tailPtr;
    Point*           currentPtr;
    M_UINT8            scannerHead;
    ILDAFormatCode   formatCode;
    short            frameNumber;
    M_UINT16           numOfPoints;

    short   okFlag;
};
#endif
