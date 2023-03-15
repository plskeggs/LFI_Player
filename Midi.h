#ifndef MIDI_H
#define MIDI_H

#include "MidiBuf.h"

// Compile-time application metrics
#define MAX_NUM_DEVICES         8       // max # of MIDI input devices

// Custom messages sent by low-level callback to application 
#define MM_MIDIINPUT    WM_APP + 0
#define EVNT_F_ERROR    0x00000001L

// MIDI status byte definitions
#define NOTEON          0x90
#define NOTEOFF         0x80
#define KEYAFTERTOUCH   0xa0
#define CONTROLCHANGE   0xb0
#define CS10            0xbf
#define PROGRAMCHANGE   0xc0
#define CHANAFTERTOUCH  0xd0
#define PITCHBEND       0xe0
#define SYSTEMMESSAGE   0xf0
#define BEGINSYSEX      0xf0
#define MTCQUARTERFRAME 0xf1
#define SONGPOSPTR      0xf2
#define SONGSELECT      0xf3

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

typedef struct callbackInstance_tag
{
    HWND                hWnd;
    UINT_32             device;
    MidiBufferPtr_T     lpBuf;
    HMIDIOUT            hMapper;
} CallbackInstanceData_T;

typedef CallbackInstanceData_T FAR *CallbackInstanceDataPtr_T;

void FAR PASCAL MidiInputHandler(HMIDIIN,
		                         UINT_16,
                                 UINT_32,
                                 UINT_32,
                                 UINT_32);

class Midi
{
public:
    USHORT Ok(void) const {return(okFlag);}
	Midi();
	Midi(HWND windowHandle);
	~Midi();
    MidiEventPtr_T FAR PASCAL GetEvent(void);
	UINT_32 FAR PASCAL EventsInBuffer(void)
	    {return lpInputBuffer->EventsInBuffer();}
	void SetContinuationEvent(MidiEventPtr_T incomingEventPtr);
    UINT_16 CheckContinuationEventReceived(MidiEventPtr_T incomingEventPtr);
	UINT_16 WaitingForEvent(void)
	    {return waitingForContinuationEvent;}
	UINT_8 GetContinuationData()
	    {return continuationEvent.data1;}
	UINT_16 GetNumberOfMidiDevices()
	    {return wNumDevices;}

private:
//	HWND hWnd;
	MidiBufferPtr_T lpInputBuffer;

    HMIDIOUT hMapper;    // Handle to MIDI Mapper
    UINT wNumDevices;    // Number of MIDI input devices opened

    MIDIINCAPS midiInCaps[MAX_NUM_DEVICES]; // Device capabilities structures
    HMIDIIN    hMidiIn[MAX_NUM_DEVICES];    // MIDI input device handles

    // Callback instance data pointers
    CallbackInstanceDataPtr_T lpCallbackInstanceData[MAX_NUM_DEVICES];
	MidiEvent_T continuationEvent;
	USHORT waitingForContinuationEvent;
    USHORT okFlag;
};

#endif