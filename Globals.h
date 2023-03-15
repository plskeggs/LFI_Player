#ifndef GLOBALS_H
#define GLOBALS_H
//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Globals.h                                  *
// * Description: This file contains a variety of constant *
// *              definitions, enumerations, structures,   *
// *              type definitions, etc. in support of the *
// *              LFI Player application.                  *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

// #include "vld.h"
#include "Compile_Switch.h"


// *********************************************************

   #define VERSION_NUMBER "1.2.3"

// *********************************************************


// Following are the defined data types for MS Visual C++ Version 6.0.
// The keywords signed and unsigned can be used to qualify integer and
// character types.
//
// BASE TYPE      DEFAULT SIGN         DESCRIPTION
// ==========     ==================   ===================
// boolean        unsigned             8-bit data item 
// byte           - (not applicable)   8-bit data item 
// char           unsigned             8-bit unsigned data item 
// double         -                    64-bit floating-point number 
// double          -                    32-bit floating-point number 
// handle_t       -                    Primitive handle type 
// hyper          signed               64-bit signed integer 
// int            signed               32-bit signed integer 
// long           signed               32-bit signed integer 
// short          signed               16-bit signed integer 
// small          signed               8-bit signed integer 
// void *         -                    32-bit context handle pointer type 
// wchar_t        unsigned             16-bit unsigned data item 

    typedef char           M_INT8;
    typedef unsigned char  M_UINT8;

    typedef short          M_INT16;
    typedef unsigned short M_UINT16;

    typedef long           M_INT32;
    typedef unsigned long  M_UINT32;
   
    const M_UINT16  MAX_FILENAME_SIZE = 500;
    const M_UINT16  MAX_BASENAME_SIZE = 200;
    const M_UINT16  MAX_FILETITLE_SIZE = 100;
    const M_UINT16  M_FAILURE = 0;
    const M_UINT16  M_SUCCESS = 1;
    const M_UINT16  M_FALSE = 0;
    const M_UINT16  M_TRUE  = 1;
    const double PI = 3.14159265359;

    const double WORLD_MIN = -32767.0;
    const double WORLD_MAX = 32766.0;

    const M_UINT16  ILDA_LEN = 4;
    const M_UINT16  FRAME_NAME_LEN = 8; 
    const M_UINT16  COMPANY_NAME_LEN = 8;
    const M_UINT16  SKIP_LEN = 3;
    const M_UINT16  WORD_LEN = 2;
    const M_UINT16  CHAR_LEN = 1;
    const M_UINT8     BLANKING_MASK = 0x40; // Binary: 0100 0000
    const M_INT16     BLANKING_MASK_WORD = 0x4000; // Binary: 0100 0000 0000 0000

    const enum    ManipulationPoint{FROM_POINT  = 0,
                                    AT_POINT    = 1,
                                    BOTH_POINTS = 2};

    const enum    DisplayColorMode{COLOR_NORMAL     = 0,
                                   COLOR_INVERT     = 1,
                                   COLOR_MONOCHROME = 2,
                                   COLOR_GRAYSCALE  = 3,
                                   COLOR_CVL        = 4,
                                   COLOR_TTL        = 5,
                                   COLOR_RG_INV_TTL = 6};

    const enum    PaletteType{PANGOLIN        = 0,
                              LASER_ILLUSIONS = 1,
                              ILDA            = 2,
                              X29             = 3,
                              TRUECOLOR       = 4};

    // Note that the world view should implies 3D, but nobody
    // else seems to write their frames this way, and Anarchy
    // editor pukes on the 3D value anyway.  Sigh, so
    // much for following the standard...
    const enum   ILDAFormatCode {INDEXED_COLOR_3D = 0,
                                 INDEXED_COLOR_2D = 1,
                                 COLOR_PALETTE    = 2,
                                 UNUSED           = 3,
                                 TRUE_COLOR_3D    = 4,
                                 TRUE_COLOR_2D    = 5};

    const enum    DisplayViewType{WORLD  = 0,
                                  EYE    = 1,
                                  NDC    = 2,
                                  DEVICE = 3};

    const enum    LaserColor{RED   = 0,
                             GREEN = 1,
                             BLUE  = 2};

    const M_INT32   MAX_TRACK_DELAY = 10000L;
    const M_INT32   MIN_TRACK_DELAY = 0L;

    const M_INT32   MAX_GLOBAL_DELAY = 1000L;
    const M_INT32   MIN_GLOBAL_DELAY = 0L;

#define MIN_DISPLAY_TIMER_VALUE   0
#define MAX_DISPLAY_TIMER_VALUE   1000

#define MAIN_WINDOW_LEFT_DEFAULT      20
#define MAIN_WINDOW_TOP_DEFAULT       20
#define MAIN_WINDOW_RIGHT_DEFAULT     (MAIN_WINDOW_LEFT_DEFAULT + 664)
#define MAIN_WINDOW_BOTTOM_DEFAULT    (MAIN_WINDOW_TOP_DEFAULT  + 488)

#define MIN_PPS   1000
#define MAX_PPS   50000

#define MIN_BLANKING_SHIFT  -50
#define MAX_BLANKING_SHIFT   50

#define MAX_LIVE_FADE_IN_FRAMES   100
#define MIN_LIVE_FADE_IN_FRAMES     1

#define MAX_LIVE_FADE_OUT_FRAMES   100
#define MIN_LIVE_FADE_OUT_FRAMES     1

#define MAX_LIVE_FRAME_REPEAT   100
#define MIN_LIVE_FRAME_REPEAT     0

#define MAX_LIVE_COLOR_INTENSITY   127
#define MIN_LIVE_COLOR_INTENSITY     0

    const M_INT32   MAXPPS = 100000L;
    const M_INT32   MINPPS = 1000L;
    const M_INT32   DELAYINCR = 200L;
    const M_UINT32  MAX_MOVE_INCREMENT   = 10000;
    const M_UINT32  MAX_ZOOM_INCREMENT   = 10000;
    const M_UINT32  MAX_ROTATE_INCREMENT = 90;
    const M_UINT32  MAX_FOCUS_REPEAT     = 100;
    const M_UINT32  MAX_PERCENT          = 100;
    const M_UINT32  MIN_PERCENT          = 1;
    const M_UINT16  INVERT_X = 0x0001;
    const M_UINT16  INVERT_Y = 0x0010;
    const M_UINT16  SCREEN_INVERT = 0x0100;

#define MIN_INT_16_VALUE     -32768
#define MAX_INT_16_VALUE     32767

#define MIN_UINT_16_VALUE     0
#define MAX_UINT_16_VALUE     65535

#define MAX_INT = 32767;
#define MIN_INT = -32768;
#define MAX_UINT = 65535;
#define MIN_UINT = 0;
#define MAX_BYTE = 0xff;
#define MIN_BYTE = 0x00;

#define MIN_UINT8_VALUE 0
#define MAX_UINT8_VALUE 255

typedef struct
{
    double    xValue;
    double    yValue;
    M_UINT8  rValue;
    M_UINT8  gValue;
    M_UINT8  bValue;
    M_UINT8  iValue;
} OutputPoint_T;

#define DegreeToRadian(degreeValue) \
        (double)((double)degreeValue*(PI/180.0))

#define GROUPBOX_RIGHT_EDGE    164


#endif