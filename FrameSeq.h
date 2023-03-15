//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: FrameSeq.h                                 *
// * Description: This file contains the interface for the *
// *              FrameSequence class.                     *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#ifndef FRAMESEQUENCE_H
#define FRAMESEQUENCE_H

#ifndef TRACK_H
#include "track.h"
#endif

#ifndef FRAME_H
#include "frame.h"
#endif

#ifndef GLOBALS_H
#include "globals.h"
#endif

#ifndef POINT_H
#include "point.h"
#endif

class FrameSequence	: public Track
{
public:
    M_UINT16 Ok(void) const {return(Track::okFlag);}

    FrameSequence();
    ~FrameSequence();

    // accessor methods
    void   ResetIterator() {currentPtr = headPtr;}
	void   SetIteratorToLast() {currentPtr = tailPtr;}
    Frame* GetNextFrame(); 
    Frame* GetPrevFrame();

    Frame* GetCurrentFrame() const {return(currentPtr);}
	M_UINT16 SetCurrentFrame(const M_UINT16 frameNumber);

    void   SetNumberOfFrames(const M_UINT16 numFrames) {numOfFrames = numFrames;}
    M_UINT16 GetNumberOfFrames() const {return(numOfFrames);}

    M_UINT16 AddFrame(Frame*);
    M_UINT16 DeleteFrame(Frame*);

    void   PrintFrameSequence(ostream&, const M_UINT16);

    M_UINT16 IsFirst(void) {return firstPass;}
	void   SetFirstFalse(void) {firstPass = M_FALSE;}

private:
    Frame*  headPtr;
    Frame*  tailPtr;
    Frame*  currentPtr;
    M_INT16   numOfFrames;

    M_UINT16   firstPass;
};

#endif
