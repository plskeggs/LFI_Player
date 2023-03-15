// **************************************************************************/
// *
// *  Adapted from code...
// *  Copyright (C) 1993 - 1997  Microsoft Corporation.  All Rights Reserved.
// * 
// **************************************************************************/

////////////////////////////////////////////////////////////////////
//
// circbuf.c - Routines to manage the circular MIDI input buffer.
//      This buffer is filled by the low-level callback function and
//      emptied by the application.  Since this buffer is accessed
//      by a low-level callback, memory for it must be allocated
//      exactly as shown in the constructor.
//
////////////////////////////////////////////////////////////////////

// #include <windows.h>
#include <iostream.h>
#include <fstream.h>

#ifndef MIDIBUF_H
    #include "MidiBuf.h"
#endif

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

    // Allocate and lock memory for the actual buffer.
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
    
    // Page lock the memory.  Global memory blocks accessed by
    // low-level callback functions must be page locked.
#ifndef _WIN32
    GlobalSmartPageLock((HGLOBAL)HIWORD(memoryPtr));
#endif
    
    midiBufferHandle = memoryHandle;
    midiBufferError  = 0;
    midiBufferSize  = INPUT_BUFFER_SIZE;
    midiBufferCount = 0L;
    midiBufferStart = memoryPtr;
    midiBufferEnd   = memoryPtr + midiBufferSize;
    midiBufferTail  = memoryPtr;
    midiBufferHead  = memoryPtr;

} // end MidiBuffer::MidiBuffer()
 
////////////////////////////////////////////////////////////////////
//
// MidiBuffer::~MidiBuffer()
// Description: Destructor
//
////////////////////////////////////////////////////////////////////
MidiBuffer::~MidiBuffer()
{
    // Free the buffer itself.
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
// Description: Gets a MIDI event from the circular
// input buffer.  Events are removed from the buffer.  The
// corresponding PutEvent() function is called by the low-level
// callback function.
//
////////////////////////////////////////////////////////////////////
MidiEventPtr_T FAR PASCAL MidiBuffer::GetEvent()
{
    // If no event available, return.
    if(midiBufferCount <= 0)
	{
        return 0;
	}
    
    // Get the event.
	MidiEventPtr_T tempEventPtr = midiBufferTail;
    
    // Decrement the byte count, bump the tail pointer.
    --midiBufferCount;
    ++midiBufferTail;
    
    // Wrap the tail pointer, if necessary.
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
// buffer is full, it sets the midiBufferError element to be non-zero.
//
////////////////////////////////////////////////////////////////////
void FAR PASCAL MidiBuffer::PutEvent(MidiEventPtr_T lpEvent)
{
    // If the buffer is full, set an error and return. 
    if(midiBufferCount >= INPUT_BUFFER_SIZE)
	{
        midiBufferError = 1;
        return;
    }
    
    // Put the event in the buffer, bump the head pointer and the byte count.
    *midiBufferHead = *lpEvent;
    
    ++midiBufferHead;
    ++midiBufferCount;

    // Wrap the head pointer, if necessary.
    if(midiBufferHead >= midiBufferEnd)
	{
        midiBufferHead = midiBufferStart;
	}

}
