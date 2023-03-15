#ifndef COMMANDLINE_H
#define COMMANDLINE_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: CommandLine.h                              *
// * Description: This file contains the interface for the *
// *              CCommandLine class.                      *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#include "Globals.h"

// msvob 3/20/2022: I have no freaking idea what MERCURY_ONLINE_TEST code
// is for but I know as soon as I delete it I'll probably regret it :-( 
// #define MERCURY_ONLINE_TEST

typedef enum CommandlineArgumentType
{
    CL_NONE             = 0,
	CL_INPUTFILENAME    = 1,  //   first argument
	CL_COLORPALETTE     = 2,  //   -p
	CL_SCANRATE         = 3,  //   -s
	CL_BLANKINGSHIFT    = 4,  //   -b
	CL_FRAMEDELAY       = 5,  //   -d
	CL_INVERTBLANKING   = 6,  //   /r
#ifdef MERCURY_ONLINE_TEST
	CL_NUMBEROFLOOPS    = 7,  //   -l
	CL_MONOCHROMEOUTPUT = 8,  //   /w
	CL_ESTIMATEDURATION = 9,  //   /e
#endif
};

#define CL_INPUTFILENAME_MASK       0x00000001
#define CL_COLORPALETTE_MASK        0x00000002
#define CL_SCANRATE_MASK            0x00000004
#define CL_BLANKINGSHIFT_MASK       0x00000008
#define CL_FRAMEDELAY_MASK          0x00000010
#define CL_INVERTBLANKING_MASK      0x00000020

#ifdef MERCURY_ONLINE_TEST
  #define CL_NUMBEROFLOOPS_MASK       0x00000040
  #define CL_MONOCHROMEOUTPUT_MASK    0x00000080
  #define CL_ESTIMATEDDURATION_MASK   0x00000100
#endif

typedef struct
{
	M_UINT32  ArgsPassedBitmap;
	char     InputFileName[80];
    PaletteType ColorPaletteType;
	M_INT16   BlankingShift;
	M_UINT16  FrameDelay;
	M_UINT16  ScanRate;
#ifdef MERCURY_ONLINE_TEST
	M_UINT16  NumberOfLoops;
#endif
} CommandLineInfo_T;

class CCommandLine : public CCommandLineInfo
{
public:
    CCommandLine();
	~CCommandLine();

	void ParseParam(LPCTSTR lpszParam,
		            BOOL bFlag,
					BOOL bLast);

    CommandLineInfo_T cmdLineInfo;

private:
    CommandlineArgumentType nextCommandLineArgumentType;

};

#endif