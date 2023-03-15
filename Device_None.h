#ifndef DEVICE_NONE_H
#define DEVICE_NONE_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Device_NONE.h                              *
// * Description: This file contains the interface for the *
// *              Device_NONE class.                       *
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

class Device_None : public Gem_PC
{
public:
    Device_None(void);
    ~Device_None(void);

	virtual void DrawLaser(const OutputPoint_T,
				           const M_UINT16);
    virtual void DisplayOff(const M_UINT16, const enum DisplayColorMode colorMode);
    virtual void LaserOff(const M_UINT16, const enum DisplayColorMode colorMode);

private:

};

#endif