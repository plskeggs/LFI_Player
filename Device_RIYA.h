//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Device_RIYA.h                              *
// * Description: This file contains the interface for the *
// *              Device_RIYA class.                       *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//
#ifndef DEVICE_RIYA_H
#define DEVICE_RIYA_H

#ifndef GLOBALS_H
#include "globals.h"
#endif

#ifndef GEM_PC_H
#include "Gem_pc.h"
#endif

#include <afxwin.h>

class Device_RIYA : public Gem_PC
{
public:
  // is setup for RIYA card
// const M_UINT16 DEVICE_MIN  = 0x0000;
// const M_UINT16 DEVICE_MAX  = 0x0fff;

// const M_UINT16 DEVICE_MIN  = 0x0000;
// const M_UINT16 DEVICE_MAX  = 0x0fff;

  // Following currently defined in RIYA documentation .chm file
  #define RIYA_BUFFER_SIZE      0x1000

  #define RIYA_DELAY_PERIOD     2800
  #define RIYA_ATTRIBUTE_VALUE  0x1E

  // Define structure 8 byte
  struct RiyaPixel
  {
      M_UINT16 X;
      M_UINT16 Y;
      M_UINT8  R;
      M_UINT8  G;
      M_UINT8  B;
      M_UINT8  I;
  };

public:
    Device_RIYA(void);
    Device_RIYA(M_UINT16 devNum, OutputDeviceTypeT riyaDeviceType);
    ~Device_RIYA(void);

    virtual void DrawLaser(const OutputPoint_T,
                           const M_UINT16);
    virtual void DisplayOff(const M_UINT16, const enum DisplayColorMode colorMode);
    virtual void LaserOff(const M_UINT16, const enum DisplayColorMode colorMode);
    virtual void WriteCurrentDeviceBuffer(void);
    
    M_UINT16 RiyaInit(M_UINT8 numberOfInit);
    void RiyaStop(void)
    {
        if (riyaOkayFlag == M_TRUE)
        {
            StopRiyaDevice(0x02);
        }
    }

    void SetDelayPeriod(M_UINT32 newPeriod)
    {
        riyaDelayPeriod = newPeriod;
    }

    M_UINT32 GetDelayPeriod(void)
    {
        return riyaDelayPeriod;
    }

    M_UINT16 GetRiyaOkayFlag(void)
    {
        return riyaOkayFlag;
    }

    void SetTempDelayPeriod(M_UINT32 newPeriod)
    {
        saveDelayPeriod = riyaDelayPeriod;
        riyaDelayPeriod = newPeriod;
    }

    void RestoreOrigDelayPeriod(void)
    {
        riyaDelayPeriod = saveDelayPeriod;
        saveDelayPeriod = 0;
    }

    // IMPORTANT: Note that this function will only
    // work correctly when called for the first RIYA
    // device.  This is because OpenAllRiyaDevices() can
    // only be called once.
    M_UINT8 GetRiyaNumberOfDevices(void)
    {
        return(riyaNumberDevices);
    }

private:
  // function pointer for library routine RiOpenDevices
  typedef unsigned char (* riyaFuncPtr1)();

  // function pointer for library routine RiCloseDevices
  typedef void (* riyaFuncPtr2)();

  // function pointer for library routine Ri_InitChanal
  typedef void (* riyaFuncPtr3)(unsigned char,              // card ID
                                unsigned long);             // attribute

  // function pointer for library routines Ri_SetIntCh & RiStopShow
  typedef unsigned char (* riyaFuncPtr4)(unsigned char);    // card ID

  // function pointer for library routine RiSetShowCadr
  typedef unsigned char (* riyaFuncPtr5)(unsigned char,     // card ID
                                         unsigned char *,   // data
                                         unsigned int,      // size
                                         unsigned int,      // output period
                                         unsigned char);    // attribute


    M_UINT16 deviceNumber;

    void RiyaAddPoint(const M_UINT16 xVal,
                      const M_UINT16 yVal,
                      const M_UINT8  rVal,
                      const M_UINT8  gVal,
                      const M_UINT8  bVal,
                      const M_UINT8  iVal);

    riyaFuncPtr1 OpenAllRiyaDevices;    // library routine RiOpenDevices
    riyaFuncPtr2 CloseAllRiyaDevices;   // library routine RiCloseDevices
    riyaFuncPtr3 InitRiyaDevice;        // library routine Ri_InitChanal
    riyaFuncPtr4 RiyaReadyForNextFrame; // library routine Ri_SetIntCh
    riyaFuncPtr5 TransferFrameToBuffer; // library routine RiSetShowCadr
    riyaFuncPtr4 StopRiyaDevice;        // library routine RiStopShow

    M_UINT32 saveDelayPeriod;
    M_UINT8  riyaNumberDevices;

public:
    // Why, oh why are all of these "public"?  I know there's a reason,
    // but I can't remember, for the life o' me, what it is.  I think
    // the buffer's really do have to be public for the RIYA board to
    // access them...but what about the other stuff?  Think, think,
    // think...  Man, getting old sucks!
    RiyaPixel Data1[RIYA_BUFFER_SIZE];
    RiyaPixel Data2[RIYA_BUFFER_SIZE];

    M_UINT16     riyaBufferIndex;
    RiyaPixel*   riyaBufferPtr;
    M_UINT8      riyaActiveBuffer;
    M_UINT16     riyaPreloadPoints;
    M_UINT16     riyaOkayFlag;
    M_UINT32     riyaDelayPeriod;	
};

#endif