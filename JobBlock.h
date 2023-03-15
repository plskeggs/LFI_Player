#ifndef JOBBLOCK_H
#define JOBBLOCK_H

//
// *************************************************************
// *                                                           *
// * Copyright 1998-2022, Lazerus Laser Systems                *
// * Author: Michael G. Svob                                   *
// *                                                           *
// * File Name: JobBlock.h                                     *
// * Description: This file contains the interface for the     *
// *              JobBlock class. A job block contains all     *
// *              of the information about the manipulation    *
// *              to be done on a track. When DisplayAllTracks *              *
// *              is called, this info is used to determine    *
// *              what manipulation, if any, needs to be       *
// *              performed on that track.                     *
// *                                                           *
// *              Note that when a "live" manipulation is done *
// *              on a track under script control, that the    *
// *              live manipulation will take precedence       *
// *              over the script manipulation for the         *
// *              duration of the live control.  For this      *
// *              reason, the job block is saved for the       *
// *              track or tracks being controlled live.       *
// *              Once the live manipulation is complete,      *
// *              the original job block is restored, and      *
// *              the script manipulation allowed to           *
// *              continue where it left off.                  *
// *                                                           *
// * This code is distributed under the GNU Public License     *
// * which can be found at...                                  *
// *                                                           *
// * http://www.gnu.org/licenses/gpl.txt                       *
// *                                                           *
// *************************************************************
//

#ifndef GLOBALS_H
#include "globals.h"
#endif

typedef enum
{
    JUST_DISPLAY,
    FADE,
    CROSS_FADE,
    ROTATE_AT,
    ROTATE_FROM,
    ROTATE_BOTH,
    ZOOM,
    MOVE_FORWARD,
    MOVE_AT,
    MOVE_FROM,
    MOVE_BOTH,
    INVALID
} COMMAND_FUNCTION_TYPE;

class JobBlock
{
public:
    JobBlock(void);
    ~JobBlock(void);

    void ResetJobBlock(void);

//    Track*  crossTrack;
    // M_UINT16  trackNumber;
    // M_UINT16  trackNumberToo;
    M_UINT16  skipManipulation;
    M_UINT32   numOfFrames;
    M_UINT16  reentry;
    double   degreeAngle;
    double   zoomIncrement;
    double   xIncrement;
    double   yIncrement;
    double   zIncrement;
    double fadeIncrement;
    enum RotationAxis      rotationAxis;
    enum ZoomDirection     zoomDirection;
    enum FadeDirection     fadeDirection;
    COMMAND_FUNCTION_TYPE  functionType;
};

#endif
