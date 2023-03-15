//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Device_RIYA.cpp                            *
// * Description: Implementation of the Device_RIYA        *
// * output device class. This code utilizes the RIYA      *
// * PCI Pro/USB SDK, Version 2.0, Copyright Copyright     *
// * 2004 RIYA Co. Ltd.                                    * 
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

#ifndef DEVICE_RIYA_H
#include "Device_RIYA.h"
#endif

#ifndef GLOBALS_H
#include "globals.h"
#endif

using namespace std; 

extern ofstream logFile;

/*******************************************************************/
/*                                                                 */
/*    Device_RIYA Constructor (void)                               */
/*                                                                 */
/*******************************************************************/

Device_RIYA::Device_RIYA():
    Gem_PC(),
    riyaOkayFlag(M_FALSE),
    riyaBufferIndex(0),
    riyaActiveBuffer(1),
    riyaPreloadPoints(0),
    riyaBufferPtr(&Data1[0])
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_RIYA::Device_RIYA(void)" << endl;
#endif

    deviceMin = 0x0000;
    deviceMax = 0x0fff;

    last_X = 0x0800;
    last_Y = 0x0800;  

    basePort    = 0x0000;

    maxBufferPoints = RIYA_BUFFER_SIZE - 1;
  
    outputDeviceType = RIYA;    

    return;
}

/*******************************************************************/
/*                                                                 */
/*    Device_RIYA Constructor (device number)                      */
/*                                                                 */
/*******************************************************************/

Device_RIYA::Device_RIYA(M_UINT16 devNum, OutputDeviceTypeT riyaDeviceType):
    Gem_PC(),
    riyaOkayFlag(M_FALSE),
    riyaBufferIndex(0),
    riyaActiveBuffer(1),
    riyaPreloadPoints(0),
    riyaBufferPtr(&Data1[0])
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_RIYA::Device_RIYA(M_UINT16)" << endl;
#endif

    deviceNumber = devNum;

    deviceMin = 0x0000;
    deviceMax = 0x0fff;

    last_X = 0x0800;
    last_Y = 0x0800;
   
    riyaDelayPeriod = 2500;
    basePort    = 0x0000;

    maxBufferPoints = RIYA_BUFFER_SIZE - 1;

    outputDeviceType = RIYA; 

    return;
}

/*******************************************************************/
/*                                                                 */
/*    Device_RIYA Destructor                                       */
/*                                                                 */
/*******************************************************************/

Device_RIYA::~Device_RIYA()
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_RIYA::~Device_RIYA()" << endl;
#endif

    StopRiyaDevice((M_UINT8)deviceNumber);
    CloseAllRiyaDevices();

    return;
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: RiyaInit                                           */
/*                                                                 */
/*******************************************************************/

M_UINT16
Device_RIYA::RiyaInit(M_UINT8 numberOfInit)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_RIYA::RiyaInit()" << endl;
#endif

    // load the RIYA DLL
    HINSTANCE hLibrary;
    hLibrary=LoadLibrary("RiyaNetServer.dll");

    if (hLibrary == NULL)
    {
      logFile << "Device_RIYA::RiyaInit() - Can't load library RiyaNetServer.dll!" << endl;
      return M_FAILURE;
    }
    
    // Retrieve a pointer to each of the RIYA library functions,
    // and check to make sure that the pointer is valid...
    OpenAllRiyaDevices=(riyaFuncPtr1)GetProcAddress(hLibrary,"RiOpenDevices");
    if (!OpenAllRiyaDevices)
    {
      logFile << "Device_RIYA::RiyaInit() - Can't load library routine RiOpenDevice!" << endl;
      return M_FAILURE;
    }

    CloseAllRiyaDevices=(riyaFuncPtr2)GetProcAddress(hLibrary,"RiCloseDevices");
    if (CloseAllRiyaDevices==NULL)
    {
      logFile << "Device_RIYA::RiyaInit() - Can't load library routine RiCloseDevices!" << endl;
      return M_FAILURE;
    }

    InitRiyaDevice=(riyaFuncPtr3)GetProcAddress(hLibrary,"Ri_InitChanal");
    if (InitRiyaDevice==NULL)
    {
      logFile << "Device_RIYA::RiyaInit() - Can't load library routine Ri_InitChanal!" << endl;
      return M_FAILURE;
    }

    RiyaReadyForNextFrame=(riyaFuncPtr4)GetProcAddress(hLibrary,"Ri_SetIntCh");
    if (RiyaReadyForNextFrame==NULL)
    {
      logFile << "Device_RIYA::RiyaInit() - Can't load library routine Ri_SetIntCh!" << endl;
      return M_FAILURE;
    }

    StopRiyaDevice=(riyaFuncPtr4)GetProcAddress(hLibrary,"RiStopShow");
    if (StopRiyaDevice==NULL)
    {
      logFile << "Device_RIYA::RiyaInit() - Can't load library routine RiStopShow!" << endl;
      return M_FAILURE;
    }

    TransferFrameToBuffer=(riyaFuncPtr5)GetProcAddress(hLibrary,"RiSetShowCadr");
    if (TransferFrameToBuffer==NULL)
    {
      logFile << "Device_RIYA::RiyaInit() - Can't load library routine RiSetShowCadr!" << endl;
      return M_FAILURE;
    }

    // We have the RIYA library routines we need.  Now look to see
    // if any physical devices are present...
    if(numberOfInit == 0)
    {
        riyaNumberDevices = OpenAllRiyaDevices();
        logFile << "Device_RIYA::RiyaInit() - Device count = " << (M_UINT16)riyaNumberDevices << endl;

        if (riyaNumberDevices==0)
        {
            logFile << "Device_RIYA::RiyaInit() - No RIYA devices found!" << endl;
            return M_FAILURE;
        }

        if (riyaNumberDevices==255)
        {
            logFile << "Device_RIYA::RiyaInit() - No RIYA Drivers found!" << endl;
            return M_FAILURE;
        }
    }

    // Initialize the RIYA device...
    // 16 - no video synchronization
    //      disable TTL safety input
    //      use 8 byte point structure
    //      enable use of function Ri_SetShuter()
    //		stop display on RiStopShow()
    InitRiyaDevice((M_UINT8)deviceNumber,
                   16);

    for (int j=0; j<RIYA_BUFFER_SIZE; j++)
    { // clear data buffers
        Data1[j].X=0x0800;
        Data1[j].Y=0x0800;
        Data1[j].R=0;
        Data1[j].G=0;
        Data1[j].B=0;
        Data1[j].I=0;
        Data2[j].X=0x0800;
        Data2[j].Y=0x0800;
        Data2[j].R=0;
        Data2[j].G=0;
        Data2[j].B=0;
        Data2[j].I=0;
    }

    TransferFrameToBuffer((M_UINT8)deviceNumber,
                          (unsigned char *)&Data1[0],
                          RIYA_BUFFER_SIZE,
                          riyaDelayPeriod,
                          RIYA_ATTRIBUTE_VALUE);

    TransferFrameToBuffer((M_UINT8)deviceNumber,
                          (unsigned char *)&Data2[0],
                          RIYA_BUFFER_SIZE,
                          riyaDelayPeriod,
                          RIYA_ATTRIBUTE_VALUE);

    riyaOkayFlag = M_TRUE;

    return M_SUCCESS;
}

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: LaserOff                                           */
/*                                                                 */
/*******************************************************************/

void 
Device_RIYA::LaserOff(const M_UINT16 blankingInvert, const enum DisplayColorMode colorMode)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_RIYA::LaserOff()" << endl;
#endif

    M_UINT8 intensitySignalValue = 0x00;

    if (blankingInvert == M_TRUE)
    {
        intensitySignalValue = 0xff;
    }

    if (colorMode == DisplayColorMode::COLOR_RG_INV_TTL)
    {
        RiyaAddPoint((M_UINT16)last_X,
                     (M_UINT16)last_Y,
                     0xff,
                     0xff,
                     0xff,
                     intensitySignalValue);
    }
    else
    {
        RiyaAddPoint((M_UINT16)last_X,
                     (M_UINT16)last_Y,
                     0x00,
                     0x00,
                     0x00,
                     intensitySignalValue);
    }

//	WriteCurrentDeviceBuffer();

#ifdef LOG_PORT_DATA
    logFile << "Device_RIYA::LaserOff() << turning off device "
            << deviceNumber << endl;
#endif

} // end function LaserOff

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DisplayOff                                         */
/*                                                                 */
/*******************************************************************/

void 
Device_RIYA::DisplayOff(const M_UINT16 blankingInvert, const enum DisplayColorMode colorMode)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_RIYA::DisplayOff()" << endl;
#endif

    M_UINT8 intensitySignalValue = 0x00;

    if (blankingInvert == M_TRUE)
    {
        intensitySignalValue = 0xff;
    }

    for (int j=0; j<RIYA_BUFFER_SIZE; j++)
    { // clear data buffers
        if (colorMode == DisplayColorMode::COLOR_RG_INV_TTL)
        {
            Data1[j].X=0x0800;
            Data1[j].Y=0x0800;
            Data1[j].R=0xff;
            Data1[j].G=0xff;
            Data1[j].B=0xff;
            Data1[j].I=intensitySignalValue;
            Data2[j].X=0x0800;
            Data2[j].Y=0x0800;
            Data2[j].R=0xff;
            Data2[j].G=0xff;
            Data2[j].B=0xff;
            Data2[j].I=intensitySignalValue;
        }
        else
        {
            Data1[j].X=0x0800;
            Data1[j].Y=0x0800;
            Data1[j].R=0;
            Data1[j].G=0;
            Data1[j].B=0;
            Data1[j].I=intensitySignalValue;
            Data2[j].X=0x0800;
            Data2[j].Y=0x0800;
            Data2[j].R=0;
            Data2[j].G=0;
            Data2[j].B=0;
            Data2[j].I=intensitySignalValue;
        }
    }

    riyaBufferIndex = 1;
    
    WriteCurrentDeviceBuffer();

#ifdef LOG_PORT_DATA
    logFile << "Device_RIYA::DisplayOff() << turning off device number "
            << deviceNumber << endl;
#endif

} // end function DisplayOff


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DrawLaser                                          */
/*                                                                 */
/*******************************************************************/
void Device_RIYA::DrawLaser(const OutputPoint_T outPoint,
                            const M_UINT16 blankingInvert)
{
#ifdef GEM_ENTRY
    logFile << "Entering Device_RIYA::DrawLaser()" << endl;
#endif
    long   temp_x, temp_y;

    M_UINT8 local_i_value = outPoint.iValue;

    if (blankingInvert == M_TRUE)
    {
         local_i_value = ~local_i_value;
    }

    MapNDCToDevice(outPoint.xValue, outPoint.yValue, &temp_x, &temp_y);

    last_X = temp_x;
    last_Y = temp_y;

    RiyaAddPoint((M_UINT16)temp_x,
                 (M_UINT16)temp_y,
                 outPoint.rValue,
                 outPoint.gValue,
                 outPoint.bValue,
                 local_i_value);

#ifdef LOG_PORT_DATA
    logFile << "Device_RIYA::DrawLaser() to X= 0x" << hex << (M_UINT16)temp_x
                          << " Y= 0x" << hex << (M_UINT16)temp_y
                        << "\n R= 0x" << hex << (M_UINT16)outPoint.rValue
                          << " G= 0x" << hex << (M_UINT16)outPoint.gValue
                          << " B= 0x" << hex << (M_UINT16)outPoint.bValue
                          << " I= 0x" << hex << (M_UINT16)local_i_value << endl;
#endif

} // end function DrawLaser


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: RiyaAddPoint                                       */
/*                                                                 */
/*******************************************************************/
void
Device_RIYA::RiyaAddPoint(const M_UINT16 xVal,
                          const M_UINT16 yVal,
                          const M_UINT8  rVal,
                          const M_UINT8  gVal,
                          const M_UINT8  bVal,
                          const M_UINT8  iVal)
{
    if (riyaOkayFlag == M_TRUE &&
        riyaBufferIndex < RIYA_BUFFER_SIZE)
    {
        riyaBufferPtr[riyaBufferIndex].X = xVal;
        riyaBufferPtr[riyaBufferIndex].Y = yVal;
        riyaBufferPtr[riyaBufferIndex].R = rVal;
        riyaBufferPtr[riyaBufferIndex].G = gVal;
        riyaBufferPtr[riyaBufferIndex].B = bVal;
        riyaBufferPtr[riyaBufferIndex].I = iVal;

#ifdef RIYA_LOG_INFO
        logFile << "Add Point - "
                << "Index: " << riyaBufferIndex
                << ", X: " << riyaBufferPtr[riyaBufferIndex].X
                << ", Y: " << riyaBufferPtr[riyaBufferIndex].Y
                << ", I: " << (UINT16)(riyaBufferPtr[riyaBufferIndex].I)
                << ", R: " << (UINT16)(riyaBufferPtr[riyaBufferIndex].R)
                << ", G: " << (UINT16)(riyaBufferPtr[riyaBufferIndex].G)
                << ", B: " << (UINT16)(riyaBufferPtr[riyaBufferIndex].B) << endl;
#endif

        if (riyaBufferIndex >= maxBufferPoints)
        {
            WriteCurrentDeviceBuffer();
        }
        else
        {
            ++riyaBufferIndex;
        }
    }
    else
    {
        logFile << "Device_RIYA::RiyaAddPoint() failed to add point " << riyaBufferIndex << endl;
    }

} // end function RiyaAddPoint

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: WriteCurrentDeviceBuffer                           */
/*                                                                 */
/*******************************************************************/
void
Device_RIYA::WriteCurrentDeviceBuffer(void)
{
    M_UINT8 loadNextFrame=0;

#ifdef RIYA_LOG_INFO
    logFile << "WriteCurrentDeviceBuffer() device: "
            << deviceNumber << " buffer index: "
            << riyaBufferIndex 
            << " delay period: " << riyaDelayPeriod << endl;
#endif
    
    if (riyaOkayFlag == M_FALSE ||
        riyaBufferIndex == 0)
    {
        return;
    }

    while (RiyaReadyForNextFrame((M_UINT8)deviceNumber) == 0)
    {
//        logFile << "Device_RIYA::WriteCurrentDeviceBuffer() - Waiting for device ready with " << riyaBufferIndex +1 << " points." << endl;
        Sleep(5);
    }

#ifdef RIYA_LOG_INFO
    logFile << "WRITE: riyaBufferIndex is " << riyaBufferIndex << endl;
    logFile << "WRITE: riyaDelayPeriod is " << riyaDelayPeriod << endl;
#endif

//    logFile << "WriteCurrentDeviceBuffer called with " << riyaBufferIndex << " points." << endl;

    TransferFrameToBuffer((M_UINT8)deviceNumber,
                          (unsigned char *)riyaBufferPtr,
                          riyaBufferIndex,
                          riyaDelayPeriod,
                          RIYA_ATTRIBUTE_VALUE);

    if (riyaBufferPtr == &Data1[0])
    {
        riyaBufferPtr = &Data2[0];
#ifdef RIYA_LOG_INFO
       logFile << "now buffer 2 " << endl;
#endif
    }
    else
    {
        riyaBufferPtr = &Data1[0];
#ifdef RIYA_LOG_INFO
       logFile << "now buffer 1 " << endl;
#endif
    }

    // DWORD interBufferDelay = riyaBufferIndex * 0.015;
    // logFile << "WRITE: interBufferDelay is " << interBufferDelay << endl;
    // Sleep(interBufferDelay);
    riyaBufferIndex = 0;

}
