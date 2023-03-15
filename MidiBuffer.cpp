//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: MidiBuffer.cpp                             *
// * Description: Implementation of the MidiBuffer class.  *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#include <iostream>
#include <fstream>

#ifndef MIDIBUFFER_H
    #include "MidiBuffer.h"
#endif

using namespace std; 

extern ofstream logFile;

////////////////////////////////////////////////////////////////////
//
// MidiBuffer::MidiBuffer()
// Description: Constructor
//
////////////////////////////////////////////////////////////////////
MidiBuffer::MidiBuffer():
midiBufferHandle(0)
{
    HANDLE  memoryHandle;
    MidiEventPtr_T memoryPtr;

    memoryHandle = GlobalAlloc(GMEM_SHARE | GMEM_MOVEABLE,
		                       INPUT_BUFFER_SIZE * sizeof(MidiEvent_T));
    if(memoryHandle == NULL)
    {
        return;
    }
    
    memoryPtr = (MidiEventPtr_T)GlobalLock(memoryHandle);
    if(memoryPtr == NULL)
    {
        GlobalFree(memoryHandle);
        return;
    }
    
#ifndef _WIN32
    GlobalSmartPageLock((HGLOBAL)HIWORD(memoryPtr));
#endif
    
    midiBufferHandle = memoryHandle;
    midiBufferError  = 0;
    midiBufferSize   = INPUT_BUFFER_SIZE;
    midiBufferCount  = 0L;
    midiBufferStart  = memoryPtr;
    midiBufferEnd    = memoryPtr + midiBufferSize;
    midiBufferTail   = memoryPtr;
    midiBufferHead   = memoryPtr;

} // end MidiBuffer::MidiBuffer()
 
////////////////////////////////////////////////////////////////////
//
// MidiBuffer::~MidiBuffer()
// Description: Destructor
//
////////////////////////////////////////////////////////////////////
MidiBuffer::~MidiBuffer()
{
    if (midiBufferHandle)
	{
#ifndef _WIN32
        GlobalSmartPageUnlock((HGLOBAL)HIWORD(midiBufferStart));
#endif
        GlobalUnlock(midiBufferHandle);
        GlobalFree(midiBufferHandle);
	}

}  // end MidiBuffer::~MidiBuffer()
 
////////////////////////////////////////////////////////////////////
//
// MidiBuffer::GetEvent()
// Description: Gets a MIDI event from the input buffer.
//
////////////////////////////////////////////////////////////////////
MidiEventPtr_T FAR PASCAL MidiBuffer::GetEvent()
{
    if(midiBufferCount <= 0)
	{
        return 0;
	}
    
	MidiEventPtr_T tempEventPtr = midiBufferTail;
    
    --midiBufferCount;
    ++midiBufferTail;
    
    if(midiBufferTail >= midiBufferEnd)
	{
        midiBufferTail = midiBufferStart;
	}

    return tempEventPtr;
}

////////////////////////////////////////////////////////////////////
//
// Midi::PutEvent()
// Description: Puts an MIDI event in the MIDI buffer.  If the
// buffer is full, the midiBufferError element is set to M_TRUE.
//
////////////////////////////////////////////////////////////////////
void FAR PASCAL MidiBuffer::PutEvent(MidiEventPtr_T lpEvent)
{
    if(midiBufferCount >= INPUT_BUFFER_SIZE)
	{
        midiBufferError = M_TRUE;
        return;
    }
    
    *midiBufferHead = *lpEvent;
    
    ++midiBufferHead;
    ++midiBufferCount;

    if(midiBufferHead >= midiBufferEnd)
	{
        midiBufferHead = midiBufferStart;
	}

}

////////////////////////////////////////////////////////////////////
//
// Midi::EmptyBuffer()
// Description: Effectively empties the MIDI buffr and sets the
// count to zero.
//
////////////////////////////////////////////////////////////////////
void FAR PASCAL MidiBuffer::EmptyBuffer(void)
{
    midiBufferHead = midiBufferStart;
    midiBufferTail = midiBufferStart;
    midiBufferCount = 0L;
}