#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include "midi.h"

#ifndef GLOBALS_H
#include "globals.h"
#endif

extern ofstream logFile;

// Lookup tables for MIDI message names
char szEventNames[8][24] =
{"Note Off",
 "Note On",
 "Key Aftertouch",
 "Control Change",
 "Program Change",
 "Channel Aftertouch",
 "Pitch Bend",
 "System Message"};

char szSysMsgNames[16][24] = 
{"System Exclusive",
 "MTC Quarter Frame",
 "Song Position Pointer",
 "Song Select",
 "Undefined",
 "Undefined",
 "Tune Request",
 "System Exclusive End",
 "Timing Clock",
 "Undefined",
 "Start",
 "Continue",
 "Stop",
 "Undefined",
 "Active Sensing",
 "System Reset"};

////////////////////////////////////////////////////////////////////
//
// Midi::Midi()
//
////////////////////////////////////////////////////////////////////
Midi::Midi(HWND windowHandle):
lpInputBuffer(0),
waitingForContinuationEvent(M_FALSE),
okFlag(M_FALSE)
{
    wNumDevices = midiInGetNumDevs();

    if (!wNumDevices)
	{
        logFile << "No MIDI devices currently connected." << endl;
        return;
    }

//	hWnd = windowHandle;

    hMapper = 0;       // Handle to MIDI Mapper
    
    // Allocate and lock a MidiBuffer instance.
    lpInputBuffer = new MidiBuffer();

    if(lpInputBuffer == NULL)
	{
        return;
	}

    // Page lock the memory.  Global memory blocks accessed by
    // low-level1callback functions must be page locked.
     
#ifndef _WIN32
    GlobalSmartPageLock((HGLOBAL)HIWORD(lpInputBuffer));
#endif

	UINT wRtn;
	char szErrorText[256];
	UINT i;

    for (i=0; (i<wNumDevices) && (i<MAX_NUM_DEVICES); i++)
	{
        wRtn = midiInGetDevCaps(i, (LPMIDIINCAPS) &midiInCaps[i],
                                sizeof(MIDIINCAPS));
        if(wRtn)
		{
            midiInGetErrorText(wRtn, (LPSTR)szErrorText, 
                               sizeof(szErrorText));
            logFile << "Error " << wRtn
				    << "\ngetting MIDI device capabilities for device " 
					<< i << endl;
        }
    }

    // Open all MIDI input devices after allocating and setting up
    // instance data for each device.  The instance data is used to
    // pass buffer management information between the application and
    // the low-level callback function.  It also includes a device ID,
    // a handle to the MIDI Mapper, and a handle to the application's
    // display window, so the callback can notify the window when input
    // data is available.  A single callback function is used to service
    // all opened input devices.
    for (i=0; (i<wNumDevices) && (i<MAX_NUM_DEVICES); i++)
    {
        lpCallbackInstanceData[i] = new CallbackInstanceData_T;
        if (lpCallbackInstanceData[i] == NULL)
        {
			logFile << "Can't create CallbackInstanceData for device "
				    << i << endl;
            delete lpInputBuffer;
            return;
        }

        lpCallbackInstanceData[i]->hWnd = windowHandle;         
        lpCallbackInstanceData[i]->device = i;
        lpCallbackInstanceData[i]->lpBuf = lpInputBuffer;
        lpCallbackInstanceData[i]->hMapper = hMapper;
        
        wRtn = midiInOpen((LPHMIDIIN)&hMidiIn[i],
                          i,
                          (UINT_32)MidiInputHandler,
                          (UINT_32)lpCallbackInstanceData[i],
                          CALLBACK_FUNCTION);
        if(wRtn)
        {
			delete lpCallbackInstanceData[i];
            logFile << "Error " << wRtn
				    << "\nopening MIDI device " 
					<< i << endl;
        }
		else
		{
            logFile << "MIDI device " << lpCallbackInstanceData[i]->device
			        << " successfully opened." << endl;
		}
    }

    // Start MIDI input.
    for (i=0; (i<wNumDevices) && (i<MAX_NUM_DEVICES); i++)
	{
        if (hMidiIn[i])
		{
            midiInStart(hMidiIn[i]);
			logFile << "MIDI input started for device " << i << endl;
		}
    }

	okFlag = M_TRUE;

} // end Midi::Midi()

////////////////////////////////////////////////////////////////////
//
// Midi::~Midi()
//
////////////////////////////////////////////////////////////////////
Midi::~Midi()
{
    /* Stop, reset, close MIDI input.  Free callback instance data.
     */
    for (UINT i=0; (i<wNumDevices) && (i<MAX_NUM_DEVICES); i++)
	{
        if (hMidiIn[i]) {
            midiInStop(hMidiIn[i]);
            midiInReset(hMidiIn[i]);
            midiInClose(hMidiIn[i]);
            delete lpCallbackInstanceData[i];
        }
    }

    /* Close the MIDI Mapper, if it's open.
     */
    if (hMapper)
        midiOutClose(hMapper);

	if (lpInputBuffer)
	{
#ifndef _WIN32
        GlobalSmartPageUnlock((HGLOBAL)HIWORD(lpInputBuffer));
#endif
        delete lpInputBuffer;
	}
}

////////////////////////////////////////////////////////////////////
//
// Midi::SetContinuationEvent()
//
////////////////////////////////////////////////////////////////////
void Midi::SetContinuationEvent(MidiEventPtr_T incomingEventPtr)
{
    if (incomingEventPtr)
	{
        waitingForContinuationEvent = M_TRUE;
        continuationEvent = *incomingEventPtr;

		if (continuationEvent.status == NOTEON)
		{
            continuationEvent.status = NOTEOFF;
        }
	}
}

////////////////////////////////////////////////////////////////////
//
// Midi::CheckContinuationEventReceived()
//
////////////////////////////////////////////////////////////////////
USHORT Midi::CheckContinuationEventReceived(MidiEventPtr_T incomingEventPtr)
{
    // A note on with a velocity of 0 is a note off
    if((incomingEventPtr->status == NOTEON) &&
       (incomingEventPtr->data2 == 0))
	{
        incomingEventPtr->status = NOTEOFF;
	}

    if (continuationEvent.status == incomingEventPtr->status &&
        continuationEvent.data1 == incomingEventPtr->data1)
	{
		waitingForContinuationEvent = M_FALSE;
        return M_TRUE;
	}

    return M_FALSE;
}

////////////////////////////////////////////////////////////////////
//
// Midi::GetEvent()
//
////////////////////////////////////////////////////////////////////
MidiEventPtr_T FAR PASCAL Midi::GetEvent(void)
{
    if (lpInputBuffer)
	{
        return (lpInputBuffer->GetEvent());
	}
	else
	{
        return 0;
	}
}


////////////////////////////////////////////////////////////////////
//
// Support Function MidiInputHandler()
//
////////////////////////////////////////////////////////////////////
void FAR PASCAL MidiInputHandler(HMIDIIN hMidiIn, 
                                 UINT_16 wMsg, 
                                 UINT_32 dwInstance, 
                                 UINT_32 dwParam1, 
                                 UINT_32 dwParam2)
{
    MidiEvent_T event;
    
    switch(wMsg)
    {
        case MIM_OPEN:
            break;

        // The only error possible is invalid MIDI data, so just pass
        // the invalid data on so we'll see it.
        case MIM_ERROR:
        case MIM_DATA:
		{
            event.fEvent = (wMsg == MIM_ERROR) ? EVNT_F_ERROR : 0;
            event.device = ((CallbackInstanceDataPtr_T)dwInstance)->device;
//            event.data = dwParam1;
            event.statusRaw  = LOBYTE(LOWORD(dwParam1));
            event.status     = (UINT_8) (event.statusRaw & (UINT_8) 0xf0);
            event.channel    = (UINT_8) (event.statusRaw & (UINT_8) 0x0f);
            event.data1      = HIBYTE(LOWORD(dwParam1));
            event.data2      = LOBYTE(HIWORD(dwParam1));
            event.timeStamp   = dwParam2;
            
            // Send the MIDI event to the MIDI Mapper, put it in the
            // circular input buffer, and notify the application that
            // data was received.
            if(((CallbackInstanceDataPtr_T)dwInstance)->hMapper)
                midiOutShortMsg( 
                            ((CallbackInstanceDataPtr_T)dwInstance)->hMapper, 
                              dwParam1);

            (((CallbackInstanceDataPtr_T)dwInstance)->lpBuf)->PutEvent((MidiEventPtr_T) &event);

//            PostMessage(((CallbackInstanceDataPtr_T)dwInstance)->hWnd,
//                          MM_MIDIINPUT, 0, 0L);

		}

            break;

        default:
            break;
    }
}


