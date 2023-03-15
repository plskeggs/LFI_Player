#ifndef DEVICE_GWS_H
#define DEVICE_GWS_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2005, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Device_GWS.h                               *
// * Description: This file contains the interface for the *
// *              Device_GWS class.                        *
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

#define X_OFFSET_GWS      0x00
#define Y_OFFSET_GWS      0x04
#define R_OFFSET_GWS      0x10
#define G_OFFSET_GWS      0x11
#define B_OFFSET_GWS      0x12
#define I_OFFSET_GWS      0x13
#define LOAD_OFFSET_GWS   0x15

class Device_GWS : public Gem_PC
{
public:
    Device_GWS(void);
    Device_GWS(M_UINT16 devNum);
    ~Device_GWS(void);

	virtual void DrawLaser(const OutputPoint_T,
				           const M_UINT16);
    virtual void DisplayOff(const M_UINT16, const enum DisplayColorMode colorMode);
    virtual void LaserOff(const M_UINT16, const enum DisplayColorMode colorMode);

};

#endif