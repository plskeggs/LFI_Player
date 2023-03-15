//
// *********************************************************
// *                                                       *
// * Copyright 1998-2005, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Device_BSOFT_RGB.cpp                       *
// * Description: Implementation of the Device_BSOFT_RGB   *
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

#ifndef DEVICE_BSOFT_RGB_H
#include "Device_BSOFT_RGB.h"
#endif

#ifndef GLOBALS_H
#include "globals.h"
#endif

using namespace std; 

extern ofstream logFile;

/*******************************************************************/
/*                                                                 */
/*    Device_BSOFT_RGB Constructor (void)                                    */
/*                                                                 */
/*******************************************************************/

Device_BSOFT_RGB::Device_BSOFT_RGB()
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_BSOFT_RGB::Device_BSOFT_RGB(void)" << endl;
#endif

    deviceMin = 0x0000;
    deviceMax = 0x0fff;

    last_X = 0x0800;
    last_Y = 0x0800;

    outputDeviceType = BSOFT_RGB;    

    basePort    = 0x0300;
  
    return;
}

/*******************************************************************/
/*                                                                 */
/*    Device_BSOFT_RGB Constructor (base port)                               */
/*                                                                 */
/*******************************************************************/

Device_BSOFT_RGB::Device_BSOFT_RGB(USHORT base_port)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_BSOFT_RGB::Device_BSOFT_RGB(USHORT)" << endl;
#endif

    basePort = base_port;

    deviceMin = 0x0000;
    deviceMax = 0x0fff;

    last_X = 0x0800;
    last_Y = 0x0800;
   
    basePort    = base_port;
  
    outputDeviceType = BSOFT_RGB;    

    return;
}

/*******************************************************************/
/*                                                                 */
/*    Device_BSOFT_RGB Destructor                                            */
/*                                                                 */
/*******************************************************************/

Device_BSOFT_RGB::~Device_BSOFT_RGB()
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_BSOFT_RGB::~Device_BSOFT_RGB()" << endl;
#endif

    return;
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: LaserOff                                           */
/*                                                                 */
/*******************************************************************/

void 
Device_BSOFT_RGB::LaserOff(const USHORT blankingInvert, const enum DisplayColorMode colorMode)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_BSOFT_RGB::LaserOff()" << endl;
#endif

    UCHAR blankingPortValue = 0x00;

	if (blankingInvert == M_TRUE)
	{
        blankingPortValue = 0xff;
	}

	if (colorMode == COLOR_RG_INV_TTL)
	{
        _outpw((basePort+X_OFFSET_BRGB), (M_UINT16)last_X);
        _outpw((basePort+Y_OFFSET_BRGB), (M_UINT16)last_Y);
        _outp((basePort+R_OFFSET_BRGB), 0xff);
        _outp((basePort+G_OFFSET_BRGB), 0xff);
        _outp((basePort+B_OFFSET_BRGB), 0xff);
        _outp((basePort+I_OFFSET_BRGB), blankingPortValue);
	}
    else
	{
        _outpw((basePort+X_OFFSET_BRGB), (M_UINT16)last_X);
        _outpw((basePort+Y_OFFSET_BRGB), (M_UINT16)last_Y);
        _outp((basePort+R_OFFSET_BRGB), 0x00);
        _outp((basePort+G_OFFSET_BRGB), 0x00);
        _outp((basePort+B_OFFSET_BRGB), 0x00);
        _outp((basePort+I_OFFSET_BRGB), blankingPortValue);
	}


#ifdef LOG_PORT_DATA
    logFile << "Device_BSOFT_RGB::LaserOff() << turning off port 0x"
		    << hex << basePort << endl;
#endif

} // end function LaserOff

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DisplayOff                                         */
/*                                                                 */
/*******************************************************************/

void 
Device_BSOFT_RGB::DisplayOff(const USHORT blankingInvert, const enum DisplayColorMode colorMode)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_BSOFT_RGB::DisplayOff()" << endl;
#endif

    UCHAR blankingPortValue = 0x00;

	if (blankingInvert == M_TRUE)
	{
        blankingPortValue = 0xff;
	}

    if (colorMode == COLOR_RG_INV_TTL)
	{
        _outpw((basePort+X_OFFSET_BRGB), 0x0800);
        _outpw((basePort+Y_OFFSET_BRGB), 0x0800);
        _outp((basePort+R_OFFSET_BRGB), 0xff);
        _outp((basePort+G_OFFSET_BRGB), 0xff);
        _outp((basePort+B_OFFSET_BRGB), 0xff);
        _outp((basePort+I_OFFSET_BRGB), blankingPortValue);
	}
	else
	{
        _outpw((basePort+X_OFFSET_BRGB), 0x0800);
        _outpw((basePort+Y_OFFSET_BRGB), 0x0800);
        _outp((basePort+R_OFFSET_BRGB), 0x00);
        _outp((basePort+G_OFFSET_BRGB), 0x00);
        _outp((basePort+B_OFFSET_BRGB), 0x00);
        _outp((basePort+I_OFFSET_BRGB), blankingPortValue);
	}

#ifdef LOG_PORT_DATA
    logFile << "Device_BSOFT_RGB::DisplayOff() << turning off port 0x"
		    << hex << basePort << endl;
#endif

} /* DisplayOff */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DrawLaser                                          */
/*                                                                 */
/*******************************************************************/

void Device_BSOFT_RGB::DrawLaser(const OutputPoint_T outPoint,
			                     const M_UINT16 blankingInvert)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_BSOFT_RGB::DrawLaser()" << endl;
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

    _outpw((basePort+X_OFFSET_BRGB), (USHORT)temp_x);
    _outpw((basePort+Y_OFFSET_BRGB), (USHORT)temp_y);
    _outp((basePort+R_OFFSET_BRGB), outPoint.rValue);
    _outp((basePort+G_OFFSET_BRGB), outPoint.gValue);
    _outp((basePort+B_OFFSET_BRGB), outPoint.bValue);
    _outp((basePort+I_OFFSET_BRGB), local_i_value);

#ifdef LOG_PORT_DATA
    logFile << "Device_BSOFT_RGB::DrawLaser() to X= 0x" << hex << (USHORT)temp_x
                          << " Y= 0x" << (USHORT)temp_y
                        << "\n R= 0x" << (USHORT)outPoint.rValue
                          << " G= 0x" << (USHORT)outPoint.gValue
                          << " B= 0x" << (USHORT)outPoint.bValue
                          << " I= 0x" << (USHORT)local_i_value << endl;
#endif

} /* DrawLaser */


