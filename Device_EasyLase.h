#ifndef DEVICE_EASYLASE_H
#define DEVICE_EASYLASE_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Device_EASYLASE.h                          *
// * Description: This file contains the interface for the *
// *              Device_EASYLASE class.                   *
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

class Device_EASYLASE : public Gem_PC
{
public:
  // is setup for EasyLase

    #define EASYLASE_BUFFER_SIZE      6400

	typedef struct 
	{
        M_UINT16 X;
        M_UINT16 Y;
        M_UINT8 R;
        M_UINT8 G;
        M_UINT8 B;
        M_UINT8 I;
    } EL_Pnt_s;


public:
    Device_EASYLASE(void);
    Device_EASYLASE(int devNum, OutputDeviceTypeT easylaseDeviceType);
    ~Device_EASYLASE(void);

	virtual void DrawLaser(const OutputPoint_T,
				           const M_UINT16);
    virtual void DisplayOff(const M_UINT16, const enum DisplayColorMode colorMode);
    virtual void LaserOff(const M_UINT16, const enum DisplayColorMode colorMode);

    virtual void WriteCurrentDeviceBuffer(void);

    M_UINT16 EasyLaseInit(void);
	void EasyLaseStop(void);

    void SetOutputPPS(M_UINT16  newPPS)
    {
        EL_PPS = newPPS;
    }

    M_UINT16  GetOutputPPS(void)
    {
        return EL_PPS;
    }

    M_UINT16 GetOkayFlag(void)
    {
        return EL_OkayFlag;
    }

private:
    EL_Pnt_s EL_PointBuffer[EASYLASE_BUFFER_SIZE];
    EL_Pnt_s* EL_BufferPtr;
	// int EL_NumPoints;
	int EL_BufferIndex;
	int EL_CardNumber;

    unsigned long   EL_DeviceNumber;
    M_UINT16        EL_OkayFlag;
    M_UINT16        EL_PPS;	

	// function pointer for library routine EasyLaseGetCardNum
    typedef int (__stdcall * easylaseFuncPtr1)();

    // function pointer for library routine EasyLaseWriteFrame
    typedef bool (__stdcall * easylaseFuncPtr2)(const int&,        // CardNum
                                                const EL_Pnt_s*,  // data
									            int,               // Bytes
									            M_UINT16);         // PPS

    // function pointer for library routine EasyLaseWriteFrameNR
    typedef bool (__stdcall * easylaseFuncPtr3)(const int&,        // CardNum
                                                const EL_Pnt_s*,  // data
									            int,               // Bytes
									            M_UINT16,          // PPS
									            M_UINT16);         // Reps

    // function pointer for library routine EasyLaseGetStatus
    typedef int (__stdcall * easylaseFuncPtr4)(const int&);        // CardNum

    // function pointer for library routine EasyLaseStop
    typedef bool (__stdcall * easylaseFuncPtr5)(const int&);       // CardNum

	// function pointer for library routine EasyLaseClose
    typedef bool (__stdcall * easylaseFuncPtr6)();

	easylaseFuncPtr1    __EasyLaseGetCardNum;
	easylaseFuncPtr2    __EasyLaseWriteFrame;
    easylaseFuncPtr3    __EasyLaseWriteFrameNR;
    easylaseFuncPtr4    __EasyLaseGetStatus;
	easylaseFuncPtr5    __EasyLaseStop;
	easylaseFuncPtr6    __EasyLaseClose;


    // int  __stdcall EasyLaseGetCardNum(void);
    // bool __stdcall EasyLaseWriteFrame(const int& CardNum, const EL_Pnt_s* data, int Bytes, M_UINT16 PPS);
    // bool __stdcall EasyLaseWriteFrameNR(const int& CardNum, const EL_Pnt_s* data, int Bytes, M_UINT16 PPS, M_UINT16 Reps);
    // int  __stdcall EasyLaseGetStatus(const int& CardNum);
    // bool __stdcall EasyLaseStop(const int& CardNum);
    // bool __stdcall EasyLaseClose(void);

    void EasyLaseAddPoint(const M_UINT16 xVal,
                          const M_UINT16 yVal,
                          const M_UINT8  rVal,
                          const M_UINT8  gVal,
                          const M_UINT8  bVal,
                          const M_UINT8  iVal);


};

#endif