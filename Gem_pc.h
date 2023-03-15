#ifndef GEM_PC_H
#define GEM_PC_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Gem_pc.h                                   *
// * Description: This file contains the function          *
// *              prototypes and miscellaneous constants   *
// *              and type declarations supporting the     *
// *              Graphics Engine Module (Gem_PC) class.   *
// *              Note that, with a few exceptions, this   *
// *              code was written to be readilly portable *
// *              and easilly integrated into a variety of *
// *              applications.                            *
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

#include <afxwin.h>

#define LEFT      1
#define RIGHT     2
#define BOTTOM    4
#define TOP       8
#define MAXSTRING 80

#define DEFAULT_DEVICE  0

typedef struct Point3DType
{
    double   x,y,z;

}Point3DType;


typedef enum
{
    NONE      = 0,
    GWS       = 1,
    BSOFT_RGB = 2,
    BSOFT_XYI = 3,
    RIYA      = 4,
    ACCESIO   = 5,
    EZAUDDAC  = 6,
    EASYLASE  = 7,
    HELIOS    = 8
} OutputDeviceTypeT;

typedef struct MatrixType
{
    Point3DType row_1;
    Point3DType row_2;
    Point3DType row_3;

}MatrixType;

typedef struct ViewType
{
    Point3DType  from_point;
    Point3DType  at_point;
    Point3DType  up_vector;
    double        view_angle;

}ViewType;

class Gem_PC
{
public:
    const enum pointsClipped {NONE_CLIPPED = 0,
                              P1_CLIPPED   = 1,
                              P2_CLIPPED   = 2,
                              BOTH_CLIPPED = 3};

    Gem_PC(void);
    Gem_PC(M_UINT16);
    virtual ~Gem_PC(void);

    void MapDeviceToNDC(long, long, double *, double *);
    void MapNDCToDevice(double, double, long *, long *);
    void MapDeviceToWorld(long, long, double *, double *);
    void MapWorldToDevice(double, double, long *, long*);
    int  SetViewport(double, double, double, double);
    void GetViewport(double *, double *, double *, double *);
    int  SetWindow(double, double, double, double);
    void SetDeviceWindow(long, long, long, long);
    void Move(double, double);
    void Draw(double, double);
    void NDCMove(CClientDC*, double, double);
    void NDCDraw(CClientDC*, double, double);
    void Find_V_Matrix();
//    double DegreeToRadian(double);
    ViewType GetCurrentView() {return current_view;}
    void SetCurrentView(const ViewType& newView)
                              {current_view = newView;}
    void               MapEyeToNDC(Point3DType, double *, double *);
    struct Point3DType MapWorldToEye(Point3DType);
    struct Point3DType RotateFromPoint(MatrixType,Point3DType);
    struct Point3DType RotateAtPoint(MatrixType,Point3DType);
    M_UINT16 Clip3DLine(Point3DType *, Point3DType *);

//    virtual void MoveLaser(const double,
//		           const double,
//				   const M_UINT16) = 0;
//    virtual void DrawLaser(const double,
//                   const double,
//                   const M_UINT8,
//                   const M_UINT8,
//                   const M_UINT8,
//                   const M_UINT8,
//				     const M_UINT16) = 0;
    virtual void DrawLaser(const OutputPoint_T,
                           const M_UINT16) = 0;

    virtual void DisplayOff(const M_UINT16, const enum DisplayColorMode colorMode) = 0;
    virtual void LaserOff(const M_UINT16, const enum DisplayColorMode colorMode) = 0;

    virtual void WriteCurrentDeviceBuffer(void);

    double RealAbs(double inVal);

    OutputDeviceTypeT GetOutputDeviceType(void){return outputDeviceType;}
    void SetOutputDeviceType(OutputDeviceTypeT newType){outputDeviceType = newType;}

//    M_UINT16 GetDeviceMin(void){return deviceMin;}
//    M_UINT16 GetDeviceMax(void){return deviceMax;}
    
    long GetDeviceMin(void){return deviceMin;}
    long GetDeviceMax(void){return deviceMax;}

    void SetBasePort(M_UINT16 newPort) {basePort = newPort;}
    M_UINT16 GetBasePort() {return basePort;}

protected:
    long last_X;
    long last_Y;
    long deviceMin;
    long deviceMax;
    M_UINT16 maxBufferPoints;

    OutputDeviceTypeT outputDeviceType;
    M_UINT16 basePort;

private:
    // private methods
    M_UINT16           vectorsEqual(Point3DType, Point3DType);
    int                code_3D(Point3DType);
    void               mapNDCToWorld(double, double, double *, double *);
    void               mapWorldToNDC(double, double, double *, double *);

    struct Point3DType addVectors(Point3DType, Point3DType);
    struct Point3DType subtractVectors(Point3DType, Point3DType);
    double              vectorMagnitude(Point3DType);
    struct Point3DType normalizeVector(Point3DType);
    struct Point3DType crossProduct(Point3DType, Point3DType);
    struct MatrixType  transposeMatrix(MatrixType);
    struct Point3DType vectorTimesMatrix(Point3DType,MatrixType);
    struct Point3DType mapEyeToClip(Point3DType);
    struct Point3DType mapClipToEye(Point3DType);

    // private attributes
    double   xmin_world;
    double   xmax_world;
    double   ymin_world;
    double   ymax_world;
    double   x_world;
    double   y_world;

    double   xmin_NDC;
    double   xmax_NDC;
    double   ymin_NDC;
    double   ymax_NDC;
    double   x_NDC;
    double   y_NDC;

    long    xmin_device;
    long    xmax_device;
    long    ymin_device;
    long    ymax_device;
    long    x_device;
    long    y_device;

    struct  ViewType   current_view;
    struct  MatrixType view_matrix;

};

#endif