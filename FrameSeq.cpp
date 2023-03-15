//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: FrameSeq.cpp                               *
// * Description: Implementation of the FrameSequence      *
// * class, representing a sequence (i.e. animation) of    *
// * ILDA laser data frames (i.e. Frame objects).          *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#ifndef FRAMESEQUENCE_H
#include "frameseq.h"
#endif

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

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: FrameSequence::FrameSequence
//
// Input Parameters:
//  None
//
// Returns: N/A
//
// Description:
// This is the constructor for the FrameSequence class.  It will 
// initialize the 'head', 'tail', and 'current' pointers to zero.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

FrameSequence::FrameSequence():
    headPtr(0),
    tailPtr(0),
    currentPtr(0),
    numOfFrames(0),
	firstPass(M_TRUE)
{
    return;
}

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: FrameSequence::~FrameSequence
//
// Input Parameters:
//  None
//
// Returns: N/A
//
// Description:
// This is the destructor for the FrameSequence class.  It will delete
// all the frames in the list, and zero the 'head', 'tail', and 
// 'current' pointers.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

FrameSequence::~FrameSequence()
{
    if (!headPtr) // list is already empty
    {
        if (tailPtr)
        {
#ifdef FRAMESEQUENCE_LOG_ERROR
            logFile << "ERR: List is empty, but tailPtr not zero, "
                    << "aborting..." << endl;
#endif
            okFlag = M_FALSE;
            exit(0);
        }
    }
    else // list is not empty
    {
        Frame* tempPtr = 0;
        currentPtr = headPtr;
        M_UINT16 i = 1;

        while (currentPtr && (i <= numOfFrames))
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

/////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: FrameSequence::AddFrame
//
// Input Parameters:
//  Frame* framePtr  - pointer to frame to be added to the frame list
//
// Returns: M_UINT16
//
// Description:
// This is function will add the frame, passed in as an argument, to
// the frame list.  It will return M_SUCCESS if the the frame was
// successfully added, and M_FAILURE otherwise.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

M_UINT16
FrameSequence::AddFrame(Frame* framePtr)
{
    if (!framePtr)
    {
#ifdef FRAMESEQUENCE_LOG_ERROR
        logFile << "ERR: NULL frame pointer passed to "
		        << "FrameSequence::AddFrame()" << endl;
#endif
        okFlag = M_FALSE;
        return(M_FAILURE);
    }

    if (!headPtr) // first in list
    {
        headPtr = framePtr;
        tailPtr = framePtr;
//        framePtr->previous = headPtr;
        framePtr->previous = 0;
        framePtr->next = 0;
    }
    else
    {
        tailPtr->next = framePtr;
        framePtr->previous = tailPtr;
        framePtr->next = 0;
        tailPtr = framePtr;
    }

    currentPtr = framePtr;

    return(M_SUCCESS);

} // end FrameSequence::AddFrame

/////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: FrameSequence::DeleteFrame
//
// Input Parameters:
//  Frame* theFrame   - pointer to the frame in the list to be
//                      deleted
//
// Returns: M_UINT16
//
// Description:
// This is function will delete the frame, passed in as a pointer,
// from the frame list.  It will return M_SUCCESS if the the frame was
// successfully deleted, and M_FAILURE otherwise.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

M_UINT16
FrameSequence::DeleteFrame(Frame* theFrame)
{
    if(headPtr == 0 && tailPtr == 0) // list is already empty
    {
#ifdef FRAMESEQUENCE_LOG_ERROR
        logFile << "ERR: List is empty, cannot delete frame." << endl;
#endif
        return(M_FAILURE);
    }

    if (theFrame == tailPtr) // if last in list
    {
        if (theFrame == headPtr) // is only frame in list
        {
            headPtr = 0;
            tailPtr = 0;
        }
        else // is last but not only frame in list
        {
            (theFrame->previous)->next = 0;
            tailPtr = theFrame->previous;
        }
    }
    else // else not last in list
    {
        if (theFrame == headPtr) // first but not only frame
        {
            headPtr = theFrame->next;
//            (theFrame->next)->previous = headPtr;
            (theFrame->next)->previous = 0;
        }
        else // somewhere in the middle
        {
            (theFrame->previous)->next = theFrame->next;
            (theFrame->next)->previous = theFrame->previous;
        }
    }

    delete(theFrame);
    theFrame = 0;
    return(M_SUCCESS);

} // end FrameSequence::DeleteFrame

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: FrameSequence::GetNextFrame
//
// Input Parameters:
//  N/A
//
// Returns: Frame*
//
// Description:
//  This function returns the next frame (pointer) in the list and
//  increments the currentPtr accordingly.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

Frame* 
FrameSequence::GetNextFrame() 
{
    currentPtr = currentPtr->next;
    return(currentPtr);
}

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2001
//
// Function: FrameSequence::SetCurrentFrame
//
// Input Parameters:
//  const M_UINT16 frameNumber  - The frame number to which you wish to
//                             set the currentPtr.
//
// Returns: M_UINT16
//
// Description:
//  This function sets the currentPtr to point to the frame number
//  specified in frameNumber.  The value frameNumber should be a
//  value between 0 and the actual total number of frames. 
//
///////////////////////////PROLOGUE_END/////////////////////////////////

M_UINT16 
FrameSequence::SetCurrentFrame(const M_UINT16 frameNumber) 
{
	if (frameNumber > numOfFrames ||
		frameNumber < 0)
	{
        return M_FAILURE;
	}

	currentPtr = headPtr;

	for (M_UINT16 i=0 ; i < frameNumber ; ++i)
	{
        currentPtr = currentPtr->next;
	}

    return M_SUCCESS;
}

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: FrameSequence::GetPrevFrame
//
// Input Parameters:
//  N/A
//
// Returns: Frame*
//
// Description:
//  This function returns the previous frame (pointer) in the list and
//  decrements the currentPtr accordingly.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

Frame* 
FrameSequence::GetPrevFrame() 
{
    currentPtr = currentPtr->previous;
    return(currentPtr);
}

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: FrameSequence::PrintFrameSequence
//
// Input Parameters:
//  ostream* stream         - The stream to which output is directed.
//  const M_UINT16 showPoints - Display points in each frame if set 
//                            to M_TRUE.
//
// Returns: void
//
// Description:
//  This function outputs the contents of the track to the stream
//  passed in.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

void
FrameSequence::PrintFrameSequence(ostream& stream, 
                                  const M_UINT16 showPoints)
{
    stream << "********** FrameSequence ********** \n\n";
    stream << "    headPtr = " << hex << headPtr << "\n"
           << "    tailPtr = " << tailPtr << "\n"
           << " currentPtr = " << currentPtr << "\n"
           << "numOfFrames = " << dec << numOfFrames << "\n"
           << "     okFlag = " << okFlag << endl;

   Frame* saveCurrent = currentPtr;
   currentPtr = headPtr;

   for (short j=1 ; j<=numOfFrames ; ++j)
   {
       if (!currentPtr)
       {
           stream << "PrintFrameSequence(): Unexpected NULL pointer "
                  << "while printing frame " << dec << j << endl;
           return;
       }

       currentPtr->PrintFrame(stream, showPoints);
       currentPtr = currentPtr->next;

   } // end loop through frames in track
   
   currentPtr = saveCurrent;

   return;
}
