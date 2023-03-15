//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: CommandLine.cpp                            *
// * Description: Implementation of the CCommandLine       *
// * class to parse command line parameters on application *
// * startup.                                              *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#include "stdafx.h"
#include "CommandLine.h"

#include <iostream>
#include <fstream>

using namespace std; 

extern ofstream logFile;

CCommandLine::CCommandLine() :
    CCommandLineInfo(),
	nextCommandLineArgumentType(CL_NONE)
{
	for (int i=0 ; i<80 ; ++i)
	{
        cmdLineInfo.InputFileName[i] = 0;
	}

    cmdLineInfo.ArgsPassedBitmap = 0x00000000;
	cmdLineInfo.ColorPaletteType = PaletteType::PANGOLIN;
	cmdLineInfo.BlankingShift    = 0;
	cmdLineInfo.FrameDelay       = 0;
	cmdLineInfo.ScanRate         = 30000;
#ifdef MERCURY_ONLINE_TEST
	cmdLineInfo.NumberOfLoops    = 1;
#endif

    return;
}

CCommandLine::~CCommandLine()
{
    CCommandLineInfo::~CCommandLineInfo();
}

void
CCommandLine::ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast )
{
	char tempString[80];

    // Check for flags...
    if( bFlag )
    {
	    if( lstrcmpi( lpszParam, TEXT( "p" )) == 0 )
		{
            nextCommandLineArgumentType = CL_COLORPALETTE;
		}
	    else if( lstrcmpi( lpszParam, TEXT( "s" )) == 0 )
		{
            nextCommandLineArgumentType = CL_SCANRATE;
		}
	    else if( lstrcmpi( lpszParam, TEXT( "b" )) == 0 )
		{
            nextCommandLineArgumentType = CL_BLANKINGSHIFT;
		}
	    else if( lstrcmpi( lpszParam, TEXT( "d" )) == 0 )
		{
            nextCommandLineArgumentType = CL_FRAMEDELAY;
		}
	    else if( lstrcmpi( lpszParam, TEXT( "r" )) == 0 )
		{
			cmdLineInfo.ArgsPassedBitmap |= CL_INVERTBLANKING_MASK;
		}
#ifdef MERCURY_ONLINE_TEST
	    else if( lstrcmpi( lpszParam, TEXT( "l" )) == 0 )
		{
            nextCommandLineArgumentType = CL_NUMBEROFLOOPS;
		}
	    else if( lstrcmpi( lpszParam, TEXT( "w" )) == 0 )
		{
			cmdLineInfo.ArgsPassedBitmap |= CL_MONOCHROMEOUTPUT_MASK;
		}
	    else if( lstrcmpi( lpszParam, TEXT( "e" )) == 0 )
		{
			cmdLineInfo.ArgsPassedBitmap |= CL_ESTIMATEDDURATION_MASK;
		}
#endif

    }//end if flag
    else
    {
        if (nextCommandLineArgumentType == CL_COLORPALETTE)
		{
		    cmdLineInfo.ArgsPassedBitmap |= CL_COLORPALETTE_MASK;
			
            if ( lstrcmpi( lpszParam, TEXT( "PANGOLIN" )) == 0 )
			{
                cmdLineInfo.ColorPaletteType = PaletteType::PANGOLIN;
			}
            else if ( lstrcmpi( lpszParam, TEXT( "LASER_ILLUSIONS" )) == 0 )
			{
                cmdLineInfo.ColorPaletteType = PaletteType::LASER_ILLUSIONS;
			}
            else if ( lstrcmpi( lpszParam, TEXT( "ILDA" )) == 0 )
			{
                cmdLineInfo.ColorPaletteType = PaletteType::ILDA;
			}
            else if ( lstrcmpi( lpszParam, TEXT( "X29" )) == 0 )
			{
                cmdLineInfo.ColorPaletteType = PaletteType::X29;
			}
		}
	    else if (nextCommandLineArgumentType == CL_SCANRATE)
		{
            lstrcpy(tempString, lpszParam);
			cmdLineInfo.ScanRate = atoi(tempString);
			cmdLineInfo.ArgsPassedBitmap |= CL_SCANRATE_MASK;
		}
	    else if (nextCommandLineArgumentType == CL_BLANKINGSHIFT)
		{
            lstrcpy(tempString, lpszParam);
			cmdLineInfo.BlankingShift = atoi(tempString);
			cmdLineInfo.ArgsPassedBitmap |= CL_BLANKINGSHIFT_MASK;
		}
	    else if (nextCommandLineArgumentType == CL_FRAMEDELAY)
		{
            lstrcpy(tempString, lpszParam);
			cmdLineInfo.FrameDelay = atoi(tempString);
			cmdLineInfo.ArgsPassedBitmap |= CL_FRAMEDELAY_MASK;
		}
#ifdef MERCURY_ONLINE_TEST
	    else if (nextCommandLineArgumentType == CL_NUMBEROFLOOPS)
		{
            lstrcpy(tempString, lpszParam);
			cmdLineInfo.NumberOfLoops = atoi(tempString);
			cmdLineInfo.ArgsPassedBitmap |= CL_NUMBEROFLOOPS_MASK;
		}
#endif
	    else
		{
            lstrcpy(cmdLineInfo.InputFileName, lpszParam);
			cmdLineInfo.ArgsPassedBitmap |= CL_INPUTFILENAME_MASK;
		}

		nextCommandLineArgumentType = CL_NONE;

	}

}//end function ParseParam();