#ifndef DEVICE_BSOFT_XYI_H
#define DEVICE_BSOFT_XYI_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2005, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Device_BSOFT_XYI.h                         *
// * Description: This file contains the interface for the *
// *              Device_BSOFT_XYI class.                  *
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

#define CARD2_OFFSET_BXYI   0x20
#define X_OFFSET_BXYI       0x14
#define Y_OFFSET_BXYI       0x16
#define I_OFFSET_BXYI       CARD2_OFFSET_BXYI + 0x1C
#define CTRL_OFFSET_BXYI    CARD2_OFFSET_BXYI + 0x1F
    
class Device_BSOFT_XYI : public Gem_PC
{
public:
    Device_BSOFT_XYI(void);
    Device_BSOFT_XYI(M_UINT16 devNum);
    ~Device_BSOFT_XYI(void);

	virtual void DrawLaser(const OutputPoint_T,
				           const M_UINT16);
    virtual void DisplayOff(const M_UINT16, const enum DisplayColorMode colorMode);
    virtual void LaserOff(const M_UINT16, const enum DisplayColorMode colorMode);

};

#endif