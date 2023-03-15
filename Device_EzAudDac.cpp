//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Device_EzAudDac.cpp                        *
// * Description: Implementation of the Device_EzAudDac    *
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

#ifndef DEVICE_EZAUDDAC_H
#include "Device_EzAudDac.h"
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
    EAD_BufferPtr(&EAD_PointBuffer[0])
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EZAUDDAC::Device_EZAUDDAC(void)" << endl;
#endif

#ifdef HELIOS_OUTPUT
    deviceMin = 0x0000;
    deviceMax = 0x0fff;

    last_X = 0x0800;
    last_Y = 0x0800;
#else
    deviceMin = MIN_INT_16_VALUE;
    deviceMax = MAX_INT_16_VALUE;

    last_X = 0x0000;
    last_Y = 0x0000;
#endif

    basePort    = 0x0000;
    // EAD_CardNumber = 1;
    EAD_CardNumber = 0;
    EAD_BufferIndex = 0;

    maxBufferPoints = EZAUDDAC_BUFFER_SIZE - 1;

#ifdef HELIOS_OUTPUT
    shutterOpen = false;
#endif

    outputDeviceType = EZAUDDAC;    

    return;
}

/*******************************************************************/
/*                                                                 */
/*    Device_EZAUDDAC Constructor (device number)                  */
/*                                                                 */
/*******************************************************************/

Device_EZAUDDAC::Device_EZAUDDAC(int devNum):
    Gem_PC(),
    EAD_OkayFlag(M_FALSE),
    EAD_BufferPtr(&EAD_PointBuffer[0])
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EZAUDDAC::Device_EZAUDDAC(int, OutputDeviceTypeT)" << endl;
#endif

    EAD_CardNumber = devNum;

#ifdef HELIOS_OUTPUT
    deviceMin = 0x0000;
    deviceMax = 0x0fff;

    last_X = 0x0800;
    last_Y = 0x0800;
#else
    deviceMin = MIN_INT_16_VALUE;
    deviceMax = MAX_INT_16_VALUE;

    last_X = 0x0000;
    last_Y = 0x0000;
#endif

    basePort    = 0x0000;
    EAD_BufferIndex = 0;

    maxBufferPoints = EZAUDDAC_BUFFER_SIZE - 1;

#ifdef HELIOS_OUTPUT
    shutterOpen = false;
#endif

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
#ifdef HELIOS_OUTPUT
    if (shutterOpen == true)
    {
        if (__EzAudDacSetShutter(EAD_CardNumber, false) != 1)
        {
            logFile << "Device_EZAUDDAC::~Device_EZAUDDAC() - Failed to set shutter closed" << endl;
        }
        else
        {
            logFile << "Device_EZAUDDAC::~Device_EZAUDDAC() - Shutter is closed" << endl;
            shutterOpen = true;
        }
    }
#endif

    __EzAudDacStop(EAD_CardNumber);
    __EzAudDacClose();

    return;
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: EzAudDacInit                                       */
/*                                                                 */
/*******************************************************************/

int
Device_EZAUDDAC::EzAudDacInit(void)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EZAUDDAC::EzAudDacInit()" << endl;
#endif

    int numberOfDevices = 0;

    // load the EzAudDac DLL
    HINSTANCE hLibrary;

#ifdef HELIOS_OUTPUT
    hLibrary = LoadLibrary("HeliosLaserDAC.dll");
#else
    hLibrary=LoadLibrary("EzAudDac.dll");
#endif

    if (hLibrary == NULL)
    {
      logFile << "Device_EZAUDDAC::EzAudDacInit() - Can't load library EzAudDac.dll!" << endl;
      return numberOfDevices;
    }
    
    // Retrieve a pointer to each of the EzAudDac library functions,
    // and check to make sure that the pointer is valid...

    __EzAudDacGetCardNum =(ezAudDacFuncPtr1)GetProcAddress(hLibrary,"EzAudDacGetCardNum");
    if (!__EzAudDacGetCardNum)
    {
      logFile << "Device_EZAUDDAC::EzAudDacInit() - Can't load library routine EzAudDacGetCardNum!" << endl;
      return numberOfDevices;
    }

    __EzAudDacWriteFrame =(ezAudDacFuncPtr2)GetProcAddress(hLibrary,"EzAudDacWriteFrame");
    if (!__EzAudDacWriteFrame)
    {
      logFile << "Device_EZAUDDAC::EzAudDacInit() - Can't load library routine EzAudDacWriteFrame!" << endl;
      return numberOfDevices;
    }

    __EzAudDacWriteFrameNR =(ezAudDacFuncPtr3)GetProcAddress(hLibrary,"EzAudDacWriteFrameNR");
    if (!__EzAudDacWriteFrameNR)
    {
      logFile << "Device_EZAUDDAC::EzAudDacInit() - Can't load library routine EzAudDacWriteFrameNR!" << endl;
      return numberOfDevices;
    }

    __EzAudDacGetStatus =(ezAudDacFuncPtr4)GetProcAddress(hLibrary,"EzAudDacGetStatus");
    if (!__EzAudDacGetStatus)
    {
      logFile << "Device_EZAUDDAC::EzAudDacInit() - Can't load library routine EzAudDacGetStatus!" << endl;
      return numberOfDevices;
    }

    __EzAudDacStop =(ezAudDacFuncPtr5)GetProcAddress(hLibrary,"EzAudDacStop");
    if (!__EzAudDacStop)
    {
      logFile << "Device_EZAUDDAC::EzAudDacInit() - Can't load library routine EzAudDacStop!" << endl;
      return numberOfDevices;
    }

    __EzAudDacClose =(ezAudDacFuncPtr6)GetProcAddress(hLibrary,"EzAudDacClose");
    if (!__EzAudDacClose)
    {
      logFile << "Device_EZAUDDAC::EzAudDacInit() - Can't load library routine __EzAudDacClose!" << endl;
      return numberOfDevices;
    }

#ifdef HELIOS_OUTPUT
    __EzAudDacSetShutter = (ezAudDacFuncPtr7)GetProcAddress(hLibrary, "EzAudDacSetShutter");
    if (!__EzAudDacSetShutter)
    {
        logFile << "Device_EZAUDDAC::EzAudDacInit() - Can't load library routine __EzAudDacSetShutter!" << endl;
        return numberOfDevices;
    }
#endif

    numberOfDevices = __EzAudDacGetCardNum();
    logFile << "Device_EZAUDDAC::EzAudDacInit() - Found " << numberOfDevices << " device." << endl;

    if (numberOfDevices > 0)
    {
#ifdef HELIOS_OUTPUT
        if (shutterOpen == false)
        {
            if (__EzAudDacSetShutter(EAD_CardNumber, true) != 1)
            {
                logFile << "Device_EZAUDDAC::EzAudDacInit() - Failed to set shutter open" << endl;
            }
            else
            {
                logFile << "Device_EZAUDDAC::EzAudDacInit() - Shutter is open" << endl;
                shutterOpen = true;
            }
        }
#endif
        EAD_OkayFlag = M_TRUE;
    }
    else
        EAD_OkayFlag = M_FALSE;

    return numberOfDevices;
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: LaserOff                                           */
/*                                                                 */
/*******************************************************************/

void 
Device_EZAUDDAC::LaserOff(const M_UINT16 blankingInvert, const enum DisplayColorMode colorMode)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EZAUDDAC::LaserOff()" << endl;
#endif

    M_UINT8 intensitySignalValue = 0x00;

    if (blankingInvert == M_TRUE)
    {
        intensitySignalValue = 0xff;
    }

    if (colorMode == DisplayColorMode::COLOR_RG_INV_TTL)
    {
        EzAudDacAddPoint((M_INT16)last_X,
                         (M_INT16)last_Y,
                         0xff,
                         0xff,
                         0xff,
                         intensitySignalValue);
    }
    else
    {
        EzAudDacAddPoint((M_INT16)last_X,
                         (M_INT16)last_Y,
                         0x00,
                         0x00,
                         0x00,
                         intensitySignalValue);
    }


//  QUESTION: WHY WOULD YOU *NOT* CALL WRITE HERE???
WriteCurrentDeviceBuffer();

if (false == __EzAudDacStop(EAD_CardNumber))
{
    logFile << "Device_EZAUDDAC::LaserOff() - Call to __EzAudDacStop failed!" << endl;
}

#ifdef LOG_PORT_DATA
    logFile << "Device_EZAUDDAC::LaserOff() << turning off laser for card "
            << EAD_CardNumber << endl;
#endif

} // end function LaserOff

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DisplayOff                                         */
/*                                                                 */
/*******************************************************************/

void 
Device_EZAUDDAC::DisplayOff(const M_UINT16 blankingInvert, const enum DisplayColorMode colorMode)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_EZAUDDAC::DisplayOff()" << endl;
#endif

   M_UINT8 intensitySignalValue = 0x00;

    if (blankingInvert == M_TRUE)
    {
        intensitySignalValue = 0xff;
    }

    for (int i=0 ; i<1 ; ++i)
    {
        for (int j=0; j<EZAUDDAC_BUFFER_SIZE; j++)
        { // clear data buffers
            EAD_PointBuffer[j].X=0;
            EAD_PointBuffer[j].Y=0;

            if (colorMode == DisplayColorMode::COLOR_RG_INV_TTL)
            {
#ifdef HELIOS_OUTPUT
                EAD_PointBuffer[j].R = 0xff;
                EAD_PointBuffer[j].G = 0xff;
                EAD_PointBuffer[j].B = 0xff;
#else
                EAD_PointBuffer[j].R = ConvertUbyteToShort(0xff);
                EAD_PointBuffer[j].G = ConvertUbyteToShort(0xff);
                EAD_PointBuffer[j].B = ConvertUbyteToShort(0xff);
#endif
            }
            else
            {
#ifdef HELIOS_OUTPUT
                EAD_PointBuffer[j].R = 0x00;
                EAD_PointBuffer[j].G = 0x00;
                EAD_PointBuffer[j].B = 0x00;
#else
                EAD_PointBuffer[j].R = ConvertUbyteToShort(0x00);
                EAD_PointBuffer[j].G = ConvertUbyteToShort(0x00);
                EAD_PointBuffer[j].B = ConvertUbyteToShort(0x00);
#endif
            }

#ifdef HELIOS_OUTPUT
            EAD_PointBuffer[j].I = intensitySignalValue;
#else
            EAD_PointBuffer[j].I = ConvertUbyteToShort(intensitySignalValue);
#endif
            EAD_PointBuffer[j].AL=0;
            EAD_PointBuffer[j].AR=0;

        }
    }

    // EAD_BufferIndex = EZAUDDAC_BUFFER_SIZE;    
    EAD_BufferIndex = 1;
    WriteCurrentDeviceBuffer();

    if (false == __EzAudDacStop(EAD_CardNumber))
    {
        logFile << "Device_EZAUDDAC::DisplayOff() - Call to __EzAudDacStop failed!" << endl;
    }

#ifdef LOG_PORT_DATA
    logFile << "Device_EZAUDDAC::DisplayOff() << turning off device number "
            << EAD_CardNumber << endl;
#endif

} /* DisplayOff */

/*******************************************************************/
/*                                                                 */
/*    PRIVATE FUNCTION: ConvertUbyteToShort                        */
/*                                                                 */
/*******************************************************************/
M_INT16
Device_EZAUDDAC::ConvertUbyteToShort(M_UINT8 ubyteVal)
{
    //             new max - new min
    // new value = ================= * (old value - old min)
    //             old max - old min
    float oldDelta = 255.0;
    float newDelta = 32766.0;

    // return (M_UINT16)((newDelta / oldDelta) * (float)ubyteVal);
    return (M_INT16)((newDelta / oldDelta) * (float)ubyteVal);

    // long tempVal = ubyteVal;
    // return (M_INT16)((tempVal << 8) - 32512); // 127 << 8 = 32512
    // return (M_INT16)((tempVal << 8) - (0xFFFF / 2));
}

#ifdef LOG_EZAUDDAC_PORT_DATA_BUFFER_IN
/*******************************************************************/
/*                                                                 */
/*    PRIVATE FUNCTION: ConvertShortToUbyte                        */
/*                                                                 */
/*******************************************************************/
M_UINT8
Device_EZAUDDAC::ConvertShortToUbyte(M_INT16 shortVal)
{
    return (M_UINT8)((shortVal + (0xFFFF / 2)) >> 8);
}
#endif //LOG_EZAUDDAC_PORT_DATA_BUFFER_IN

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

    long      temp_x, temp_y;

    M_UINT8 local_i_value = outPoint.iValue;

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
#endif // if LOG_PORT_DATA

} /* DrawLaser */

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
    if (EAD_OkayFlag == M_TRUE &&
        EAD_BufferIndex < EZAUDDAC_BUFFER_SIZE)
    {
#ifdef HELIOS_OUTPUT
        EAD_BufferPtr[EAD_BufferIndex].X = (M_INT16)xVal;
        EAD_BufferPtr[EAD_BufferIndex].Y = (M_INT16)yVal;
#else
        EAD_BufferPtr[EAD_BufferIndex].X = xVal;
        EAD_BufferPtr[EAD_BufferIndex].Y = yVal;
#endif

#ifdef HELIOS_OUTPUT
        EAD_BufferPtr[EAD_BufferIndex].R = (M_INT16)rVal;
        EAD_BufferPtr[EAD_BufferIndex].G = (M_INT16)gVal;
        EAD_BufferPtr[EAD_BufferIndex].B = (M_INT16)bVal;
        EAD_BufferPtr[EAD_BufferIndex].I = (M_INT16)iVal;
#else
        EAD_BufferPtr[EAD_BufferIndex].R = ConvertUbyteToShort(rVal);
        EAD_BufferPtr[EAD_BufferIndex].G = ConvertUbyteToShort(gVal);
        EAD_BufferPtr[EAD_BufferIndex].B = ConvertUbyteToShort(bVal);
        EAD_BufferPtr[EAD_BufferIndex].I = ConvertUbyteToShort(iVal);
#endif

// msvob 5/4/2022: Following doesn't look right.
// Remove and see what happens...
/*
        if (EAD_BufferPtr[EAD_BufferIndex].I != 0)
        {
            EAD_BufferPtr[EAD_BufferIndex].R = 0;
            EAD_BufferPtr[EAD_BufferIndex].G = 0;
            EAD_BufferPtr[EAD_BufferIndex].B = 0;
        }
*/

        EAD_BufferPtr[EAD_BufferIndex].AL = 0;
        EAD_BufferPtr[EAD_BufferIndex].AR = 0;

#ifdef LOG_EZAUDDAC_PORT_DATA_BUFFER_IN
        logFile << "Add Point (Incoming Values) - "
            << "Index: " << EAD_BufferIndex
            << ", X: " << xVal
            << ", Y: " << yVal
            << ", I: " << (M_UINT16)iVal
            << ", R: " << (M_UINT16)rVal
            << ", G: " << (M_UINT16)gVal
            << ", B: " << (M_UINT16)bVal << endl;

        logFile << "Add Point (Values Added) - "
                << "Index: " << EAD_BufferIndex
                << ", X: " << EAD_BufferPtr[EAD_BufferIndex].X
                << ", Y: " << EAD_BufferPtr[EAD_BufferIndex].Y
                << ", I: " << EAD_BufferPtr[EAD_BufferIndex].I
                << ", R: " << EAD_BufferPtr[EAD_BufferIndex].R
                << ", G: " << EAD_BufferPtr[EAD_BufferIndex].G
                << ", B: " << EAD_BufferPtr[EAD_BufferIndex].B << endl << endl;

/*        
        M_UINT8 i_byte = ConvertShortToUbyte(EAD_BufferPtr[EAD_BufferIndex].I);
        M_UINT8 r_byte = ConvertShortToUbyte(EAD_BufferPtr[EAD_BufferIndex].R);
        M_UINT8 g_byte = ConvertShortToUbyte(EAD_BufferPtr[EAD_BufferIndex].G);
        M_UINT8 b_byte = ConvertShortToUbyte(EAD_BufferPtr[EAD_BufferIndex].B);

        logFile << "Add Point (Helios Original DLL) - "
            << "Index: " << EAD_BufferIndex
            << ", X: " << EAD_BufferPtr[EAD_BufferIndex].X
            << ", Y: " << EAD_BufferPtr[EAD_BufferIndex].Y
            << ", I: " << (M_INT16)i_byte
            << ", R: " << (M_INT16)r_byte
            << ", G: " << (M_INT16)g_byte
            << ", B: " << (M_INT16)b_byte << endl;
*/
#endif



        if (EAD_BufferIndex >= maxBufferPoints)
        {
            WriteCurrentDeviceBuffer();
        }
        else
        {
            ++EAD_BufferIndex;
        }
    }
    else
    {
        logFile << "Device_EZAUDDAC::EzAudDacAddPoint() failed to add point " << EAD_BufferIndex << endl;
    }

} /* EzAudDacAddPoint */

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: WriteCurrentDeviceBuffer                           */
/*                                                                 */
/*******************************************************************/
void
Device_EZAUDDAC::WriteCurrentDeviceBuffer(void)
{
    if (EAD_OkayFlag == M_FALSE ||
        EAD_BufferIndex == 0)
    {
        return;
    }

    bool retVal;

    while (__EzAudDacGetStatus(EAD_CardNumber) == 0)
    {
//        logFile << "Device_EZAUDDAC::WriteCurrentDeviceBuffer() - Waiting for device ready with " << EAD_BufferIndex+1 << " points." << endl;
        Sleep(5);
    }

    // logFile << "WriteCurrentDeviceBuffer called with " << EAD_BufferIndex << " points." << endl;

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

#else

    // retVal = __EzAudDacWriteFrameNR(EAD_CardNumber, EAD_BufferPtr, EAD_BufferIndex*sizeof(EAD_Pnt_s), GetOutputPPS(), 0);
    retVal = __EzAudDacWriteFrameNR(EAD_CardNumber, EAD_BufferPtr, EAD_BufferIndex*sizeof(EAD_Pnt_s), GetOutputPPS(), 0);

    if (retVal != TRUE)
    {
        logFile << "Device_EZAUDDAC::WriteCurrentDeviceBuffer() - function EzAudDacWriteFrameNR() returned false!" << endl;
        EAD_OkayFlag = M_FALSE;

    }

#endif
        
    EAD_BufferIndex = 0;

}
    
