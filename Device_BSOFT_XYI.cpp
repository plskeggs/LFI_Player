//
// *********************************************************
// *                                                       *
// * Copyright 1998-2005, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Device_BSOFT_XYI.cpp                       *
// * Description: Implementation of the Device_BSOFT_XYI   *
// * output device class.                                  *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#include <conio.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <afxwin.h>

#ifndef DEVICE_BSOFT_XYI_H
#include "Device_BSOFT_XYI.h"
#endif

#ifndef GLOBALS_H
#include "globals.h"
#endif

using namespace std; 

extern ofstream logFile;

/*******************************************************************/
/*                                                                 */
/*    Device_BSOFT_XYI Constructor (void)                                    */
/*                                                                 */
/*******************************************************************/

Device_BSOFT_XYI::Device_BSOFT_XYI()
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_BSOFT_XYI::Device_BSOFT_XYI(void)" << endl;
#endif

    deviceMin = 0x0000;
    deviceMax = 0x0fff;

    last_X = 0x0800;
    last_Y = 0x0800;

    outputDeviceType = BSOFT_XYI;    

    basePort    = 0x0300;
  
    return;
}

/*******************************************************************/
/*                                                                 */
/*    Device_BSOFT_XYI Constructor (base port)                               */
/*                                                                 */
/*******************************************************************/

Device_BSOFT_XYI::Device_BSOFT_XYI(USHORT base_port)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_BSOFT_XYI::Device_BSOFT_XYI(USHORT)" << endl;
#endif

    basePort = base_port;

    deviceMin = 0x0000;
    deviceMax = 0x0fff;

    last_X = 0x0800;
    last_Y = 0x0800;
   
    basePort    = base_port;
  
    outputDeviceType = BSOFT_XYI;    

    return;
}

/*******************************************************************/
/*                                                                 */
/*    Device_BSOFT_XYI Destructor                                            */
/*                                                                 */
/*******************************************************************/

Device_BSOFT_XYI::~Device_BSOFT_XYI()
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_BSOFT_XYI::~Device_BSOFT_XYI()" << endl;
#endif

    return;
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: LaserOff                                           */
/*                                                                 */
/*******************************************************************/

void 
Device_BSOFT_XYI::LaserOff(const USHORT blankingInvert, const enum DisplayColorMode colorMode)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_BSOFT_XYI::LaserOff()" << endl;
#endif

    UCHAR blankingPortValue = 0x00;

	if (blankingInvert == M_TRUE)
	{
        blankingPortValue = 0xff;
	}

    _outpw((basePort+X_OFFSET_BXYI), (M_UINT16)last_X);
    _outpw((basePort+Y_OFFSET_BXYI), (M_UINT16)last_Y);
    _outp((basePort+CTRL_OFFSET_BXYI), 0x80);
    _outp((basePort+I_OFFSET_BXYI), blankingPortValue);

#ifdef LOG_PORT_DATA
    logFile << "Device_BSOFT_XYI::LaserOff() << turning off port 0x"
		    << hex << basePort << endl;
#endif

} // end function LaserOff

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DisplayOff                                         */
/*                                                                 */
/*******************************************************************/

void 
Device_BSOFT_XYI::DisplayOff(const USHORT blankingInvert, const enum DisplayColorMode colorMode)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_BSOFT_XYI::DisplayOff()" << endl;
#endif

    UCHAR blankingPortValue = 0x00;

	if (blankingInvert == M_TRUE)
	{
        blankingPortValue = 0xff;
	}

    _outpw((basePort+X_OFFSET_BXYI), 0x0800);
    _outpw((basePort+Y_OFFSET_BXYI), 0x0800);
    _outp((basePort+CTRL_OFFSET_BXYI), 0x80);
    _outp((basePort+I_OFFSET_BXYI), blankingPortValue);

#ifdef LOG_PORT_DATA
    logFile << "Device_BSOFT_XYI::DisplayOff() << turning off port 0x"
		    << hex << basePort << endl;
#endif

} /* DisplayOff */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DrawLaser                                          */
/*                                                                 */
/*******************************************************************/
void Device_BSOFT_XYI::DrawLaser(const OutputPoint_T outPoint,
			                     const M_UINT16 blankingInvert)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_BSOFT_XYI::DrawLaser()" << endl;
#endif
    long   temp_x, temp_y;

	UCHAR local_i_value = outPoint.iValue;

	if (blankingInvert == M_TRUE)
	{
         local_i_value = ~local_i_value;
	}

    MapNDCToDevice(outPoint.xValue, outPoint.yValue, &temp_x, &temp_y);

    last_X = temp_x;
    last_Y = temp_y;

    _outpw((basePort+X_OFFSET_BXYI), (USHORT)temp_x);
    _outpw((basePort+Y_OFFSET_BXYI), (USHORT)temp_y);
    _outp((basePort+CTRL_OFFSET_BXYI), 0x80);
    _outp((basePort+I_OFFSET_BXYI), (local_i_value & 0x80));

#ifdef LOG_PORT_DATA
    logFile << "Device_BSOFT_XYI::DrawLaser() to X= 0x" << hex << (USHORT)temp_x
                          << " Y= 0x" << (USHORT)temp_y
                        << "\n R= 0x" << (USHORT)outPoint.rValue
                          << " G= 0x" << (USHORT)outPoint.gValue
                          << " B= 0x" << (USHORT)outPoint.bValue
                          << " I= 0x" << (USHORT)local_i_value << endl;
#endif

} /* DrawLaser */


