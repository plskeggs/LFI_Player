//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Device_EasyLase.cpp                        *
// * Description: Implementation of the Device_EasyLase    *
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

#include "stdafx.h"

#ifndef DEVICE_EASYLASE_H
#include "Device_EasyLase.h"
#endif

#ifndef GLOBALS_H
#include "globals.h"
#endif

// #define LOG_EASYLASE_PORT_DATA_BUFFER_IN
// #define LOG_EASYLASE_PORT_DATA_BUFFER_OUT

using namespace std; 

extern ofstream logFile;

/*******************************************************************/
/*                                                                 */
/*    Device_EASYLASE Constructor (void)                           */
/*                                                                 */
/*******************************************************************/

Device_EASYLASE::Device_EASYLASE():
    Gem_PC(),
    EL_OkayFlag(M_FALSE),
    EL_DeviceNumber(0),
    EL_BufferPtr(&EL_PointBuffer[0])
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EASYLASE::Device_EASYLASE(void)" << endl;
#endif

    deviceMin = 0x0000;
    deviceMax = 0x0fff;

    last_X = 0x0000;
    last_Y = 0x0000;
   
    EL_PPS = 24000;
    basePort    = 0x0000;
	EL_CardNumber = 0;
	EL_BufferIndex = 0;
   
    outputDeviceType = EASYLASE;    

    return;
}

/*******************************************************************/
/*                                                                 */
/*    Device_EASYLASE Constructor (device number)                   */
/*                                                                 */
/*******************************************************************/

Device_EASYLASE::Device_EASYLASE(int devNum, OutputDeviceTypeT deviceType):
    Gem_PC(),
    EL_OkayFlag(M_FALSE),
    EL_BufferPtr(&EL_PointBuffer[0])
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EASYLASE::Device_EASYLASE(int, OutputDeviceTypeT)" << endl;
#endif

    EL_CardNumber = devNum-1; //0 indexed devices in easylase.dll

    deviceMin = 0x0000;
    deviceMax = 0x0fff;

    last_X = 0x0000;
    last_Y = 0x0000;
   
    EL_PPS = 24000;
    basePort    = 0x0000;
	EL_BufferIndex = 0;
  
    outputDeviceType = EASYLASE; 

    return;
}

/*******************************************************************/
/*                                                                 */
/*    Device_EASYLASE Destructor                                   */
/*                                                                 */
/*******************************************************************/

Device_EASYLASE::~Device_EASYLASE()
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EASYLASE::~Device_EASYLASE()" << endl;
#endif

// Note that the #ifdef EASYLASE_OUTPUT compiler directives are scattered
// scattered throughout this file so that the compiler doesn't barf if the
// EasyLase.dll isn't around.
#ifdef EASYLASE_OUTPUT
    __EasyLaseStop(EL_CardNumber);
    __EasyLaseClose();
#endif

    return;
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: EasyLaseInit                                       */
/*                                                                 */
/*******************************************************************/

M_UINT16
Device_EASYLASE::EasyLaseInit(void)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EASYLASE::EasyLaseInit()" << endl;
#endif

    // load the EasyLase DLL
	HINSTANCE hLibrary;
	hLibrary=LoadLibrary("EasyLase.dll");

    if (hLibrary == NULL)
	{
      logFile << "Device_EASYLASE::EasyLaseInit() - Can't load library EasyLase.dll!" << endl;
   	  return M_FAILURE;
	}
	
	// Retrieve a pointer to each of the EasyLase library functions,
	// and check to make sure that the pointer is valid...

    __EasyLaseGetCardNum =(easylaseFuncPtr1)GetProcAddress(hLibrary,"EasyLaseGetCardNum");
    if (!__EasyLaseGetCardNum)
    {
      logFile << "Device_EASYLASE::EasyLaseInit() - Can't load library routine EasyLaseGetCardNum!" << endl;
   	  return M_FAILURE;
    }

    __EasyLaseWriteFrame =(easylaseFuncPtr2)GetProcAddress(hLibrary,"EasyLaseWriteFrame");
    if (!__EasyLaseWriteFrame)
    {
      logFile << "Device_EASYLASE::EasyLaseInit() - Can't load library routine EasyLaseWriteFrame!" << endl;
   	  return M_FAILURE;
    }

    __EasyLaseWriteFrameNR =(easylaseFuncPtr3)GetProcAddress(hLibrary,"EasyLaseWriteFrameNR");
    if (!__EasyLaseWriteFrameNR)
    {
      logFile << "Device_EASYLASE::EasyLaseInit() - Can't load library routine EasyLaseWriteFrameNR!" << endl;
   	  return M_FAILURE;
    }

    __EasyLaseGetStatus =(easylaseFuncPtr4)GetProcAddress(hLibrary,"EasyLaseGetStatus");
    if (!__EasyLaseGetStatus)
    {
      logFile << "Device_EASYLASE::EasyLaseInit() - Can't load library routine EasyLaseGetStatus!" << endl;
   	  return M_FAILURE;
    }

    __EasyLaseStop =(easylaseFuncPtr5)GetProcAddress(hLibrary,"EasyLaseStop");
    if (!__EasyLaseStop)
    {
      logFile << "Device_EASYLASE::EasyLaseInit() - Can't load library routine EasyLaseStop!" << endl;
   	  return M_FAILURE;
    }

    __EasyLaseClose =(easylaseFuncPtr6)GetProcAddress(hLibrary,"EasyLaseClose");
    if (!__EasyLaseClose)
    {
      logFile << "Device_EASYLASE::EasyLaseInit() - Can't load library routine __EasyLaseClose!" << endl;
   	  return M_FAILURE;
    }

	EL_DeviceNumber = __EasyLaseGetCardNum();
	logFile << "Device_EASYLASE::EasyLaseInit() - Found " << EL_DeviceNumber << " devices." << endl;

	if (EL_DeviceNumber > 0){
	EL_OkayFlag = M_TRUE;
	return M_SUCCESS;
	}
	EL_OkayFlag = M_FALSE;
	return M_FAILURE;

}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: LaserOff                                           */
/*                                                                 */
/*******************************************************************/

void 
Device_EASYLASE::LaserOff(const M_UINT16 blankingInvert, const enum DisplayColorMode colorMode)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EASYLASE::LaserOff()" << endl;
#endif

#ifdef EASYLASE_OUTPUT
    M_UINT8 intensitySignalValue = 0x00;

	if (blankingInvert == M_TRUE)
	{
        intensitySignalValue = 0xff;
	}

	if (colorMode == DisplayColorMode::COLOR_RG_INV_TTL)
	{
        EasyLaseAddPoint((M_INT16)last_X,
                         (M_INT16)last_Y,
                         0xff,
                         0xff,
                         0xff,
                         intensitySignalValue);
	}
	else
	{
        EasyLaseAddPoint((M_INT16)last_X,
                         (M_INT16)last_Y,
                         0x00,
                         0x00,
                         0x00,
                         intensitySignalValue);
	}


//  QUESTION: WHY WOULD YOU *NOT* CALL WRITE HERE???
WriteCurrentDeviceBuffer();

#ifdef LOG_PORT_DATA
    logFile << "Device_EASYLASE::LaserOff() << turning off laser for card "
		    << EL_CardNumber << endl;
#endif
#endif

} // end function LaserOff

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DisplayOff                                         */
/*                                                                 */
/*******************************************************************/

void 
Device_EASYLASE::DisplayOff(const M_UINT16 blankingInvert, const enum DisplayColorMode colorMode)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EASYLASE::DisplayOff()" << endl;
#endif

 #ifdef EASYLASE_OUTPUT
   M_INT16 intensitySignalValue = 0x00;

	if (blankingInvert == M_TRUE)
	{
        intensitySignalValue = 0xff;
	}

    for (int i=0 ; i<1 ; ++i)
    {
	    for (int j=0; j<EASYLASE_BUFFER_SIZE; j++)
	    { // clear data buffers
  		    EL_PointBuffer[j].X=2048;
		    EL_PointBuffer[j].Y=2048;

		    if (colorMode == DisplayColorMode::COLOR_RG_INV_TTL)
			{
		        EL_PointBuffer[j].R=0xff;
		        EL_PointBuffer[j].G=0xff;
		        EL_PointBuffer[j].B=0xff;
			}
			else
			{
		        EL_PointBuffer[j].R=0x00;
		        EL_PointBuffer[j].G=0x00;
		        EL_PointBuffer[j].B=0x00;
			}


		    EL_PointBuffer[j].I=intensitySignalValue;

	    }

        // EL_BufferIndex = EASYLASE_BUFFER_SIZE;    
        EL_BufferIndex = 1;    
        WriteCurrentDeviceBuffer();
    }

#ifdef LOG_PORT_DATA
    logFile << "Device_EASYLASE::DisplayOff() << turning off device number "
		    << EL_DeviceNumber << endl;
#endif
#endif

} /* DisplayOff */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DrawLaser                                          */
/*                                                                 */
/*******************************************************************/
void Device_EASYLASE::DrawLaser(const OutputPoint_T outPoint,
			                    const M_UINT16 blankingInvert)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EASYLASE::DrawLaser()" << endl;
#endif
 #ifdef EASYLASE_OUTPUT
   long   temp_x, temp_y;

	M_UINT8 local_i_value = outPoint.iValue;

	if (blankingInvert == M_TRUE)
	{
         local_i_value = ~local_i_value;
	}

    MapNDCToDevice(outPoint.xValue, outPoint.yValue, &temp_x, &temp_y);

    last_X = temp_x;
    last_Y = temp_y;

    EasyLaseAddPoint((M_UINT16)temp_x,
                     (M_UINT16)temp_y,
                     outPoint.rValue,
                     outPoint.gValue,
                     outPoint.bValue,
                     local_i_value);

#ifdef LOG_PORT_DATA
    logFile << "Device_EASYLASE::DrawLaser() to X= 0x" << hex << (M_UINT16)temp_x
                          << " Y= 0x" << (M_UINT16)temp_y
                        << "\n R= 0x" << (M_UINT16)outPoint.rValue
                          << " G= 0x" << (M_UINT16)outPoint.gValue
                          << " B= 0x" << (M_UINT16)outPoint.bValue
                          << " I= 0x" << (M_UINT16)local_i_value << endl;
#endif
#endif

} /* DrawLaser */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: EasyLaseAddPoint                                   */
/*                                                                 */
/*******************************************************************/
void
Device_EASYLASE::EasyLaseAddPoint(const M_UINT16 xVal,
                                  const M_UINT16 yVal,
                                  const M_UINT8 rVal,
                                  const M_UINT8 gVal,
                                  const M_UINT8 bVal,
                                  const M_UINT8 iVal)
{


#ifdef EASYLASE_OUTPUT
    if (EL_OkayFlag == M_TRUE &&
        EL_BufferIndex < EASYLASE_BUFFER_SIZE)
    {
        EL_BufferPtr[EL_BufferIndex].X = xVal;
        EL_BufferPtr[EL_BufferIndex].Y = yVal;
        EL_BufferPtr[EL_BufferIndex].R = rVal;
        EL_BufferPtr[EL_BufferIndex].G = gVal;
        EL_BufferPtr[EL_BufferIndex].B = bVal;
        EL_BufferPtr[EL_BufferIndex].I = iVal;

#ifdef LOG_EASYLASE_PORT_DATA_BUFFER_IN
        logFile << "Add Point - "
		        << "Index: " << EL_BufferIndex
			    << ", X: " << EL_BufferPtr[EL_BufferIndex].X
			    << ", Y: " << EL_BufferPtr[EL_BufferIndex].Y
				<< ", I: " << EL_BufferPtr[EL_BufferIndex].I
				<< ", R: " << EL_BufferPtr[EL_BufferIndex].R
				<< ", G: " << EL_BufferPtr[EL_BufferIndex].G
				<< ", B: " << EL_BufferPtr[EL_BufferIndex].B << endl;
#endif

        ++EL_BufferIndex;

    }
#endif
} /* EasyLaseAddPoint */

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: WriteCurrentDeviceBuffer                           */
/*                                                                 */
/*******************************************************************/
void
Device_EASYLASE::WriteCurrentDeviceBuffer(void)
{
#ifdef EASYLASE_OUTPUT
	if (EL_OkayFlag == M_FALSE ||
		EL_BufferIndex == 0)
    {
        return;
    }

    bool retVal;
	M_UINT16 numSleeps = 0;

	while (__EasyLaseGetStatus(EL_CardNumber) != 1)
	{
		// logFile << "DAC not ready [" << ++numSleeps << " , wait 10 ms" << endl;
        Sleep(5);
	}

#ifdef LOG_EASYLASE_PORT_DATA_BUFFER_OUT
	logFile << "WriteCurrentDeviceBuffer called with " << EL_BufferIndex << " points." << endl;
	for (int i=0; i<EL_BufferIndex; ++i)
	{
        logFile << "Write Buffer - "
			    << "Index: " << i
                << ", X: " << EL_BufferPtr[i].X
			    << ", Y: " << EL_BufferPtr[i].Y
				<< ", I: " << EL_BufferPtr[i].I
				<< ", R: " << EL_BufferPtr[i].R
				<< ", G: " << EL_BufferPtr[i].G
				<< ", B: " << EL_BufferPtr[i].B << endl;
	}
#endif

    // retVal = __EasyLaseWriteFrameNR(EL_CardNumber, EL_BufferPtr, EL_BufferIndex*sizeof(EL_Pnt_s), EL_PPS, 1);
    retVal = __EasyLaseWriteFrame(EL_CardNumber, EL_BufferPtr, EL_BufferIndex*sizeof(EL_Pnt_s), EL_PPS);

    if (retVal != TRUE)
    {
        logFile << "Device_EASYLASE::WriteCurrentDeviceBuffer() - function EasyLaseWriteFrameNR() returned false!" << endl;
        EL_OkayFlag = M_FALSE;

	}

    // logFile << "Device_EASYLASE::WriteCurrentDeviceBuffer() - Writing " << EL_BufferIndex+1 << " points." << endl;
        
    EL_BufferIndex = 0;

#endif
}
	
