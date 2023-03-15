#ifndef ROUTINES_H
#define ROUTINES_H

//
// ****************************************************************
// *                                                              *
// * Copyright 1998-2022, Lazerus Laser Systems                   *
// * Author: Michael G. Svob                                      *
// *                                                              *
// * File Name: Routines.h                                        *
// * Description: This file contains the function prototypes for  *
// *               all of the miscellaneous helper and wrapper    *
// *               routines which can't find a home elsewhere.    *
// *                                                              *
// * This code is distributed under the GNU Public License        *
// * which can be found at...                                     *
// *                                                              *
// * http://www.gnu.org/licenses/gpl.txt                          *
// *                                                              *
// ****************************************************************
//

#include <stdio.h>

#ifndef FRAMESEQUENCE_H
#include "FrameSeq.h"
#endif

#ifndef GLOBALS_H
#include "globals.h"
#endif

#include "Serial.h"

typedef enum
{
    SERIAL_INVALID = 0,
    SERIAL_PAGE    = 1,
    SERIAL_CUE     = 2

} SerialCommandT;

extern SerialCommandT ProcessSerialString(char* serialString,
                                          M_UINT16* commandValue);

extern void AdjustColor(enum DisplayColorMode  colorMode,
                        UINT                   colorIntensity,
                        double                 redThreshold,
                        double                 greenThreshold,
                        double                 blueThreshold,
                        M_UINT8*                 redValue,
                        M_UINT8*                 greenValue,
                        M_UINT8*                 blueValue);

extern double ResetCycle(double cycleValue, double cycleThreshold);
extern double ResetCycle(double cycleValue, double cycleThreshold, M_UINT16 &blnkThis);
extern void MapAbsToNDC(const M_INT16 x_abstract,
                        const M_INT16 y_abstract,
                        double* x_NDC,
                        double* y_NDC);
extern void MapNDCToRadian(const double x_NDC,
                           const double y_NDC,
                           double* x_radian,
                           double* y_radian);

extern void GenericMapAtoB(const double x_A,
                           const double y_A,
                           const double A_min,
                           const double A_max,
                           const double B_min,
                           const double B_max,
                           double* x_B,
                           double* y_B);

extern M_UINT16 GetWord(FILE*, short&);

extern void PutWord(FILE* outFile, M_INT16 theWord);
extern void PutByte(FILE* outFile, M_UINT8 theByte);

extern M_UINT8 GetMsbFromUshort(M_UINT16 word);
extern M_UINT8 GetLsbFromUshort(M_UINT16 word);

extern M_UINT16 ReadFile(FILE*, FrameSequence*);

extern M_UINT16 DoReadILDAFile(const char* const fileName,
                             FrameSequence**  frameSeqPtr);

extern int comStringToIndex(CString comString);
extern int baudrateToIndex (CSerial::EBaudrate eBaudrate);
extern int databitsToIndex (CSerial::EDataBits eDataBits);
extern int parityToIndex   (CSerial::EParity eParity);
extern int stopbitsToIndex (CSerial::EStopBits  eStopBits);
extern int handshakeToIndex(CSerial::EHandshake eHandshake);

extern CSerial::EBaudrate  indexToBaudrate(int index);
extern CSerial::EDataBits  indexToDatabits(int index);
extern CSerial::EParity    indexToParity(int index);
extern CSerial::EStopBits  indexToStopbits(int index);
extern CSerial::EHandshake indexToHandshake(int index);



#endif