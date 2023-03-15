//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Gem_pc.cpp                                 *
// * Description: Implementation of the Graphics Engine    *
// * Module (GEM)Gem_PC class.  This class provides the    *
// * methods to translate between different views (e.g.    *
// * world, device), and perform the manipulations         *
// * necessary to manipulate an image in 3D space.         *
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

#ifndef GEM_PC_H
#include "Gem_pc.h"
#endif

#ifndef GLOBALS_H
#include "globals.h"
#endif

using namespace std; 

extern ofstream logFile;

/*******************************************************************/
/*                                                                 */
/*    Gem_PC Constructor (void)                                    */
/*                                                                 */
/*******************************************************************/

Gem_PC::Gem_PC():
    xmin_world(0),
    xmax_world(0),
    ymin_world(0),
    ymax_world(0),
    x_world(0),
    y_world(0),
    xmin_NDC(0),
    xmax_NDC(0),
    ymin_NDC(0),
    ymax_NDC(0),
    x_NDC(0),
    y_NDC(0),
    xmin_device(0),
    xmax_device(0),
    ymin_device(0),
    ymax_device(0),
    x_device(0),
    y_device(0)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::Gem_PC(void)" << endl;
#endif

    current_view.from_point.x = 0;
    current_view.from_point.y = 0;
    current_view.from_point.z = 0;
    current_view.at_point.x   = 0;
    current_view.at_point.y   = 0;
    current_view.at_point.z   = 0;
    current_view.up_vector.x  = 0;
    current_view.up_vector.y  = 0;
    current_view.up_vector.z  = 0;
    current_view.view_angle   = 0;

    view_matrix.row_1.x = 0;
    view_matrix.row_1.y = 0;
    view_matrix.row_1.z = 0;
    view_matrix.row_2.x = 0;
    view_matrix.row_2.y = 0;
    view_matrix.row_2.z = 0;
    view_matrix.row_3.x = 0;
    view_matrix.row_3.y = 0;
    view_matrix.row_3.z = 0;

    return;
}

/*******************************************************************/
/*                                                                 */
/*    Gem_PC Constructor (base port)                               */
/*                                                                 */
/*******************************************************************/

Gem_PC::Gem_PC(M_UINT16 base_port):
    xmin_world(0),
    xmax_world(0),
    ymin_world(0),
    ymax_world(0),
    x_world(0),
    y_world(0),
    xmin_NDC(0),
    xmax_NDC(0),
    ymin_NDC(0),
    ymax_NDC(0),
    x_NDC(0),
    y_NDC(0),
    xmin_device(0),
    xmax_device(0),
    ymin_device(0),
    ymax_device(0),
    x_device(0),
    y_device(0)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::Gem_PC(M_UINT16)" << endl;
#endif
    basePort = base_port;

    current_view.from_point.x = 0;
    current_view.from_point.y = 0;
    current_view.from_point.z = 0;
    current_view.at_point.x   = 0;
    current_view.at_point.y   = 0;
    current_view.at_point.z   = 0;
    current_view.up_vector.x  = 0;
    current_view.up_vector.y  = 0;
    current_view.up_vector.z  = 0;
    current_view.view_angle   = 0;

    view_matrix.row_1.x = 0;
    view_matrix.row_1.y = 0;
    view_matrix.row_1.z = 0;
    view_matrix.row_2.x = 0;
    view_matrix.row_2.y = 0;
    view_matrix.row_2.z = 0;
    view_matrix.row_3.x = 0;
    view_matrix.row_3.y = 0;
    view_matrix.row_3.z = 0;

    return;
}

/*******************************************************************/
/*                                                                 */
/*    Gem_PC Destructor                                            */
/*                                                                 */
/*******************************************************************/

Gem_PC::~Gem_PC()
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::~Gem_PC()" << endl;
#endif

    return;
}

/*******************************************************************/
/*                                                                 */
/*    VIRTUAL FUNCTION: WriteCurrentDeviceBuffer                   */
/*                                                                 */
/*******************************************************************/

void
Gem_PC::WriteCurrentDeviceBuffer(void)
{
    return;
}

/*******************************************************************/
/*                                                                 */
/*    INTERNAL FUNCTION: code_3D                                   */
/*                                                                 */
/*******************************************************************/

int 
Gem_PC::code_3D(Point3DType Pc)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::code_3D()" << endl;
#endif
    int c = 0;

    if (Pc.x < (-Pc.z))
        c = LEFT;
    else if (Pc.x > Pc.z)
        c = RIGHT;

    if (Pc.y < (-Pc.z))
        c = c | BOTTOM;
    else if (Pc.y > Pc.z)
        c = c | TOP;

    return(c);

} /* code_3D */

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: Clip3DLine                                         */
/*                                                                 */
/*******************************************************************/

M_UINT16 
Gem_PC::Clip3DLine(Point3DType* Pe_1,
                   Point3DType* Pe_2)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::Clip3DLine()" << endl;
#endif
    int      c, c1, c2;
    double  t;
    Point3DType  Pc_1,Pc_2,P,original_P1,original_P2;

    original_P1 = *Pe_1;
    original_P2 = *Pe_2;

    Pc_1 = mapEyeToClip(*Pe_1);
    Pc_2 = mapEyeToClip(*Pe_2);

    c1 = code_3D(Pc_1);
    c2 = code_3D(Pc_2);

    while ((c1 | c2) != 0) /* not trivially accepted */
    {
        if ((c1 & c2) != 0) /* P1P2 trivially rejected */
        {
            (*Pe_1).x = 0.0;  (*Pe_1).y = 0.0;  (*Pe_1).z = 0.0;
            (*Pe_2).x = 0.0;  (*Pe_2).y = 0.0;  (*Pe_2).z = 0.0;
            return(BOTH_CLIPPED);

        } /* if P1P2 trivially rejected */

        c = c1;

        if (c == 0)
            c = c2;

        if ((c & LEFT) != 0) /* crosses left edge */
        {
            t = (Pc_1.x + Pc_1.z) / (Pc_1.z - Pc_2.z + Pc_1.x - Pc_2.x);
            P.x = ((1 - t) * Pc_1.x) + (t * Pc_2.x);
            P.y = ((1 - t) * Pc_1.y) + (t * Pc_2.y);
            P.z = -P.x;

        }
        else if ((c & RIGHT) != 0) /* crosses right edge */
        {
            t = (Pc_1.x - Pc_1.z) / (Pc_2.z - Pc_1.z + Pc_1.x - Pc_2.x);
            P.x = ((1 - t) * Pc_1.x) + (t * Pc_2.x);
            P.y = ((1 - t) * Pc_1.y) + (t * Pc_2.y);
            P.z = P.x;

        }
        else if ((c & BOTTOM) != 0) /* crosses bottom edge */
        {
            t = (Pc_1.y + Pc_1.z) / (Pc_1.z - Pc_2.z + Pc_1.y - Pc_2.y);
            P.x = ((1 - t) * Pc_1.x) + (t * Pc_2.x);
            P.y = ((1 - t) * Pc_1.y) + (t * Pc_2.y);
            P.z = -P.y;

        }
        else if ((c & TOP) != 0) /* crosses top edge */
        {
            t = (Pc_1.y - Pc_1.z) / (Pc_2.z - Pc_1.z + Pc_1.y - Pc_2.y);
            P.x = ((1 - t) * Pc_1.x) + (t * Pc_2.x);
            P.y = ((1 - t) * Pc_1.y) + (t * Pc_2.y);
            P.z = P.y;

        }

        if (c == c1)
        {
            Pc_1 = P;
            (*Pe_1) = mapClipToEye(P);
            c1 = code_3D(P);

        } /* if c equals c1 */
        else
        {
            Pc_2 = P;
            (*Pe_2) = mapClipToEye(P);
            c2 = code_3D(P);

        } /* else */

    } /* end while */

    if (vectorsEqual(*Pe_1,original_P1) == M_FALSE)
    {
        return(P1_CLIPPED);
    }
    else if (vectorsEqual(*Pe_2,original_P2) == M_FALSE)
    {
        return(P2_CLIPPED);
    }
    else
    {
        return(NONE_CLIPPED);
    }

} /* Clip3DLine */


/*******************************************************************/
/*                                                                 */
/*    INTERNAL FUNCTION: mapNDCToWorld                             */
/*                                                                 */
/*******************************************************************/

void 
Gem_PC::mapNDCToWorld(double x_NDC,
                      double y_NDC,
                      double* x_world,
                      double* y_world)

{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::mapNDCToWorld()" << endl;
#endif
    double   a, b, c, d;

    a = ((xmax_world-xmin_world)/(xmax_NDC-xmin_NDC));
    b = xmin_world-(a*xmin_NDC);
    c = ((ymax_world-ymin_world)/(ymax_NDC-ymin_NDC));
    d = ymin_world-(c*ymin_NDC);

    *x_world = (a*x_NDC)+b;
    *y_world = (c*y_NDC)+d;

} /* mapNDCToWorld */


/*******************************************************************/
/*                                                                 */
/*    INTERNAL FUNCTION: mapWorldToNDC                             */
/*                                                                 */
/*******************************************************************/

void 
Gem_PC::mapWorldToNDC(double x_world,
                      double y_world,
                      double* x_NDC,
                      double* y_NDC)

{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::mapWorldToNDC()" << endl;
#endif
    double   a, b, c, d;

    a = ((xmax_NDC-xmin_NDC)/(xmax_world-xmin_world));
    b = xmin_NDC-(a*xmin_world);
    c = ((ymax_NDC-ymin_NDC)/(ymax_world-ymin_world));
    d = ymin_NDC-(c*ymin_world);

    *x_NDC = (a*x_world)+b;
    *y_NDC = (c*y_world)+d;

} /* mapWorldToNDC */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: MapDeviceToNDC                                     */
/*                                                                 */
/*******************************************************************/

void 
Gem_PC::MapDeviceToNDC(long x_device,
                       long y_device,
                       double* x_NDC,
                       double* y_NDC)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::MapDeviceToNDC()" << endl;
#endif
    double   a, b, c, d;
    double   x_min_d, x_max_d, y_min_d, y_max_d;
    double   xd_temp, yd_temp;

    x_min_d = (double)xmin_device;
    x_max_d = (double)xmax_device;
    y_min_d = (double)ymin_device;
    y_max_d = (double)ymax_device;

    xd_temp = (double)x_device;
    yd_temp = (double)y_device;

    a = 1/(x_max_d-x_min_d);
    b = 0-(a*x_min_d);
    c = 1/(y_max_d-y_min_d);
    d = 0-(c*y_min_d);

    *x_NDC = (a*xd_temp)+b;
    *y_NDC = (c*yd_temp)+d;

} /* MapDeviceToNDC */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: MapNDCToDevice                                     */
/*                                                                 */
/*******************************************************************/

void 
Gem_PC::MapNDCToDevice(double x_NDC,
                       double y_NDC,
                       long* x_device,
                       long* y_device)

{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::MapNDCToDevice()" << endl;
#endif
    double   a, b, c, d;
    double   temp_x, temp_y;
    double   x_min_d, x_max_d, y_min_d, y_max_d;

    x_min_d = (double)xmin_device;
    x_max_d = (double)xmax_device;
    y_min_d = (double)ymin_device;
    y_max_d = (double)ymax_device;

    a = (x_max_d-x_min_d);
    b = x_min_d;
    c = (y_max_d-y_min_d);
    d = y_min_d;

    temp_x = (a*x_NDC)+b;
    temp_y = (c*y_NDC)+d;

    *x_device = (long)temp_x;
    *y_device = (long)temp_y;

} /* MapNDCToDevice */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: MapDeviceToWorld                                   */
/*                                                                 */
/*******************************************************************/

void 
Gem_PC::MapDeviceToWorld(long x_device,
                         long y_device,
                         double* x_world,
                         double* y_world)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::MapDeviceToWorld()" << endl;
#endif
    double   x_NDC, y_NDC;

    MapDeviceToNDC(x_device, y_device, &x_NDC, &y_NDC);
    mapNDCToWorld(x_NDC, y_NDC, x_world, y_world);

} /* MapDeviceToWorld */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: MapWorldToDevice                                   */
/*                                                                 */
/*******************************************************************/

void 
Gem_PC::MapWorldToDevice(double x_world,
                         double y_world,
                         long* x_device,
                         long* y_device)

{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::MapWorldToDevice()" << endl;
#endif
    double   x_NDC, y_NDC;

    mapWorldToNDC(x_world, y_world, &x_NDC, &y_NDC);
    MapNDCToDevice(x_NDC, y_NDC, x_device, y_device);

} /* MapWorldToDevice */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: SetViewport                                        */
/*                                                                 */
/*******************************************************************/

int 
Gem_PC::SetViewport(double xv_min,
                    double xv_max,
                    double yv_min,
                    double yv_max)

{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::SetViewport()" << endl;
#endif
    double   delta_x, delta_y;
    long   xd_min, xd_max, yd_min, yd_max;

    delta_x = xv_max-xv_min;
    delta_y = yv_max-yv_min;

    if ((delta_x != 0) && (delta_y != 0))
    {
        xmin_NDC = xv_min;
        xmax_NDC = xv_max;
        ymin_NDC = yv_min;
        ymax_NDC = yv_max;

        MapNDCToDevice(xv_min, yv_min, &xd_min, &yd_min);
        MapNDCToDevice(xv_max, yv_max, &xd_max, &yd_max);

        return(0);
    } /* if viewport coordinates are valid */

    else return(1);

} /* SetViewport */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: GetViewport                                        */
/*                                                                 */
/*******************************************************************/

void 
Gem_PC::GetViewport(double* xv_min,
                    double* xv_max,
                    double* yv_min,
                    double* yv_max)

{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::GetViewport()" << endl;
#endif
        *xv_min = xmin_NDC;
        *xv_max = xmax_NDC;
        *yv_min = ymin_NDC;
        *yv_max = ymax_NDC;

} /* GetViewport */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: SetWindow                                          */
/*                                                                 */
/*******************************************************************/

int 
Gem_PC::SetWindow(double xw_min,
                  double xw_max,
                  double yw_min,
                  double yw_max)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::SetWindow()" << endl;
#endif
    double   delta_x, delta_y;

    delta_x = xw_max-xw_min;
    delta_y = yw_max-yw_min;

    if (delta_x < 0)
        return(1);

    else if (delta_y < 0)
        return(1);

    else if (!((delta_x > 0) && (delta_y > 0)))
        return(1);

    else
    {
        xmin_world = xw_min;
        xmax_world = xw_max;
        ymin_world = yw_min;
        ymax_world = yw_max;
        return(0);
    } /* else window coordinates are valid */

} /* SetWindow */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: Move                                               */
/*                                                                 */
/*******************************************************************/

void 
Gem_PC::Move(double x_world,
             double y_world)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::Move()" << endl;
#endif
    long   temp_x, temp_y;

    MapWorldToDevice(x_world, y_world, &temp_x, &temp_y);
//    moveto(temp_x, temp_y);

} /* Move */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: Draw                                            */
/*                                                                 */
/*******************************************************************/

void 
Gem_PC::Draw(double x_world,
             double y_world)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::Draw()" << endl;
#endif
    long   temp_x, temp_y;

    MapWorldToDevice(x_world, y_world, &temp_x, &temp_y);
//    lineto(temp_x, temp_y);

} /* Draw */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: NDCMove                                            */
/*                                                                 */
/*******************************************************************/
void 
Gem_PC::NDCMove(CClientDC* dc,
                double      x_NDC,
                double      y_NDC)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::NDCMove()" << endl;
#endif
    long   temp_x, temp_y;

    MapNDCToDevice(x_NDC, y_NDC, &temp_x, &temp_y);
    dc->MoveTo(temp_x, temp_y);

} /* NDCMove */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: NDCDraw                                            */
/*                                                                 */
/*******************************************************************/

void 
Gem_PC::NDCDraw(CClientDC* dc,
                double      x_NDC,
                double      y_NDC)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::NDCDraw()" << endl;
#endif
    long   temp_x, temp_y;

    MapNDCToDevice(x_NDC, y_NDC, &temp_x, &temp_y);
    dc->LineTo(temp_x, temp_y);

} /* NDCDraw */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: SetDeviceWindow                                    */
/*                                                                 */
/*******************************************************************/

void 
Gem_PC::SetDeviceWindow(long xMinDev,
                        long xMaxDev,
                        long yMinDev,
                        long yMaxDev)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::SetDeviceWindow()" << endl;
#endif
    xmin_device = xMinDev;
    xmax_device = xMaxDev;
    ymin_device = yMinDev;
    ymax_device = yMaxDev;

} /* SetDeviceWindow */


/*******************************************************************/
/*                   VECTOR & MATRIX FUNCTIONS                     */
/*******************************************************************/
/*******************************************************************/
/*                                                                 */
/*    INTERNAL FUNCTION: vectorsEqual                              */
/*                                                                 */
/*******************************************************************/

M_UINT16 
Gem_PC::vectorsEqual(Point3DType v1,
                     Point3DType v2)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::vectorsEqual()" << endl;
#endif
    M_UINT16  v_equal = M_FALSE;

    if (v1.x == v2.x)
        if (v1.y == v2.y)
            if (v1.z == v2.z)
                v_equal = M_TRUE;

    return(v_equal);

} /* end internal function vectorsEqual */


/*******************************************************************/
/*                                                                 */
/*    INTERNAL FUNCTION: addVectors                                */
/*                                                                 */
/*******************************************************************/

struct Point3DType 
Gem_PC::addVectors(Point3DType v1,
                   Point3DType v2)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::addVectors()" << endl;
#endif
    Point3DType tempsum;

    tempsum.x = v1.x + v2.x;
    tempsum.y = v1.y + v2.y;
    tempsum.z = v1.z + v2.z;

    return(tempsum);

} /* end internal function addVectors */


/*******************************************************************/
/*                                                                 */
/*    INTERNAL FUNCTION: subtractVectors                           */
/*                                                                 */
/*******************************************************************/

struct Point3DType 
Gem_PC::subtractVectors(Point3DType v1,
                        Point3DType v2)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::subtractVectors()" << endl;
#endif
    Point3DType tempdiff;

    tempdiff.x = v1.x - v2.x;
    tempdiff.y = v1.y - v2.y;
    tempdiff.z = v1.z - v2.z;

    return(tempdiff);

} /* end internal function subtractVectors */


/*******************************************************************/
/*                                                                 */
/*    INTERNAL FUNCTION: vectorMagnitude                           */
/*                                                                 */
/*******************************************************************/

double 
Gem_PC::vectorMagnitude(Point3DType vector)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::vectorMagnitude()" << endl;
#endif
    double x_sqrd,y_sqrd,z_sqrd;
    double vec_mag;
    double  returnview_matrixal;

    x_sqrd = pow((double)(vector.x),2);
    y_sqrd = pow((double)(vector.y),2);
    z_sqrd = pow((double)(vector.z),2);

    vec_mag = sqrt(x_sqrd+y_sqrd+z_sqrd);
    returnview_matrixal = vec_mag;

    return(returnview_matrixal);

} /* end internal function vectorMagnitude */


/*******************************************************************/
/*                                                                 */
/*    INTERNAL FUNCTION: normalizeVector                           */
/*                                                                 */
/*******************************************************************/

struct Point3DType 
Gem_PC::normalizeVector(Point3DType vector)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::normalizeVector()" << endl;
#endif
    Point3DType  tempvec;
    double               tempmag;

    tempmag = vectorMagnitude(vector);

    tempvec.x = vector.x/tempmag;
    tempvec.y = vector.y/tempmag;
    tempvec.z = vector.z/tempmag;

    return(tempvec);

} /* end internal function normalizeVector */


/*******************************************************************/
/*                                                                 */
/*    INTERNAL FUNCTION: crossProduct                              */
/*                                                                 */
/*******************************************************************/

struct Point3DType 
Gem_PC::crossProduct(Point3DType v1,
                     Point3DType v2)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::crossProduct()" << endl;
#endif
    Point3DType tempprod;

    tempprod.x = ((v1.y*v2.z)-(v2.y*v1.z));
    tempprod.y = ((v1.z*v2.x)-(v2.z*v1.x));
    tempprod.z = ((v1.x*v2.y)-(v2.x*v1.y));

    return(tempprod);

} /* end internal function crossProduct */


/*******************************************************************/
/*                                                                 */
/*    INTERNAL FUNCTION: transposeMatrix                           */
/*                                                                 */
/*******************************************************************/

struct MatrixType 
Gem_PC::transposeMatrix(MatrixType matrix)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::transposeMatrix()" << endl;
#endif
    MatrixType  temp_matrix;

    temp_matrix.row_1.x = matrix.row_1.x;
    temp_matrix.row_1.y = matrix.row_2.x;
    temp_matrix.row_1.z = matrix.row_3.x;

    temp_matrix.row_2.x = matrix.row_1.y;
    temp_matrix.row_2.y = matrix.row_2.y;
    temp_matrix.row_2.z = matrix.row_3.y;

    temp_matrix.row_3.x = matrix.row_1.z;
    temp_matrix.row_3.y = matrix.row_2.z;
    temp_matrix.row_3.z = matrix.row_3.z;

    return(temp_matrix);

} /* end internal function transposeMatrix */


/*******************************************************************/
/*                                                                 */
/*    INTERNAL FUNCTION: vectorTimesMatrix                         */
/*                                                                 */
/*******************************************************************/

struct Point3DType 
Gem_PC::vectorTimesMatrix(Point3DType v1,
                          MatrixType m1)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::vextorTimesMatrix()" << endl;
#endif
    Point3DType  tempvec;

    tempvec.x = (v1.x*m1.row_1.x) + (v1.y*m1.row_2.x) + (v1.z*m1.row_3.x);
    tempvec.y = (v1.x*m1.row_1.y) + (v1.y*m1.row_2.y) + (v1.z*m1.row_3.y);
    tempvec.z = (v1.x*m1.row_1.z) + (v1.y*m1.row_2.z) + (v1.z*m1.row_3.z);

    return(tempvec);

} /* end internal function vectorTimesMatrix */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: Find_V_Matrix                                      */
/*                                                                 */
/*******************************************************************/

void 
Gem_PC::Find_V_Matrix()
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::Find_V_Matrix()" << endl;
#endif
    MatrixType  temp_matrix;
    Point3DType  a,b,c;
    Point3DType  A,F,AminF,U;
    Point3DType  tempvec;

    A = current_view.at_point;
    F = current_view.from_point;
    U = current_view.up_vector;

    AminF = subtractVectors(A,F);
    c = normalizeVector(AminF);

    tempvec = crossProduct(AminF,U);
    a = normalizeVector(tempvec);

    tempvec = crossProduct(tempvec,AminF);
    b = normalizeVector(tempvec);

    temp_matrix.row_1 = a;
    temp_matrix.row_2 = b;
    temp_matrix.row_3 = c;

    temp_matrix = transposeMatrix(temp_matrix);

    view_matrix = temp_matrix;

} /* end internal function Find_V_Matrix */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: RotateFromPoint                                    */
/*                                                                 */
/*******************************************************************/

struct Point3DType 
Gem_PC::RotateFromPoint(MatrixType  rotation_matrix,
                        Point3DType atPoint)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::RotateFromPoint()" << endl;
#endif
    Point3DType  tempvec;

    tempvec = subtractVectors(current_view.from_point,atPoint);
    tempvec = vectorTimesMatrix(tempvec,rotation_matrix);
    tempvec = addVectors(tempvec,atPoint);

    return(tempvec);

} /* end internal function RotateFromPoint */

/*******************************************************************/
/*                                                                 */
/*    FUNCTION: RotateAtPoint                                      */
/*                                                                 */
/*******************************************************************/

struct Point3DType 
Gem_PC::RotateAtPoint(MatrixType  rotation_matrix,
                      Point3DType fromPoint)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::RotateAtPoint()" << endl;
#endif
    Point3DType  tempvec;

    tempvec = subtractVectors(current_view.at_point,fromPoint);
    tempvec = vectorTimesMatrix(tempvec,rotation_matrix);
    tempvec = addVectors(tempvec,fromPoint);

    return(tempvec);

} /* end internal function RotateAtPoint */

/*******************************************************************/
/*                      MAPPING FUNCTIONS                          */
/*******************************************************************/

/*******************************************************************/
/*                                                                 */
/*    INTERNAL FUNCTION: mapEyeToClip                              */
/*                                                                 */
/*******************************************************************/

struct Point3DType 
Gem_PC::mapEyeToClip(Point3DType Pe)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::mapEyeToClip()" << endl;
#endif
    Point3DType  tempvec;
    double     rad_angle;

    rad_angle = DegreeToRadian(current_view.view_angle);

    tempvec.x = (double)(Pe.x / tan(rad_angle/2.0));
    tempvec.y = (double)(Pe.y / tan(rad_angle/2.0));
    tempvec.z = (double)Pe.z;

    return(tempvec);

} /* end internal function mapEyeToClip */


/*******************************************************************/
/*                                                                 */
/*    INTERNAL FUNCTION: mapClipToEye                              */
/*                                                                 */
/*******************************************************************/

struct Point3DType 
Gem_PC::mapClipToEye(Point3DType Pc)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::mapClipToEye()" << endl;
#endif
    Point3DType  tempvec;
    double     rad_angle;

    rad_angle = DegreeToRadian(current_view.view_angle);

    tempvec.x = (double)(Pc.x * tan(rad_angle/2.0));
    tempvec.y = (double)(Pc.y * tan(rad_angle/2.0));
    tempvec.z = (double)Pc.z;

    return(tempvec);

} /* end internal function mapClipToEye */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: MapWorldToEye                                      */
/*                                                                 */
/*******************************************************************/

struct Point3DType 
Gem_PC::MapWorldToEye(Point3DType Pw)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::MapWorldToEye()" << endl;
#endif
    Point3DType tempvec;

    tempvec = subtractVectors(Pw,current_view.from_point);
    tempvec = vectorTimesMatrix(tempvec,view_matrix);

    return(tempvec);

} /* end internal function MapWorldToEye */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: MapEyeToNDC                                        */
/*                                                                 */
/*******************************************************************/

void 
Gem_PC::MapEyeToNDC(Point3DType Pe,
         double* x_NDC,
         double* y_NDC)
{
#ifdef GEM_ENTRY
    logFile << "Entering Gem_PC::MapEyeToNDC()" << endl;
#endif
    double  Vxl,Vxr,Vyt,Vyb;
    double    v;

    Vxl = xmin_NDC;
    Vxr = xmax_NDC;
    Vyb = ymin_NDC;
    Vyt = ymax_NDC;

    v = DegreeToRadian(current_view.view_angle);

    if (Pe.z != 0.0)
    {
        (*x_NDC) = (double)(((Vxr-Vxl)/(2*tan(v/2)))*(Pe.x/Pe.z))+((Vxr+Vxl)/2.0);
        (*y_NDC) = (double)(((Vyt-Vyb)/(2*tan(v/2)))*(Pe.y/Pe.z))+((Vyt+Vyb)/2.0);

    } /* end if not divide by zero */

    else if ((Pe.x == 0.0) && (Pe.y == 0.0) && (Pe.z == 0.0))
    {
        (*x_NDC) = 0.0;
        (*y_NDC) = 0.0;

    } /* end else if */

} /* end internal function MapEyeToNDC */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: DegreeToRadian                                     */
/*                                                                 */
/*******************************************************************/
//
//double 
//Gem_PC::DegreeToRadian(double deg_angle)
//{
//#ifdef GEM_ENTRY
//    logFile << "Entering Gem_PC::DegreeToRadian()" << endl;
//#endif
//    double  rad_angle;
//
//    rad_angle = deg_angle*(PI/180.0);
//    return(rad_angle);
//
//} /* end internal function DegreeToRadian */


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: RealAbs                                            */
/*                                                                 */
/*******************************************************************/

double
Gem_PC::RealAbs(double inVal)
{
    if (inVal < 0.0)
        return -inVal;
    else
        return inVal;

} /* end function RealAbs */



