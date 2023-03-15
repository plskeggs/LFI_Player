//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Frame.cpp                                  *
// * Description: Implementation of the Frame class,       *
// * representing a single frame of ILDA laser data.       *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#ifndef FRAME_H
#include "frame.h"
#endif

#ifndef POINT_H
#include "point.h"
#endif

#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std; 

extern ofstream logFile;
#define FRAME_LOG_ERROR

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Frame::Frame
//
// Input Parameters:
//  None
//
// Returns: N/A
//
// Description:
// This is the constructor for the Frame class.  It will initialize
// the 'head', 'tail', and 'current' pointers to zero.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

Frame::Frame():
    headPtr(0),
    tailPtr(0),
    currentPtr(0),
    scannerHead(0),
    formatCode(ILDAFormatCode::INDEXED_COLOR_3D),
    frameNumber(0),
    numOfPoints(0),
	next(0),
	previous(0),
    okFlag(M_TRUE)
{
    return;
}

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Frame::~Frame
//
// Input Parameters:
//  None
//
// Returns: N/A
//
// Description:
// This is the destructor for the Frame class.  It will delete all
// the points in the list, and zero the 'head', 'tail', and 'current'
// pointers.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

Frame::~Frame()
{
    if (!headPtr) // list is already empty
    {
        if (tailPtr)
        {
#ifdef FRAME_LOG_ERROR
            logFile << "ERR: List is empty, but tailPtr not zero, "
                    << "aborting..." << endl;
#endif
            okFlag = M_FALSE;
            exit(0);
        }
    }
    else // list is not empty
    {
        Point* tempPtr = 0;
        currentPtr = headPtr;
        M_UINT16 i = 1;

        while (currentPtr && (i <= numOfPoints))
        {
            tempPtr = currentPtr->next;
            delete(currentPtr);
            currentPtr = tempPtr;
            ++i;
        }

        currentPtr = 0;
        tailPtr = 0;
    }
}

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Frame::GetNextPoint
//
// Input Parameters:
//  N/A
//
// Returns: Point*
//
// Description:
//  This function returns the next point (pointer) in the list and
//  increments the currentPtr accordingly.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

Point* 
Frame::GetNextPoint() 
{
    currentPtr = currentPtr->next;
    return(currentPtr);
}

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Frame::GetPrevPoint
//
// Input Parameters:
//  N/A
//
// Returns: Point*
//
// Description:
//  This function returns the previous point (pointer) in the list and
//  decrements the currentPtr accordingly.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

Point* 
Frame::GetPrevPoint() 
{
    currentPtr = currentPtr->previous;
    return(currentPtr);
}

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Frame::PrintFrame
//
// Input Parameters:
//  ostream* stream         - The stream to which output is directed.
//  const M_UINT16 showPoints - Display points in frame if set to M_TRUE.
//
// Returns: void
//
// Description:
//  This function outputs the contents of the frame to the stream
//  passed in.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

void
Frame::PrintFrame(ostream& stream, const M_UINT16 showPoints)
{
    stream << "********** Frame Number " << dec << frameNumber
           << " ********** \n\n";
    stream << "       next = " << hex << next << "\n"
           << "   previous = " << previous << "\n"
           << "    headPtr = " << headPtr << "\n"
           << "    tailPtr = " << tailPtr << "\n"
       << " currentPtr = " << currentPtr << "\n\n";
    stream << "scannerHead = " << dec << (M_UINT16)scannerHead << "\n"
           << " formatCode = " << (M_UINT16)formatCode << "\n"
           << "numOfPoints = " << numOfPoints << "\n"
           << "     okFlag = " << okFlag << endl;
                
    if (showPoints == M_TRUE)
    {
        Point* saveCurrent = currentPtr;
        currentPtr = headPtr;

       for (short i=1 ; i<=numOfPoints ; ++i)
       {
           if (!currentPtr)
           {
               stream << "PrintFrame(): Unexpected NULL pointer while "
                      << "printing point number " << dec << i << endl;
               currentPtr = saveCurrent;
               return;
           }

           stream << "Point number " << dec << i << " = "
                  << *currentPtr << endl;

           currentPtr = currentPtr->next;

       } // end loop through points in frame

       currentPtr = saveCurrent;

    } // end if showPoints

    return;
}

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Frame::operator=
//
// Input Parameters:
//  const Frame&      - the Frame to be assigned
//
// Returns: N/A
//
// Description:
//  This is the definition for the assignment operator for the Frame
//  class.
//
///////////////////////////PROLOGUE_END/////////////////////////////////
//
//Frame&
//operator=(const Frame& theFrame)
//{
//    next        = theFrame.next;
//  previous    = theFrame.previous;
//  headPtr     = theFrame.headPtr;
//  tailPtr     = theFrame.tailPtr;
//  currentPtr  = theFrame.currentPtr;
//  scannerHead = theFrame.scannerHead;
//  formatCode  = theFrame.formatCode;
//  frameNumber = theFrame.frameNumber;
//  numOfPoints = theFrame.numOfPoints;
//  okFlag      = theFrame.okFlag;
//
//    return *this;
//
//} // end Point assignment operator

/////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Frame::AddPoint
//
// Input Parameters:
//  const Point& newPoint   - the point to be added to the point list
//
// Returns: M_UINT16
//
// Description:
// This is function will add the point, passed in as an argument, to
// the point list.  It will return M_SUCCESS if the the point was
// successfully added, and M_FAILURE otherwise.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

M_UINT16
Frame::AddPoint(Point* pointPtr)
{
    if (!pointPtr)
    {
#ifdef FRAME_LOG_ERROR
        logFile << "ERR: NULL point pointer passed in to Frame::AddPoint()" << endl;
#endif
        okFlag = M_FALSE;
        return(M_FAILURE);
    }

    if (!headPtr) // first in list
    {
        headPtr = pointPtr;
        tailPtr = pointPtr;
        pointPtr->previous = headPtr;
        pointPtr->next = 0;
    }
    else
    {
        tailPtr->next = pointPtr;
        pointPtr->previous = tailPtr;
        pointPtr->next = 0;
        tailPtr = pointPtr;
    }

    currentPtr = pointPtr;

    return(M_SUCCESS);

} // end Frame::AddPoint

/////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Frame::DeletePoint
//
// Input Parameters:
//  Point* thePoint   - pointer to the point in the list to be
//                      deleted
//
// Returns: M_UINT16
//
// Description:
// This is function will delete the point, passed in as a pointer,
// from the point list.  It will return M_SUCCESS if the the point was
// successfully deleted, and M_FAILURE otherwise.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

M_UINT16
Frame::DeletePoint(Point* thePoint)
{
    if(headPtr == 0 && tailPtr == 0) // list is already empty
    {
#ifdef FRAME_LOG_ERROR
        logFile << "ERR: List is empty, cannot delete point." << endl;
#endif
        return(M_FAILURE);
    }

    if (thePoint == tailPtr) // if last in list
    {
        if (thePoint == headPtr) // is only point in list
        {
            headPtr = 0;
            tailPtr = 0;
        }
        else // is last but not only point in list
        {
            (thePoint->previous)->next = 0;
            tailPtr = thePoint->previous;
        }
    }
    else // else not last in list
    {
        if (thePoint == headPtr) // first but not only point
        {
            headPtr = thePoint->next;
            (thePoint->next)->previous = headPtr;
        }
        else // somewhere in the middle
        {
            (thePoint->previous)->next = thePoint->next;
            (thePoint->next)->previous = thePoint->previous;
        }
    }

    delete(thePoint);
    thePoint = 0;
    return(M_SUCCESS);

} // end Frame::DeletePoint