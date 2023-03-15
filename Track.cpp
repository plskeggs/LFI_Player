//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Track.cpp                                  *
// * Description: Implementation of the Track              *
// * class.                                                *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#include "stdafx.h"

#ifndef TRACK_H
#include "track.h"
#endif

#ifndef FRAME_H
#include "frame.h"
#endif

#ifndef FRAMESEQUENCE_H
#include "frameseq.h"
#endif

#ifndef ABSTRACT_H
#include "abstract.h"
#endif

Track::Track() :
    okFlag(M_TRUE),
    trackType(INVALID),
	outputDevice(DEFAULT_DEVICE),
	trackPointBuff(),
	ZAxisGain(0),
    ZAxisOffset(0),
	ZAxisPeriod(1.0),
	ZAxisFormula(1),
    IncrementValue(0.0)
{
    return;
}

M_UINT16
Track::GetNumberOfPoints()
{
	if (trackType == ABSTRACT)
	{
        return ((Abstract*)this)->GetAbsPointsPerFrame();
	}
	else if (trackType == ILDA)
	{
		Frame* tempFrame = ((FrameSequence*)this)->GetCurrentFrame();
        return tempFrame->GetNumberOfPoints();
	}
	else
	{
		return 0;
	}
}