//
// **********************************************************
// *                                                        *
// * Copyright 1998-2022, Lazerus Laser Systems             *
// * Author: Michael G. Svob                                *
// *                                                        *
// * File Name: View.cpp                                    *
// * Description: This file contains routines to initialize *
// *              and manipulate the orientation (i.e. from *
// *              point, at point, and up vector)           *
// *              coordinates.                              *
// *                                                        *
// * This code is distributed under the GNU Public License  *
// * which can be found at...                               *
// *                                                        *
// * http://www.gnu.org/licenses/gpl.txt                    *
// *                                                        *
// **********************************************************
//

/////////////////////////////////////////////////////////////
//
// Try Rick's formulas sometime...
//
// RotateZ   X := X*Cos(th) + Y(Sin(th)
//           Y := Y*Cos(th) -Y(Sin(th)
//
// RotateX   Z := Z*Cos(th) + Z(Sin(th)
//           Y := Y*Cos(th) -Y(Sin(th)
//
// RotateY   X := X*Cos(th) + Z(Sin(th)
//           Z := Z*Cos(th) -X (Sin(th)
//
/////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <cmath>
#include <fstream>

#ifndef VIEW_H
#include "view.h"
#endif

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
/*    FUNCTION: InitializeCurrentView()                            */
/*                                                                 */
/*******************************************************************/

void InitializeCurrentView(ViewType&  newView)
{
#ifdef VIEW_FUNC_ENTRY
    logFile << "Entering InitializeCurrentView()" << endl;
#endif

    newView.from_point.x = 0.0;
    newView.from_point.y = 0.0;
//  want the result to be approximately 74,750
//  newView.from_point.z = (double)((WORLD_MAX - WORLD_MIN) * 1.1406118867780);
    newView.from_point.z = 80000;

    newView.at_point.x = 0.0;
    newView.at_point.y = 0.0;
    newView.at_point.z = 0.0;

    newView.up_vector.x = 0.0;
    newView.up_vector.y = 1.0;
    newView.up_vector.z = 0.0;

    newView.view_angle = 45.0;

} // end function InitializeCurrentView()


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: InitializeCurrentView(...)                         */
/*                                                                 */
/*******************************************************************/

void
InitializeCurrentView(const Point3DType fromPoint,
                      const Point3DType atPoint,
                      const Point3DType upVector,
                      const double       viewAngle,
                      ViewType&         newView)
{
#ifdef VIEW_FUNC_ENTRY
    logFile << "Entering InitializeCurrentView(...)" << endl;
#endif

    newView.from_point.x = fromPoint.x;
    newView.from_point.y = fromPoint.y;
    newView.from_point.z = fromPoint.z;

    newView.at_point.x = atPoint.x;
    newView.at_point.y = atPoint.y;
    newView.at_point.z = atPoint.z;

    newView.up_vector.x = upVector.x;
    newView.up_vector.y = upVector.y;
    newView.up_vector.z = upVector.z;

    newView.view_angle = viewAngle;

} // end function InitializeCurrentView(...)


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: Rotate_X                                           */
/*                                                                 */
/*******************************************************************/

void Rotate_X(Gem_PC*   gemObject,
              double     rotationAngle,
              M_UINT16    pointToRotate,
              M_UINT16    use0At)
{
#ifdef VIEW_FUNC_ENTRY
    logFile << "Entering Rotate_X()" << endl;
#endif

    ViewType theView   = gemObject->GetCurrentView();
    double    rad_angle = DegreeToRadian(rotationAngle);
    Point3DType atPoint, fromPoint;

#ifdef ROTATE_TEST
    fromPoint = theView.from_point;
    fromPoint.z = (fromPoint.z * ::cos(rad_angle)) + (fromPoint.y * ::sin(rad_angle));
    fromPoint.y = (fromPoint.y * ::cos(rad_angle)) - (fromPoint.z * ::sin(rad_angle));
    theView.from_point = fromPoint;
    gemObject->SetCurrentView(theView);
    return;
#endif

    if (use0At == M_TRUE)
    {
        atPoint.x = 0.0;
        atPoint.y = 0.0;
        atPoint.z = 0.0;
        fromPoint.x = 0.0;
        fromPoint.y = 0.0;
        fromPoint.z = 0.0;
    }
    else
    {
        atPoint   = theView.at_point;
        fromPoint = theView.from_point;
    }

    MatrixType  rot_matrix;
    rot_matrix.row_1.x = 1.0;
    rot_matrix.row_1.y = 0.0;
    rot_matrix.row_1.z = 0.0;
    rot_matrix.row_2.x = 0.0;
    rot_matrix.row_2.y = (double)::cos((double)rad_angle);
    rot_matrix.row_2.z = (double)::sin((double)rad_angle);
    rot_matrix.row_3.x = 0.0;
    rot_matrix.row_3.y = -(double)::sin((double)rad_angle);
    rot_matrix.row_3.z = (double)::cos((double)rad_angle);

    if (pointToRotate == ManipulationPoint::FROM_POINT)
    {
        theView.from_point = gemObject->RotateFromPoint(rot_matrix,atPoint);
    }
    else if (pointToRotate == ManipulationPoint::AT_POINT)
    {
        theView.at_point = gemObject->RotateAtPoint(rot_matrix, fromPoint);
    }
    else
    {
        theView.from_point = gemObject->RotateFromPoint(rot_matrix,atPoint);
        theView.at_point = gemObject->RotateAtPoint(rot_matrix, fromPoint);
    }

    gemObject->SetCurrentView(theView);

} // end function Rotate_X


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: Rotate_Y                                           */
/*                                                                 */
/*******************************************************************/

void Rotate_Y(Gem_PC*   gemObject,
              double     rotationAngle,
              M_UINT16    pointToRotate,
              M_UINT16    use0At)
{
#ifdef VIEW_FUNC_ENTRY
    logFile << "Entering Rotate_Y()" << endl;
#endif

    ViewType theView   = gemObject->GetCurrentView();
    double    rad_angle = DegreeToRadian(rotationAngle);
    Point3DType atPoint, fromPoint;

#ifdef ROTATE_TEST
    fromPoint = theView.from_point;
    fromPoint.x = (fromPoint.x * ::cos(rad_angle)) + (fromPoint.z * ::sin(rad_angle));
    fromPoint.z = (fromPoint.z * ::cos(rad_angle)) - (fromPoint.x * ::sin(rad_angle));
    theView.from_point = fromPoint;
    gemObject->SetCurrentView(theView);
    return;
#endif

    if (use0At == M_TRUE)
    {
        atPoint.x = 0.0;
        atPoint.y = 0.0;
        atPoint.z = 0.0;
        fromPoint.x = 0.0;
        fromPoint.y = 0.0;
        fromPoint.z = 0.0;
    }
    else
    {
        atPoint = theView.at_point;
        fromPoint = theView.from_point;
    }

    MatrixType  rot_matrix;
    rot_matrix.row_1.x = (double)::cos((double)rad_angle);
    rot_matrix.row_1.y = 0.0;
    rot_matrix.row_1.z = -(double)::sin((double)rad_angle);
    rot_matrix.row_2.x = 0.0;
    rot_matrix.row_2.y = 1.0;
    rot_matrix.row_2.z = 0.0;
    rot_matrix.row_3.x = (double)::sin((double)rad_angle);
    rot_matrix.row_3.y = 0.0;
    rot_matrix.row_3.z = (double)::cos((double)rad_angle);

    if (pointToRotate == ManipulationPoint::FROM_POINT)
    {
        theView.from_point = gemObject->RotateFromPoint(rot_matrix,atPoint);
    }
    else if (pointToRotate == ManipulationPoint::AT_POINT)
    {
        theView.at_point = gemObject->RotateAtPoint(rot_matrix, fromPoint);
    }
    else
    {
        theView.from_point = gemObject->RotateFromPoint(rot_matrix,atPoint);
        theView.at_point = gemObject->RotateAtPoint(rot_matrix, fromPoint);
    }

    gemObject->SetCurrentView(theView);

} // end function Rotate_Y


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: Rotate_Z                                           */
/*                                                                 */
/*******************************************************************/

void Rotate_Z(Gem_PC*   gemObject,
              double     rotationAngle,
              M_UINT16    pointToRotate,
              M_UINT16    use0At)
{
#ifdef VIEW_FUNC_ENTRY
    logFile << "Entering Rotate_Z()" << endl;
#endif

    ViewType theView   = gemObject->GetCurrentView();
    double    rad_angle = DegreeToRadian(rotationAngle);
    Point3DType atPoint, fromPoint;

#ifdef ROTATE_TEST
    fromPoint = theView.from_point;
    fromPoint.x = (fromPoint.x * ::cos(rad_angle)) + (fromPoint.y * ::sin(rad_angle));
    fromPoint.y = (fromPoint.y * ::cos(rad_angle)) - (fromPoint.x * ::sin(rad_angle));
    theView.from_point = fromPoint;
    gemObject->SetCurrentView(theView);
    return;
#endif

    if (use0At == M_TRUE)
    {
        atPoint.x = 0.0;
        atPoint.y = 0.0;
        atPoint.z = 0.0;
        fromPoint.x = 0.0;
        fromPoint.y = 0.0;
        fromPoint.z = 0.0;
    }
    else
    {
        atPoint = theView.at_point;
        fromPoint = theView.from_point;
    }

    MatrixType  rot_matrix;
    rot_matrix.row_1.x = (double)::cos((double)rad_angle);
    rot_matrix.row_1.y = (double)::sin((double)rad_angle);
    rot_matrix.row_1.z = 0.0;
    rot_matrix.row_2.x = -(double)::sin((double)rad_angle);
    rot_matrix.row_2.y = (double)::cos((double)rad_angle);
    rot_matrix.row_2.z = 0.0;
    rot_matrix.row_3.x = 0.0;
    rot_matrix.row_3.y = 0.0;
    rot_matrix.row_3.z = 1.0;

    if (pointToRotate == ManipulationPoint::FROM_POINT)
    {
        theView.from_point = gemObject->RotateFromPoint(rot_matrix,atPoint);
    }
    else if (pointToRotate == ManipulationPoint::AT_POINT)
    {
        theView.at_point = gemObject->RotateAtPoint(rot_matrix, fromPoint);
    }
    else
    {
        theView.from_point = gemObject->RotateFromPoint(rot_matrix,atPoint);
        theView.at_point = gemObject->RotateAtPoint(rot_matrix, fromPoint);
    }

    gemObject->SetCurrentView(theView);

} // end function Rotate_Z


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: ZoomIn                                             */
/*                                                                 */
/*******************************************************************/
//
//void ZoomIn(ViewType& theView,
//            double     zoomIncrement)
//{
//#ifdef VIEW_FUNC_ENTRY
//    logFile << "Entering ZoomIn()" << endl;
//#endif
//
//    double newangle = theView.view_angle - zoomIncrement;
//
//    if (newangle > 0.0)
//    {
//        theView.view_angle = newangle;
//    }
//
//} // end function ZoomIn
//
//
/*******************************************************************/
/*                                                                 */
/*    FUNCTION: ZoomOut                                            */
/*                                                                 */
/*******************************************************************/
//
//void ZoomOut(ViewType& theView,
//             double   zoomIncrement)
//{
//#ifdef VIEW_FUNC_ENTRY
//    logFile << "Entering ZoomOut()" << endl;
//#endif
//    double     newangle;
//
//    if (theView.view_angle < 180.0)
//    {
//        newangle = theView.view_angle + zoomIncrement;
//        theView.view_angle = newangle;
//    }
//
//} // end function ZoomOut
//

/*******************************************************************/
/*                                                                 */
/*    LOCAL FUNCTION: AbsFloat                                     */
/*                                                                 */
/*******************************************************************/

double AbsFloat(double inputValue)
{
    if (inputValue < 0.0)
    {
        return -inputValue;
    }
    else
    {
        return inputValue;
    }
}


/*******************************************************************/
/*                                                                 */
/*    FUNCTION: MoveForward                                        */
/*                                                                 */
/*******************************************************************/

void MoveForward(ViewType& theView,
                 double     increment,
                 M_UINT16    pointsToMove)
{
#ifdef VIEW_FUNC_ENTRY
    logFile << "Entering MoveForward()" << endl;
#endif

    Point3DType currAtPoint = theView.at_point;
    Point3DType currFromPoint = theView.from_point;

    double xDiff = (currAtPoint.x) - (currFromPoint.x);
    double yDiff = (currAtPoint.y) - (currFromPoint.y);
    double zDiff = (currAtPoint.z) - (currFromPoint.z);

    if ((xDiff == 0.0 && yDiff == 0.0 && zDiff == 0.0) || increment == 0.0)
    {
        // nowhere to go
//logFile << "x,y, & zDiff all = 0.0" << endl;
        return;
    }

    double xIncrement;
    double yIncrement;
    double zIncrement;

    if (yDiff == 0.0)
    {
//logFile << "yDiff == 0.0" << endl;
        if (xDiff == 0.0)
        {
//logFile << "    xDiff == 0.0" << endl;
            // Only Z coordinate moves forward.
            zIncrement = increment;
            if (((increment > 0.0) && (AbsFloat(currAtPoint.z - (currFromPoint.z + zIncrement)) > AbsFloat(zDiff))) ||
                ((increment < 0.0) && (AbsFloat(currAtPoint.z - (currFromPoint.z + zIncrement)) < AbsFloat(zDiff)))) 
            {
                zIncrement = -zIncrement;
            }

            theView.from_point.z += zIncrement;

            if (pointsToMove == ManipulationPoint::BOTH_POINTS)
            {
                theView.at_point.z   += zIncrement;
            }
        }
        else
        {
//logFile << "    xDiff != 0.0: " << xDiff << endl;
            // That we're here indicates both xDiff and zDiff
            // are non-zero, so calculate new X and Z coordinates
            // accordingly.
            xIncrement = (double)::cos(::atan2((double)zDiff,(double)xDiff)) * increment;
            if (((increment > 0.0) && (AbsFloat(currAtPoint.x - (currFromPoint.x + xIncrement)) > AbsFloat(xDiff))) ||
                ((increment < 0.0) && (AbsFloat(currAtPoint.x - (currFromPoint.x + xIncrement)) < AbsFloat(xDiff)))) 
            {
                xIncrement = -xIncrement;
            }

            double xzSlope = zDiff / xDiff;
            double zIntercept = (currAtPoint.z) - (xzSlope * (currAtPoint.x)); 
            theView.from_point.x += xIncrement;
            theView.from_point.z = ((xzSlope * theView.from_point.x) + zIntercept);

            if (pointsToMove == ManipulationPoint::BOTH_POINTS)
            {
                theView.at_point.x   += xIncrement;
                theView.at_point.z = ((xzSlope * theView.at_point.x) + zIntercept);
            }
        } 
//logFile << "FROM: (" << theView.from_point.x << "," << theView.from_point.y << "," << theView.from_point.z << ")\n"
//        << "  AT: (" << theView.at_point.x << "," << theView.at_point.y << "," << theView.at_point.z << "\n" << endl;
    
        return;
    } // end if no Y delta

    double yIntercept;
    
    if (xDiff == 0.0)
    {
//logFile << "xDiff == 0.0" << endl;
        if (zDiff == 0.0)
        {
//logFile << "    zDiff == 0.0" << endl;
            // Only Y coordinate moves forward.
            yIncrement = increment;
            if (((increment > 0.0) && (AbsFloat(currAtPoint.y - (currFromPoint.y + yIncrement)) > AbsFloat(yDiff))) ||
                ((increment < 0.0) && (AbsFloat(currAtPoint.y - (currFromPoint.y + yIncrement)) < AbsFloat(yDiff)))) 
            {
                yIncrement = -yIncrement;
            }

            theView.from_point.y += yIncrement;

            if (pointsToMove == ManipulationPoint::BOTH_POINTS)
            {
                theView.at_point.y   += yIncrement;
            }
        }
        else
        {
//logFile << "    zDiff != 0.0: " << zDiff << endl;
            // That we're here indicates both zDiff and yDiff
            // are non-zero, so calculate new Z and Y coordinates
            // accordingly.
            zIncrement = (double)::cos(::atan2((double)yDiff,(double)zDiff)) * increment;
            if (((increment > 0.0) && (AbsFloat(currAtPoint.z - (currFromPoint.z + zIncrement)) > AbsFloat(zDiff))) ||
                ((increment < 0.0) && (AbsFloat(currAtPoint.z - (currFromPoint.z + zIncrement)) < AbsFloat(zDiff)))) 
            {
                zIncrement = -zIncrement;
            }

            double zySlope = yDiff / zDiff;
            yIntercept = (currAtPoint.y) - (zySlope * (currAtPoint.z)); 
            theView.from_point.z += zIncrement;
            theView.from_point.y = ((zySlope * theView.from_point.z) + yIntercept);

            if (pointsToMove == ManipulationPoint::BOTH_POINTS)
            {
                theView.at_point.z   += zIncrement;
                theView.at_point.y = ((zySlope * theView.at_point.z) + yIntercept);
            }
        } 
//logFile << "FROM: (" << theView.from_point.x << "," << theView.from_point.y << "," << theView.from_point.z << ")\n"
//        << "  AT: (" << theView.at_point.x << "," << theView.at_point.y << "," << theView.at_point.z << "\n" << endl;
    
        return;
    } // end if no X delta

//logFile << "Both x & yDiff are not = 0.0: (" << xDiff << "," << yDiff << ")" << endl;
    // If we get here, we know both xDiff and yDiff are non-
    // zero.
    xIncrement = (double)::cos(::atan2((double)yDiff,(double)xDiff)) * increment;
    if (((increment > 0.0) && (AbsFloat(currAtPoint.x - (currFromPoint.x + xIncrement)) > AbsFloat(xDiff))) ||
        ((increment < 0.0) && (AbsFloat(currAtPoint.x - (currFromPoint.x + xIncrement)) < AbsFloat(xDiff)))) 
    {
        xIncrement = -xIncrement;
    }

    double xySlope = yDiff / xDiff;
    yIntercept = (currAtPoint.y) - (xySlope * (currAtPoint.x)); 
    theView.from_point.x += xIncrement;
    theView.from_point.y = ((xySlope * theView.from_point.x) + yIntercept);

    if (pointsToMove == ManipulationPoint::BOTH_POINTS)
    {
        theView.at_point.x   += xIncrement;
        theView.at_point.y = ((xySlope * theView.at_point.x) + yIntercept);
    }

    if (zDiff != 0.0)
    {
//logFile << "    zDiff != 0.0: " << zDiff << endl;
        double xzSlope = zDiff / xDiff;
        double zIntercept = (currAtPoint.z) - (xzSlope * (currAtPoint.x)); 
        theView.from_point.z = ((xzSlope * theView.from_point.x) + zIntercept);

        if (pointsToMove == ManipulationPoint::BOTH_POINTS)
        {
            theView.at_point.z = ((xzSlope * theView.at_point.x) + zIntercept);
        }
    }

//logFile << "FROM: (" << theView.from_point.x << "," << theView.from_point.y << "," << theView.from_point.z << ")\n"
//        << "  AT: (" << theView.at_point.x << "," << theView.at_point.y << "," << theView.at_point.z << "\n" << endl;


} // end function MoveForward






