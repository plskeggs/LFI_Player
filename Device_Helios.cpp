//
// *********************************************************
// *                                                       *
// * Copyright 1998-2021, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Device_Helios.cpp                          *
// * Description: Implementation of the Device_HELIOS      *
// * class supporting the Helios Laser DAC.                *
// *              https://bitlasers.com/helios-laser-dac   *
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

#ifndef DEVICE_HELIOS_H
#include "Device_Helios.h"
#endif

#ifndef GLOBALS_H
#include "globals.h"
#endif

// #define LOG_EZAUDDAC_PORT_DATA_BUFFER_IN
// #define LOG_EZAUDDAC_PORT_DATA_BUFFER_OUT

using namespace std; 

extern ofstream logFile;

/*******************************************************************/
/*                                                                 */
/*    Device_EZAUDDAC Constructor (void)                           */
/*                                                                 */
/*******************************************************************/

Device_EZAUDDAC::Device_EZAUDDAC():
    Gem_PC(),
    EAD_OkayFlag(M_FALSE),
    EAD_DeviceNumber(0),
    EAD_BufferPtr(&EAD_PointBuffer[0])
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EZAUDDAC::Device_EZAUDDAC(void)" << endl;
#endif

    deviceMin = MIN_INT_16_VALUE;
    deviceMax = MAX_INT_16_VALUE;

    last_X = 0x0000;
    last_Y = 0x0000;
   
    EAD_PPS = 24000;
    basePort    = 0x0000;
    EAD_CardNumber = 1;
    EAD_BufferIndex = 0;
   
    outputDeviceType = EZAUDDAC;    

    return;
}

/*******************************************************************/
/*                                                                 */
/*    Device_EZAUDDAC Constructor (device number)                   */
/*                                                                 */
/*******************************************************************/

Device_EZAUDDAC::Device_EZAUDDAC(int devNum, OutputDeviceTypeT deviceType):
    Gem_PC(),
    EAD_OkayFlag(M_FALSE),
    EAD_BufferPtr(&EAD_PointBuffer[0])
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EZAUDDAC::Device_EZAUDDAC(int, OutputDeviceTypeT)" << endl;
#endif

    EAD_CardNumber = devNum;

    deviceMin = MIN_INT_16_VALUE;
    deviceMax = MAX_INT_16_VALUE;

    last_X = 0x0000;
    last_Y = 0x0000;
   
    EAD_PPS = 24000;
    basePort    = 0x0000;
    EAD_BufferIndex = 0;
  
    outputDeviceType = EZAUDDAC; 

    return;
}

/*******************************************************************/
/*                                                                 */
/*    Device_EZAUDDAC Destructor                                   */
/*                                                                 */
/*******************************************************************/

Device_EZAUDDAC::~Device_EZAUDDAC()
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EZAUDDAC::~Device_EZAUDDAC()" << endl;
#endif

// Note that the #ifdef EZAUDDAC_OUTPUT compiler directives are scattered
// scattered throughout this file so that the compiler doesn't barf if the
// EzAudDac.dll isn't around.
#ifdef EZAUDDAC_OUTPUT
    __EzAudDacStop(EAD_CardNumber);
    __EzAudDacClose();
#endif

    return;
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: EzAudDacInit                                       */
/*                                                                 */
/*******************************************************************/

M_UINT16
Device_EZAUDDAC::EzAudDacInit(void)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EZAUDDAC::EzAudDacInit()" << endl;
#endif

    // load the EzAudDac DLL
    HINSTANCE hLibrary;
    hLibrary=LoadLibrary("EzAudDac.dll");

    if (hLibrary == NULL)
    {
      logFile << "Device_EZAUDDAC::EzAudDacInit() - Can't load library EzAudDac.dll!" << endl;
      return M_FAILURE;
    }
    
    // Retrieve a pointer to each of the EzAudDac library functions,
    // and check to make sure that the pointer is valid...

    __EzAudDacGetCardNum =(ezAudDacFuncPtr1)GetProcAddress(hLibrary,"EzAudDacGetCardNum");
    if (!__EzAudDacGetCardNum)
    {
      logFile << "Device_EZAUDDAC::EzAudDacInit() - Can't load library routine EzAudDacGetCardNum!" << endl;
      return M_FAILURE;
    }

    __EzAudDacWriteFrame =(ezAudDacFuncPtr2)GetProcAddress(hLibrary,"EzAudDacWriteFrame");
    if (!__EzAudDacWriteFrame)
    {
      logFile << "Device_EZAUDDAC::EzAudDacInit() - Can't load library routine EzAudDacWriteFrame!" << endl;
      return M_FAILURE;
    }

    __EzAudDacWriteFrameNR =(ezAudDacFuncPtr3)GetProcAddress(hLibrary,"EzAudDacWriteFrameNR");
    if (!__EzAudDacWriteFrameNR)
    {
      logFile << "Device_EZAUDDAC::EzAudDacInit() - Can't load library routine EzAudDacWriteFrameNR!" << endl;
      return M_FAILURE;
    }

    __EzAudDacGetStatus =(ezAudDacFuncPtr4)GetProcAddress(hLibrary,"EzAudDacGetStatus");
    if (!__EzAudDacGetStatus)
    {
      logFile << "Device_EZAUDDAC::EzAudDacInit() - Can't load library routine EzAudDacGetStatus!" << endl;
      return M_FAILURE;
    }

    __EzAudDacStop =(ezAudDacFuncPtr5)GetProcAddress(hLibrary,"EzAudDacStop");
    if (!__EzAudDacStop)
    {
      logFile << "Device_EZAUDDAC::EzAudDacInit() - Can't load library routine EzAudDacStop!" << endl;
      return M_FAILURE;
    }

    __EzAudDacClose =(ezAudDacFuncPtr6)GetProcAddress(hLibrary,"EzAudDacClose");
    if (!__EzAudDacClose)
    {
      logFile << "Device_EZAUDDAC::EzAudDacInit() - Can't load library routine __EzAudDacClose!" << endl;
      return M_FAILURE;
    }

    EAD_DeviceNumber = __EzAudDacGetCardNum();
    logFile << "Device_EZAUDDAC::EzAudDacInit() - Found " << EAD_DeviceNumber << " audio devices." << endl;

    if (EAD_DeviceNumber > 0){
    EAD_OkayFlag = M_TRUE;
    return M_SUCCESS;
    }
    EAD_OkayFlag = M_FALSE;
    return M_FAILURE;
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: LaserOff                                           */
/*                                                                 */
/*******************************************************************/

void 
Device_EZAUDDAC::LaserOff(const USHORT blankingInvert, const enum DisplayColorMode colorMode)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EZAUDDAC::LaserOff()" << endl;
#endif

#ifdef EZAUDDAC_OUTPUT
    UCHAR blankingPortValue = 0x00;

    if (blankingInvert == M_TRUE)
    {
        blankingPortValue = 0xff;
    }

    if (colorMode == COLOR_RG_INV_TTL)
    {
        EzAudDacAddPoint((M_INT16)last_X,
                         (M_INT16)last_Y,
                         0xff,
                         0xff,
                         0xff,
                         blankingPortValue);
    }
    else
    {
        EzAudDacAddPoint((M_INT16)last_X,
                         (M_INT16)last_Y,
                         0x00,
                         0x00,
                         0x00,
                         blankingPortValue);
    }


//  QUESTION: WHY WOULD YOU *NOT* CALL WRITE HERE???
WriteCurrentDeviceBuffer();

#ifdef LOG_PORT_DATA
    logFile << "Device_EZAUDDAC::LaserOff() << turning off laser for card "
            << EAD_CardNumber << endl;
#endif
#endif

} // end function LaserOff

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DisplayOff                                         */
/*                                                                 */
/*******************************************************************/

void 
Device_EZAUDDAC::DisplayOff(const USHORT blankingInvert, const enum DisplayColorMode colorMode)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EZAUDDAC::DisplayOff()" << endl;
#endif

 #ifdef EZAUDDAC_OUTPUT
   M_INT16 blankingPortValue = 0;

    if (blankingInvert == M_TRUE)
    {
        blankingPortValue = MAX_INT_16_VALUE;
    }

    for (int i=0 ; i<1 ; ++i)
    {
        for (int j=0; j<EZAUDDAC_BUFFER_SIZE; j++)
        { // clear data buffers
            EAD_PointBuffer[j].X=0;
            EAD_PointBuffer[j].Y=0;

            if (colorMode == COLOR_RG_INV_TTL)
            {
                EAD_PointBuffer[j].R=MAX_INT_16_VALUE;
                EAD_PointBuffer[j].G=MAX_INT_16_VALUE;
                EAD_PointBuffer[j].B=MAX_INT_16_VALUE;
            }
            else
            {
                EAD_PointBuffer[j].R=0;
                EAD_PointBuffer[j].G=0;
                EAD_PointBuffer[j].B=0;
            }


            EAD_PointBuffer[j].I=blankingPortValue;
            EAD_PointBuffer[j].AL=0;
            EAD_PointBuffer[j].AR=0;

        }

        // EAD_BufferIndex = EZAUDDAC_BUFFER_SIZE;    
        EAD_BufferIndex = 1;    
        WriteCurrentDeviceBuffer();
    }

#ifdef LOG_PORT_DATA
    logFile << "Device_EZAUDDAC::DisplayOff() << turning off device number "
            << EAD_DeviceNumber << endl;
#endif
#endif

} /* DisplayOff */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DrawLaser                                          */
/*                                                                 */
/*******************************************************************/
void Device_EZAUDDAC::DrawLaser(const OutputPoint_T outPoint,
                            const M_UINT16 blankingInvert)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EZAUDDAC::DrawLaser()" << endl;
#endif
 #ifdef EZAUDDAC_OUTPUT
   long   temp_x, temp_y;

    UCHAR local_i_value = outPoint.iValue;

    if (blankingInvert == M_TRUE)
    {
         local_i_value = ~local_i_value;
    }

    MapNDCToDevice(outPoint.xValue, outPoint.yValue, &temp_x, &temp_y);

    last_X = temp_x;
    last_Y = temp_y;

    EzAudDacAddPoint((M_INT16)temp_x,
                     (M_INT16)temp_y,
                     outPoint.rValue,
                     outPoint.gValue,
                     outPoint.bValue,
                     local_i_value);

#ifdef LOG_PORT_DATA
    logFile << "Device_EZAUDDAC::DrawLaser() to X= 0x" << hex << (USHORT)temp_x
                          << " Y= 0x" << (USHORT)temp_y
                          << "\n R= 0x" << (USHORT)outPoint.rValue
                          << " G= 0x" << (USHORT)outPoint.gValue
                          << " B= 0x" << (USHORT)outPoint.bValue
                          << " I= 0x" << (USHORT)local_i_value << endl;
#endif
#endif

} /* DrawLaser */

M_UINT16
Device_EZAUDDAC::ConvertByteToShort(M_UINT8 byteVal)
{
//             new max - new min
// new value = ================= * (old value - old min)
//             old max - old min
    double oldDelta = 255.0;
    double newDelta = 32766.0;

    return (M_UINT16)((newDelta/oldDelta)*(double)byteVal);
}
/*******************************************************************/
/*                                                                 */
/*    FUNCTION: EzAudDacAddPoint                                   */
/*                                                                 */
/*******************************************************************/
void
Device_EZAUDDAC::EzAudDacAddPoint(const M_INT16 xVal,
                                  const M_INT16 yVal,
                                  const M_UINT8 rVal,
                                  const M_UINT8 gVal,
                                  const M_UINT8 bVal,
                                  const M_UINT8 iVal)
{


#ifdef EZAUDDAC_OUTPUT
    if (EAD_OkayFlag == M_TRUE &&
        EAD_BufferIndex < EZAUDDAC_BUFFER_SIZE)
    {
        EAD_BufferPtr[EAD_BufferIndex].X = xVal;
        EAD_BufferPtr[EAD_BufferIndex].Y = yVal;
        EAD_BufferPtr[EAD_BufferIndex].R = ConvertByteToShort(rVal);
        EAD_BufferPtr[EAD_BufferIndex].G = ConvertByteToShort(gVal);
        EAD_BufferPtr[EAD_BufferIndex].B = ConvertByteToShort(bVal);
        EAD_BufferPtr[EAD_BufferIndex].I = ConvertByteToShort(iVal);
        EAD_BufferPtr[EAD_BufferIndex].AL = 0;
        EAD_BufferPtr[EAD_BufferIndex].AR = 0;

#ifdef LOG_EZAUDDAC_PORT_DATA_BUFFER_IN
        logFile << "Add Point - "
                << "Index: " << EAD_BufferIndex
                << ", X: " << EAD_BufferPtr[EAD_BufferIndex].X
                << ", Y: " << EAD_BufferPtr[EAD_BufferIndex].Y
                << ", I: " << EAD_BufferPtr[EAD_BufferIndex].I
                << ", R: " << EAD_BufferPtr[EAD_BufferIndex].R
                << ", G: " << EAD_BufferPtr[EAD_BufferIndex].G
                << ", B: " << EAD_BufferPtr[EAD_BufferIndex].B << endl;
#endif

        ++EAD_BufferIndex;

    }
#endif
} /* EzAudDacAddPoint */

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: WriteCurrentDeviceBuffer                           */
/*                                                                 */
/*******************************************************************/
void
Device_EZAUDDAC::WriteCurrentDeviceBuffer(void)
{
#ifdef EZAUDDAC_OUTPUT
    if (EAD_OkayFlag == M_FALSE ||
        EAD_BufferIndex == 0)
    {
        return;
    }

    bool retVal;
    M_UINT16 numSleeps = 0;

    while (__EzAudDacGetStatus(EAD_DeviceNumber) != 1)
    {
        // logFile << "DAC not ready [" << ++numSleeps << " , wait 10 ms" << endl;
        Sleep(5);
    }

#ifdef LOG_EZAUDDAC_PORT_DATA_BUFFER_OUT
    logFile << "WriteCurrentDeviceBuffer called with " << EAD_BufferIndex << " points." << endl;
    for (int i=0; i<EAD_BufferIndex; ++i)
    {
        logFile << "Write Buffer - "
                << "Index: " << i
                << ", X: " << EAD_BufferPtr[i].X
                << ", Y: " << EAD_BufferPtr[i].Y
                << ", I: " << EAD_BufferPtr[i].I
                << ", R: " << EAD_BufferPtr[i].R
                << ", G: " << EAD_BufferPtr[i].G
                << ", B: " << EAD_BufferPtr[i].B << endl;
    }
#endif

    // retVal = __EzAudDacWriteFrameNR(EAD_CardNumber, EAD_BufferPtr, EAD_BufferIndex*sizeof(EAD_Pnt_s), EAD_PPS, 1);
    retVal = __EzAudDacWriteFrame(EAD_CardNumber, EAD_BufferPtr, EAD_BufferIndex*sizeof(EAD_Pnt_s), EAD_PPS);

    if (retVal != TRUE)
    {
        logFile << "Device_EZAUDDAC::WriteCurrentDeviceBuffer() - function EzAudDacWriteFrameNR() returned false!" << endl;
        EAD_OkayFlag = M_FALSE;

    }

    // logFile << "Device_EZAUDDAC::WriteCurrentDeviceBuffer() - Writing " << EAD_BufferIndex+1 << " points." << endl;
        
    EAD_BufferIndex = 0;

#endif
}
    
