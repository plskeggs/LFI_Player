//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: MidiIn.cpp                                 *
// * Description: Implementation of the Midi class.        *
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
#include <stdio.h>
#include "MidiIn.h"

#ifndef GLOBALS_H
#include "globals.h"
#endif

using namespace std; 

extern ofstream logFile;


////////////////////////////////////////////////////////////////////
//
// Midi::Midi()
//
////////////////////////////////////////////////////////////////////
Midi::Midi(HWND windowHandle):
inputBufferPtr(0),
waitingForContinuationEvent(M_FALSE),
midiNotesOn(0),
processMidiEvents(M_FALSE),
okFlag(M_FALSE)
{
    numMidiDevices = midiInGetNumDevs();

    if (!numMidiDevices)
	{
        logFile << "No MIDI devices currently connected." << endl;
        return;
    }

    midiMapperHandle = 0; 
    inputBufferPtr = new MidiBuffer();

    if(inputBufferPtr == NULL)
	{
        return;
	}

	UINT returnVal;
	char errorString[256];
	UINT i;

    for (i=0; (i<numMidiDevices) && (i<MAX_NUM_DEVICES); i++)
	{
        returnVal = midiInGetDevCaps(i, (LPMIDIINCAPS) &midiInDeviceCapabilities[i],
                                sizeof(MIDIINCAPS));
        if(returnVal)
		{
            midiInGetErrorText(returnVal, (LPSTR)errorString, 
                               sizeof(errorString));
            logFile << "Midi::Midi(): Error " << returnVal
				    << "\ngetting MIDI device capabilities for device " 
					<< i << endl;
        }
    }

    // Open each MIDI input devices after setting up corresponding
	// instance data.
    for (i=0; (i<numMidiDevices) && (i<MAX_NUM_DEVICES); i++)
    {
        callbackInstanceDataPtrArray[i] = new CallbackInstanceData_T;
        if (callbackInstanceDataPtrArray[i] == NULL)
        {
			logFile << "Midi::Midi(): Can't create MIDI instance data for device "
				    << i << endl;
            delete inputBufferPtr;
            return;
        }

        callbackInstanceDataPtrArray[i]->windowHandle = windowHandle;         
        callbackInstanceDataPtrArray[i]->device = i;
        callbackInstanceDataPtrArray[i]->bufferPtr = inputBufferPtr;
        callbackInstanceDataPtrArray[i]->midiMapperHandle = midiMapperHandle;
        
        returnVal = midiInOpen((LPHMIDIIN)&midiInDeviceHandles[i],
                          i,
                          (M_UINT32)MidiInputHandler,
                          (M_UINT32)callbackInstanceDataPtrArray[i],
                          CALLBACK_FUNCTION);
        if(returnVal)
        {
			delete callbackInstanceDataPtrArray[i];
            logFile << "Midi::Midi(): Error " << returnVal
				    << " while opening MIDI device " 
					<< i << endl;
        }
		else
		{
            logFile << "Midi::Midi(): MIDI device " << callbackInstanceDataPtrArray[i]->device
			        << " successfully opened." << endl;
		}
    }

    // start MIDI input for each device
    for (i=0; (i<numMidiDevices) && (i<MAX_NUM_DEVICES); i++)
	{
        if (midiInDeviceHandles[i])
		{
            midiInStart(midiInDeviceHandles[i]);
			logFile << "Midi::Midi(): MIDI input started for device " << i << endl;
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
    // shut down each MIDI input, and deallocate the
	// instance data
    for (UINT i=0; (i<numMidiDevices) && (i<MAX_NUM_DEVICES); i++)
	{
        if (midiInDeviceHandles[i]) {
            midiInStop(midiInDeviceHandles[i]);
            midiInReset(midiInDeviceHandles[i]);
            midiInClose(midiInDeviceHandles[i]);
			delete callbackInstanceDataPtrArray[i];
        }
    }

    if (midiMapperHandle)
	{
        midiOutClose(midiMapperHandle);
	}

	if (inputBufferPtr)
	{
#ifndef _WIN32
        GlobalSmartPageUnlock((HGLOBAL)HIWORD(inputBufferPtr));
#endif
        delete inputBufferPtr;
	}
}

////////////////////////////////////////////////////////////////////
//
// Midi::SetContinuationEvent()
//
////////////////////////////////////////////////////////////////////
void Midi::SetContinuationEvent(MidiEventPtr_T incomingEventPtr)
{
	// Note that caller verifies incoming event is a
	// valid trigger for a continuation event
    if (incomingEventPtr)
	{
        waitingForContinuationEvent = M_TRUE;
        continuationEvent = *incomingEventPtr;

		// if (continuationEvent.status == NOTEON)
		{
            continuationEvent.status = SB_NOTEOFF;
        }
	}
}

////////////////////////////////////////////////////////////////////
//
// Midi::CheckContinuationEventReceived()
//
////////////////////////////////////////////////////////////////////
M_UINT16 Midi::CheckContinuationEventReceived(MidiEventPtr_T incomingEventPtr)
{
    // A note on with a velocity of 0 is a note off
    if((incomingEventPtr->status == SB_NOTEON) &&
       (incomingEventPtr->data2 == 0))
	{
        incomingEventPtr->status = SB_NOTEOFF;
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
    if (inputBufferPtr && (processMidiEvents == M_TRUE))
	{
        return (inputBufferPtr->GetEvent());
	}
	else
	{
        return 0;
	}
}

////////////////////////////////////////////////////////////////////
//
// Midi::StopMidiProcessing()
//
////////////////////////////////////////////////////////////////////
void FAR PASCAL Midi::StopMidiProcessing(void)
{
    inputBufferPtr->EmptyBuffer();
	processMidiEvents = M_FALSE;
	waitingForContinuationEvent = M_FALSE;
	midiNotesOn = 0;
}

////////////////////////////////////////////////////////////////////
//
// Midi::StartMidiProcessing()
//
////////////////////////////////////////////////////////////////////
void FAR PASCAL Midi::StartMidiProcessing(void)
{
    inputBufferPtr->EmptyBuffer();
	processMidiEvents = M_TRUE;
	midiNotesOn = 0;
}

// End Midi Class

////////////////////////////////////////////////////////////////////
//
// MIDI Callback Function MidiInputHandler()
// Description: A pointer to this function is passed to
// midiInOpen().  This callback function queues the incoming
// MIDI event in the MIDI buffer.  Note that this function is
// NOT a member of the MIDI class, but was placed here out of
// laziness.
//
////////////////////////////////////////////////////////////////////
void FAR PASCAL MidiInputHandler(HMIDIIN midiInDeviceHandles, 
                                 M_UINT16 midiMsg,
								 M_UINT32 callbackInstancePtr,
                                 M_UINT32 param1, 
                                 M_UINT32 param2)
{
    MidiEvent_T event;
    
    switch(midiMsg)
    {
        case MIM_OPEN:
            break;

        case MIM_ERROR:
        case MIM_DATA:
		{
            event.flags = (midiMsg == MIM_ERROR) ? EVENT_FLAG_ERROR : 0;
            event.device = ((CallbackInstanceDataPtr_T)callbackInstancePtr)->device;
            event.statusRaw  = LOBYTE(LOWORD(param1));
            event.status     = (M_UINT8) (event.statusRaw & (M_UINT8) 0xf0);
            event.channel    = (M_UINT8) (event.statusRaw & (M_UINT8) 0x0f);
            event.data1      = HIBYTE(LOWORD(param1));
            event.data2      = LOBYTE(HIWORD(param1));
            event.timeStamp  = param2;

			if((event.status == SB_NOTEON) && (event.data2 == 0))
			{
                event.status = SB_NOTEOFF;
            }
            
            if(((CallbackInstanceDataPtr_T)callbackInstancePtr)->midiMapperHandle)
                midiOutShortMsg( 
                            ((CallbackInstanceDataPtr_T)callbackInstancePtr)->midiMapperHandle, 
                              param1);

			(((CallbackInstanceDataPtr_T)callbackInstancePtr)->bufferPtr)->PutEvent((MidiEventPtr_T) &event);

		}

            break;

        default:
            break;
    }
}


