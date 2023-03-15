//
// *********************************************************
// *                                                       *
// * Copyright 1998-2005, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Device_GWS.cpp                             *
// * Description: Implementation of the Device_GWS         *
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

#ifndef DEVICE_GWS_H
#include "Device_GWS.h"
#endif

#ifndef GLOBALS_H
#include "globals.h"
#endif

using namespace std; 

extern ofstream logFile;

/*******************************************************************/
/*                                                                 */
/*    Device_GWS Constructor (void)                                    */
/*                                                                 */
/*******************************************************************/

Device_GWS::Device_GWS()
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_GWS::Device_GWS(void)" << endl;
#endif

    deviceMin = 0x0000;
    deviceMax = 0xffff;

    last_X = 0x8000;
    last_Y = 0x8000;
    
    outputDeviceType = GWS;    

    basePort    = 0x0300;
  
    return;
}

/*******************************************************************/
/*                                                                 */
/*    Device_GWS Constructor (base port)                               */
/*                                                                 */
/*******************************************************************/

Device_GWS::Device_GWS(USHORT base_port)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_GWS::Device_GWS(USHORT)" << endl;
#endif

    basePort = base_port;

    deviceMin = 0x0000;
    deviceMax = 0xffff;

    last_X = 0x8000;
    last_Y = 0x8000;
   
    basePort    = base_port;
  
    outputDeviceType = GWS;    

    return;
}

/*******************************************************************/
/*                                                                 */
/*    Device_GWS Destructor                                            */
/*                                                                 */
/*******************************************************************/

Device_GWS::~Device_GWS()
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_GWS::~Device_GWS()" << endl;
#endif

    return;
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: LaserOff                                           */
/*                                                                 */
/*******************************************************************/

void 
Device_GWS::LaserOff(const USHORT blankingInvert, const enum DisplayColorMode colorMode)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_GWS::LaserOff()" << endl;
#endif

    UCHAR blankingPortValue = 0x00;

    if (blankingInvert == M_TRUE)
    {
        blankingPortValue = 0xff;
    }

#ifndef GWS_DEBUG
    if (colorMode == COLOR_RG_INV_TTL)
    {
        _outpw((basePort+X_OFFSET_GWS), (M_UINT16)last_X);
        _outpw((basePort+Y_OFFSET_GWS), (M_UINT16)last_Y);
        _outp((basePort+R_OFFSET_GWS), 0xff);
        _outp((basePort+G_OFFSET_GWS), 0xff);
        _outp((basePort+B_OFFSET_GWS), 0xff);
        _outp((basePort+I_OFFSET_GWS), blankingPortValue);
        _outp((basePort+LOAD_OFFSET_GWS), 0x0);
    }
    else
    {
        _outpw((basePort+X_OFFSET_GWS), (M_UINT16)last_X);
        _outpw((basePort+Y_OFFSET_GWS), (M_UINT16)last_Y);
        _outp((basePort+R_OFFSET_GWS), 0x00);
        _outp((basePort+G_OFFSET_GWS), 0x00);
        _outp((basePort+B_OFFSET_GWS), 0x00);
        _outp((basePort+I_OFFSET_GWS), blankingPortValue);
        _outp((basePort+LOAD_OFFSET_GWS), 0x0);
    }
#endif

#ifdef LOG_PORT_DATA
    logFile << "Device_GWS::LaserOff() << turning off port 0x"
            << hex << basePort << endl;
#endif

} // end function LaserOff

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DisplayOff                                         */
/*                                                                 */
/*******************************************************************/

void 
Device_GWS::DisplayOff(const USHORT blankingInvert, const enum DisplayColorMode colorMode)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_GWS::DisplayOff()" << endl;
#endif

    UCHAR blankingPortValue = 0x00;

    if (blankingInvert == M_TRUE)
    {
        blankingPortValue = 0xff;
    }

#ifndef GWS_DEBUG
    if (colorMode == COLOR_RG_INV_TTL)
    {
        _outpw((basePort+X_OFFSET_GWS), 0x8000);
        _outpw((basePort+Y_OFFSET_GWS), 0x8000);
        _outp((basePort+R_OFFSET_GWS), 0xff);
        _outp((basePort+G_OFFSET_GWS), 0xff);
        _outp((basePort+B_OFFSET_GWS), 0xff);
        _outp((basePort+I_OFFSET_GWS), blankingPortValue);
        _outp((basePort+LOAD_OFFSET_GWS), 0x0);
    }
    else
    {
        _outpw((basePort+X_OFFSET_GWS), 0x8000);
        _outpw((basePort+Y_OFFSET_GWS), 0x8000);
        _outp((basePort+R_OFFSET_GWS), 0x00);
        _outp((basePort+G_OFFSET_GWS), 0x00);
        _outp((basePort+B_OFFSET_GWS), 0x00);
        _outp((basePort+I_OFFSET_GWS), blankingPortValue);
        _outp((basePort+LOAD_OFFSET_GWS), 0x0);
    }
#endif

#ifdef LOG_PORT_DATA
    logFile << "Device_GWS::DisplayOff() << turning off port 0x"
            << hex << basePort << endl;
#endif

} /* DisplayOff */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DrawLaser                                          */
/*                                                                 */
/*******************************************************************/
void Device_GWS::DrawLaser(const OutputPoint_T outPoint,
                           const M_UINT16 blankingInvert)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_GWS::DrawLaser()" << endl;
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

 #ifndef GWS_DEBUG
   _outpw((basePort+X_OFFSET_GWS), (USHORT)temp_x);
    _outpw((basePort+Y_OFFSET_GWS), (USHORT)temp_y);
    _outp((basePort+R_OFFSET_GWS), outPoint.rValue);
    _outp((basePort+G_OFFSET_GWS), outPoint.gValue);
    _outp((basePort+B_OFFSET_GWS), outPoint.bValue);
    _outp((basePort+I_OFFSET_GWS), local_i_value);
    _outp((basePort+LOAD_OFFSET_GWS), 0x0);
#endif

#ifdef LOG_PORT_DATA
    logFile << "Device_GWS::DrawLaser() to X= 0x" << hex << (USHORT)temp_x
                          << " Y= 0x" << (USHORT)temp_y
                        << "\n R= 0x" << (USHORT)outPoint.rValue
                          << " G= 0x" << (USHORT)outPoint.gValue
                          << " B= 0x" << (USHORT)outPoint.bValue
                          << " I= 0x" << (USHORT)local_i_value << endl;
#endif

} /* DrawLaser */


