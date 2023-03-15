#ifndef COMPILE_SWITCH_H
#define COMPILE_SWITCH_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Compile_Switch.h                           *
// * Description: This file contains the definitions       *
// *              needed to turn on/off various            *
// *              pre-compiler directives.                 *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

///////////////////////////////////////////////////
//
// Output Device Type Selection
//
///////////////////////////////////////////////////

// #define LEGACY_OUTPUT

// #define RIYA_OUTPUT
// #define EASYLASE_OUTPUT

// NOTE: Since the Helios DAC is leveraging the EzAudDac
// interface, need to define both of the following
// for the Helios, and just EZAUDDAC if using that.
// This is because while they share a common interface,
// Helios and EzAudDac use different DLLs, the later which
// does not include a shutter control.
#define EZAUDDAC_OUTPUT
#define HELIOS_OUTPUT

///////////////////////////////////////////////////
//
// Function Entry Logging
//
///////////////////////////////////////////////////

// #define ABSTRACT_LOG_ENTRY
// #define GEM_ENTRY
// #define VIEW_FUNC_ENTRY


///////////////////////////////////////////////////
//
// Error Logging
//
///////////////////////////////////////////////////

#define ABSTRACT_LOG_ERROR
#define ABSTRACTCONTROL_LOG_ERROR
#define CHEAP3DVIEW_LOG_ERROR
#define FRAMESEQUENCE_LOG_ERROR
#define LIVECONTROL_LOG_ERROR
#define ROUTINE_LOG_ERROR
#define TRACKLIST_LOG_ERROR
#define VIEW_LOG_ERROR

///////////////////////////////////////////////////
//
// Warning Logging
//
///////////////////////////////////////////////////

#define ABSTRACTCONTROL_LOG_WARNING

///////////////////////////////////////////////////
//
// Information Logging
//
///////////////////////////////////////////////////

#define BASIC_LOG_INFO
// #define ABSTRACT_LOG_INFO
// #define ABSTRACTCONTROL_LOG_INFO
// #define CHEAP3DVIEW_LOG_INFO
// #define LIVECONTROL_LOG_INFO
// #define ROUTINE_LOG_INFO
// #define RIYA_LOG_INFO
// #define LOG_PORT_DATA
// #define LOG_MIDI_DATA
// #define CTS_SCRIPT_INFO
// #define POINT_INFO

// #define STATUS_BAR_LOG_INFO

///////////////////////////////////////////////////
//
// Other Stuff
//
///////////////////////////////////////////////////

// #define CLR_STATUS_BAR

// multiply global delay by track delay (new) instead
// of adding (old)
#define NEW_DELAY

// turn on/off display of the message box when CTS
// script is aborted
#define ABORT_SCRIPT_MESSAGE_BOX

// adds a Z-axis component to the scrolling text
#define ZTEXT

// Try to reduce latency in processing MIDI events
// #define REDUCED_MIDI_LATENCY

// #define LOG_PORT_DATA

// #define ROTATE_TEST

#define DISPLAY_PREBLANK

#define SHIFT_BLANK_DELAY

// logs coordinates of both window and client rectangles
// #define DEBUG_WINDOW

#define LATEST_BLANKING_MOD
#define TRACK_FADE_FRAMES
#define WRITE_ALL_TRACKS_AT_ONCE

#define USE_SERIAL_COM

#endif


