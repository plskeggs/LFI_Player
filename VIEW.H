#ifndef VIEW_H
#define VIEW_H

//
// **********************************************************
// *                                                        *
// * Copyright 1998-2022, Lazerus Laser Systems             *
// * Author: Michael G. Svob                                *
// *                                                        *
// * File Name: View.h                                      *
// * Description: This file contains the prototypes of the  *
// *              routines used for initializing and        *
// *              manipulating the orientation coordinates. *
// *                                                        *
// * This code is distributed under the GNU Public License  *
// * which can be found at...                               *
// *                                                        *
// * http://www.gnu.org/licenses/gpl.txt                    *
// *                                                        *
// **********************************************************
//

#ifndef GLOBALS_H
#include "globals.h"
#endif

#ifndef GEM_PC_H
#include "Gem_pc.h"
#endif

#ifndef JOBBLOCK_H
#include "jobblock.h"
#endif

const double VIEW_MIN = -1000000.0;
const double VIEW_MAX = 1000000.0;
    
extern void InitializeCurrentView(ViewType&  newView);

extern void InitializeCurrentView(const Point3DType fromPoint,
                                  const Point3DType atPoint,
                                  const Point3DType upVector,
                                  const double       viewAngle,
								  ViewType&         newView);

extern void Rotate_X(Gem_PC*   gemObject,
                     double     rotationAngle,
					 M_UINT16    pointToRotate,
					 M_UINT16    use0At = M_TRUE);

extern void Rotate_Y(Gem_PC*   gemObject,
                     double     rotationAngle,
					 M_UINT16    pointToRotate,
					 M_UINT16    use0At = M_TRUE);

extern void Rotate_Z(Gem_PC*   gemObject,
                     double     rotationAngle,
					 M_UINT16    pointToRotate,
					 M_UINT16    use0At = M_TRUE);

extern void  MoveForward(ViewType& theView,
                         double     increment,
						 M_UINT16    pointsToMove);

// Following are obsolete; MoveForward() used instead.
//extern void  ZoomIn(ViewType& theView,
//                    double     zoomIncrement);
//
//extern void  ZoomOut(ViewType& theView,
//                     double   zoomIncrement);

#endif

