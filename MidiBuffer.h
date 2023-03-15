#ifndef MIDIBUFFER_H
#define MIDIBUFFER_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: MidiBuffer.h                               *
// * Description: This file contains the interface for the *
// *              MidiBuffer class.                        *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#include <windows.h>
#include <mmsystem.h>
#include "Globals.h"

#define INPUT_BUFFER_SIZE       200     // size of input buffer in events

typedef struct event_tag
{
    M_UINT32 flags;
    M_UINT32 device;
    M_UINT32 timeStamp;
    M_UINT8 status;
	M_UINT8 statusRaw;
	M_UINT8 channel;
	M_UINT8 data1;
	M_UINT8 data2;
} MidiEvent_T;

typedef MidiEvent_T FAR *MidiEventPtr_T;

class MidiBuffer
{
public:
	MidiBuffer(void);
	~MidiBuffer(void);

    MidiEventPtr_T FAR PASCAL GetEvent();
    void FAR PASCAL PutEvent(MidiEventPtr_T lpEvent);
	M_UINT32 FAR PASCAL EventsInBuffer() {return midiBufferCount;}
    void FAR PASCAL MidiBuffer::EmptyBuffer(void);

private:
     HANDLE    midiBufferHandle;     // buffer handle
     M_UINT16   midiBufferError;      // error flags
     M_UINT32   midiBufferSize;       // buffer size (in MIDI events)
     M_UINT32   midiBufferCount;      // byte count (in MIDI events)
     MidiEventPtr_T midiBufferStart; // ptr to start of buffer
     MidiEventPtr_T midiBufferEnd;   // ptr to end of buffer (last byte + 1)
     MidiEventPtr_T midiBufferHead;  // ptr to head (next location to fill)
     MidiEventPtr_T midiBufferTail;  // ptr to tail (next location to empty)

    MidiEventPtr_T memoryPtr;        // pointer to the buffer memory
 
}; // end class MidiBuffer

typedef MidiBuffer FAR *MidiBufferPtr_T;

#endif