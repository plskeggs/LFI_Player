#ifndef PALETTE_H
#define PALETTE_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Palette.h                                  *
// * Description: This file contains the palette entry     *
// *              structure.                               *
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

typedef struct
{
    M_UINT8  paletteIndex;
    M_UINT8  redValue;
    M_UINT8  greenValue;
    M_UINT8  blueValue;

}PaletteEntryType;

typedef PaletteEntryType* PALETTE_PTR_T;

#endif