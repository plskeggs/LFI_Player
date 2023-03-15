//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: JobBlock.cpp                               *
// * Description: Implementation of the JobBlock           *
// * class.                                                *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#ifndef JOBBLOCK_H
#include "jobblock.h"
#endif

JobBlock::JobBlock(void):
    // trackNumber(0),
	// trackNumberToo(0),
//	crossTrack(0),
	numOfFrames(0),
	reentry(M_FALSE),
	skipManipulation(M_FALSE),
	degreeAngle(0.0),
	zoomIncrement(0.0),
	xIncrement(0.0),
	yIncrement(0.0),
	zIncrement(0.0),
	functionType(INVALID)
{
    return;
}

JobBlock::~JobBlock(void)
{
	return;
}

void JobBlock::ResetJobBlock(void)
{
	numOfFrames = 0;
	reentry = M_FALSE;
	skipManipulation = M_FALSE;
	degreeAngle = 0.0;
	zoomIncrement = 0.0;
	xIncrement = 0.0;
	yIncrement = 0.0;
	zIncrement = 0.0;
	functionType = INVALID;
}	
