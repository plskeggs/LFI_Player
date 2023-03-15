#ifndef DEVICE_EZAUDDAC_H
#define DEVICE_EZAUDDAC_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Device_EZAUDDAC.h                          *
// * Description: This file contains the interface for the *
// *              Device_EZAUDDAC class.                   *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#ifndef GLOBALS_H
#include "globals.h"
#endif

#ifndef GEM_PC_H
#include "Gem_pc.h"
#endif

#include <afxwin.h>

class Device_EZAUDDAC : public Gem_PC
{
public:
  // is setup for EzAudDac

    // Following currently defined in driver file HeliosDac.h as HELIOS_MAX_POINTS
    #define EZAUDDAC_BUFFER_SIZE      0x1000

	typedef struct 
	{
        M_INT16 X;
        M_INT16 Y;
        M_INT16 R;
        M_INT16 G;
        M_INT16 B;
        M_INT16 I;
        M_INT16 AL;
        M_INT16 AR;
    } EAD_Pnt_s;


public:
    Device_EZAUDDAC(void);
    Device_EZAUDDAC(int devNum);
    ~Device_EZAUDDAC(void);

	virtual void DrawLaser(const OutputPoint_T,
				           const M_UINT16);
    virtual void DisplayOff(const M_UINT16, const enum DisplayColorMode colorMode);
    virtual void LaserOff(const M_UINT16, const enum DisplayColorMode colorMode);

    virtual void WriteCurrentDeviceBuffer(void);

    int EzAudDacInit(void);
	void EzAudDacStop(void);

    void SetOutputPPS(M_UINT16  newPPS)
    {
        EAD_PPS = newPPS;
    }

    M_UINT16  GetOutputPPS(void)
    {
        return EAD_PPS;
    }

    M_UINT16 GetOkayFlag(void)
    {
        return EAD_OkayFlag;
    }

private:
    EAD_Pnt_s EAD_PointBuffer[EZAUDDAC_BUFFER_SIZE];
    EAD_Pnt_s* EAD_BufferPtr;
	// int EAD_NumPoints;
	int EAD_BufferIndex;
	int EAD_CardNumber;

    M_UINT16        EAD_OkayFlag;
    M_UINT16        EAD_PPS;

    M_INT16 ConvertUbyteToShort(M_UINT8 byteVal);
    M_UINT8 ConvertShortToUbyte(M_INT16 shortVal);

#ifdef HELIOS_OUTPUT
    // Flag indicating whether shutter has been opened.
    // Note that there is no shutter control function
    // exposed by the EzAudDac.dll
    bool shutterOpen;
#endif

	// function pointer for library routine EzAudDacGetCardNum
    typedef int (__stdcall * ezAudDacFuncPtr1)();

    // function pointer for library routine EzAudDacWriteFrame
    typedef bool (__stdcall * ezAudDacFuncPtr2)(const int&,        // CardNum
                                                const EAD_Pnt_s*,  // data
									            int,               // Bytes
									            M_UINT16);         // PPS

    // function pointer for library routine EzAudDacWriteFrameNR
    typedef bool (__stdcall * ezAudDacFuncPtr3)(const int&,        // CardNum
                                                const EAD_Pnt_s*,  // data
									            int,               // Bytes
									            M_UINT16,          // PPS
									            M_UINT16);         // Reps

    // function pointer for library routine EzAudDacGetStatus
    typedef int (__stdcall * ezAudDacFuncPtr4)(const int&);        // CardNum

    // function pointer for library routine EzAudDacStop
    typedef bool (__stdcall * ezAudDacFuncPtr5)(const int&);       // CardNum

	// function pointer for library routine EzAudDacClose
    typedef bool (__stdcall * ezAudDacFuncPtr6)();

#ifdef HELIOS_OUTPUT
    // function pointer for library routine EzAudDacSetShutter
    typedef int (__stdcall * ezAudDacFuncPtr7)(const int&,          // CardNum
                                               bool);               // Shutter Level
#endif

	ezAudDacFuncPtr1    __EzAudDacGetCardNum;
	ezAudDacFuncPtr2    __EzAudDacWriteFrame;
    ezAudDacFuncPtr3    __EzAudDacWriteFrameNR;
    ezAudDacFuncPtr4    __EzAudDacGetStatus;
	ezAudDacFuncPtr5    __EzAudDacStop;
	ezAudDacFuncPtr6    __EzAudDacClose;
#ifdef HELIOS_OUTPUT
    ezAudDacFuncPtr7    __EzAudDacSetShutter;
#endif

    // int  __stdcall EzAudDacGetCardNum(void);
    // bool __stdcall EzAudDacWriteFrame(const int& CardNum, const EAD_Pnt_s* data, int Bytes, M_UINT16 PPS);
    // bool __stdcall EzAudDacWriteFrameNR(const int& CardNum, const EAD_Pnt_s* data, int Bytes, M_UINT16 PPS, M_UINT16 Reps);
    // int  __stdcall EzAudDacGetStatus(const int& CardNum);
    // bool __stdcall EzAudDacStop(const int& CardNum);
    // bool __stdcall EzAudDacClose(void);
    // int  __stdcall EzAudDacSetShutter(const int& CardNum, bool);

    void EzAudDacAddPoint(const M_INT16 xVal,
                          const M_INT16 yVal,
                          const M_UINT8 rVal,
                          const M_UINT8 gVal,
                          const M_UINT8 bVal,
                          const M_UINT8 iVal);

};

#endif