#ifndef MIDIBUF_H
#define MIDIBUF_H

#include <windows.h>
#include <mmsystem.h>
#include "Globals.h"

#define INPUT_BUFFER_SIZE       200     // size of input buffer in events

typedef struct event_tag
{
    UINT_32 fEvent;
    UINT_32 device;
    UINT_32 timeStamp;
//    UINT_32 data;
    UINT_8 status;
	UINT_8 statusRaw;
	UINT_8 channel;
	UINT_8 data1;
	UINT_8 data2;
} MidiEvent_T;

typedef MidiEvent_T FAR *MidiEventPtr_T;

class MidiBuffer
{
public:
	MidiBuffer(void);
	~MidiBuffer(void);

    MidiEventPtr_T FAR PASCAL GetEvent();
    void FAR PASCAL PutEvent(MidiEventPtr_T lpEvent);
	UINT_32 FAR PASCAL EventsInBuffer() {return midiBufferCount;}

private:
     HANDLE    midiBufferHandle;     // buffer handle
     UINT_16   midiBufferError;      // error flags
     UINT_32   midiBufferSize;       // buffer size (in MIDI events)
     UINT_32   midiBufferCount;      // byte count (in MIDI events)
     MidiEventPtr_T midiBufferStart; // ptr to start of buffer
     MidiEventPtr_T midiBufferEnd;   // ptr to end of buffer (last byte + 1)
     MidiEventPtr_T midiBufferHead;  // ptr to head (next location to fill)
     MidiEventPtr_T midiBufferTail;  // ptr to tail (next location to empty)

    MidiEventPtr_T memoryPtr;        // pointer to the buffer memory
 
}; // end class MidiBuffer

typedef MidiBuffer FAR *MidiBufferPtr_T;

#endif