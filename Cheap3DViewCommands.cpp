//
// *********************************************************
// *                                                       *
// * Copyright 1998-2005, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Cheap3DViewCommands.cpp                    *
// * Description: Contains the implementation of those     *
// * methods on the Cheap3DView class which pertain to the *
// * manipulation of images, particularly under script     *
// * "command" control.                                    *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#include <iostream>
#include <fstream>

using namespace std;
using std::cout;

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::DoTrackManipulation()
//
////////////////////////////////////////////////////////////////////

USHORT
CCheap3DView::DoTrackManipulation(void)
{
    USHORT retVal = 0;
    trackPtr = trackList.GetTrack(trackNumber);
    if (!trackPtr)
    {
#ifdef CHEAP3DVIEW_LOG_WARNING
    logFile << "DoTrackManipulation(): NULL track pointer." << endl;
#endif
        return M_FAILURE;
    }

    displayView = trackPtr->GetTrackView();
    displayParams = trackPtr->GetTrackParams();

    switch(trackPtr->jobBlock.functionType)
    {
        case JUST_DISPLAY:
            retVal = DoJustDisplay();
            break;
        case ROTATE_FROM:
            retVal = DoRotate(ManipulationPoint::FROM_POINT);
            break;
        case ROTATE_AT:
            retVal = DoRotate(ManipulationPoint::AT_POINT);
            break;
        case ROTATE_BOTH:
            retVal = DoRotate(ManipulationPoint::BOTH_POINTS);
            break;
        case FADE:
            retVal = DoFade();
            break;
        case ZOOM:
            retVal = DoZoom();
            break;
        case MOVE_FORWARD:
            retVal = DoMoveForward(BOTH_POINTS);
            break;
        case MOVE_AT:
            retVal = DoMoveAt(M_TRUE);
            break;
        case MOVE_FROM:
            retVal = DoMoveFrom(M_TRUE);
            break;
        case MOVE_BOTH:
            {
                // only decrement frame count once
                retVal = DoMoveAt(M_TRUE);
                retVal = DoMoveFrom(M_FALSE);
            }
            break;
//		case CROSS_FADE:
//            retVal = DoCrossFade();
//			break;
        default:
            break;
    }
                        
    if (!retVal)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
    logFile << "DoTrackManipulation(): Command in progress function call failure." << endl;
#endif
    }

    return retVal;

} // end CCheap3DView::DoTrackManipulation()


////////////////////////////////////////////////////////////////////
//
// CCheap3DView::DoJustDisplay()
//
////////////////////////////////////////////////////////////////////

USHORT
CCheap3DView::DoJustDisplay(void)
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering DoJustDisplay()" << endl;
#endif

    --trackPtr->jobBlock.numOfFrames;

    return(M_SUCCESS);

} // end CCheap3DView::DoJustDisplay

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::DoCrossFade()
//
////////////////////////////////////////////////////////////////////
//
//USHORT
//CCheap3DView::DoCrossFade(void)
//{
//#ifdef CHEAP3DVIEW_LOG_INFO
//    logFile << "Entering CCheap3DView::DoCrossFade()" << endl;
//#endif
//
//    Track* fadeInTrack  = trackList.GetTrack(trackPtr->jobBlock.trackNumber);
//    Track* fadeOutTrack = trackList.GetTrack(trackPtr->jobBlock.trackNumberToo);
//
//	if (!fadeInTrack || !fadeOutTrack)
//	{
//#ifdef CHEAP3DVIEW_LOG_ERROR
//        logFile << "DoFade(): NULL Track pointer." << endl;
//#endif
//        return M_FAILURE;
//	}
//
//	DisplayParams fadeInParams  = fadeInTrack->GetTrackParams();
//	DisplayParams fadeOutParams = fadeOutTrack->GetTrackParams();
//
//    if (trackPtr->jobBlock.reentry == M_FALSE)
//	{
//	    // first pass through
//	    trackPtr->jobBlock.fadeIncrement = 1.0 / (float)(trackPtr->jobBlock.numOfFrames);
//	    fadeInParams.colorFactor  = 0.0;
//		fadeOutParams.colorFactor = 1.0;
//
//		trackPtr->jobBlock.reentry = M_TRUE;
//	}
//
//	if (fadeInParams.colorFactor < 1.0)
//	{
//        fadeInParams.colorFactor = 
//            fadeInParams.colorFactor + trackPtr->jobBlock.fadeIncrement;
//
//        if (fadeInParams.colorFactor > 1.0)
//        {
//            fadeInParams.colorFactor = 1.0;
//        }
//    }
//
//    if (fadeOutParams.colorFactor > 0.0)
//    {
//        fadeOutParams.colorFactor = 
//            fadeOutParams.colorFactor - trackPtr->jobBlock.fadeIncrement;
//
//        if (fadeOutParams.colorFactor < 0.0)
//        {
//            fadeOutParams.colorFactor = 0.0;
//        }
//    }
//
//	fadeInTrack->SetTrackParams(fadeInParams);
//    fadeOutTrack->SetTrackParams(fadeOutParams);
//    DisplayAllTracks();
//
//    if (!(--trackPtr->jobBlock.numOfFrames))
//    {
//        fadeInParams.colorFactor = 1.0;
//	    fadeOutParams.colorFactor = 0.0;
//   }
//
//	fadeInTrack->SetTrackParams(fadeInParams);
//	fadeOutTrack->SetTrackParams(fadeOutParams);
//
//    return(M_SUCCESS);
//
//} // end CCheap3DView::DoCrossFade

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::DoFade()
//
////////////////////////////////////////////////////////////////////

USHORT
CCheap3DView::DoFade(void)
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering DoFade()" << endl;
#endif

    if(trackPtr->jobBlock.numOfFrames == 0)
        return(M_SUCCESS);

    if (trackPtr->jobBlock.reentry == M_FALSE)
    {
        // first pass through
        if (trackPtr->jobBlock.fadeDirection == FADE_IN)
        {
            if (displayParams.colorFactor < 1.0)
            {
                trackPtr->jobBlock.fadeIncrement =	1.0 - displayParams.colorFactor;
            }
            else
            {
                trackPtr->jobBlock.fadeIncrement = 1.0;
            }
        }
        else // fadeDirection == FADE_OUT
        {
            if (displayParams.colorFactor > 0.0)
            {
                trackPtr->jobBlock.fadeIncrement = displayParams.colorFactor;
            }
            else
            {
                trackPtr->jobBlock.fadeIncrement = 1.0;
            }
        }

        trackPtr->jobBlock.fadeIncrement = trackPtr->jobBlock.fadeIncrement / (float)(trackPtr->jobBlock.numOfFrames);
        trackPtr->jobBlock.reentry = M_TRUE;
    
    } // end if first pass through

    switch(trackPtr->jobBlock.fadeDirection)
    {
        case FADE_IN:
        {
            if (displayParams.colorFactor < 1.0)
            {
                displayParams.colorFactor = 
                    displayParams.colorFactor + trackPtr->jobBlock.fadeIncrement;

                if (displayParams.colorFactor > 1.0)
                {
                    displayParams.colorFactor = 1.0;
                }
            }

        } // case FADE_IN
        break;

        case FADE_OUT:
        {
            if (displayParams.colorFactor > 0.0)
            {
                displayParams.colorFactor = 
                    displayParams.colorFactor - trackPtr->jobBlock.fadeIncrement;

                if (displayParams.colorFactor < 0.0)
                {
                    displayParams.colorFactor = 0.0;
                }
            }

        } // case FADE_OUT
        break;

        default:
#ifdef CHEAP3DVIEW_LOG_ERROR
            logFile << "DoFade(): Invalid fade direction specified." << endl;
#endif
            return(M_FAILURE);
            break;
        } // end switch on fade direction

//		trackPtr->SetTrackParams(displayParams);

    if (!(--trackPtr->jobBlock.numOfFrames))
    {
        if (trackPtr->jobBlock.fadeDirection == FADE_IN)
        {
            displayParams.colorFactor = 1.0;
        }
        else
        {
            displayParams.colorFactor = 0.0;
        }
    }

    trackPtr->SetTrackParams(displayParams);

    return(M_SUCCESS);

} // end CCheap3DView::DoFade

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::DoRotate()
//
////////////////////////////////////////////////////////////////////

USHORT
CCheap3DView::DoRotate(USHORT pointToRotate)
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering DoRotate()" << endl;
#endif
    BOOL   tempBool;
    ViewType  tempView = trackPtr->GetTrackView();
    gemScreenPtr->SetCurrentView(tempView);
    USHORT rotateAboutZeroAtPoint = M_FALSE;

    if (pointToRotate == FROM_POINT ||
        pointToRotate == BOTH_POINTS)
    {
        switch (trackNumber)
        {
            case 1:
                tempBool = m_RotateAbout0_1;
                break;
            case 2:
                tempBool = m_RotateAbout0_2;
                break;
            case 3:
                tempBool = m_RotateAbout0_3;
                break;
            case 4:
                tempBool = m_RotateAbout0_4;
                break;
            default:
#ifdef CHEAP3DVIEW_LOG_ERROR
                logFile << "DoRotate(): Invalid track number specified." << endl;
#endif
                return(M_FAILURE);
                break;
        } // end switch on track number

        if (tempBool == TRUE)
        {
            rotateAboutZeroAtPoint = M_TRUE;
        }

    } // end if pointToRotate is FROM_POINT

    switch(trackPtr->jobBlock.rotationAxis)
    {
        case X_AXIS:
            Rotate_X(gemScreenPtr,
                     trackPtr->jobBlock.degreeAngle,
                     pointToRotate,
                     rotateAboutZeroAtPoint);
            break;
        case Y_AXIS:
            Rotate_Y(gemScreenPtr,
                     trackPtr->jobBlock.degreeAngle,
                     pointToRotate,
                     rotateAboutZeroAtPoint);
            break;
        case Z_AXIS:
            Rotate_Z(gemScreenPtr,
                     trackPtr->jobBlock.degreeAngle,
                     pointToRotate,
                     rotateAboutZeroAtPoint);
            break;
        default:
#ifdef CHEAP3DVIEW_LOG_ERROR
            logFile << "DoRotate(): Invalid rotation axis specified." << endl;
#endif
            return(M_FAILURE);
            break;
    } // end switch on rotation axis

    displayView = gemScreenPtr->GetCurrentView();
    trackPtr->SetTrackView(displayView);

    --trackPtr->jobBlock.numOfFrames;

    return(M_SUCCESS);

} // end CCheap3DView::DoRotate

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::DoZoom()
//
////////////////////////////////////////////////////////////////////

USHORT
CCheap3DView::DoZoom(void)
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering DoZoom()" << endl;
#endif

    switch(trackPtr->jobBlock.zoomDirection)
    {            
        case ZOOM_IN:
//            ZoomIn(displayView, trackPtr->jobBlock.zoomIncrement);
              trackPtr->jobBlock.xIncrement = trackPtr->jobBlock.zoomIncrement;
            break;
        case ZOOM_OUT:
//            ZoomOut(displayView, trackPtr->jobBlock.zoomIncrement);
              trackPtr->jobBlock.xIncrement = -(trackPtr->jobBlock.zoomIncrement);
            break;
        default:
#ifdef CHEAP3DVIEW_LOG_ERROR
            logFile << "DoZoom(): Invalid zoom direction specified." << endl;
#endif
            return(M_FAILURE);
            break;
    } // end switch on zoom direction

    DoMoveForward(FROM_POINT);
    
    return(M_SUCCESS);

} // end CCheap3DView::DoZoom


////////////////////////////////////////////////////////////////////
//
// CCheap3DView::DoMoveForward()
//
////////////////////////////////////////////////////////////////////

USHORT
CCheap3DView::DoMoveForward(USHORT pointsToMove)
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering DoMoveForward()" << endl;
#endif

    MoveForward(displayView,
                trackPtr->jobBlock.xIncrement,
                pointsToMove);
    
    if (displayView.at_point.x > VIEW_MAX)
    {
        displayView.at_point.x = VIEW_MAX;
    }
    if (displayView.at_point.x < VIEW_MIN)
    {
        displayView.at_point.x = VIEW_MIN;
    }
    if (displayView.at_point.y > VIEW_MAX)
    {
        displayView.at_point.y = VIEW_MAX;
    }
    if (displayView.at_point.y < VIEW_MIN)
    {
        displayView.at_point.y = VIEW_MIN;
    }
    if (displayView.at_point.z > VIEW_MAX)
    {
        displayView.at_point.z = VIEW_MAX;
    }
    if (displayView.at_point.z < VIEW_MIN)
    {
        displayView.at_point.z = VIEW_MIN;
    }

    if (displayView.from_point.x > VIEW_MAX)
    {
        displayView.from_point.x = VIEW_MAX;
    }
    if (displayView.from_point.x < VIEW_MIN)
    {
        displayView.from_point.x = VIEW_MIN;
    }
    if (displayView.from_point.y > VIEW_MAX)
    {
        displayView.from_point.y = VIEW_MAX;
    }
    if (displayView.from_point.y < VIEW_MIN)
    {
        displayView.from_point.y = VIEW_MIN;
    }
    if (displayView.from_point.z > VIEW_MAX)
    {
        displayView.from_point.z = VIEW_MAX;
    }
    if (displayView.from_point.z < VIEW_MIN)
    {
        displayView.from_point.z = VIEW_MIN;
    }

    trackPtr->SetTrackView(displayView);

    --trackPtr->jobBlock.numOfFrames;

    return(M_SUCCESS);

} // end CCheap3DView::DoMoveForward


////////////////////////////////////////////////////////////////////
//
// CCheap3DView::DoMoveAt()
//
////////////////////////////////////////////////////////////////////
USHORT 
CCheap3DView::DoMoveAt(USHORT decrementFrameCount)
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering DoMoveAt()" << endl;
#endif

    // Move at point in the X axis
    if (trackPtr->jobBlock.xIncrement > 0.0)
    {
        if (displayView.at_point.x < VIEW_MAX)
        {
            displayView.at_point.x = displayView.at_point.x + 
                                 trackPtr->jobBlock.xIncrement;
            if (displayView.at_point.x > VIEW_MAX)
            {
                displayView.at_point.x = VIEW_MAX;
            }
        }
    }
    else if (trackPtr->jobBlock.xIncrement < 0)
    {
        if (displayView.at_point.x > VIEW_MIN)
        {
            displayView.at_point.x = displayView.at_point.x + 
                                 trackPtr->jobBlock.xIncrement;
            if (displayView.at_point.x < VIEW_MIN)
            {
                displayView.at_point.x = VIEW_MIN;
            }
        }
    }

    // Move at point in the Y axis
    if (trackPtr->jobBlock.yIncrement > 0.0)
    {
        if (displayView.at_point.y < VIEW_MAX)
        {
            displayView.at_point.y = displayView.at_point.y + 
                                 trackPtr->jobBlock.yIncrement;
            if (displayView.at_point.y > VIEW_MAX)
            {
                displayView.at_point.y = VIEW_MAX;
            }
        }
    }
    else if (trackPtr->jobBlock.yIncrement < 0)
    {
        if (displayView.at_point.y > VIEW_MIN)
        {
            displayView.at_point.y = displayView.at_point.y + 
                                 trackPtr->jobBlock.yIncrement;
            if (displayView.at_point.y < VIEW_MIN)
            {
                displayView.at_point.y = VIEW_MIN;
            }
        }
    }

    // Move at point in the Z axis
    if (trackPtr->jobBlock.zIncrement > 0.0)
    {
        if (displayView.at_point.z < VIEW_MAX)
        {
            displayView.at_point.z = displayView.at_point.z + 
                                 trackPtr->jobBlock.zIncrement;
            if (displayView.at_point.z > VIEW_MAX)
            {
                displayView.at_point.z = VIEW_MAX;
            }
        }
    }
    else if (trackPtr->jobBlock.zIncrement < 0)
    {
        if (displayView.at_point.z > VIEW_MIN)
        {
            displayView.at_point.z = displayView.at_point.z + 
                                 trackPtr->jobBlock.zIncrement;
            if (displayView.at_point.z < VIEW_MIN)
            {
                displayView.at_point.z = VIEW_MIN;
            }
        }
    }

    trackPtr->SetTrackView(displayView);

    if (decrementFrameCount == M_TRUE)
    {
        --trackPtr->jobBlock.numOfFrames;
    }

    return(M_SUCCESS);

} // end CCheap3DView::DoMoveAt

                       
////////////////////////////////////////////////////////////////////
//
// CCheap3DView::DoMoveFrom()
//
////////////////////////////////////////////////////////////////////
USHORT 
CCheap3DView::DoMoveFrom(USHORT decrementFrameCount)
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering DoMoveFrom()" << endl;
#endif

    // Move from point in the X axis
    if (trackPtr->jobBlock.xIncrement > 0.0)
    {
        if (displayView.from_point.x < VIEW_MAX)
        {
            displayView.from_point.x = displayView.from_point.x + 
                                   trackPtr->jobBlock.xIncrement;
            if (displayView.from_point.x > VIEW_MAX)
            {
                displayView.from_point.x = VIEW_MAX;
            }
        }
    }
    else if (trackPtr->jobBlock.xIncrement < 0.0)
    {
        if (displayView.from_point.x > VIEW_MIN)
        {
            displayView.from_point.x = displayView.from_point.x + 
                                   trackPtr->jobBlock.xIncrement;
            if (displayView.from_point.x < VIEW_MIN)
            {
                displayView.from_point.x = VIEW_MIN;
            }
        }
    }

    // Move from point in the Y axis
    if (trackPtr->jobBlock.yIncrement > 0.0)
    {
        if (displayView.from_point.y < VIEW_MAX)
        {
            displayView.from_point.y = displayView.from_point.y + 
                                   trackPtr->jobBlock.yIncrement;
            if (displayView.from_point.y > VIEW_MAX)
            {
                displayView.from_point.y = VIEW_MAX;
            }
        }
    }
    else if (trackPtr->jobBlock.yIncrement < 0.0)
    {
        if (displayView.from_point.y > VIEW_MIN)
        {
            displayView.from_point.y = displayView.from_point.y + 
                                   trackPtr->jobBlock.yIncrement;
            if (displayView.from_point.y < VIEW_MIN)
            {
                displayView.from_point.y = VIEW_MIN;
            }
        }
    }

    // Move from point in the Z axis
    if (trackPtr->jobBlock.zIncrement > 0.0)
    {
        if (displayView.from_point.z < VIEW_MAX)
        {
            displayView.from_point.z = displayView.from_point.z + 
                                   trackPtr->jobBlock.zIncrement;
            if (displayView.from_point.z > VIEW_MAX)
            {
                displayView.from_point.z = VIEW_MAX;
            }
        }
    }
    else if (trackPtr->jobBlock.zIncrement < 0.0)
    {
        if (displayView.from_point.z > VIEW_MIN)
        {
            displayView.from_point.z = displayView.from_point.z + 
                                   trackPtr->jobBlock.zIncrement;
            if (displayView.from_point.z < VIEW_MIN)
            {
                displayView.from_point.z = VIEW_MIN;
            }
        }
    }

    trackPtr->SetTrackView(displayView);

    if (decrementFrameCount == M_TRUE)
    {
        --trackPtr->jobBlock.numOfFrames;
    }

    return(M_SUCCESS);

} // end CCheap3DView::DoMoveFrom

                       

