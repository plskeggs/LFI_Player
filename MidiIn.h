#ifndef MIDI_IN_H
#define MIDI_IN_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2021, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: MidiIn.h                                   *
// * Description: This file contains the interface for the *
// *              Midi class.                              *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#include "MidiBuffer.h"

// max # of MIDI input devices
#define MAX_NUM_DEVICES         8

// only one error currently defined
#define EVENT_FLAG_ERROR    0x00000001L

// MIDI Status Byte Definitions
#define SB_NOTEON           0x90
#define SB_NOTEOFF          0x80
#define SB_KEYAFTERTOUCH    0xa0

#define SB_CONTROLCHANGE00  0xb0
#define SB_CONTROLCHANGE15  0xbf

#define SB_PROGRAMCHANGE    0xc0
#define SB_CHANAFTERTOUCH   0xd0
#define SB_PITCHBEND        0xe0
#define SB_SYSTEMMESSAGE    0xf0
#define SB_BEGINSYSEX       0xf0
#define SB_MTCQUARTERFRAME  0xf1
#define SB_SONGPOSPTR       0xf2
#define SB_SONGSELECT       0xf3

// MIDI Control Change Value Definitions
#define CC_MODWHEEL         0x01 // Modulation Wheel
#define CC_VOLUME           0x07 // Volume
#define CC_PAN              0x0A // Pan
#define CC_EXPRESSION       0x0B // Expression
#define CC_SUSTAINPEDAL     0x40 // Sustain Pedal
#define CC_FILTERRES        0x47 // Filter Resonance
#define CC_RELEASETIME      0x49 // Release Time
#define CC_ATTACKTIME       0x49 // Attack Time
#define CC_CUTOFFFREQ       0x4A // Cutoff Frequency
#define CC_DECAYTIME        0x4B // Decay Time
#define CC_VIBRATORATE      0x4C // Vibrato Rate
#define CC_VIBRATODEPTH     0x4D // Vibrato Depth
#define CC_VIBRATODELAY     0x4E // Vibrato Delay
#define CC_REGPARAMNUMLSB   0x64 // Registered Parameter Number LSB
#define CC_REGPARAMNUMLSB   0x65 // Registered Parameter Number MSB
#define CC_ALLCTRLRSOFF     0x79 // All Controllers Off
#define CC_ALLNOTESOFF      0x7B // All Notes Off

#define CC_DECRTRACK        0x6E
#define CC_INCRTRACK        0x6F

#define MIDIVALUEMIN        0x00
#define MIDIVALUEMAX        0x7F

// JL Cooper CS-10 controls.  All messages sent from CS-10 are
// on MIDI channel 16.  Messages are of the form...
//
// 0x0BF  cc   vv
//
// ...where cc = switch or wheel number, and vv = value
//        Control               "cc"    "vv"
//        ====================  ====    ===============================
#define   MUTE_BUTTON_1         0x00    // 0x00=Release, 0x7f=depressed
#define   MUTE_BUTTON_2         0x01    // 0x00=Release, 0x7f=depressed
#define   MUTE_BUTTON_3         0x02    // 0x00=Release, 0x7f=depressed
#define   MUTE_BUTTON_4         0x03    // 0x00=Release, 0x7f=depressed
#define   MUTE_BUTTON_5         0x04    // 0x00=Release, 0x7f=depressed
#define   MUTE_BUTTON_6         0x05    // 0x00=Release, 0x7f=depressed
#define   MUTE_BUTTON_7         0x06    // 0x00=Release, 0x7f=depressed
#define   MUTE_BUTTON_8         0x07    // 0x00=Release, 0x7f=depressed
#define   MODE_BUTTON           0x08    // 0x00=Release, 0x7f=depressed
#define   SHIFT_BUTTON          0x09    // 0x00=Release, 0x7f=depressed
#define   F1_BUTTON             0x0A    // 0x00=Release, 0x7f=depressed
#define   F2_BUTTON             0x0B    // 0x00=Release, 0x7f=depressed
#define   F3_BUTTON             0x0C    // 0x00=Release, 0x7f=depressed
#define   F4_BUTTON             0x0D    // 0x00=Release, 0x7f=depressed
#define   F5_BUTTON             0x0E    // 0x00=Release, 0x7f=depressed
#define   F6_BUTTON             0x0F    // 0x00=Release, 0x7f=depressed
#define   F7_BUTTON             0x10    // 0x00=Release, 0x7f=depressed
#define   F8_BUTTON             0x11    // 0x00=Release, 0x7f=depressed
#define   F9_BUTTON             0x12    // 0x00=Release, 0x7f=depressed
#define   REW_BUTTON            0x13    // 0x00=Release, 0x7f=depressed
#define   FF_BUTTON             0x14    // 0x00=Release, 0x7f=depressed
#define   STOP_BUTTON           0x15    // 0x00=Release, 0x7f=depressed
#define   PLAY_BUTTON           0x16    // 0x00=Release, 0x7f=depressed
#define   REC_BUTTON            0x17    // 0x00=Release, 0x7f=depressed
#define   LEFT_WHEEL_BUTTON     0x18    // 0x00=Release, 0x7f=depressed
#define   RIGHT_WHEEL_BUTTON    0x19    // 0x00=Release, 0x7f=depressed
#define   UP_CURSOR_BUTTON      0x1A    // 0x00=Release, 0x7f=depressed
#define   DOWN_CURSOR_BUTTON    0x1B    // 0x00=Release, 0x7f=depressed
#define   LEFT_CURSOR_BUTTON    0x1C    // 0x00=Release, 0x7f=depressed
#define   RIGHT_CURSOR_BUTTON   0x1D    // 0x00=Release, 0x7f=depressed
#define   FOOTSWITCH            0x1E    // 0x00=Release, 0x7f=depressed

#define   FADER_1               0x40    // Range 0x00 to 0x7f
#define   FADER_2               0x41    // Range 0x00 to 0x7f
#define   FADER_3               0x42    // Range 0x00 to 0x7f
#define   FADER_4               0x43    // Range 0x00 to 0x7f
#define   FADER_5               0x44    // Range 0x00 to 0x7f
#define   FADER_6               0x45    // Range 0x00 to 0x7f
#define   FADER_7               0x46    // Range 0x00 to 0x7f
#define   FADER_8               0x47    // Range 0x00 to 0x7f
#define   BOOST_CUT             0x48    // Range 0x00 to 0x7f
#define   FREQUENCY             0x49    // Range 0x00 to 0x7f
#define   BANDWIDTH             0x4A    // Range 0x00 to 0x7f
#define   SEND_1                0x4B    // Range 0x00 to 0x7f
#define   SEND_2                0x4C    // Range 0x00 to 0x7f
#define   PAN                   0x4D    // Range 0x00 to 0x7f
// Wheel data is "2's complement" value, with bit 6 as
// the sign bit.  Positive numbers are sent for clockwise
// rotation.  The number sent represents the number of wheel
// counts accumulated since last transmission.  Resolution is
// approximately 180 counts per revolution.  Transmission are
// sent at about 10 millisecond intervals while wheel is turning. 
#define   JOG_WHEEL             0x60

typedef struct
{
    HWND                windowHandle;
	// HANDLE              hSelf;  
    M_UINT32             device;
    MidiBufferPtr_T     bufferPtr;
    HMIDIOUT            midiMapperHandle;
} CallbackInstanceData_T;

typedef CallbackInstanceData_T FAR *CallbackInstanceDataPtr_T;

CallbackInstanceDataPtr_T FAR PASCAL AllocCallbackInstanceData(void);
void FAR PASCAL FreeCallbackInstanceData(CallbackInstanceDataPtr_T lpBuf);


void FAR PASCAL MidiInputHandler(HMIDIIN midiInDeviceHandles,
		                         M_UINT16 midiMsg,
								 M_UINT32 callbackInstancePtr,
                                 M_UINT32 param1,
                                 M_UINT32 param2);

class Midi
{
public:
    M_UINT16 Ok(void) const {return(okFlag);}
	Midi();
	Midi(HWND windowHandle);
	~Midi();

    MidiEventPtr_T FAR PASCAL GetEvent(void);
	M_UINT32 FAR PASCAL EventsInBuffer(void)
	    {return inputBufferPtr->EventsInBuffer();}
	void SetContinuationEvent(MidiEventPtr_T incomingEventPtr);
    M_UINT16 CheckContinuationEventReceived(MidiEventPtr_T incomingEventPtr);
	M_UINT16 WaitingForEvent(void)
	    {return waitingForContinuationEvent;}
	M_UINT8 GetContinuationData()
	    {return continuationEvent.data1;}
	M_UINT16 GetNumberOfMidiDevices()
	    {return numMidiDevices;}

    void FAR PASCAL Midi::StopMidiProcessing(void);
	void FAR PASCAL Midi::StartMidiProcessing(void);

	M_UINT16  midiNotesOn;

private:
	MidiBufferPtr_T inputBufferPtr;

    HMIDIOUT midiMapperHandle;
    UINT     numMidiDevices;

    MIDIINCAPS midiInDeviceCapabilities[MAX_NUM_DEVICES];
    HMIDIIN    midiInDeviceHandles[MAX_NUM_DEVICES];

    CallbackInstanceDataPtr_T callbackInstanceDataPtrArray[MAX_NUM_DEVICES];
	MidiEvent_T continuationEvent;
	M_UINT16 waitingForContinuationEvent;
	M_UINT16 processMidiEvents;
    M_UINT16 okFlag;
};

#endif