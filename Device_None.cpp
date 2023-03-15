//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Device_None.cpp                            *
// * Description: Implementation of the Device_None        *
// * output device class (i.e. no output device used).     *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <afxwin.h>

#include "stdafx.h"

#ifndef DEVICE_GWS_H
#include "Device_None.h"
#endif

#ifndef GLOBALS_H
#include "globals.h"
#endif

using namespace std; 

extern ofstream logFile;

/*******************************************************************/
/*                                                                 */
/*    Device_None Constructor (void)                                    */
/*                                                                 */
/*******************************************************************/

Device_None::Device_None()
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_None::Device_None(void)" << endl;
#endif

    deviceMin = 0x0000;
    deviceMax = 0xffff;

    last_X = 0x8000;
    last_Y = 0x8000;
    
    outputDeviceType = NONE;    

    basePort    = 0x0300;
  
    return;
}

/*******************************************************************/
/*                                                                 */
/*    Device_None Destructor                                            */
/*                                                                 */
/*******************************************************************/

Device_None::~Device_None()
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_None::~Device_None()" << endl;
#endif

    return;
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: LaserOff                                           */
/*                                                                 */
/*******************************************************************/

void 
Device_None::LaserOff(const M_UINT16 blankingInvert, const enum DisplayColorMode colorMode)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_None::LaserOff()" << endl;
#endif

    return;

} // end function LaserOff

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DisplayOff                                         */
/*                                                                 */
/*******************************************************************/

void 
Device_None::DisplayOff(const M_UINT16 blankingInvert, const enum DisplayColorMode colorMode)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_None::DisplayOff()" << endl;
#endif

    return;

} /* DisplayOff */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DrawLaser                                          */
/*                                                                 */
/*******************************************************************/
void Device_None::DrawLaser(const OutputPoint_T outPoint,
			                const M_UINT16 blankingInvert)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_None::DrawLaser()" << endl;
#endif

    return;

} /* DrawLaser */


