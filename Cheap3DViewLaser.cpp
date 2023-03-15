//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Cheap3DViewLaser.cpp                       *
// * Description: Contains the implementation of those     *
// * methods on the Cheap3DView class which pertain to the *
// * processing and output of both computer screen preview *
// * and laser output.                                     *
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
#include <cmath>

using namespace std;
using std::cout;

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::InitTrackVariables()
//
////////////////////////////////////////////////////////////////////
void
CCheap3DView::InitTrackVariables()
{
    m_ArmTrack1      = FALSE;
    m_ArmTrack2      = FALSE;
    m_ArmTrack3      = FALSE;
    m_ArmTrack4      = FALSE;
    m_DisplayTrack1  = FALSE;
    m_DisplayTrack2  = FALSE;
    m_DisplayTrack3  = FALSE;
    m_DisplayTrack4  = FALSE;
    m_EffectTrack1   = FALSE;
    m_EffectTrack2   = FALSE;
    m_EffectTrack3   = FALSE;
    m_EffectTrack4   = FALSE;
    m_RotateAbout0_1 = FALSE;
    m_RotateAbout0_2 = FALSE;
    m_RotateAbout0_3 = FALSE;
    m_RotateAbout0_4 = FALSE;

    m_TrackFocusButton = 0;
    m_PointsOnly       = FALSE;
    m_Reverse          = FALSE;
    m_DisplayBeamView  = FALSE;
    UpdateData(FALSE);
}


////////////////////////////////////////////////////////////////////
//
// GetExtendedPoint()
// Used to find and extend the vertex between the origin in
// beam view and the point out to the viewport boundary.
//
////////////////////////////////////////////////////////////////////
void GetExtendedPoint(double xVal, double yVal,
                       double xOrigin, double yOrigin,
                       double* newX, double* newY)
{
    double mVal = (yVal-yOrigin)/(xVal-xOrigin);
    double bVal = yVal - (mVal*xVal);
    double abs_m;

    if (mVal<0)
    {
        abs_m = -mVal;
    }
    else
    {
        abs_m = mVal;
    }

    double slopeThreshold;

    if((yVal>yOrigin) && (xVal<xOrigin)) // upper left quad
    {
        slopeThreshold = (1.0-yOrigin) / (0.0 - xOrigin);

        if (slopeThreshold<0)
       {
           slopeThreshold = -slopeThreshold;
       }

       if (abs_m > slopeThreshold) 
       {
           *newY = 1.0;
           *newX = ((*newY)-bVal)/mVal;
       }
       else
       {
           *newX = 0.0;
           *newY = (mVal*(*newX))+bVal;
       }
   }
   else if((yVal>yOrigin) && (xVal>xOrigin)) // upper right quad
   {
       slopeThreshold = (1.0-yOrigin) / (1.0 - xOrigin);

       if (slopeThreshold<0)
       {
           slopeThreshold = -slopeThreshold;
       }

       if (abs_m > slopeThreshold) 
       {
           *newY = 1.0;
           *newX = ((*newY)-bVal)/mVal;
       }
       else
       {
           *newX = 1.0;
           *newY = (mVal*(*newX))+bVal;
       }
   }
   else if ((yVal<yOrigin) && (xVal<xOrigin)) // lower left quad
   {
       slopeThreshold = (0.0 - yOrigin) / (0.0 - xOrigin);

       if (slopeThreshold<0)
       {
           slopeThreshold = -slopeThreshold;
       }

       if (abs_m > slopeThreshold) 
       {
           *newY = 0.0;
           *newX = ((*newY)-bVal)/mVal;
       }
       else
       {
           *newX = 0.0;
           *newY = (mVal*(*newX))+bVal;
       }
   }
   else if ((yVal<yOrigin) && (xVal>xOrigin)) // lower right quad
   {
                                                                                                                                                                                                                                                                                                                       slopeThreshold = (0.0 - yOrigin) / (1.0 - xOrigin);
       slopeThreshold = (0.0 - yOrigin) / (1.0 - xOrigin);

       if (slopeThreshold<0)
       {
           slopeThreshold = -slopeThreshold;
       }

       if (abs_m > slopeThreshold) 
       {
           *newY = 0.0;
           *newX = ((*newY)-bVal)/mVal;
       }
       else
       {
           *newX = 1.0;
           *newY = (mVal*(*newX))+bVal;
       }
   }
   else if (yVal == yOrigin) // along the x axis
   {
       *newY = yOrigin;
               
       if (xVal > xOrigin)
       {
           *newX = 1.0;
       }
       else if (xVal < xOrigin)
       {
           *newX = 0.0;
       }
       else
       {
           *newX = xOrigin;
       }
   }
   else if (xVal == xOrigin) // along the y axis
   {
       *newX = xOrigin;

       if (yVal > yOrigin)
       {
           *newY = 1.0;
       }
       else if (yVal < yOrigin)
       {
           *newY = 0.0;
//           *newY = beamHeight;
       }
       else
       {
           *newY = yOrigin;
       }
   }
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::AddBlankingShift()
//
////////////////////////////////////////////////////////////////////
OutputPoint_T
CCheap3DView::AddBlankingShift(double xVal,
                               double yVal,
                               M_UINT8 rVal,
                               M_UINT8 gVal,
                               M_UINT8 bVal,
                               M_UINT8 iVal,
                               M_INT8  shift)
{
    OutputPoint_T tempBuffPoint;

    tempBuffPoint.xValue = xVal;
    tempBuffPoint.yValue = yVal;
    tempBuffPoint.rValue = rVal;
    tempBuffPoint.gValue = gVal;
    tempBuffPoint.bValue = bVal;
    tempBuffPoint.iValue = iVal;
    
    trackPtr->AddBufferPoint(tempBuffPoint);

    if (shift >= 0)
    {
        tempBuffPoint = trackPtr->GetBufferPoint(shift);
        // x and y unchanged, color
        // and intensity shifted
        tempBuffPoint.xValue = xVal;
        tempBuffPoint.yValue = yVal;
    }
    else
    {
        tempBuffPoint = trackPtr->GetBufferPoint((M_UINT8)(-shift));
        // color and intensity are
        // unchanged, x and y shifted
        tempBuffPoint.rValue = rVal;
        tempBuffPoint.gValue = gVal;
        tempBuffPoint.bValue = bVal;
        tempBuffPoint.iValue = iVal;
    }

    return tempBuffPoint; 
}							   

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::GetRecordFormatCode()
//
////////////////////////////////////////////////////////////////////
ILDAFormatCode 
CCheap3DView::GetRecordFormatCode(void)
{
    if (recordViewType == WORLD)
    {
        if (recordPaletteType == PaletteType::TRUECOLOR)
        {
            return ILDAFormatCode::TRUE_COLOR_3D;
        }
        else
        {
            return ILDAFormatCode::INDEXED_COLOR_3D;
        }
    }
    else
    {
        if (recordPaletteType == PaletteType::TRUECOLOR)
        {
            return ILDAFormatCode::TRUE_COLOR_2D;
        }
        else
        {
            return ILDAFormatCode::INDEXED_COLOR_2D;
        }
    }
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::DisplaySegment()
//
////////////////////////////////////////////////////////////////////
M_UINT16
CCheap3DView::DisplaySegment(Point*     currPointPtr,
                             Point*     nextPointPtr,
                             M_UINT8      redValue,
                             M_UINT8      greenValue,
                             M_UINT8      blueValue,
                             M_INT16     pointNumber)
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering DisplaySegment()" << endl;
#endif

    if (!currPointPtr)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "DisplaySegment(): NULL current point pointer!" << endl;
#endif
        return(M_FAILURE);
    }

    if (!nextPointPtr)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "DisplaySegment(): NULL next point pointer!" << endl;
#endif 
        return(M_FAILURE);
    }

    Point3DType p1e,p2e,p1w,p2w;
    double  x1v,y1v,x2v,y2v;
    M_UINT8  currStatus = 0;
    M_UINT8  nextStatus = 0;
    M_UINT8  tempColorIndex = 0;
    M_UINT8  tempRedValue = 0;
    M_UINT8  tempGreenValue = 0;
    M_UINT8  tempBlueValue = 0;
    M_UINT16 clipReturn;
    M_INT16  x, y, z;
    M_UINT16 screenOrLaser = 0;
    static BOOL lastBothClipped;

    OutputPoint_T delayedPoint;

    M_UINT8 screenRed, screenGreen, screenBlue;

    // Perform a "global" (i.e. screen, ILDA record, and laser output)
    // adjustment of the color based on the color preference settings.
    AdjustColor(colorMode,
                colorIntensity,
                ttlThresholdRed,
                ttlThresholdGreen,
                ttlThresholdBlue,
                &redValue,
                &greenValue,
                &blueValue);	
    
    currPointPtr->GetPointInfo(x, y, z, currStatus, tempColorIndex, tempRedValue, tempGreenValue, tempBlueValue);
    p1w.x = x;
    p1w.y = y;
    p1w.z = z;

    nextPointPtr->GetPointInfo(x, y, z, nextStatus, tempColorIndex, tempRedValue, tempGreenValue, tempBlueValue);
    p2w.x = x;
    p2w.y = y;
    p2w.z = z;

    M_UINT8 blankCurrent = currStatus & BLANKING_MASK;
    M_UINT8 blankNext    = nextStatus & BLANKING_MASK;

// logFile << "next: ("  << pointNumber 
//                 << ", " << p2w.x
//                 << ", " << p2w.y
//				 << ", " << p2w.z
//				 << ", " << blankNext
//				 << ", " << (M_UINT16)redValue
//				 << ", " << (M_UINT16)greenValue
//				 << ", " << (M_UINT16)blueValue << ")" << endl;

    // Now perform a screen only adjustment of the color...
    if (displayScreen == TRUE && displayParams.colorFactor != 0.0)
    {
        if(displayParams.displayBeamView == M_TRUE &&
           beamYagView == TRUE)
        {
            screenRed   = 99;
            screenGreen = 255;
            screenBlue  = 0;
        }
        else if(blankClipView && blankNext)
        {
            screenRed   = 90;
            screenGreen = 90;
            screenBlue  = 90;
//            screenRed   = 255;
//            screenGreen = 0;
//            screenBlue  = 0;
        }
//		else if(blankClipView && pointNumber == 1)
//		{
//            screenRed   = 0;
//            screenGreen = 255;
//            screenBlue  = 0;
//		}
        else if (colorMode == DisplayColorMode::COLOR_CVL)
        {
            screenRed   = redValue;
            if(redValue)
            {
                M_UINT16 tempGreen = (M_UINT16)( (double)greenValue + (double)redValue);

                if(tempGreen > 255)
                {
                    screenGreen = 255; 
                }
                else
                {
                    screenGreen = (M_UINT8)tempGreen;
                }
            }
            else
            {
                screenGreen = greenValue;
            }
            screenBlue  = 0;
        }
        else
        {
            screenRed   = redValue;
            screenGreen = greenValue;
            screenBlue  = blueValue;
        }

        ++screenOrLaser;

        p1e = gemScreenPtr->MapWorldToEye(p1w);
        p2e = gemScreenPtr->MapWorldToEye(p2w);

        clipReturn = gemScreenPtr->Clip3DLine(&p1e,&p2e);

        if (clipReturn == Gem_PC::BOTH_CLIPPED)
        {
            lastBothClipped = TRUE;
#ifndef LATEST_BLANKING_MOD
            return M_SUCCESS;
#endif
        }
#ifdef LATEST_BLANKING_MOD
        else
        {
#endif

        gemScreenPtr->MapEyeToNDC(p1e,&x1v,&y1v);
        gemScreenPtr->MapEyeToNDC(p2e,&x2v,&y2v);

        // Add this somewhat (but not completely) kludgey offset to
        // each point based on the current track number being displayed.
        // This is needed because, unlike the actual laser display, beams
        // which are directly overlayed on top of each other on the screen
        // obscure the beams behind them.  That is, they are not translucent.
        // In some situations, this causes the screen display to not accurately
        // represent "reality".  While we cannot (at least I haven't figured it
        // out yet) make the screen beams translucent, we can simulate this by
        // slightly offsetting the display of each track.
        double  screenPointOffset = 0.8 / (lowerRight.x - upperLeft.x);

        switch (trackNumber)
        {
            case 1:
            {
                x2v -= screenPointOffset;
                y2v += screenPointOffset;
            }
            break;

            case 2:
            {
                x2v += screenPointOffset;
                y2v += screenPointOffset;
            }
            break;

            case 3:
            {
                x2v -= screenPointOffset;
                y2v -= screenPointOffset;
            }
            break;

            case 4:
            {
                x2v += screenPointOffset;
                y2v -= screenPointOffset;
            }
            break;

            default:
            break;
        }

        CPen* newPen = 0;
        CPen* oldPen = 0;

        CClientDC* contextPtr;

        // Select the appropriate display context based on
        // "flicker-free (or not) mode.
        if (flickerFree == TRUE)
        {
            contextPtr = (CClientDC*)&memoryContext;
        }
        else
        {
            contextPtr = clientDCPtr;
        }

        long tempX1,tempY1,tempX2,tempY2;
        double x2Save = x2v;
        double y2Save = y2v;
        double x3v, y3v;

        if (displayParams.displayBeamView == M_TRUE)
        {
            if(beamMode == SHOWPOINTS)
            {
                gemScreenPtr->MapNDCToDevice(x2Save+.0050, y2Save+.0050,&tempX1,&tempY1);
                gemScreenPtr->MapNDCToDevice(x2Save-.0050, y2Save-.0050,&tempX2,&tempY2);
            }
            else
            {
                if(beamInvertX1==TRUE)
                    GetExtendedPoint(1.0-x2Save, y2Save+beamHeight1, beamOriginX1, beamOriginY1, &x2v, &y2v);  
                else
                    GetExtendedPoint(x2Save, y2Save+beamHeight1, beamOriginX1, beamOriginY1, &x2v, &y2v);  

                if(beamInvertX2==TRUE)
                    GetExtendedPoint(1.0-x2Save, y2Save+beamHeight2, beamOriginX2, beamOriginY2, &x3v, &y3v);  
                else
                    GetExtendedPoint(x2Save, y2Save+beamHeight2, beamOriginX2, beamOriginY2, &x3v, &y3v);  
            }
        } // end if beam view

        if (blankClipView || !blankNext)
        {
            if (pointNumber != -1 && // last current & next point always the same
                (displayParams.displayBeamView == M_FALSE &&
                ((displayParams.displayMode & POINTS_ONLY) ||
                (!blankClipView &&((gemScreenPtr->RealAbs(x2v - x1v) < 0.0015) &&
                                   (gemScreenPtr->RealAbs(y2v - y1v) < 0.0015))))))
            {
                newPen = new CPen(PS_SOLID,
                                  2,
                                  RGB(screenRed,screenGreen,screenBlue));
                oldPen = contextPtr->SelectObject(newPen);

                    if(displayParams.displayMode & POINTS_ONLY)
                    {
                        gemScreenPtr->NDCMove(contextPtr,x2v,y2v);
                    }
                    else
                    {
                        gemScreenPtr->NDCMove(contextPtr,x1v,y1v);
                    }

                    gemScreenPtr->NDCDraw(contextPtr,x2v+(double).0015,y2v+(double).0015);
            } // end if points only, non-beam view mode
            else
            {
                newPen = new CPen(PS_SOLID,
                                  1,
                                  RGB(screenRed,screenGreen,screenBlue));
                oldPen = contextPtr->SelectObject(newPen);

                // If this is the first point after being blanked,
                // and if we are not in blankClipView mode,
                // we don't want to draw the connecting line.  I.e,,
                // we need to MOVE to the next point rather than
                // just DRAW to the next point.
                if ((blankCurrent && !blankNext) ||
                    clipReturn == Gem_PC::P1_CLIPPED ||
                    (blankClipView && (lastBothClipped || clipReturn == Gem_PC::P2_CLIPPED)) ||
                    pointNumber == 1 ||
                    pointNumber == -1)
                {
                    if (displayParams.displayBeamView == M_TRUE)
                    {
                        switch (beamMode)
                        {
                            case SHOWPOINTS:
                            {
                                CBrush* newBrush = 0;
                                CBrush* oldBrush = 0;
                                newBrush = new CBrush(RGB(screenRed/2,screenGreen/2,screenBlue/2));
                                oldBrush = contextPtr->SelectObject(newBrush);
                                contextPtr->Ellipse(tempX1,tempY1,tempX2,tempY2);
                                contextPtr->SelectObject(oldBrush);			
                                delete newBrush;

                                gemScreenPtr->NDCMove(contextPtr,beamOriginX1,beamOriginY1);
                                gemScreenPtr->NDCDraw(contextPtr,x2v,y2v);
                            }
                            break;

                            case NORMAL:
                            default:
                            {
                                if(beamOn1)
                                {
                                    gemScreenPtr->NDCMove(contextPtr,beamOriginX1,beamOriginY1);
                                    gemScreenPtr->NDCDraw(contextPtr,x2v,y2v);
                                }

                                if(beamOn2)
                                {
                                    gemScreenPtr->NDCMove(contextPtr,beamOriginX2,beamOriginY2);
                                    gemScreenPtr->NDCDraw(contextPtr,x3v,y3v);
                                }
                            }
                            break;

                        } // end switch on beam mode

                    } // end if beam view on
                    else
                    {
                        gemScreenPtr->NDCMove(contextPtr,x1v,y1v);
                        gemScreenPtr->NDCDraw(contextPtr,x2v,y2v);
                    }

                } // end if first point after being blanked
                else
                {
                    if (displayParams.displayBeamView == M_TRUE)
                    {
                        switch (beamMode)
                        {
                            case SHOWPOINTS:
                            {
                                CBrush* newBrush = 0;
                                CBrush* oldBrush = 0;
                                newBrush = new CBrush(RGB(screenRed/2,screenGreen/2,screenBlue/2));
                                oldBrush = contextPtr->SelectObject(newBrush);
                                contextPtr->Ellipse(tempX1,tempY1,tempX2,tempY2);
                                contextPtr->SelectObject(oldBrush);			
                                delete newBrush;

                                gemScreenPtr->NDCMove(contextPtr,beamOriginX1,beamOriginY1);
                                gemScreenPtr->NDCDraw(contextPtr,x2v,y2v);
                            }
                            break;

                            case NORMAL:
                            default:
                            {
                                if(beamOn1)
                                {
                                    gemScreenPtr->NDCMove(contextPtr,beamOriginX1,beamOriginY1);
                                    gemScreenPtr->NDCDraw(contextPtr,x2v,y2v);
                                }

                                if(beamOn2)
                                {
                                    gemScreenPtr->NDCMove(contextPtr,beamOriginX2,beamOriginY2);
                                    gemScreenPtr->NDCDraw(contextPtr,x3v,y3v);
                                }
                            }
                            break;

                        } // end switch on beam mode
                    }
                    else
                    {
                        gemScreenPtr->NDCDraw(contextPtr,x2v,y2v);
                    }

                } // end else NOT first point after being blanked

            } // end else not points only mode

        } // else not blank next
                    
        if ((blankCurrent && !blankNext) ||
#ifdef DISPLAY_PREBLANK
            (!blankCurrent && blankNext) ||
#endif
            clipReturn == Gem_PC::P1_CLIPPED ||
            (blankClipView && (lastBothClipped || clipReturn == Gem_PC::P2_CLIPPED)) ||
            pointNumber == 1 ||
            pointNumber == -1)
        {
            if(blankClipView)
            {
                gemScreenPtr->MapNDCToDevice(x1v+.0100, y1v+.0100,&tempX1,&tempY1);
                gemScreenPtr->MapNDCToDevice(x1v-.0100, y1v-.0100,&tempX2,&tempY2);
                CBrush* newBrush = 0;
                CBrush* oldBrush = 0;

#ifdef DISPLAY_PREBLANK
                POINT  polyArray[3];
                polyArray[0].x = tempX1;
                polyArray[0].y = tempY1;

                polyArray[1].x = tempX2;
                polyArray[1].y = tempY1;


                polyArray[2].x = ((tempX2-tempX1)/2)+tempX1;
                polyArray[2].y = tempY2;
#endif

                if (pointNumber == 1) // CYAN SQUARE
                {
                    newBrush = new CBrush(RGB(0,255,255)); 
                }
                else if (clipReturn == Gem_PC::P1_CLIPPED) // RED SQUARE
                {
                    newBrush = new CBrush(RGB(255,0,0));
                }
                else if (clipReturn == Gem_PC::P2_CLIPPED) // GREEN SQUARE
                {
                    newBrush = new CBrush(RGB(0,255,0));
                }
                else if(lastBothClipped) // YELLOW SQUARE
                {
                    newBrush = new CBrush(RGB(255,255,0));
                }
                else if (pointNumber == -1) // WHITE
                {
                    newBrush = new CBrush(RGB(255,255,255));
                }
#ifdef DISPLAY_PREBLANK
                else if(!blankCurrent && blankNext) // MAGENTA TRIANGLE
                {
                    newBrush = new CBrush(RGB(255,0,255));
                }
#endif
                else // BLUE SQUARE
                {
                    newBrush = new CBrush(RGB(0,0,255)); 
                }
                oldBrush = contextPtr->SelectObject(newBrush);

#ifdef DISPLAY_PREBLANK
                if(!blankCurrent && blankNext)
                {
                    contextPtr->Polygon(&polyArray[0],3);
                }
                else
#endif
                {
                    contextPtr->Rectangle(tempX1,tempY1,tempX2,tempY2);
                }
                                
                gemScreenPtr->MapNDCToDevice(x2Save+.0100, y2Save+.0100,&tempX1,&tempY1);
                gemScreenPtr->MapNDCToDevice(x2Save-.0100, y2Save-.0100,&tempX2,&tempY2);
                if (pointNumber != -1)
                {
                    contextPtr->Ellipse(tempX1,tempY1,tempX2,tempY2);
                }
                contextPtr->SelectObject(oldBrush);			
                delete newBrush;
                newBrush = 0;
                oldBrush = 0;

            } // end if blankClipView

        } // if one of the special cases
                
        contextPtr->SelectObject(oldPen);
        delete newPen;

#ifdef LATEST_BLANKING_MOD
        } // end if not both points clipped
#endif

    } // end if display screen

    if(outFile)
    {
        // msvob 4/8/2022: For now I'm ignoring the "Last Point" bit in
        // the Status Code, so Setting/Clearing the Blanking bit by setting
        // the entire Byte directly is okay.  However, if this changes in the
        // future, will need to preserve the Last Point bit.
        p1e = gemScreenPtr->MapWorldToEye(p1w);
        p2e = gemScreenPtr->MapWorldToEye(p2w);

        M_UINT8 statusByte = 0x00;
        M_UINT8 colorIndex = 0x00;
        M_UINT8 tempRed = redValue;
        M_UINT8 tempGreen = greenValue;
        M_UINT8 tempBlue = blueValue;

        clipReturn = gemScreenPtr->Clip3DLine(&p1e,&p2e);

        if (clipReturn != Gem_PC::BOTH_CLIPPED)
        {
            gemScreenPtr->MapEyeToNDC(p1e,&x1v,&y1v);
            gemScreenPtr->MapEyeToNDC(p2e,&x2v,&y2v);

            PALETTE_PTR_T   recordPalettePtr;

            switch (recordPaletteType)
            {
                case PaletteType::PANGOLIN:
                    recordPalettePtr = &PangolinColorPalette[0];
                    break;
                case PaletteType::LASER_ILLUSIONS:
                    recordPalettePtr = &LaserIllusionsColorPalette[0];
                    break;
                case PaletteType::ILDA:
                    recordPalettePtr = &IldaColorPalette[0];
                    break;
                case PaletteType::X29:
                    recordPalettePtr = &X29ColorPalette[0];
                    break;
                default:
                    recordPalettePtr = &PangolinColorPalette[0];
                    break;
            }

            long  longTempX, longTempY;
            double P1TempX, P1TempY, P2TempX, P2TempY;
            static M_INT16 saveX, saveY;

            colorIndex = GetClosestPaletteIndex(recordPalettePtr,
                                                tempRed,
                                                tempGreen,
                                                tempBlue);

            // If P1 is clipped, move to the new P1 position
            // prior to drawing to P2.
            if (clipReturn == Gem_PC::P1_CLIPPED ||
                clipReturn == Gem_PC::P2_CLIPPED ||
                pointNumber == 1)
            {
                if (recordViewType == WORLD)
                {
                    // World View (3D)
                    PutWord(outFile,(M_UINT16)p1w.x);
                    PutWord(outFile,(M_UINT16)p1w.y);
                    PutWord(outFile,(M_UINT16)p1w.z);
                }
                else
                {
                    // Eye View (2D)
                    gemScreenPtr->MapNDCToDevice(x1v,y1v,&longTempX,&longTempY);
                    gemScreenPtr->MapDeviceToWorld(longTempX,longTempY,&P1TempX,&P1TempY);

                    PutWord(outFile,(M_UINT16)P1TempX);
                    PutWord(outFile,(M_UINT16)P1TempY);
                    // PutWord(outFile,0);
                }

                // PutByte(outFile, BLANKING_MASK);
                PutByte(outFile, 0x00);

                if (recordPaletteType == PaletteType::TRUECOLOR)
                {
                    PutByte(outFile, tempBlue);
                    PutByte(outFile, tempGreen);
                    PutByte(outFile, tempRed);
                }
                else
                {
                    PutByte(outFile, colorIndex);
                }

                saveX = (M_INT16)P1TempX;
                saveY = (M_INT16)P1TempY;

                ++currentNumberPoints;

            } // end if P1 or P2 clipped

            if (displayParams.displayMode & POINTS_ONLY)
            {
                if (recordViewType == WORLD)
                {
                    // World View (3D)
                    PutWord(outFile,(M_UINT16)p2w.x);
                    PutWord(outFile,(M_UINT16)p2w.y);
                    PutWord(outFile,(M_UINT16)p2w.z);
                }
                else
                {
                    // Eye View (2D)
                    gemScreenPtr->MapNDCToDevice(x2v,y2v,&longTempX,&longTempY);
                    gemScreenPtr->MapDeviceToWorld(longTempX,longTempY,&P2TempX,&P2TempY);

                    PutWord(outFile,(M_UINT16)P2TempX);
                    PutWord(outFile,(M_UINT16)P2TempY);
                    // PutWord(outFile,0);
                }

                // PutByte(outFile, BLANKING_MASK);
                PutByte(outFile, 0x00);

                if (recordPaletteType == PaletteType::TRUECOLOR)
                {
                    PutByte(outFile, tempBlue);
                    PutByte(outFile, tempGreen);
                    PutByte(outFile, tempRed);
                }
                else
                {
                    PutByte(outFile, colorIndex);
                }

                saveX = (M_INT16)P2TempX;
                saveY = (M_INT16)P2TempY;

                ++currentNumberPoints;

            } // end if points only mode

            if (recordViewType == WORLD)
            {
                // World View (3D)
                PutWord(outFile,(M_INT16)p2w.x);
                PutWord(outFile,(M_INT16)p2w.y);
                PutWord(outFile,(M_INT16)p2w.z);
            }
            else
            {
                // Eye View (2D)
                gemScreenPtr->MapNDCToDevice(x2v,y2v,&longTempX,&longTempY);
                gemScreenPtr->MapDeviceToWorld(longTempX,longTempY,&P2TempX,&P2TempY);

                PutWord(outFile,(M_INT16)P2TempX);
                PutWord(outFile,(M_INT16)P2TempY);
                // PutWord(outFile,0);
            }

            M_UINT16 deltaX = 0;
            M_UINT16 deltaY = 0;

            if (currentNumberPoints >= 1)
            {
                deltaX = abs((M_INT16)P2TempX - saveX);			
                deltaY = abs((M_INT16)P2TempY - saveY);			
            }

            // Blank the next point if the delta between it and
            // the previous point exceeds the threshold.
            if(deltaX > 32767 || deltaY > 32767)
            {
                blankNext |= BLANKING_MASK;
                tempRed = 0x00;
                tempGreen = 0x00;
                tempBlue = 0x00;
            }

            PutByte(outFile, blankNext);

            if (recordPaletteType == PaletteType::TRUECOLOR)
            {
                PutByte(outFile, tempBlue);
                PutByte(outFile, tempGreen);
                PutByte(outFile, tempRed);
            }
            else
            {
                PutByte(outFile, colorIndex);
            }

            saveX = (M_INT16)P2TempX;
            saveY = (M_INT16)P2TempY;

            ++currentNumberPoints;

        } // end if not both clipped

    } // end if outFile

    if (displayLaser == TRUE)
    {
        ++screenOrLaser;
        M_UINT8 intensity = 0x00;
        M_UINT8 blankVal = 0x00;

        if (colorMode == DisplayColorMode::COLOR_RG_INV_TTL)
        {
            blankVal = 0xff;
        }

        if (!blankNext
#ifdef LATEST_BLANKING_MOD
            && (redValue || greenValue || blueValue)
#endif
            )
        {
            intensity = 0xff;
        }

        p1e = gemLaserPtr->MapWorldToEye(p1w);
        p2e = gemLaserPtr->MapWorldToEye(p2w);

        clipReturn = gemLaserPtr->Clip3DLine(&p1e,&p2e);
        M_INT8 shiftVal = blankingShift;
        M_UINT16 i;

        M_UINT16 blankDelay;

        ///////////////////////////////////////////////////////////////////////
        //
        // Calculate the blanking delay points based on what's being
        // done at the moment.  Since the first point in each frame
        // and the first point after both points were clipped, entail
        // (potentially) the greatest jump of the scanners, this scenario
        // requires the greatest amount of delay.  Normal blanking between
        // points requires the least delay.
        //
        ///////////////////////////////////////////////////////////////////////
        if ((lastBothClipped  && clipReturn != Gem_PC::P2_CLIPPED) ||
            clipReturn == Gem_PC::P1_CLIPPED)
        {
            blankDelay = displayParams.blankingDelay * 5;
        }
        else if (clipReturn == Gem_PC::P2_CLIPPED)
        {
            blankDelay = displayParams.blankingDelay * 5;
        }
        else if (pointNumber == 1 || pointNumber == -1)
        {
            blankDelay = displayParams.blankingDelay * 5;
        }
        else
        {
            blankDelay = displayParams.blankingDelay;
        }

        if (clipReturn != Gem_PC::BOTH_CLIPPED)
        {

        gemLaserPtr->MapEyeToNDC(p1e,&x1v,&y1v);
        gemLaserPtr->MapEyeToNDC(p2e,&x2v,&y2v);

#ifdef LATEST_BLANKING_MOD
        lastValidNDC_X = x2v;
        lastValidNDC_Y = y2v;
#endif

        if (colorMode == DisplayColorMode::COLOR_RG_INV_TTL)
        {
            redValue = ~redValue;
            greenValue = ~greenValue;
            blueValue = 0xff;

// logFile << "DisplaySegment(): Red: " << (M_UINT16)redValue << ", Green: " << (M_UINT16)greenValue << ", Blue: " << (M_UINT16)blueValue << endl;
        }
        else
        {
            redValue   = (M_UINT8)((double)redValue   * masterRed);
            greenValue = (M_UINT8)((double)greenValue * masterGreen);
            blueValue  = (M_UINT8)((double)blueValue  * masterBlue);
        }
           
        if (blankNext)
        {
            if (!blankCurrent && blankNext ||
                pointNumber == 1)
            {
                for (i=1 ; 
                     i<= blankDelay ; 
                     ++i)
                {
#ifdef SHIFT_BLANK_DELAY
                    delayedPoint = AddBlankingShift(x1v, y1v, blankVal, blankVal, blankVal, 0, shiftVal);

#else
                    delayedPoint = AddBlankingShift(x1v, y1v, blankVal, blankVal, blankVal, 0, 0);

#endif
                    gemLaserPtr->DrawLaser(delayedPoint,
                                           invertBlanking);

                }

                delayedPoint = AddBlankingShift(x2v,
                                                y2v,
                                                blankVal,
                                                blankVal,
                                                blankVal,
                                                0,
                                                shiftVal);
                gemLaserPtr->DrawLaser(delayedPoint,
                                       invertBlanking);

            }
            else 
            {
                delayedPoint = AddBlankingShift(x2v, y2v, blankVal, blankVal, blankVal, 0, shiftVal);
                gemLaserPtr->DrawLaser(delayedPoint,
                                       invertBlanking);
            }

            if (pointNumber == -1)
            {
                for (i=1 ; 
                     i<= blankDelay; 
                     ++i)
                {
#ifdef SHIFT_BLANK_DELAY
                    delayedPoint = AddBlankingShift(x2v, y2v, blankVal, blankVal, blankVal, 0, shiftVal);
#else
                    delayedPoint = AddBlankingShift(x2v, y2v, blankVal, blankVal, blankVal, 0, 0);
#endif
                    gemLaserPtr->DrawLaser(delayedPoint,
                                           invertBlanking);
                }
            }

        }
        else if (displayParams.displayMode & POINTS_ONLY)
        {
            // CURRENTLY BROKEN!!!!!
/*			delayedPoint = AddBlankingShift(x2v, y2v, blankVal, blankVal, blankVal, 0, shiftVal);
            gemLaserPtr->DrawLaser(delayedPoint,
                                   invertBlanking);

            delayedPoint = AddBlankingShift(x2v,
                                            y2v,
                                            redValue,
                                            greenValue,
                                            blueValue,
                                            intensity,
                                            shiftVal);
            gemLaserPtr->DrawLaser(delayedPoint,
                                   invertBlanking);
            delayedPoint = AddBlankingShift(x2v, y2v, blankVal, blankVal, blankVal, 0, shiftVal);
            gemLaserPtr->DrawLaser(delayedPoint,
                                   invertBlanking); */

            if (pointNumber == 1)
            {
                for (i=1 ; 
                     i<= blankDelay ; 
                     ++i)
                {
#ifdef SHIFT_BLANK_DELAY
                    delayedPoint = AddBlankingShift(x1v, y1v, blankVal, blankVal, blankVal, 0, shiftVal);
#else
                    delayedPoint = AddBlankingShift(x1v, y1v, blankVal, blankVal, blankVal, 0, 0);
#endif
                    gemLaserPtr->DrawLaser(delayedPoint,
                                           invertBlanking);

                }

                for (i=1 ; 
                     i<= blankDelay ; 
                     ++i)
                {
#ifdef SHIFT_BLANK_DELAY
                    delayedPoint = AddBlankingShift(x2v, y2v, blankVal, blankVal, blankVal, 0, shiftVal);
#else
                    delayedPoint = AddBlankingShift(x2v, y2v, blankVal, blankVal, blankVal, 0, 0);
#endif
                    gemLaserPtr->DrawLaser(delayedPoint,
                                           invertBlanking);

                }
            }
            else if (pointNumber == -1)
            {
                for (i=1 ; 
                     i<= blankDelay ; 
                     ++i)
                {
#ifdef SHIFT_BLANK_DELAY
                    delayedPoint = AddBlankingShift(x2v, y2v, blankVal, blankVal, blankVal, 0, shiftVal);
#else
                    delayedPoint = AddBlankingShift(x2v, y2v, blankVal, blankVal, blankVal, 0, 0);
#endif
                    gemLaserPtr->DrawLaser(delayedPoint,
                                           invertBlanking);

                }

            }
            else
            {
                for (i=1 ; 
                     i<= blankDelay ; 
                     ++i)
                {
#ifdef SHIFT_BLANK_DELAY
                    delayedPoint = AddBlankingShift(x2v, y2v, blankVal, blankVal, blankVal, 0, shiftVal);
#else
                    delayedPoint = AddBlankingShift(x2v, y2v, blankVal, blankVal, blankVal, 0, 0);
#endif
                    gemLaserPtr->DrawLaser(delayedPoint,
                                           invertBlanking);

                }

                for (i=1 ; 
                     i<= blankDelay ; 
                     ++i)
                {
#ifdef SHIFT_BLANK_DELAY
                    delayedPoint = AddBlankingShift(x2v,
                                                    y2v,
                                                    redValue,
                                                    greenValue,
                                                    blueValue,
                                                    intensity,
                                                    shiftVal);
#else
                    delayedPoint = AddBlankingShift(x2v,
                                                    y2v,
                                                    redValue,
                                                    greenValue,
                                                    blueValue,
                                                    intensity,
                                                    0);
#endif
                    gemLaserPtr->DrawLaser(delayedPoint,
                                           invertBlanking);

                }

                for (i=1 ; 
                     i<= blankDelay ; 
                     ++i)
                {
#ifdef SHIFT_BLANK_DELAY
                    delayedPoint = AddBlankingShift(x2v, y2v, blankVal, blankVal, blankVal, 0, shiftVal);
#else
                    delayedPoint = AddBlankingShift(x2v, y2v, blankVal, blankVal, blankVal, 0, 0);
#endif
                    gemLaserPtr->DrawLaser(delayedPoint,
                                           invertBlanking);

                }

            }


        }
        else
        {
            if ((blankCurrent && !blankNext) ||
                 clipReturn == Gem_PC::P1_CLIPPED ||
                 (lastBothClipped  && clipReturn != Gem_PC::P2_CLIPPED) ||
                 pointNumber == 1)
            {

                for (i=1 ; 
                     i<= blankDelay ; 
                     ++i)
                {
#ifdef SHIFT_BLANK_DELAY
                    delayedPoint = AddBlankingShift(x1v, y1v, blankVal, blankVal, blankVal, 0, shiftVal);
#else
                    delayedPoint = AddBlankingShift(x1v, y1v, blankVal, blankVal, blankVal, 0, 0);
#endif
                    gemLaserPtr->DrawLaser(delayedPoint,
                                           invertBlanking);

                }

                delayedPoint = AddBlankingShift(x2v,
                                                y2v,
                                                redValue,
                                                greenValue,
                                                blueValue,
                                                intensity,
                                                shiftVal);
                gemLaserPtr->DrawLaser(delayedPoint,
                                       invertBlanking);

            }
            else
            {
                delayedPoint = AddBlankingShift(x2v,
                                                y2v,
                                                redValue,
                                                greenValue,
                                                blueValue,
                                                intensity,
                                                shiftVal);
                gemLaserPtr->DrawLaser(delayedPoint,
                                       invertBlanking);

                // In the following case, P2 has been clipped, meaning
                // the value of the "next" point has changed.  Since
                // this new point may require a significant jump, add
                // some points.  Following case also covers the last
                // point in the frame, which we may need to give a
                // little extra time to turn off the laser.
                if (clipReturn == Gem_PC::P2_CLIPPED ||
                    pointNumber == -1)
                {
                    for (i=1 ; 
                         i<= blankDelay; 
                         ++i)
                    {
#ifdef SHIFT_BLANK_DELAY
                        delayedPoint = AddBlankingShift(x2v, y2v, blankVal, blankVal, blankVal, 0, shiftVal);
#else
                        delayedPoint = AddBlankingShift(x2v, y2v, blankVal, blankVal, blankVal, 0, 0);
#endif
                        gemLaserPtr->DrawLaser(delayedPoint,
                                               invertBlanking);
                    }
                }
            }

        } // end else not points only mode

        } // end if both points not clipped
#ifdef LATEST_BLANKING_MOD
        else
        {
            lastBothClipped = TRUE;

  #ifdef SHIFT_BLANK_DELAY
            delayedPoint = AddBlankingShift(lastValidNDC_X, lastValidNDC_Y, blankVal, blankVal, blankVal, 0, shiftVal);
  #else
            delayedPoint = AddBlankingShift(lastValidNDC_X, lastValidNDC_Y, blankVal, blankVal, blankVal, 0, 0);
  #endif
            gemLaserPtr->DrawLaser(delayedPoint,
                                   invertBlanking);
        }
#endif
    } // end if display laser

    if (screenOrLaser)
    {
        ++(numPointsDisplayed);
    }

    lastBothClipped = FALSE;

    return(M_SUCCESS);

} // end function DisplaySegment


////////////////////////////////////////////////////////////////////
//
// CCheap3DView::InsertPointDelay()
//
////////////////////////////////////////////////////////////////////
#pragma optimize("", off)
void
CCheap3DView::InsertPointDelay(void)
{
        int dummy;

#ifndef NEW_DELAY
    for (unsigned long i=1 ;
         i<=(displayParams.delayValue + globalTrackDelay) ;
         ++i)
    {
        dummy = 0;
    }
#else
    unsigned long delayVal = globalTrackDelay * displayParams.delayValue;

    for (unsigned long i=1 ;
         i <= delayVal ;
         ++i)
    {
        dummy = 0;
//        for (unsigned long j=1 ;
//             j <= globalTrackDelay ;
//             ++j);
    }
#endif

}
#pragma optimize("", on)


////////////////////////////////////////////////////////////////////
//
// CCheap3DView::DisplayIldaFrame()
//
////////////////////////////////////////////////////////////////////
M_UINT16
CCheap3DView::DisplayIldaFrame(Frame*  framePtr)
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering DisplayIldaFrame()" << endl;
#endif

    M_UINT16 blankDelay = displayParams.blankingDelay;;

    if (!framePtr)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "DisplayIldaFrame(): NULL FramePtr!" << endl;
#endif
        return(M_FAILURE);
    }

    if ((displayParams.colorFactor < 0.0) || (displayParams.colorFactor > 1.0))
    {
// #ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "DisplayIldaFrame(): Invalid colorFactor " << displayParams.colorFactor 
                << "\n" << "setting to 1.0" << endl;
// #endif 
        displayParams.colorFactor = 1.0;
    }

    M_UINT16 numOfPoints = framePtr->GetNumberOfPoints();
    ILDAFormatCode frameFormatCode = framePtr->GetFormatCode();

    framePtr->ResetIterator();
    Point* currPointPtr = framePtr->GetCurrentPoint();
    Point* nextPointPtr = 0;

    if (!currPointPtr)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "DisplayIldaFrame(): No points in frame." << endl; 
#endif
        return(M_FAILURE);
    }

    M_INT16 pointNumber = 1;

    while (currPointPtr && (pointNumber <= numOfPoints))
    {
        InsertPointDelay();

        nextPointPtr = framePtr->GetNextPoint();

        if (!nextPointPtr)
        {
#ifdef CHEAP3DVIEW_LOG_INFO
            logFile << "DisplayIldaFrame(): Reached NULL next pointer at point " 
                    << dec << pointNumber << endl;
#endif

            DisplaySegment(currPointPtr,
                           currPointPtr,
                           0,0,0,
                           -1);

            return(M_SUCCESS);
        }

        M_UINT8 rawRed = 0x00;
        M_UINT8 rawGreen = 0x00;
        M_UINT8 rawBlue = 0x00;

        M_UINT8 nextStatus = nextPointPtr->GetStatusCode();
        M_UINT8 nextColorIndex = nextPointPtr->GetColorIndex();

        if (frameFormatCode == ILDAFormatCode::INDEXED_COLOR_2D ||
            frameFormatCode == ILDAFormatCode::INDEXED_COLOR_3D)
        {
            M_UINT8 nextColorIndex = nextPointPtr->GetColorIndex();
            PaletteEntryType paletteEntry = GetPaletteEntry(displayParams.palettePtr, nextColorIndex);
            rawRed = paletteEntry.redValue;
            rawGreen = paletteEntry.greenValue;
            rawBlue = paletteEntry.blueValue;
        }
        else
        {
            rawRed = nextPointPtr->GetColorValue(LaserColor::RED);
            rawGreen = nextPointPtr->GetColorValue(LaserColor::GREEN);
            rawBlue = nextPointPtr->GetColorValue(LaserColor::BLUE);
        }

        if (DisplaySegment(currPointPtr,
                           nextPointPtr,
                           (M_UINT8)(displayParams.colorFactor*displayParams.redLevel*(double)(rawRed)),
                           (M_UINT8)(displayParams.colorFactor*displayParams.greenLevel*(double)(rawGreen)),
                           (M_UINT8)(displayParams.colorFactor*displayParams.blueLevel*(double)(rawBlue)),
                           pointNumber) != M_SUCCESS)
        {
            return(M_FAILURE);
        }

        currPointPtr = nextPointPtr;

        ++pointNumber;

    } // end loop through points in frame

//    gemLaserPtr->LaserOff(invertBlanking, colorMode);
    DisplaySegment(currPointPtr,
                   currPointPtr,
                   0,0,0,
                   -1);

    return(M_SUCCESS);

} // end CCheap3DView::DisplayIldaFrame


////////////////////////////////////////////////////////////////////
//
// CCheap3DView::DisplayAbstractFrame()
//
////////////////////////////////////////////////////////////////////
M_UINT16
CCheap3DView::DisplayAbstractFrame(Abstract* const abstractPtr,
                                   M_UINT32     numberOfFrames)
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering DisplayAbstractFrame()" << endl;
#endif

    if (!abstractPtr)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "DisplayAbstractFrame(): Unexpected NULL abstractPtr!" 
                << endl;
#endif
        return(M_FAILURE);
    }

    M_UINT8  redValue;
    M_UINT8  greenValue;
    M_UINT8  blueValue;

    Point currPoint;
    Point nextPoint;
    M_UINT16 useDiscreet;
    M_INT16 j;

    for (M_UINT32 i=1 ; i<=numberOfFrames ; ++i)
    {
        for (j=1 ; j<=abstractPtr->AbsPtsPerFrame ; ++j)
        {
            // Add the delay.
            InsertPointDelay();

            if (abstractPtr->GetNextSegment(&currPoint,
                                            &nextPoint,
                                            redValue,
                                            greenValue,
                                            blueValue,
                                            useDiscreet) != M_SUCCESS)
            {
#ifdef CHEAP3DVIEW_LOG_ERROR
                logFile << "DisplayAbstractFrame(): Call to "
                        << "Abstract::GetNextSegment() failed!" << endl;
#endif
                return(M_FAILURE);
            }

            if (useDiscreet == M_TRUE)
            { // begin discreet color block
                M_UINT8 paletteIndex = GetClosestPaletteIndex(displayParams.palettePtr,
                                                              redValue,
                                                              greenValue,
                                                              blueValue);

                PaletteEntryType paletteEntry = GetPaletteEntry(displayParams.palettePtr,
                                                                paletteIndex);

                redValue   = paletteEntry.redValue;
                greenValue = paletteEntry.greenValue;
                blueValue  = paletteEntry.blueValue;

            } // end discreet color block

            if (DisplaySegment(&currPoint,
                               &nextPoint,
                               (M_UINT8)(displayParams.colorFactor*displayParams.redLevel*redValue),
                               (M_UINT8)(displayParams.colorFactor*displayParams.greenLevel*greenValue),
                               (M_UINT8)(displayParams.colorFactor*displayParams.blueLevel*blueValue),
                               j) != M_SUCCESS)
            {
#ifdef CHEAP3DVIEW_LOG_ERROR
                logFile << "DisplayAbstractFrame(): Call to "
                        << "DisplaySegment() failed!" << endl;
#endif
                return(M_FAILURE);
            }

        } // loop through segments in "frame"

    } // end loop through specified number of segments

//    gemLaserPtr->LaserOff(invertBlanking, colorMode);
    DisplaySegment(&nextPoint,
                   &nextPoint,
                   0,0,0,
                   -1);

    return(M_SUCCESS);

} // end function DisplayAbstractFrame


////////////////////////////////////////////////////////////////////
//
// CCheap3DView::DisplayTextFrame()
//
////////////////////////////////////////////////////////////////////
M_UINT16
CCheap3DView::DisplayTextFrame(TextScroll* const textPtr)
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering DisplayTextFrame()" << endl;
#endif

    if (!textPtr)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "DisplayTextFrame(): Unexpected NULL textPtr!" 
                << endl;
#endif
        return(M_FAILURE);
    }

    static double oldZ;

    M_UINT8  redValue   = 255;
    M_UINT8  greenValue = 255;
    M_UINT8  blueValue  = 255;

    Point currDisplayPoint;
    Point nextDisplayPoint;

//	FontPointPtr_T currPoint;
    FontPointPtr_T nextPoint;

    M_UINT16 i, j;
    M_UINT16 isBlanked = M_FALSE;
    M_INT16 pointNumber = 1;
        
    double newX;
    double newY;
    double newZ;

    textPtr->IncrementValue = ResetCycle(textPtr->IncrementValue+((double)(textPtr->ZAxisOffset)/10.0), 360, isBlanked);

    for(i=0 ; i<textPtr->GetTextStringLength() ; ++i)
        {
            double startPlusOffset;
            double endPlusOffset;
            
            TextCharacterPtr_T currentTextCharPtr = textPtr->GetTextCharacter(i);
            M_INT16 fontCharacterIndex = currentTextCharPtr->characterIndex;
            FontCharacterPtr_T currentFontCharPtr = FontList::GetFontCharacter(fontCharacterIndex);

            startPlusOffset = currentTextCharPtr->startPosition + textPtr->GetCurrentXOffset();
            endPlusOffset   = currentTextCharPtr->endPosition + textPtr->GetCurrentXOffset();
            
//		    if ((textString[i].endPosition + currentXOffset) >= (double)TEXT_WORLD_MIN &&
//			    (textString[i].startPosition + currentXOffset) <= (double)(TEXT_WORLD_MAX) &&
            if (startPlusOffset > (double)TEXT_WORLD_MIN &&
                endPlusOffset < (double)TEXT_WORLD_MAX &&
                fontCharacterIndex >= 0)
            {
                FontList::ResetIterator(fontCharacterIndex);

                double tempLongX = 0.0;
    
                for (j=0 ; j<=currentFontCharPtr->numOfPoints ; ++j)
                {
                    nextPoint = FontList::GetPoint(NEXT, fontCharacterIndex);
        
                    if (nextPoint)
                    {
                        isBlanked = nextPoint->isBlanked;

                        tempLongX = (nextPoint->xValue - currentFontCharPtr->xMinValue) + startPlusOffset;
            
                        if(tempLongX > (double)TEXT_WORLD_MAX)
                        {
                            tempLongX = (double)TEXT_WORLD_MAX;
                            isBlanked = M_TRUE;
                        }
                        else if(tempLongX < (double)TEXT_WORLD_MIN)
                        {
                            tempLongX = (double)TEXT_WORLD_MIN;
                            isBlanked = M_TRUE;
                        }

// TEST //		                if(j==0)
//						{
//							isBlanked = M_TRUE;
//						}
            
                        if(pointNumber == 1)
                        {
                            isBlanked  = M_TRUE;
                        }
                
/*		                if(isBlanked == M_TRUE)
                        {
                            redValue   = 0x00; 
                            greenValue = 0x00; 
                            blueValue  = 0x00;
                        } */

                        newX = tempLongX-(double)FONT_NORMALIZER;
                        newY = (nextPoint->yValue)-(double)FONT_NORMALIZER;
                        newZ = 0.0;


#ifdef ZTEXT
                        double xRadian, yRadian;

                        GenericMapAtoB(newX,
                                       newY,
                                       (double)WORLD_MIN,
                                       (double)WORLD_MAX,
                                       (double)0,
                                       ((textPtr->ZAxisPeriod*2.0) * PI),
                                       &xRadian,
                                       &yRadian);

                        switch(textPtr->ZAxisFormula)
                        {
                            case 1:
                            {
                                newZ = ::sin( (sqrt(pow(newX,2) + pow(newY,2)) / 6000.0) + DegreeToRadian(textPtr->IncrementValue)) * 10000.0;

                            }
                            break;
            
                            case 2:
                            {
                                newZ = ::sin(xRadian+DegreeToRadian((M_UINT16)textPtr->ZAxisOffset));
                                newZ = (newZ * (double)WORLD_MAX);
                            }
                            break;

                            case 3:
                            {
                                newZ = (- sqrt(pow(newX,2) + pow(newX,2)) * 1.5) + (textPtr->ZAxisOffset * 100);
                            }
                            break;

                            default:
                            {
                                newZ = 0;
                            }
                            break;

                        } // end switch on Z-Axis formula
                        
                        newZ = (M_INT16)(newZ*((double)textPtr->ZAxisGain / 100.0));

                        if (abs((int)newZ - (int)oldZ) > 5000)
                        {
                            isBlanked = M_TRUE;
                        }

#endif

                        M_UINT8 pointStatusCode = 0x00;
                        M_UINT8   redVal = 0x00;;
                        M_UINT8 greenVal = 0x00;
                        M_UINT8  blueVal = 0x00;

                        if (isBlanked)
                        {
                            pointStatusCode |= BLANKING_MASK;
                        }
                        else
                        {
                            redVal   = (M_UINT8)(displayParams.colorFactor * displayParams.redLevel * (double)redValue);
                            greenVal = (M_UINT8)(displayParams.colorFactor * displayParams.greenLevel * (double)greenValue);
                            blueVal  = (M_UINT8)(displayParams.colorFactor * displayParams.blueLevel * (double)blueValue);
                        }

                        nextDisplayPoint.SetPointInfo((M_INT16)newX,
                                                      (M_INT16)newY,
                                                      (M_INT16)newZ,
                                                      pointStatusCode,
                                                      0x00,
                                                      redVal,
                                                      greenVal,
                                                      blueVal);

// TEST //				if (pointNumber == 1)
//						{
//                            currDisplayPoint.SetPointInfo(0,0,0,(M_INT16)M_TRUE << 14);
//						}

                        if (DisplaySegment(&currDisplayPoint,
                                           &nextDisplayPoint,
                                           redVal,
                                           greenVal,
                                           blueVal,
                                           pointNumber) != M_SUCCESS)
                        {
#ifdef CHEAP3DVIEW_LOG_ERROR
                            logFile << "DisplayTextFrame(): Call to "
                                    << "DisplaySegment() failed!" << endl;
#endif
                            return(M_FAILURE);
                        }

                        if(pointNumber == 1)
                        {
                            oldZ = 0.0;
                        }
                        else
                        {
                            oldZ = newZ;
                        }

                        ++pointNumber;
                        currDisplayPoint = nextDisplayPoint;

                    } // end if nextPoint

                } // end for each point in the font character

            } // end if scrolling character is within field of view

        } // end loop through each character in the text string
        
        textPtr->SetNextXOffset();
        
//        gemLaserPtr->LaserOff(invertBlanking, colorMode);
    DisplaySegment(&currDisplayPoint,
                   &currDisplayPoint,
                   0,0,0,
                   -1);

    return(M_SUCCESS);

} // end function DisplayTextFrame


////////////////////////////////////////////////////////////////////
//
// CCheap3DView::UpdateDisplayWindowValues()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::UpdateDisplayWindowValues(void)
{
        CRect clientRect;

        GetClientRect(&clientRect);

        upperLeft  = clientRect.TopLeft();
        lowerRight = clientRect.BottomRight();

#ifdef DEBUG_WINDOW
        RECT  windowRect;

        GetWindowRect(&windowRect);


logFile << "Client Rectangle: (" << upperLeft.x  << ","
                                 << upperLeft.y  << ","
                                 << lowerRight.x << ","
                                 << lowerRight.y << ")" << endl;

logFile << "Window Rectangle: (" << (int)(windowRect.left)   << ","
                                 << (int)(windowRect.top)    << ","
                                 << (int)(windowRect.right)  << ","
                                 << (int)(windowRect.bottom) << ")" << endl;

        if (firstTime2 == FALSE)
        {
logFile << "Main Window Rectangle: (" << (int)(mainWindowRect.left)   << ","
                                      << (int)(mainWindowRect.top)    << ","
                                      << (int)(mainWindowRect.right)  << ","
                                      << (int)(mainWindowRect.bottom) << ")" << endl;
        }
#endif
        if(initMainWindow == TRUE &&
           firstTime2 == FALSE)
        {
            CRect rect;
            rect.SetRect((int)(mainWindowRect.left),
                         (int)(mainWindowRect.top),
                         (int)(mainWindowRect.right),
                         (int)(mainWindowRect.bottom));

            CWnd* tempMainWnd = AfxGetMainWnd();

            if (tempMainWnd)
            {
                tempMainWnd->MoveWindow(&rect);
                tempMainWnd->UpdateWindow();

// #define EDIT_COLOR

#ifdef EDIT_COLOR
                CWnd* tempEditWindow = tempMainWnd->GetDescendantWindow(IDC_EDIT_FOCUS_TRACK_REPEAT);
                if(tempEditWindow)
                {
                    CDC* tempEditContext = tempEditWindow->GetDC();

                    tempEditContext->SetBkColor(RGB(50,150,255));
                    tempEditContext->SetTextColor(RGB(255,255,255));
                    tempEditWindow->RedrawWindow();
                    tempMainWnd->RedrawWindow();
                }
#endif

            }

            initMainWindow = FALSE;

        }
                                 
        // adjust for offset of display window to right of controls
        upperLeft.x += leftPreviewEdgePixels;
        upperLeft.y += 6;
        lowerRight.x -= 9;
        lowerRight.y -= 9;

#ifdef CLR_STATUS_BAR
        parentFramePtr->m_wndStatusBar.PostMessage(SB_SETTEXT,1 | SBT_OWNERDRAW);
        parentFramePtr->m_wndStatusBar.PostMessage(SB_SETTEXT,2 | SBT_OWNERDRAW);
        parentFramePtr->m_wndStatusBar.PostMessage(SB_SETTEXT,3 | SBT_OWNERDRAW);
        parentFramePtr->m_wndStatusBar.PostMessage(SB_SETTEXT,4 | SBT_OWNERDRAW);
#endif

        if(firstTime2 == TRUE)
            firstTime2 = FALSE;

}


////////////////////////////////////////////////////////////////////
//
// CCheap3DView::CleanViewPort()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::CleanViewPort()
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering CleanViewPort()" << endl;
#endif

    CPen* newPen = 0;
    CPen* oldPen = 0;

    newPen = new CPen(PS_SOLID,
                      3,
                      RGB(0,50,255));

    oldPen = clientDCPtr->SelectObject(newPen);

    UpdateDisplayWindowValues();

    clientDCPtr->FillSolidRect(upperLeft.x,
                               upperLeft.y,
                               lowerRight.x-upperLeft.x, 
                               lowerRight.y-upperLeft.y,
                               RGB(5,5,5));
                               // RGB(0,0,0));

    clientDCPtr->SelectObject(oldPen);
    delete newPen;
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::DisplayAllTracks()
//
////////////////////////////////////////////////////////////////////
void
CCheap3DView::DisplayAllTracks()
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering DisplayAllTracks()" << endl;
#endif
    if (flickerFree == FALSE)
        CleanViewPort();

    M_UINT16 i;
    int cx, cy;
    CBitmap* pOldBitmap;
    M_UINT16  tempFrameRepeat;

    M_INT32 tempFilePos = 0;

    if (outFile)
    {
        CString strRecording;
        strRecording.Format("%u of %u", currentFrameNumber, numFramesToWrite);
        CCheap3DView::SetAppStatusPane(strRecording);

        currentNumberPoints = 0;

        // BYTES 1 - 4: The ASCII characters "I" "L" "D" "A"
        fprintf(outFile,"%c%c%c%c",'I','L','D','A');

        // BYTES 5 - 7: Not used, but must be 0, 0, 0
        fprintf(outFile,"%c%c%c",0,0,0);

        // BYTE 8: Format Code
        fprintf(outFile,"%c",(M_UINT8)CCheap3DView::GetRecordFormatCode());

        // BYTES 9 - 16: Frame Name; 8 ASCII characters
        fprintf(outFile,"%s","Test Frm");

        // BYTES 17 - 24: Company Name; 8 ASCII characters
        fprintf(outFile,"%s","Lazerus ");

        // BYTES 25 - 26: Total Points; Total # of points in image
        // Save the file pointer since we'll need to write
        // the number of points when we know what it is...
        tempFilePos = ftell(outFile);
        // ...then fill the space with a place holder
        fprintf(outFile,"%c%c",0,0);

        // BYTES 27 - 28: Frame number if part of sequence
        PutWord(outFile, currentFrameNumber);
        //fwrite(&currentFrameNumber,sizeof(M_INT16),1,outFile);

        // BYTES 29 - 30: Total Frames; Total frames in sequence
        frameNumPosArray[currentFrameNumber] = ftell(outFile);
        PutWord(outFile, numFramesToWrite);
        //fwrite(&maxFramesToWrite,sizeof(M_INT16),1,outFile);

        // BYTE 31: Scanner Head; Scanner head or projector number
        fprintf(outFile,"%c", 0);

        // BYTE 32: Future; Must be set for binary zero
        fprintf(outFile,"%c", 0);
    }

    commandInProgress = M_FALSE;

    trackList.ResetIterator();
    trackPtr = trackList.GetCurrentTrack();

    // prepare to buffer the screen display if we're
    // in flicker free mode
    if (displayScreen == TRUE)
    {
        UpdateDisplayWindowValues();

        if(flickerFree == TRUE)
        {
            cx = lowerRight.x-upperLeft.x;
            cy = lowerRight.y-upperLeft.y;

            memoryContext.CreateCompatibleDC(clientDCPtr);
            memoryBitmap.CreateCompatibleBitmap(clientDCPtr, cx, cy);
            pOldBitmap = memoryContext.SelectObject(&memoryBitmap);
    
            memoryContext.FillSolidRect(0,
                                        0,
                                        lowerRight.x-upperLeft.x, 
                                        lowerRight.y-upperLeft.y, 
                                        // RGB(0,0,0)
                                        RGB(5,5,5));
        }
    }

    // see if there's any "inertia" remaining from the
    // last drag and rotate, and add additional rotation
    // as necessary
    if(dragObjectPtr &&
       dragObjectPtr->inertiaRemaining)
    {
        ProcessRemainingInertia();
    }

    // now loop through, process, and display each track
    // as appropriate
    for (i=FIRST_TRACK ; i<=MAX_TRACKS ; ++i)
    {
        if (trackPtr)
        {
            M_UINT8 trackDev = trackPtr->GetTrackDevice();

            // msvob 4/6/2022: Following seems suspicious.  If we don't have
            // a unique device for each track, then we need to make sure the
            // each location in the outputDeviceArray[] is actually referencing
            // a valid Gem_PC object.
            gemLaserPtr = outputDeviceArray[trackDev];

            displayParams = trackPtr->GetTrackParams();

            if (displayParams.frameRepeat == -1)
            {
                tempFrameRepeat = globalFrameRepeat;
            }
            else
            {
                tempFrameRepeat = (M_UINT16)(displayParams.frameRepeat);
            }

            trackNumber = i;

            displayView = trackPtr->GetTrackView();

            if (trackPtr->jobBlock.numOfFrames)
            {
                // check first to see if the manipulation has
                // already been performed...
                if (trackPtr->jobBlock.skipManipulation == M_TRUE)
                {
                    trackPtr->jobBlock.skipManipulation = M_FALSE;
                }
                else if (DoTrackManipulation() != M_SUCCESS)
                {
#ifdef CHEAP3DVIEW_LOG_ERROR
    logFile << "DisplayAllTracks(): DoTrackManipulation failed." << endl;
#endif
                }

            } // end if number of frames not zero

            // See if we need to restore a saved JobBlock.  If
            // the number of frames is 0 but the function type
            // is valid, it means we're returning from executing
            // live control.  We swap the saved job block back in
            // and make the "save" job block invalid just to be
            // on the safe side.
            if (!trackPtr->jobBlock.numOfFrames &&
                trackPtr->jobBlock.functionType != INVALID)
            {
                if (trackPtr->saveJobBlock.functionType != INVALID)
                {
                    trackPtr->jobBlock = trackPtr->saveJobBlock;
                    trackPtr->saveJobBlock.functionType = INVALID;
                }
            }

            // if there's still work to be done, set the
            // global commandInProgress flag accordingly.
            if (trackPtr->jobBlock.numOfFrames)
            {
                commandInProgress = M_TRUE;
            }

            if (displayParams.displayMode & DISPLAY_ON)
            {
                long tempValue;
                
                // Don't do any more calculations than we have to...
                if (displayScreen == TRUE)
                {
                    long xMin;
                    long xMax;
                    long yMin;
                    long yMax;

                    if (flickerFree == TRUE)
                    {
                        xMin = 0;
                        xMax = lowerRight.x - upperLeft.x;
                        yMax = 0;
                        yMin = lowerRight.y - upperLeft.y;
                    }
                    else
                    {
                        xMin = upperLeft.x;
                        xMax = lowerRight.x;
                        yMax = upperLeft.y;
                        yMin = lowerRight.y;
                    }

//                    long xAdjMax = MaxScreenValue(adjustScale(displayParams.trackScale), xMax-xMin);
                    long xAdjMax = (long)((adjustScale(displayParams.trackScale)*(xMax-xMin)) + xMin);

                    long xAdjMin = xMin + (xMax - xAdjMax);
                    long yAdjMin = MaxScreenValue(adjustScale(verticalScale)*adjustScale(displayParams.trackScale),yMin);;
                    long yAdjMax = yMax + (yMin - yAdjMin);;

                    if (invertBitmap & SCREEN_INVERT)
                    {
                        if (invertBitmap & INVERT_X)
                        {
                            tempValue = xAdjMax;
                            xAdjMax = xAdjMin;
                            xAdjMin = tempValue;
                        }
                        if (invertBitmap & INVERT_Y)
                        {
                            tempValue = yAdjMax;
                            yAdjMax = yAdjMin;
                            yAdjMin = tempValue;
                        }
                    }
                
                    gemScreenPtr->SetDeviceWindow(xAdjMin,
                                                  xAdjMax, 
                                                  yAdjMin, 
                                                  yAdjMax);

                    gemScreenPtr->SetCurrentView(displayView);
                    gemScreenPtr->Find_V_Matrix();
                }

                if (displayLaser == TRUE)
                {
                    long xMaxLaser = MaxLaserValue(adjustScale(displayParams.trackScale), gemLaserPtr->GetDeviceMax());
                    long xMinLaser = MinDeviceValue(xMaxLaser, gemLaserPtr->GetDeviceMax(), gemLaserPtr->GetDeviceMin());
                    long yMaxLaser = MaxLaserValue((adjustScale(verticalScale)*adjustScale(displayParams.trackScale)), gemLaserPtr->GetDeviceMax());
                    long yMinLaser = MinDeviceValue(yMaxLaser, gemLaserPtr->GetDeviceMax(), gemLaserPtr->GetDeviceMin());

                    if (invertBitmap & INVERT_X)
                    {
                        tempValue = xMaxLaser;
                        xMaxLaser = xMinLaser;
                        xMinLaser = tempValue;
                    }
                    if (invertBitmap & INVERT_Y)
                    {
                        tempValue = yMaxLaser;
                        yMaxLaser = yMinLaser;
                        yMinLaser = tempValue;
                    }

                    gemLaserPtr->SetDeviceWindow(xMinLaser, 
                                                 xMaxLaser, 
                                                 yMinLaser, 
                                                 yMaxLaser);

                    gemLaserPtr->SetCurrentView(displayView);
                    gemLaserPtr->Find_V_Matrix();
                }

                trackType = trackPtr->GetTrackType();

                if (trackType == Track::ILDA)
                {
                    if (M_TRUE == ((FrameSequence*)trackPtr)->IsFirst())
                    {
                        if (displayParams.displayReverse == M_TRUE &&
                            displayParams.startFrameNumber < 0)
                        {
                            ((FrameSequence*)trackPtr)->SetIteratorToLast();
                        }

                        ((FrameSequence*)trackPtr)->SetFirstFalse();
                    }

                    framePtr = ((FrameSequence*)trackPtr)->GetCurrentFrame();

                    if (framePtr)
                    {
                        if (DisplayIldaFrame(framePtr) != M_SUCCESS)
                        {
                            logFile << "DisplayAllTracks(): DisplayIldaFrame() failed." 
                                    << endl;
                        }
                    }

                    if (displayParams.repeatCount >= tempFrameRepeat)
                    {
                        if (displayParams.displayReverse == M_FALSE)
                        {
                            if (displayParams.startFrameNumber >= 0)
                            {
                                // displaying a subset of entire frame sequence
                                if (displayParams.stopFrameNumber >
                                    displayParams.startFrameNumber)
                                {
                                    framePtr = ((FrameSequence*)trackPtr)->GetNextFrame();
                                    ++displayParams.currentFrameNumber;

                                    if ( (displayParams.currentFrameNumber >
                                          displayParams.stopFrameNumber)    ||
                                          !framePtr )
                                    {
                                        displayParams.currentFrameNumber =
                                            (M_UINT16)(displayParams.startFrameNumber);
                                        ((FrameSequence*)trackPtr)->
                                            SetCurrentFrame((M_UINT16)(displayParams.startFrameNumber));
                                    }
                                }
                            }
                            else
                            {
                                framePtr = ((FrameSequence*)trackPtr)->GetNextFrame();

                                if (!framePtr)
                                {
                                    ((FrameSequence*)trackPtr)->ResetIterator();
                                }
                            } // else displaying entire frame sequence

                        } // end if display reverse is false

                        else // display in reverse direction
                        {
                            if (displayParams.startFrameNumber >= 0)
                            {
                                // displaying a subset of entire frame sequence
                                if (displayParams.startFrameNumber >
                                    displayParams.stopFrameNumber)
                                {
                                    framePtr = ((FrameSequence*)trackPtr)->GetPrevFrame();
                                    --displayParams.currentFrameNumber;

                                    if ( (displayParams.currentFrameNumber <
                                          displayParams.stopFrameNumber)    ||
                                          !framePtr )
                                    {
                                        displayParams.currentFrameNumber =
                                            (M_UINT16)(displayParams.startFrameNumber);
                                        ((FrameSequence*)trackPtr)->
                                            SetCurrentFrame((M_UINT16)(displayParams.startFrameNumber));
                                    }
                                }
                            }
                            else
                            {
                                framePtr = ((FrameSequence*)trackPtr)->GetPrevFrame();

                                if (!framePtr)
                                {
                                    ((FrameSequence*)trackPtr)->SetIteratorToLast();
                                }
                            } // else displaying entire frame sequence

                        }

                        displayParams.repeatCount = 0;
                    }
                    else
                    {
                        ++(displayParams.repeatCount);
                    }

                } // end if ILDA track type
                else if (trackType == Track::ABSTRACT)
                {
                   Abstract tempAbs = *((Abstract*)trackPtr);

                   if (DisplayAbstractFrame((Abstract*)trackPtr, 1) != M_SUCCESS)
                   {
                       logFile << "DisplayAllTracks(): DisplayAbstractFrame() failed." 
                               << endl;
                   }

                   if (displayParams.repeatCount < tempFrameRepeat)
                   {
                       *((Abstract*)trackPtr) = tempAbs;
                       ++(displayParams.repeatCount);
                   }
                   else
                   {
                       displayParams.repeatCount = 0;
                   }

                } // end else if track type is ABSTRACT

                else if (trackType == Track::TEXT)
                {
                   TextScroll tempText = *((TextScroll*)trackPtr);

                   if (DisplayTextFrame((TextScroll*)trackPtr) != M_SUCCESS)
                   {
                       logFile << "DisplayAllTracks(): DisplayTextFrame() failed." 
                               << endl;
                   }

                   if (displayParams.repeatCount < tempFrameRepeat)
                   {
                       *((TextScroll*)trackPtr) = tempText;
                       ++(displayParams.repeatCount);
                   }
                   else
                   {
                       displayParams.repeatCount = 0;
                   }

                } // end else if track type is TEXT

                trackPtr->SetTrackParams(displayParams);

            } // end if DISPLAY_ON for current track

            if (trackNumber == m_TrackFocus &&
                m_DisplayCoordinates == TRUE)
            {
                m_FromPointX = displayView.from_point.x;
                m_FromPointY = displayView.from_point.y;
                m_FromPointZ = displayView.from_point.z;
                m_AtPointX   = displayView.at_point.x;
                m_AtPointY   = displayView.at_point.y;
                m_AtPointZ   = displayView.at_point.z;

                UpdateData(FALSE);

            }
#ifndef WRITE_ALL_TRACKS_AT_ONCE
          OutputDeviceTypeT outputType =  gemLaserPtr->GetOutputDeviceType();
          if (outputType != NONE)
          {
                gemLaserPtr->WriteCurrentDeviceBuffer();
          }
#endif
        } // end if track exists

        trackPtr = trackList.GetNextTrack();

    } // while more tracks to process

#ifdef WRITE_ALL_TRACKS_AT_ONCE
// msvob 3/30/22: Moving the write outside the track loop is a workaround,
// and not something I want to leave as a final solution.  Need to figure
// out why, when trying to write the track buffers one at a time, only
// one track worth of points gets output and the others are "lost".
// It seems to have to do with the fact that the points are being added
// to a single buffer cumulatively, as evidenced by the fact that outputting
// the data outside the track loop actually does produce the desired result.
// If multiple devices were being utilized, then each device should have its
// own buffer.  But with a single device, all tracks are sharing the same
// buffer, so I suppose it doesn't make sense to write that shared buffer
// each loop unless you were actually clearing the buffer correctly after each write
// to the device.  Otherwise, the first write would output the points for track
// 1.  Then the next time thru the loop, it would write the points for track 1 (again)
// and track 2.  The next time thru would write track 1 points (a third time), and now
// track 2 points (for a second time) and track 3...and so on.  I believe this is actually
// what's happening.
// Need to think about this, but I think I understand what's happening.  For the moment,
// it probably doesn't matter as actually associating each track with a different device
// is going to be significant work anyway.

    OutputDeviceTypeT outputType = gemLaserPtr->GetOutputDeviceType();
    if (outputType != NONE)
    {
        gemLaserPtr->WriteCurrentDeviceBuffer();
    }
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#endif

    if (displayScreen == TRUE &&
        flickerFree == TRUE)
    {
        clientDCPtr->BitBlt(upperLeft.x,
                            upperLeft.y,
                            cx, 
                            cy, 
                            &memoryContext,
                            0,
                            0,
                            SRCCOPY);

        memoryContext.SelectObject(pOldBitmap);
        memoryContext.DeleteDC();
        memoryBitmap.DeleteObject();
    }

    if(dragObjectPtr && dragObjectPtr->inertiaRemaining)
    {
        if(--dragObjectPtr->inertiaRemaining == 0)
        {
            delete dragObjectPtr;
            dragObjectPtr = 0;
        }
    }

    if (outFile)
    {
        M_INT32 savePos = ftell(outFile);
        fseek(outFile, tempFilePos, SEEK_SET);
        PutWord(outFile, currentNumberPoints);
        fseek(outFile, savePos, SEEK_SET);
        ++currentFrameNumber;

        if (currentFrameNumber >= numFramesToWrite)
        {
            CCheap3DView::SetAppStatusPane("Record Complete");
            OnFileClosetracklist();
            Sleep(3000);
            CCheap3DView::SetAppStatusPane("Okay");
        }
    }

//	clientDCPtr->Attach(tempHDC);

} // end CCheap3DView::DisplayAllTracks()

////////////////////////////////////////////////////////////////////
//
//  CCheap3DView::ReadCtsFile()
//
////////////////////////////////////////////////////////////////////
void
CCheap3DView::ReadCtsFile()
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering ReadCtsFile()" << endl;
#endif
    if (!ctsFilePtr)
    {
        MessageBox("ReadCtsFile(): Called with NULL file pointer.");
        displayParams.displayMode = 
                    displayParams.displayMode & ~DISPLAY_ON;
        KillTimer(DISPLAY_TIMER);
        displayTimerSet = FALSE;
        KillTimer(POINTS_PER_SECOND_TIMER);
        KillTimer(PLAYBACK_TIMER);
        playbackTime = 0;
        SetPlaybackTimerPane((long)playbackTime);
        scriptActive = M_FALSE;
        return;
    }

    char   tempString1[300];
    char   tempString2[300];
    char   commandString[5];

    if (firstTime == M_TRUE)
    {
        // since we're executing the script for the first time,
        // clean up the track list in preparation...
//		trackList.DeleteTrackList();

        if (!fscanf(ctsFilePtr,"%s", tempString1))
        {
            AbortScript("ReadCtsFile(): Unable to read CTS header info.");
            return;
        }

        if (strcmp(tempString1, "LFI_PLAYER_SCRIPT") != 0 &&
            strcmp(tempString1, "LAZERUS_3D_SCRIPT") != 0)
        {
            AbortScript("ReadCtsFile(): Invalid CTS header info.");
            return;
        }

        loopCounter   = 0;
        numberOfLoops = 0;

        globalTrackDelay = defaultGlobalDelay;
        verticalScale    = defaultVerticalScale;
        laserScale       = defaultLaserScale;

        firstTime = M_FALSE;
    }

    M_UINT16 readNextCommand = M_TRUE;

    while (readNextCommand == M_TRUE)
    {

    if (fscanf(ctsFilePtr,"%s", commandString) == EOF)
    {
        if (scriptRepeat == TRUE)
        {
            loopCounter   = 0;
            numberOfLoops = 0;

            rewind(ctsFilePtr);
            if (!fscanf(ctsFilePtr,"%s", tempString1))
            {
                AbortScript("ReadCtsFile(): Unable to read CTS header info.");
                return;
            }
        }
        else
        {
//            AbortScript("ReadCtsFile(): Script complete.");
            return;
        }
    }

    Track*         origTrackPtr;
    ViewType       origTrackView;
    DisplayParams  origTrackParams;
    M_UINT8        origOutputDevice;
    DisplayParams tempParams;
    M_UINT16 blankingDelay = 0;
    M_UINT16 setDefaults = M_FALSE;
    int retVal;

    // ********** Comment Line **********
    if (strcmp(commandString, "//") == 0)
    {
        char tempChar;

        do
        {
            tempChar = (char)fgetc(ctsFilePtr);
        }
        while (tempChar != '\n' && tempChar != '\r');
    
        readNextCommand = M_TRUE;

    }
	// ********** Blank Line **********
    else if (strcmp(commandString, "") == 0)
    {
        readNextCommand = M_TRUE;
    }

    // ********** End Script **********
    else if (strcmp(commandString, "ES") == 0)
    {
        if (scriptRepeat == TRUE)
        {
            rewind(ctsFilePtr);
            if (!fscanf(ctsFilePtr,"%s", tempString1))
            {
                AbortScript("ReadCtsFile(): Unable to read CTS header info.");
                return;
            }
        }
        else
        {
//            AbortScript("ReadCtsFile(): Reached End Script command.");
            return;
        }
    }

    // ********** Begin Loop (BL) **********
    else if (strcmp(commandString, "BL") == 0)
    {
        loopCounter = 0;

        if (!fscanf(ctsFilePtr,"%ld",
            &numberOfLoops))
        {
            AbortScript("ReadCtsFile(): Can't retrieve loop data.");
            return;
        }

        saveFilePos = ftell(ctsFilePtr);

        if (saveFilePos == -1)
        {
            AbortScript("ReadCtsFile(): Unable to get file position.");
            return;
        }

        readNextCommand = M_TRUE;

    } // end else if Begin Loop

    // ********** End Loop (EL) **********
    else if (strcmp(commandString, "EL") == 0)
    {
        if (loopCounter >= numberOfLoops)
        {
            loopCounter = 0;
            numberOfLoops = 0;
        }
        else
        {
            retVal = fseek(ctsFilePtr, saveFilePos, SEEK_SET);
            ++loopCounter;

            if (!ctsFilePtr || retVal)
            {
                AbortScript("ReadCtsFile(): Invalid saved file pointer at end of loop.");
                return;
            }

        }

        readNextCommand = M_TRUE;

    } // end else if End Loop

    // ********** Clear All Tracks (CAT) **********
    else if (strcmp(commandString, "CAT") == 0)
    {
        for(M_UINT8 s=0 ; s<MAX_TRACKS ; ++s)
        {
            if(outputDeviceArray[s])
            {
                (outputDeviceArray[s])->DisplayOff(invertBlanking, colorMode);
            }
        }

        trackList.DeleteTrackList();
        InitTrackVariables();

    } // end else if Clear All Tracks


    // ********** Reset All Tracks (RAT) **********
    else if (strcmp(commandString, "RAT") == 0)
    {
        Track* tempTrackPtr = 0;

        for (M_UINT16 i=1 ; i<=MAX_TRACKS ; ++i)
        {
            tempTrackPtr = trackList.GetTrack(i);
            if (tempTrackPtr)
            {
                tempTrackPtr->jobBlock.ResetJobBlock();               
                tempTrackPtr->saveJobBlock.ResetJobBlock();              
            }
        }

    } // end else if Reset All Tracks

    // ********** Read ILDA File (RIF) **********
    else if (strcmp(commandString, "RIF") == 0)
    {
        SetAppStatusPane("Loading");
        if (!fscanf(ctsFilePtr,"%d %s %d",
                    &trackNumber,
                    &tempString1,
                    &setDefaults))
        {
            AbortScript("ReadCtsFile(): Can't retrieve filename.");
            SetAppStatusPane("Okay");
            return;
        }
        else
        {
            strcpy(tempString2, contentDirectoryName);
            strcat(tempString2, tempString1);
            FrameSequence* frameSeqPtr = 0;
            M_UINT16 readStatus = DoReadILDAFile(tempString2, &frameSeqPtr);

            if (readStatus == M_FAILURE)
            {
                AbortScript("ReadCtsFile(): Call to DoReadILDAFile() failed.");
                SetAppStatusPane("Okay");
                return;
            }

            frameSeqPtr->ResetIterator();
            frameSeqPtr->SetTrackType(Track::ILDA);
            trackList.SetTrack(trackNumber,frameSeqPtr);

// New ->
            origTrackPtr = trackList.GetTrack(trackNumber);

            if(origTrackPtr)
            {
                origTrackView = origTrackPtr->GetTrackView();
                origTrackParams = origTrackPtr->GetTrackParams();
                origOutputDevice = origTrackPtr->GetTrackDevice();
            }
// <- New
            trackPtr = trackList.GetTrack(trackNumber);

            if (!trackPtr)
            {
                AbortScript("ReadCtsFile(): Unable to retrieve trackPtr for ILDA track.");
                SetAppStatusPane("Okay");
                return;
            }

            if (setDefaults == M_TRUE)
            {
// should check for valid pointer here...
                trackPtr->saveJobBlock.functionType = INVALID;
                trackPtr->jobBlock.functionType = INVALID;

#ifdef CTS_SCRIPT_INFO
                logFile << "ReadCtsFile(): Script calling InitTrack(ILDA)..." << endl;
#endif
                InitTrack(Track::ILDA);

            } // end if set the default values
// New ->
            else
            {
                if(origTrackPtr)
                {
                    trackPtr->SetTrackView(origTrackView);
                    trackPtr->SetTrackParams(origTrackParams);
                    trackPtr->SetTrackDevice(origOutputDevice);
                }
            }
// <- New

            m_TrackFocusButton = trackNumber - 1;
            SetTrackArmValue(trackNumber, TRUE);
            readNextCommand = M_TRUE;
            UpdateData(FALSE);
            SetAppStatusPane("Okay");

        }

    } // end else if Read ILDA File

    // ********** Read Abstract Descriptor (RAD) **********
    else if (strcmp(commandString, "RAD") == 0)
    {
        char newAbsFile[MAX_FILENAME_SIZE];

       SetAppStatusPane("Loading");
       if (!fscanf(ctsFilePtr,
                    "%d %s %d",
                    &trackNumber,
                    &newAbsFile[0],
                    &setDefaults))
        {
            AbortScript("ReadCtsFile(): Can't get data for read abstract descriptor.");
            SetAppStatusPane("Okay");
            return;
        }
        else
        {
            strcpy(tempString2, contentDirectoryName);
            strcat(tempString2, newAbsFile);

            Abstract* abstractPtr = new Abstract(&tempString2[0]);

            if (!abstractPtr)
            {
                AbortScript("ReadCtsFile(): Unable to new Abstract object.");
                SetAppStatusPane("Okay");
                return;
            }

            if (!abstractPtr->Ok())
            {
                AbortScript("ReadCtsFile(): Unable to correctly initialize Abstract object.");
                delete abstractPtr;
                SetAppStatusPane("Okay");
                return;
            }

            if (abstractPtr->SetPattern(1) != M_SUCCESS)
            {
                AbortScript("ReadCtsFile(): Call to	Abstract::SetPattern() failed.");
                delete abstractPtr;
                SetAppStatusPane("Okay");
                return;
            }

// New ->
            origTrackPtr = trackList.GetTrack(trackNumber);

            if(origTrackPtr)
            {
                origTrackView = origTrackPtr->GetTrackView();
                origTrackParams = origTrackPtr->GetTrackParams();
                origOutputDevice = origTrackPtr->GetTrackDevice();
            }
// <- New

            // Now add the abstract to the track list.
            abstractPtr->SetTrackType(Track::ABSTRACT);
            trackList.SetTrack(trackNumber, abstractPtr);
// New ->
            trackPtr = trackList.GetTrack(trackNumber);

            if (!trackPtr)
            {
                AbortScript("ReadCtsFile(): Unable to retrieve trackPtr for Abstract track.");
                SetAppStatusPane("Okay");
                return;
            }
// <- New

            if (setDefaults == M_TRUE)
            {
// New ->
// <- New
// should check for valid pointer here...
                trackPtr->saveJobBlock.functionType = INVALID;
                trackPtr->jobBlock.functionType = INVALID;

#ifdef CTS_SCRIPT_INFO
                logFile << "ReadCtsFile(): Script calling InitTrack(ABSTRACT)..." << endl;
#endif
                InitTrack(Track::ABSTRACT);

            } // end if set the default values
// New ->
            else
            {
                if(origTrackPtr)
                {
                    trackPtr->SetTrackView(origTrackView);
                    trackPtr->SetTrackParams(origTrackParams);
                    trackPtr->SetTrackDevice(origOutputDevice);
                }
            }
// <- New

            if (abstractDialog)
            {
                SetTrackFocus(trackList.GetCurrentTrackNumber());   
            }
            else
            {
                m_TrackFocusButton = (trackList.GetCurrentTrackNumber()) - 1;
            }

//            m_TrackFocusButton = trackNumber - 1;

            SetTrackArmValue(trackNumber, TRUE);
            readNextCommand = M_TRUE;
            UpdateData(FALSE);
            SetAppStatusPane("Okay");

        }

    } // end else if Read Abstract Descriptor

    // ********** Read Text File (RTF) **********
    else if (strcmp(commandString, "RTF") == 0)
    {
        char newTextFile[MAX_FILENAME_SIZE];

        SetAppStatusPane("Loading");
        if (!fscanf(ctsFilePtr,
                    "%d %s %d",
                    &trackNumber,
                    &newTextFile[0],
                    &setDefaults))
        {
            AbortScript("ReadCtsFile(): Can't get data for read text file.");
            SetAppStatusPane("Okay");
            return;
        }
        else
        {
            if(FontList::Ok())
            {
                strcpy(tempString2, contentDirectoryName);
                strcat(tempString2, newTextFile);

                TextScroll* textScrollPtr = new TextScroll();

                if (!textScrollPtr)
                {
                    AbortScript("ReadCtsFile(): Unable to create Scrolling Text object.");
                    SetAppStatusPane("Okay");
                    return;        
                }

                FILE* txtFilePtr = fopen(&tempString2[0], "rb");

                if (!txtFilePtr)
                {
                    AbortScript("ReadCtsFile(): failed to open text file for reading.");
                    SetAppStatusPane("Okay");
                    return;
                }

                if (textScrollPtr->ReadTextFile(txtFilePtr) == M_FAILURE)
                {
                    AbortScript("ReadCtsFile(): Unable to read Text file.");
                    SetAppStatusPane("Okay");
                    return;
                }

                fclose(txtFilePtr);

// New ->
                origTrackPtr = trackList.GetTrack(trackNumber);

                // Save the original track info in case we want to
                // maintain the original track parameters and use
                // them with the new track content...
                if(origTrackPtr)
                {
                    origTrackView = origTrackPtr->GetTrackView();
                    origTrackParams = origTrackPtr->GetTrackParams();
                    origOutputDevice = origTrackPtr->GetTrackDevice();
                }
// <- New
                // Now add the text scroll object to the track list.
                textScrollPtr->SetTrackType(Track::TEXT);
                trackList.SetTrack(trackNumber, textScrollPtr);

                trackPtr = trackList.GetTrack(trackNumber);

                if (!trackPtr)
                {
                    AbortScript("ReadCtsFile(): Unable to retrieve trackPtr for Text track.");
                    SetAppStatusPane("Okay");
                    return;
                }

                if (setDefaults == M_TRUE)
                {
// should check for valid pointer here...
                    trackPtr->saveJobBlock.functionType = INVALID;
                    trackPtr->jobBlock.functionType = INVALID;

#ifdef CTS_SCRIPT_INFO
                    logFile << "ReadCtsFile(): Script calling InitTrack(TEXT)..." << endl;
#endif
                    InitTrack(Track::TEXT);

                } // end if set the default values
// New ->
                else
                {
                    if(origTrackPtr)
                    {
                        trackPtr->SetTrackView(origTrackView);
                        trackPtr->SetTrackParams(origTrackParams);
                        trackPtr->SetTrackDevice(origOutputDevice);
                    }
                }
// <- New

                m_TrackFocusButton = trackNumber - 1;

                SetTrackArmValue(trackNumber, TRUE);
                readNextCommand = M_TRUE;
                UpdateData(FALSE);
                SetAppStatusPane("Okay");

            } // end if FontList OK
            else
            {
                AbortScript("ReadCtsFile(): Font not properly initialized; scrolling text disabled.");
                SetAppStatusPane("Okay");
                return;
            }

        } // else got read text file data okay

    } // end else if Read Text File

    // ********** Set Global Delay (SGD) **********
    else if (strcmp(commandString, "SGD") == 0)
    {
        M_UINT16 tempGlobalDelay;

        if (!fscanf(ctsFilePtr,
                    "%d",
                    &tempGlobalDelay))
        {
            AbortScript("ReadCtsFile(): Can't get global delay value.");
            return;
        }
        else
        {
            if ((tempGlobalDelay >= MIN_GLOBAL_DELAY) && (tempGlobalDelay <= MAX_GLOBAL_DELAY))
            {
                globalTrackDelay = tempGlobalDelay;
            }
            else
            {
                AbortScript("ReadCtsFile(): Invalid global delay value specified.");
                return;
            }
        }

        readNextCommand = M_TRUE;

    } // end else if Set Laser Delay

    // ********** Set Laser Scale (SLS) **********
    else if (strcmp(commandString, "SLS") == 0)
    {
        M_UINT16 tempLaserScale;

        if (!fscanf(ctsFilePtr,
                    "%d",
                    &tempLaserScale))
        {
            AbortScript("ReadCtsFile(): Can't get laser scale value.");
            return;
        }
        else
        {
            if (tempLaserScale >= 1 && tempLaserScale <= 100)
            {
                laserScale = tempLaserScale;
            }
            else
            {
                AbortScript("ReadCtsFile(): Invalid laser scale value specified.");
                return;
            }
        }

        readNextCommand = M_TRUE;

    } // end else if Set Laser Scale

/*
    // ********** Set Device PPS (SDP) **********
    else if (strcmp(commandString, "SDP") == 0)
    {
        M_UINT16 tempPPS;

        if (!fscanf(ctsFilePtr,
                    "%hd ",
                    &trackNumber,
                    &tempPPS))
        {
            AbortScript("ReadCtsFile(): Can't get PPS value.");
            return;
        }
        else
        {
            if (outputDeviceArray[trackNumber]->GetOutputDeviceType() != RIYA_PCI &&
                outputDeviceArray[trackNumber]->GetOutputDeviceType() != RIYA_USB)
            {
                AbortScript("ReadCtsFile(): Invalid device type for SDP operation.");
                return;
            }

            if ((tempPPS < MINPPS) || (tempPPS > MAXPPS))
            {
                AbortScript("ReadCtsFile(): Invalid PPS value specified.");
                return;
            }
                
            SetDelayPeriodValue((UINT)((1.0/(double)m_EditPointsPerSecond)/0.00000003));
            globalTrackDelay = tempPPS;
        }

        readNextCommand = M_TRUE;

    } // end else if Set Device PPS
*/

    // ********** Set From Point (SFP) **********
    else if (strcmp(commandString, "SFP") == 0)
    {
        Point3DType fromPoint;

        if (!fscanf(ctsFilePtr,
                    "%d %lf %lf %lf",
                    &trackNumber,
                    &(fromPoint.x),
                    &(fromPoint.y),
                    &(fromPoint.z)))
        {
            AbortScript("ReadCtsFile(): Can't get from point data.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                ViewType newView = trackPtr->GetTrackView();
                newView.from_point = fromPoint;
                trackPtr->SetTrackView(newView);
            }
        }

        readNextCommand = M_TRUE;

    } // end else if Set From Point

    // ********** Set At Point (SAP) **********
    else if (strcmp(commandString, "SAP") == 0)
    {
        Point3DType atPoint;

        if (!fscanf(ctsFilePtr,
                    "%d %lf %lf %lf",
                    &trackNumber,
                    &(atPoint.x),
                    &(atPoint.y),
                    &(atPoint.z)))
        {
            AbortScript("ReadCtsFile(): Can't get at point data.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                ViewType newView = trackPtr->GetTrackView();
                newView.at_point = atPoint;
                trackPtr->SetTrackView(newView);
            }
        }

        readNextCommand = M_TRUE;

    } // end else if Set At Point

    // ********** Set Up Vector (SUV) **********
    else if (strcmp(commandString, "SUV") == 0)
    {
        Point3DType upVector;

        if (!fscanf(ctsFilePtr,
                    "%d %lf %lf %lf",
                    &trackNumber,
                    &(upVector.x),
                    &(upVector.y),
                    &(upVector.z)))
        {
            AbortScript("ReadCtsFile(): Can't get up vector data.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                ViewType newView = trackPtr->GetTrackView();
                newView.up_vector = upVector;
                trackPtr->SetTrackView(newView);
            }
        }

        readNextCommand = M_TRUE;

    } // end else if Set Up Vector

    // ********** Set Camera Angle (SCA) **********
    else if (strcmp(commandString, "SCA") == 0)
    {
        double cameraAngle;

        if (!fscanf(ctsFilePtr,
                    "%d %lf",
                    &trackNumber,
                    &cameraAngle))
        {
            AbortScript("ReadCtsFile(): Can't get camera angle data.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                ViewType newView = trackPtr->GetTrackView();
                newView.view_angle = cameraAngle;
                trackPtr->SetTrackView(newView);
                m_TrackCameraAngle = (UINT)cameraAngle;
                m_SliderTrackCameraAngle.SetPos(m_TrackCameraAngle);
            }
        }

        readNextCommand = M_TRUE;

    } // end else if Set Camera Angle

    // ********** Set Color Palette (SCP) **********
    else if (strcmp(commandString, "SCP") == 0)
    {
        M_UINT8 paletteType;

        if (!fscanf(ctsFilePtr,
                    "%d %d",
                    &trackNumber,
                    &paletteType))
        {
            AbortScript("ReadCtsFile(): Can't get color palette data.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                tempParams = trackPtr->GetTrackParams();

                switch ((enum PaletteType)paletteType)
                {
                    case PaletteType::PANGOLIN:
                        tempParams.palettePtr = &PangolinColorPalette[0];
                        break;
                    case PaletteType::LASER_ILLUSIONS:
                        tempParams.palettePtr = &LaserIllusionsColorPalette[0];
                        break;
                    case PaletteType::ILDA:
                        tempParams.palettePtr = &IldaColorPalette[0];
                        break;
                    case PaletteType::X29:
                        tempParams.palettePtr = &X29ColorPalette[0];
                        break;
                    default:
                        tempParams.palettePtr = &PangolinColorPalette[0];
                        break;
                }

                m_ILDA_Palette_Button = (int)paletteType;
                trackPtr->SetTrackParams(tempParams);
            }
        }

        readNextCommand = M_TRUE;

    } // end else if Set Color Palette

     // ********** Set Track Delay (STD) **********
    else if (strcmp(commandString, "STD") == 0)
    {
        long tempDelay;
        if (!fscanf(ctsFilePtr,
                    "%d %ld",
                    &trackNumber,
                    &tempDelay))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set track delay.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                tempParams = trackPtr->GetTrackParams();
                tempParams.delayValue = tempDelay;
                trackPtr->SetTrackParams(tempParams);

                if (trackNumber == m_TrackFocus)
                {
                    m_FocusTrackDelay = tempParams.delayValue;
                    m_SliderFocusTrackDelay.SetPos(m_FocusTrackDelay);
                    UpdateData(FALSE);
                }
            }
        }

        readNextCommand = M_TRUE;

    } // end if Set Track Delay

     // ********** Set Number of Fade Frames (SNFF) **********
    else if (strcmp(commandString, "SNFF") == 0)
    {
        M_UINT16 tempInFrames;
        M_UINT16 tempOutFrames;

        if (!fscanf(ctsFilePtr,
                    "%d %d %d",
                    &trackNumber,
                    &tempInFrames,
                    &tempOutFrames))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set fade frames.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                tempParams = trackPtr->GetTrackParams();
                tempParams.fadeFramesIn  = tempInFrames;
                tempParams.fadeFramesOut = tempOutFrames;
                trackPtr->SetTrackParams(tempParams);
            }
        }

        readNextCommand = M_TRUE;

    } // end if Set Fade Frames

     // ********** Set Track Output Device (STOD) **********
    else if (strcmp(commandString, "STOD") == 0)
    {
        M_UINT8 tempDevice;
        if (!fscanf(ctsFilePtr,
                    "%d %d",
                    &trackNumber,
                    &tempDevice))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set track output device.");
            return;
        }
        else
        {
            if(tempDevice >= 1 && tempDevice <= 4)
            {
                // Convert user input to array index...
                tempDevice -= 1;

                trackPtr = trackList.GetTrack(trackNumber);

                if (trackPtr)
                {
                    trackPtr->SetTrackDevice(tempDevice);
                }
            }
            else
            {
                AbortScript("ReadCtsFile(): Invalid device specified for set track output device.");
                return;
            }
        }

        readNextCommand = M_TRUE;

    } // end if Set Track Output Device

     // ********** Set Blanking Delay (SBD) **********
    else if (strcmp(commandString, "SBD") == 0)
    {
        M_UINT16 tempBlankDelay;

        if (!fscanf(ctsFilePtr,
                    "%d %d",
                    &trackNumber,
                    &tempBlankDelay))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set blanking delay.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                tempParams = trackPtr->GetTrackParams();
                tempParams.blankingDelay = tempBlankDelay;
                trackPtr->SetTrackParams(tempParams);

                if (trackNumber == m_TrackFocus)
                {
                    m_BlankingDelay = tempParams.blankingDelay;
                    m_SliderBlankDelay.SetPos(m_BlankingDelay);
                    UpdateData(FALSE);
                }
            }
        }

        readNextCommand = M_TRUE;

    } // end if Set Blanking Delay

     // ********** Set Track Scale (STS) **********
    else if (strcmp(commandString, "STS") == 0)
    {
        UINT tempTrackScale;

        if (!fscanf(ctsFilePtr,
                    "%d %d",
                    &trackNumber,
                    &tempTrackScale))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set track scale.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                tempParams = trackPtr->GetTrackParams();
                tempParams.trackScale = tempTrackScale;
                trackPtr->SetTrackParams(tempParams);

                if (trackNumber == m_TrackFocus)
                {
                    m_TrackScale = tempParams.trackScale;
                    m_SliderTrackScale.SetPos(m_TrackScale);
                    UpdateData(FALSE);
                }
            }
        }

        readNextCommand = M_TRUE;

    } // end if Set Laser Scale

     // ********** Set Track Arm State (STAS) **********
    else if (strcmp(commandString, "STAS") == 0)
    {
        UINT tempTrackArmState;

        if (!fscanf(ctsFilePtr,
                    "%d %d",
                    &trackNumber,
                    &tempTrackArmState))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set track arm state.");
            return;
        }
        else
        {
            SetTrackArmState(trackNumber, (EnumArmState)tempTrackArmState);
        }

        readNextCommand = M_TRUE;

    } // end if Set Track Arm State

    // ********** Set Display Mode (SDM) **********
    else if (strcmp(commandString, "SDM") == 0)
    {
          
        enum DisplayMode tempDisplayMode;

        if (!fscanf(ctsFilePtr,
                    "%d %d",
                    &trackNumber,
                    &tempDisplayMode))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set display mode.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                tempParams = trackPtr->GetTrackParams();

                switch ((enum DisplayMode)tempDisplayMode)
                {
                    case NO_DISPLAY:
                        tempParams.displayMode = 0x0000;
                        break;
                    case NORMAL_DISPLAY:
                    {
                        tempParams.displayMode = 0x0001;
                    }
                        break;
                    case POINTS_ONLY_DISPLAY:
                    {
                        tempParams.displayMode = 0x0011;
                    }
                        break;
                    default:
                        break;
                }

                trackPtr->SetTrackParams(tempParams);

                if (trackNumber == m_TrackFocus)
                {
                    if (tempParams.displayMode & POINTS_ONLY)
                    {
                        m_PointsOnly = TRUE;
                    }
                    else
                    {
                        m_PointsOnly = FALSE;
                    }

                    UpdateData(FALSE);
                }
            }
        }

        readNextCommand = M_TRUE;

    } // end if Set Display Mode

     // ********** Set Color Factor (SCF) **********
    else if (strcmp(commandString, "SCF") == 0)
    {
        double tempColorFactor;

        if (!fscanf(ctsFilePtr,
                    "%d %lf",
                    &trackNumber,
                    &tempColorFactor))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set color factor.");
            return;
        }
        else
        {
            if (tempColorFactor > 1.0 || tempColorFactor < 0.0)
            {
                AbortScript("ReadCtsFile(): Invalid color factor specified.");
                return;
            }

            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                tempParams = trackPtr->GetTrackParams();
                tempParams.colorFactor = tempColorFactor;
                trackPtr->SetTrackParams(tempParams);

                switch(trackNumber)
                {
                    case 1:
                    {
                        if (tempParams.colorFactor > 0.0)
                        {
                            m_DisplayTrack1 = TRUE;
                        }
                        else
                        {
                            m_DisplayTrack1 = FALSE;
                        }
                    }
                    break;
                    case 2:
                    {
                        if (tempParams.colorFactor > 0.0)
                        {
                            m_DisplayTrack2 = TRUE;
                        }
                        else
                        {
                            m_DisplayTrack2 = FALSE;
                        }
                    }
                    break;
                    case 3:
                    {
                        if (tempParams.colorFactor > 0.0)
                        {
                            m_DisplayTrack3 = TRUE;
                        }
                        else
                        {
                            m_DisplayTrack3 = FALSE;
                        }
                    }
                    break;
                    case 4:
                    {
                        if (tempParams.colorFactor > 0.0)
                        {
                            m_DisplayTrack4 = TRUE;
                        }
                        else
                        {
                            m_DisplayTrack4 = FALSE;
                        }
                    }
                    break;
                    default:
                    break;
                }

                UpdateData(FALSE);
            }
        }

        readNextCommand = M_TRUE;

    } // end if Set Color Factor

     // ********** Set Color Balance (SCB) **********
    else if (strcmp(commandString, "SCB") == 0)
    {
        double tempRedLevel;
        double tempGreenLevel;
        double tempBlueLevel;

        if (!fscanf(ctsFilePtr,
                    "%d %lf %lf %lf",
                    &trackNumber,
                    &tempRedLevel,
                    &tempGreenLevel,
                    &tempBlueLevel))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set color balance.");
            return;
        }
        else
        {
            if (tempRedLevel > 1.0   || tempRedLevel < 0.0   ||
                tempGreenLevel > 1.0 || tempGreenLevel < 0.0 ||
                tempBlueLevel > 1.0  || tempBlueLevel < 0.0)
            {
                AbortScript("ReadCtsFile(): Invalid color balance value specified.");
                return;
            }

            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                tempParams = trackPtr->GetTrackParams();
                tempParams.redLevel   = tempRedLevel;
                tempParams.greenLevel = tempGreenLevel;
                tempParams.blueLevel  = tempBlueLevel;
                trackPtr->SetTrackParams(tempParams);

                if (trackNumber == m_TrackFocus)
                {
                    m_TrackRedBalance = (UINT)(tempParams.redLevel * 100);
                    m_SliderTrackRedBalance.SetPos(m_TrackRedBalance);

                    m_TrackGreenBalance = (UINT)(tempParams.greenLevel * 100);
                    m_SliderTrackGreenBalance.SetPos(m_TrackGreenBalance);

                    m_TrackBlueBalance = (UINT)(tempParams.blueLevel * 100);
                    m_SliderTrackBlueBalance.SetPos(m_TrackBlueBalance);
                }
            }
         }

        readNextCommand = M_TRUE;

    } // end if Set Color Balance

     // ********** Set Frame Display Range (SFDR) **********
    else if (strcmp(commandString, "SFDR") == 0)
    {
        long   startFrameNumber;
        M_UINT16 stopFrameNumber;

        if (!fscanf(ctsFilePtr,
                    "%d %ld %d",
                    &trackNumber,
                    &startFrameNumber,
                    &stopFrameNumber))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set frame display range.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                if (trackPtr->GetTrackType() != Track::ILDA)
                {
                    AbortScript("ReadCtsFile(): Track type incorrect for set frame display range.");
                    return;
                }

                M_UINT16 numOfFrames = ((FrameSequence *)trackPtr)->GetNumberOfFrames();
                DisplayParams localTempParams = trackPtr->GetTrackParams();

                if ( ((localTempParams.displayReverse == M_FALSE) && (stopFrameNumber < startFrameNumber)) ||
                     ((localTempParams.displayReverse == M_TRUE)  && (stopFrameNumber > startFrameNumber)) ||
                     (startFrameNumber > numOfFrames)     ||
                     (stopFrameNumber > numOfFrames) )
                {
                    AbortScript("ReadCtsFile(): Invalid parameters specified for set frame display range.");
                    return;
                }
            
                if (startFrameNumber >= 0)
                {
                    if (((FrameSequence*)trackPtr)->SetCurrentFrame((M_UINT16)startFrameNumber)    
                        != M_SUCCESS)
                    {
                        AbortScript("ReadCtsFile(): Failure calling SetCurrentFrame().");
                        return;
                    }
                }

                localTempParams.startFrameNumber   = startFrameNumber;
                localTempParams.currentFrameNumber = (M_UINT16)startFrameNumber;
                localTempParams.stopFrameNumber    = stopFrameNumber;
                trackPtr->SetTrackParams(localTempParams);
            }

        } // end else retrieved data okay

        readNextCommand = M_TRUE;

    } // end if Set Frame Display Range

    // ********** Set Text Font (STF) **********
    else if (strcmp(commandString, "STF") == 0)
    {
        M_UINT16 newFontValue;

        if (!fscanf(ctsFilePtr,
                    "%d",
                    &newFontValue))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set text font.");
            return;
        }
        else
        {
            if (newFontValue >= INVALID_FONT)
            {
                AbortScript("ReadCtsFile(): Invalid font value specified for set text font.");
                return;
            }
            else
            {
                FontList::SetFontList((FontType_T)newFontValue);
            }

        } // end else retrieved data okay

        readNextCommand = M_TRUE;

    } // end if Set Text Font

    // ********** Read Font File (RFF) **********
    else if (strcmp(commandString, "RFF") == 0)
    {
        char newFontFile[MAX_FILENAME_SIZE];
        M_UINT16 newFontListValue;
        FontType_T oldFontListValue;

        if (!fscanf(ctsFilePtr,
                    "%d %s",
                    &newFontListValue,
                    &newFontFile[0]))
        {
            AbortScript("ReadCtsFile(): Can't get data for read font file.");
            return;
        }
        else
        {
            if(newFontListValue > DEFAULT_FONT &&
               newFontListValue < INVALID_FONT)
            {
                strcpy(tempString2, contentDirectoryName);
                strcat(tempString2, newFontFile);

                oldFontListValue = FontList::GetFontListValue();

                FontList::SetFontList((FontType_T)newFontListValue);
                FontList::DeleteFontList();
                
                if (FontList::ReadFontFile(&tempString2[0]) == M_FAILURE)
                {
                    AbortScript("ReadCtsFile(): Unable to open and read specified font file.");
                    return;
                }

                FontList::SetFontList(oldFontListValue);

            } // end if valid font list value
            else
            {
                AbortScript("ReadCtsFile(): Invalid font list value specified.");
                return;
            }

        } // else got read font file data okay

    } // end else if Read Font File

    // ********** Set Text Scroll Rate (STSR) **********
    else if (strcmp(commandString, "STSR") == 0)
    {
        M_UINT16 newScrollRate;

        if (!fscanf(ctsFilePtr,
                    "%d %d",
                    &trackNumber,
                    &newScrollRate))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set text scroll rate.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                if (trackPtr->GetTrackType() != Track::TEXT)
                {
                    AbortScript("ReadCtsFile(): Track type incorrect for set text scroll rate.");
                    return;
                }
                
                ((TextScroll *)trackPtr)->SetScrollQuickness(newScrollRate);

            }

        } // end else retrieved data okay

        readNextCommand = M_TRUE;

    } // end if Set Text Scroll Rate

    // ********** Set Z-axis Parameters (SZP) **********
    else if (strcmp(commandString, "SZP") == 0)
    {
        M_UINT16 newZAxisFormula;
        M_UINT16 newZAxisOffset;
        M_UINT16 newZAxisGain;
        double   newZAxisPeriod;

        if (!fscanf(ctsFilePtr,
                    "%d %d %d %d %lf",
                    &trackNumber,
                    &newZAxisFormula,
                    &newZAxisOffset,
                    &newZAxisGain,
                    &newZAxisPeriod))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set Z axis parameters.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                if (trackPtr->GetTrackType() != Track::TEXT)
                {
                    AbortScript("ReadCtsFile(): Track type incorrect for set Z axis parameters.");
                    return;
                }
                
                if (newZAxisFormula < 1 || newZAxisFormula > 8)
                {
                    AbortScript("ReadCtsFile(): Invalid Z axis formula specified.");
                    return;
                }

                if (newZAxisOffset > 360)
                {
                    AbortScript("ReadCtsFile(): Invalid Z axis offset specified.");
                    return;
                }

                if (newZAxisGain > 100)
                {
                    AbortScript("ReadCtsFile(): Invalid Z axis gain specified.");
                    return;
                }

                if (newZAxisPeriod == 0.0 || newZAxisPeriod > 20.0)
                {
                    AbortScript("ReadCtsFile(): Invalid Z axis period specified.");
                    return;
                }

                trackPtr->ZAxisFormula = newZAxisFormula;
                trackPtr->ZAxisOffset  = newZAxisOffset;
                trackPtr->ZAxisGain    = newZAxisGain;
                trackPtr->ZAxisPeriod  = newZAxisPeriod;

 logFile << "ZAxisOffset is: " << newZAxisOffset << endl;
            }

        } // end else retrieved data okay

        readNextCommand = M_TRUE;

    } // end if Set Z-axis Parameters

    // ********** Set Frame Sequence Reverse (SFSR) **********
    else if (strcmp(commandString, "SFSR") == 0)
    {
        M_UINT16 frameSequenceReverse;

        if (!fscanf(ctsFilePtr,
                    "%d %d",
                    &trackNumber,
                    &frameSequenceReverse))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set frame sequence reverse.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                if (trackPtr->GetTrackType() != Track::ILDA)
                {
                    AbortScript("ReadCtsFile(): Track type incorrect for set frame sequence reverse.");
                    return;
                }

                if ( frameSequenceReverse != M_FALSE &&
                     frameSequenceReverse != M_TRUE)
                {
                    AbortScript("ReadCtsFile(): Invalid parameters specified for set frame sequence reverse.");
                    return;
                }
            
                DisplayParams localTempParams = trackPtr->GetTrackParams();
                localTempParams.displayReverse = frameSequenceReverse;
                trackPtr->SetTrackParams(localTempParams);

                if (trackNumber == m_TrackFocus)
                {
                    if (frameSequenceReverse == M_TRUE)
                    {
                        m_Reverse = TRUE;
                    }
                    else
                    {
                        m_Reverse = FALSE;
                    }

                    UpdateData(FALSE);
                }
            }

        } // end else retrieved data okay

        readNextCommand = M_TRUE;

    } // end if Set Frame Sequence Reverse

    // ********** Set Beam View (SBV) **********
    else if (strcmp(commandString, "SBV") == 0)
    {
        M_UINT16 trackBeamView;

        if (!fscanf(ctsFilePtr,
                    "%d %d",
                    &trackNumber,
                    &trackBeamView))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set beam view.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                if ( trackBeamView != M_FALSE &&
                     trackBeamView != M_TRUE)
                {
                    AbortScript("ReadCtsFile(): Invalid parameters specified for set beam view.");
                    return;
                }
            
                DisplayParams localTempParams = trackPtr->GetTrackParams();
                localTempParams.displayBeamView = trackBeamView;
                trackPtr->SetTrackParams(localTempParams);

                if (trackNumber == m_TrackFocus)
                {
                    if (trackBeamView == M_TRUE)
                    {
                        m_DisplayBeamView = TRUE;
                    }
                    else
                    {
                        m_DisplayBeamView = FALSE;
                    }

                    UpdateData(FALSE);
                }
            }

        } // end else retrieved data okay

        readNextCommand = M_TRUE;

    } // end if Set Beam View

    // ********** Set Color Factor Incrementally (SCFI) **********
    else if (strcmp(commandString, "SCFI") == 0)
    {
        double tempColorFactor;
        double colorFactorIncrement;

        if (!fscanf(ctsFilePtr,
                    "%d %lf",
                    &trackNumber,
                    &colorFactorIncrement))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set color factor incrementally.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                tempParams = trackPtr->GetTrackParams();
                tempColorFactor = tempParams.colorFactor + colorFactorIncrement;

                if (tempColorFactor > 1.0)
                {
                    tempColorFactor = 1.0;
                }
                else if (tempColorFactor < 0.0)
                {
                    tempColorFactor = 0.0;
                }
            
                tempParams.colorFactor = tempColorFactor;
                trackPtr->SetTrackParams(tempParams);

                switch(trackNumber)
                {
                    case 1:
                    {
                        if (tempParams.colorFactor > 0.0)
                        {
                            m_DisplayTrack1 = TRUE;
                        }
                        else
                        {
                            m_DisplayTrack1 = FALSE;
                        }
                    }
                    break;

                    case 2:
                    {
                        if (tempParams.colorFactor > 0.0)
                        {
                            m_DisplayTrack2 = TRUE;
                        }
                        else
                        {
                            m_DisplayTrack2 = FALSE;
                        }
                    }
                    break;

                    case 3:
                    {
                        if (tempParams.colorFactor > 0.0)
                        {
                            m_DisplayTrack3 = TRUE;
                        }
                        else
                        {
                            m_DisplayTrack3 = FALSE;
                        }
                    }
                    break;

                    case 4:
                    {
                        if (tempParams.colorFactor > 0.0)
                        {
                            m_DisplayTrack4 = TRUE;
                        }
                        else
                        {
                            m_DisplayTrack4 = FALSE;
                        }
                    }
                    break;

                    default:
                    break;

                } // end switch on track number

                UpdateData(FALSE);

            } // end if active track pointer
       }

        readNextCommand = M_TRUE;

    } // end if Set Color Factor Incrementally

     // ********** Set Color Balance Incrementally (SCBI) **********
    else if (strcmp(commandString, "SCBI") == 0)
    {
        double tempRedLevel;
        double tempGreenLevel;
        double tempBlueLevel;
        double redLevelIncrement;
        double greenLevelIncrement;
        double blueLevelIncrement;

        if (!fscanf(ctsFilePtr,
                    "%d %lf %lf %lf",
                    &trackNumber,
                    &redLevelIncrement,
                    &greenLevelIncrement,
                    &blueLevelIncrement))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set color balance incrementally.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                tempParams = trackPtr->GetTrackParams();
                tempRedLevel   = tempParams.redLevel   + redLevelIncrement;
                tempGreenLevel = tempParams.greenLevel + greenLevelIncrement;
                tempBlueLevel  = tempParams.blueLevel  + blueLevelIncrement;

                if (tempRedLevel > 1.0)
                {
                    tempRedLevel = 1.0;
                }
                else if (tempRedLevel < 0.0)
                {
                    tempRedLevel = 0.0;
                }
            
                if (tempGreenLevel > 1.0)
                {
                    tempGreenLevel = 1.0;
                }
                else if (tempGreenLevel < 0.0)
                {
                    tempGreenLevel = 0.0;
                }
            
                if (tempBlueLevel > 1.0)
                {
                    tempBlueLevel = 1.0;
                }
                else if (tempBlueLevel < 0.0)
                {
                    tempBlueLevel = 0.0;
                }

                tempParams.redLevel   = tempRedLevel;
                tempParams.greenLevel = tempGreenLevel;
                tempParams.blueLevel  = tempBlueLevel;
                trackPtr->SetTrackParams(tempParams);

                m_TrackRedBalance = (UINT)(tempParams.redLevel * 100);
                m_SliderTrackRedBalance.SetPos(m_TrackRedBalance);

                m_TrackGreenBalance = (UINT)(tempParams.greenLevel * 100);
                m_SliderTrackGreenBalance.SetPos(m_TrackGreenBalance);

                m_TrackBlueBalance = (UINT)(tempParams.blueLevel * 100);
                m_SliderTrackBlueBalance.SetPos(m_TrackBlueBalance);
            }
        }

        readNextCommand = M_TRUE;

    } // end if Set Color Balance Incrementally

     // ********** Set Track Repeat (STR) **********
    else if (strcmp(commandString, "STR") == 0)
    {
        M_INT32 tempRepeat;

        if (!fscanf(ctsFilePtr,
                    "%d %d",
                    &trackNumber,
                    &tempRepeat))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set track repeat.");
            return;
        }
        else
        {
            trackPtr = trackList.GetTrack(trackNumber);
            if (trackPtr)
            {
                tempParams = trackPtr->GetTrackParams();
                tempParams.frameRepeat = tempRepeat;
                trackPtr->SetTrackParams(tempParams);

                if (trackNumber == m_TrackFocus)
                {
                    m_FocusTrackRepeat = tempParams.frameRepeat;
                    m_SliderFocusTrackRepeat.SetPos(m_FocusTrackRepeat);
                    UpdateData(FALSE);
                }
            }
        }

        readNextCommand = M_TRUE;

    } // end if Set Frame Repeat

    // ********** Set Rotate About Zero (SRAZ) **********
    else if (strcmp(commandString, "SRAZ") == 0)
    {
        M_UINT16 rotateAboutZero;

        if (!fscanf(ctsFilePtr,
                    "%d %d",
                    &trackNumber,
                    &rotateAboutZero))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for set rotate about zero.");
            return;
        }
        else
        {
            switch(trackNumber)
            {
            case 1:
                if (rotateAboutZero == 0)
                {
                    m_RotateAbout0_1 = FALSE;
                }
                else
                {
                    m_RotateAbout0_1 = TRUE;
                }
                break;
            case 2:
                if (rotateAboutZero == 0)
                {
                    m_RotateAbout0_2 = FALSE;
                }
                else
                {
                    m_RotateAbout0_2 = TRUE;
                }
                break;
            case 3:
                if (rotateAboutZero == 0)
                {
                    m_RotateAbout0_3 = FALSE;
                }
                else
                {
                    m_RotateAbout0_3 = TRUE;
                }
                break;
            case 4:
                if (rotateAboutZero == 0)
                {
                    m_RotateAbout0_4 = FALSE;
                }
                else
                {
                    m_RotateAbout0_4 = TRUE;
                }
                break;
            default:
                return;
                AbortScript("ReadCtsFile(): Invalid track specified for set rotate about zero.");
                break;

            } // end switch on track number

            UpdateData(FALSE);
        }

        readNextCommand = M_TRUE;

    } // end if Set Rotate About Zero

   // ********** Remove Track (RT) **********
    else if (strcmp(commandString, "RT") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%d",
                    &trackNumber))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for remove track.");
            return;
        }
        else
        {
            if (trackList.SetTrack(trackNumber, 0) != M_SUCCESS)
            {
                AbortScript("ReadCtsFile(): Call to	SetTrack() failed.");
                return;
            }

            SetTrackArmValue(trackNumber, FALSE);
            SetTrackDisplayValue(trackNumber, FALSE);
            readNextCommand = M_TRUE;
            UpdateData(FALSE);

        } // end if successfully read data

    } // end if Remove Track

   // ********** Just Display (JD) **********
    else if (strcmp(commandString, "JD") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%ld",
                    &(jobBlock.numOfFrames)))
        {
            AbortScript("ReadCtsFile(): Can't retrieve data for just display.");
            return;
        }
        else
        {
            jobBlock.functionType = JUST_DISPLAY;
            jobBlock.skipManipulation = M_TRUE;

            for (int i=1 ; i<=MAX_TRACKS ; ++i)
            {
                SetTrackJobBlock(jobBlock, i);
            }
            
            DisplayAllTracks();
        }

        // We never read the next command here, so don't even
        // bother setting readNextCommand.
        return;

    } // end else if Just Display

    // ********** Manipulate Fade In (MFI) **********
    else if (strcmp(commandString, "MFI") == 0)
    {
        M_UINT16 tempNumberOfFrames;

        if (!fscanf(ctsFilePtr,
                    "%d %d %d",
                    &trackNumber,
                    &tempNumberOfFrames,
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for fade in.");
            return;
        }
        else
        {
            if (tempNumberOfFrames == 0)
            {
                trackPtr = trackList.GetTrack(trackNumber);

                if (trackPtr)
                {
                    tempParams = trackPtr->GetTrackParams();

                    if (tempParams.fadeFramesIn == 0)
                    {
                        jobBlock.numOfFrames = CCheap3DView::fadeInFrames;
                    }
                    else
                    {
                        jobBlock.numOfFrames = tempParams.fadeFramesIn;
                    }
                }
                else
                {
                    jobBlock.numOfFrames = 1;
                }
            }
            else
            {
                jobBlock.numOfFrames = (M_UINT32)(tempNumberOfFrames);
            }

            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = FADE;
            jobBlock.fadeDirection = FADE_IN;
            jobBlock.reentry = M_FALSE;

            SetTrackJobBlock(jobBlock, trackNumber);
            SetTrackDisplayValue(trackNumber, TRUE);
            UpdateData(FALSE);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }

        } // end if successfully read data

    } // end else if Manipulate Fade In

    // ********** Manipulate Fade Out (MFO) **********
    else if (strcmp(commandString, "MFO") == 0)
    {
        M_UINT16 tempNumberOfFrames;

        if (!fscanf(ctsFilePtr,
                    "%d %d %d",
                    &trackNumber,
                    &tempNumberOfFrames,
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for fade out.");
            return;
        }
        else
        {
            if (tempNumberOfFrames == 0)
            {
                trackPtr = trackList.GetTrack(trackNumber);

                if (trackPtr)
                {
                    tempParams = trackPtr->GetTrackParams();

                    if (tempParams.fadeFramesOut == 0)
                    {
                        jobBlock.numOfFrames = CCheap3DView::fadeOutFrames;
                    }
                    else
                    {
                        jobBlock.numOfFrames = tempParams.fadeFramesOut;
                    }
                }
                else
                {
                    jobBlock.numOfFrames = 1;
                }
            }
            else
            {
                jobBlock.numOfFrames = (M_UINT32)(tempNumberOfFrames);
            }

            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = FADE;
            jobBlock.fadeDirection = FADE_OUT;
            jobBlock.reentry = M_FALSE;

            SetTrackJobBlock(jobBlock, trackNumber);
            SetTrackDisplayValue(trackNumber, FALSE);
            UpdateData(FALSE);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }

        } // end if successfully read data

    } // end else if Manipulate Fade Out

    // ********** Manipulate Cross Fade (MCF) **********
    else if (strcmp(commandString, "MCF") == 0)
    {
        M_UINT16 fadeInTrackNumber;
        M_UINT16 fadeOutTrackNumber;
        M_UINT16 tempNumberOfFrames;
//		M_UINT16 tempFadeOutFrames;
        
        if (!fscanf(ctsFilePtr,
                    "%d %d %ld %d",
                    &fadeInTrackNumber,
                    &fadeOutTrackNumber,
                    &tempNumberOfFrames,
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for cross fade.");
            return;
        }
        else
        {
            if (tempNumberOfFrames == 0)
            {
//                DisplayParams tempParams2;
//				Track* trackPtr2 = trackList.GetTrack(fadeOutTrackNumber);

                trackPtr = trackList.GetTrack(fadeInTrackNumber);

//                if (trackPtr && trackPtr2)
                if (trackPtr)
                {
                    tempParams  = trackPtr->GetTrackParams();
//					tempParams2 = trackPtr2->GetTrackParams();

                    if (tempParams.fadeFramesIn == 0)
                    {
                        jobBlock.numOfFrames = CCheap3DView::fadeInFrames;
                    }
                    else
                    {
                        jobBlock.numOfFrames = tempParams.fadeFramesIn;
                    }

//					if (tempParams2.fadeFrames == 0)
//					{
//                        tempFadeOutFrames = CCheap3DView::fadeOutFrames;
//					}
//					else
//					{
//                        tempFadeOutFrames = tempParams2.fadeFrames;
//					}
                }
                else
                {
                    jobBlock.numOfFrames = 1;
//					tempFadeOutFrames = 1;
                }
            }
            else
            {
                jobBlock.numOfFrames = (M_UINT32)(tempNumberOfFrames);
//				tempFadeOutFrames = tempNumberOfFrames;
            }

            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = FADE;
            jobBlock.reentry = M_FALSE;

            jobBlock.fadeDirection = FADE_IN;
            SetTrackJobBlock(jobBlock, fadeInTrackNumber);
            SetTrackDisplayValue(fadeInTrackNumber, TRUE);

            jobBlock.fadeDirection = FADE_OUT;
//			jobBlock.numOfFrames = tempFadeOutFrames;
            SetTrackJobBlock(jobBlock, fadeOutTrackNumber);
            SetTrackDisplayValue(fadeOutTrackNumber, FALSE);

            UpdateData(FALSE);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }

        } // end if successfully read data

    } // end else if Manipulate Cross Fade

    // ********** Manipulate From Rotate X (MFRX) **********
    else if (strcmp(commandString, "MFRX") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%d %ld %f %d",
                    &trackNumber,
                    &(jobBlock.numOfFrames),
                    &(jobBlock.degreeAngle),
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for from point X rotation.");
            return;
        }
        else
        {
            if (readNextCommand == M_TRUE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = ROTATE_FROM;
            jobBlock.rotationAxis = X_AXIS;

            SetTrackJobBlock(jobBlock, trackNumber);
            if (readNextCommand == M_TRUE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }
        }

    } // end else if Manipulate From Rotate X

    // ********** Manipulate Rotate Y (MFRY) **********
    else if (strcmp(commandString, "MFRY") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%d %ld %f %d",
                    &trackNumber,
                    &(jobBlock.numOfFrames),
                    &(jobBlock.degreeAngle),
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for from point Y rotation.");
            return;
        }
        else
        {
            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = ROTATE_FROM;
            jobBlock.rotationAxis = Y_AXIS;

            SetTrackJobBlock(jobBlock, trackNumber);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }
        }

    } // end else if Manipulate From Rotate Y

    // ********** Manipulate Rotate Z (MFRZ) **********
    else if (strcmp(commandString, "MFRZ") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%d %ld %f %d",
                    &trackNumber,
                    &(jobBlock.numOfFrames),
                    &(jobBlock.degreeAngle),
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for from point Z rotation.");
            return;
        }
        else
        {
            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = ROTATE_FROM;
            jobBlock.rotationAxis = Z_AXIS;

            SetTrackJobBlock(jobBlock, trackNumber);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }
        }

    } // end else if Manipulate From Rotate Z

    // ********** Manipulate At Rotate X (MARX) **********
    else if (strcmp(commandString, "MARX") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%d %ld %f %d",
                    &trackNumber,
                    &(jobBlock.numOfFrames),
                    &(jobBlock.degreeAngle),
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for at point X rotation.");
            return;
        }
        else
        {
            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = ROTATE_AT;
            jobBlock.rotationAxis = X_AXIS;

            SetTrackJobBlock(jobBlock, trackNumber);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }
        }

    } // end else if Manipulate At Rotate X

    // ********** Manipulate Rotate Y (MARY) **********
    else if (strcmp(commandString, "MARY") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%d %ld %f %d",
                    &trackNumber,
                    &(jobBlock.numOfFrames),
                    &(jobBlock.degreeAngle),
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for at point Y rotation.");
            return;
        }
        else
        {
            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = ROTATE_AT;
            jobBlock.rotationAxis = Y_AXIS;

            SetTrackJobBlock(jobBlock, trackNumber);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }
        }

    } // end else if Manipulate At Rotate Y

    // ********** Manipulate Rotate Z (MARZ) **********
    else if (strcmp(commandString, "MARZ") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%d %ld %f %d",
                    &trackNumber,
                    &(jobBlock.numOfFrames),
                    &(jobBlock.degreeAngle),
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for at point Z rotation.");
            return;
        }
        else
        {
            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = ROTATE_AT;
            jobBlock.rotationAxis = Z_AXIS;

            SetTrackJobBlock(jobBlock, trackNumber);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }
        }

    } // end else if Manipulate At Rotate Z

    // ********** Manipulate At Rotate X (MBRX) **********
    else if (strcmp(commandString, "MBRX") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%d %ld %f %d",
                    &trackNumber,
                    &(jobBlock.numOfFrames),
                    &(jobBlock.degreeAngle),
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for both point X rotation.");
            return;
        }
        else
        {
            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = ROTATE_BOTH;
            jobBlock.rotationAxis = X_AXIS;

            SetTrackJobBlock(jobBlock, trackNumber);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }
        }

    } // end else if Manipulate Both Rotate X

    // ********** Manipulate Rotate Y (MBRY) **********
    else if (strcmp(commandString, "MBRY") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%d %ld %f %d",
                    &trackNumber,
                    &(jobBlock.numOfFrames),
                    &(jobBlock.degreeAngle),
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for both point Y rotation.");
            return;
        }
        else
        {
            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = ROTATE_BOTH;
            jobBlock.rotationAxis = Y_AXIS;

            SetTrackJobBlock(jobBlock, trackNumber);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }
        }

    } // end else if Manipulate Both Rotate Y

    // ********** Manipulate Rotate Z (MBRZ) **********
    else if (strcmp(commandString, "MBRZ") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%d %ld %f %d",
                    &trackNumber,
                    &(jobBlock.numOfFrames),
                    &(jobBlock.degreeAngle),
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for both point Z rotation.");
            return;
        }
        else
        {
            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = ROTATE_BOTH;
            jobBlock.rotationAxis = Z_AXIS;

            SetTrackJobBlock(jobBlock, trackNumber);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }
        }

    } // end else if Manipulate Both Rotate Z

    // ********** Manipulate Zoom In (MZI) **********
    else if (strcmp(commandString, "MZI") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%d %ld %f %d",
                    &trackNumber,
                    &(jobBlock.numOfFrames),
                    &(jobBlock.zoomIncrement),
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for zoom in.");
            return;
        }
        else
        {
            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = ZOOM;
            jobBlock.zoomDirection = ZOOM_IN;

            SetTrackJobBlock(jobBlock, trackNumber);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }
        }

    } // end else if Manipulate Zoom In

    // ********** Manipulate Zoom Out (MZO) **********
    else if (strcmp(commandString, "MZO") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%d %ld %f %d",
                    &trackNumber,
                    &(jobBlock.numOfFrames),
                    &(jobBlock.zoomIncrement),
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for zoom out.");
            return;
        }
        else
        {
            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = ZOOM;
            jobBlock.zoomDirection = ZOOM_OUT;

            SetTrackJobBlock(jobBlock, trackNumber);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }
        }

    } // end else if Manipulate Zoom Out

    // ********** Manipulate Move Forward (MMF) **********
    else if (strcmp(commandString, "MMF") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%d %ld %f %d",
                    &trackNumber,
                    &(jobBlock.numOfFrames),
                    &(jobBlock.xIncrement),
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for moving forward.");
            return;
        }
        else
        {
            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = MOVE_FORWARD;

            SetTrackJobBlock(jobBlock, trackNumber);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }
        }

    } // end else if Manipulate Move Forward

    // ********** Manipulate Move At Point (MAP) **********
    else if (strcmp(commandString, "MAP") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%d %ld %f %f %f %d",
                    &trackNumber,
                    &(jobBlock.numOfFrames),
                    &(jobBlock.xIncrement),
                    &(jobBlock.yIncrement),
                    &(jobBlock.zIncrement),
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for moving at point.");
            return;
        }
        else
        {
            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = MOVE_AT;

            SetTrackJobBlock(jobBlock, trackNumber);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }
        }

    } // end else if Manipulate Move At Point

    // ********** Manipulate Move From Point (MFP) **********
    else if (strcmp(commandString, "MFP") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%d %ld %f %f %f %d",
                    &trackNumber,
                    &(jobBlock.numOfFrames),
                    &(jobBlock.xIncrement),
                    &(jobBlock.yIncrement),
                    &(jobBlock.zIncrement),
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for moving from point.");
            return;
        }
        else
        {
            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = MOVE_FROM;
            SetTrackJobBlock(jobBlock, trackNumber);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }
        }

    } // end else if Manipulate Move From point

    // ********** Manipulate Move Both Points (MBP) **********
    else if (strcmp(commandString, "MBP") == 0)
    {
        if (!fscanf(ctsFilePtr,
                    "%d %ld %f %f %f %d",
                    &trackNumber,
                    &(jobBlock.numOfFrames),
                    &(jobBlock.xIncrement),
                    &(jobBlock.yIncrement),
                    &(jobBlock.zIncrement),
                    &readNextCommand))
        {
            AbortScript("ReadCtsFile(): Can't get data for moving both points.");
            return;
        }
        else
        {
            if (readNextCommand != M_FALSE)
            {
                jobBlock.skipManipulation = M_TRUE;
            }
            else
            {
                jobBlock.skipManipulation = M_FALSE;
            }
            jobBlock.functionType = MOVE_BOTH;
            SetTrackJobBlock(jobBlock, trackNumber);
            if (readNextCommand != M_FALSE)
            {
                DoTrackManipulation();
            }
            else
            {
                DisplayAllTracks();
            }
        }

    } // end else if Manipulate Move Both Points

    else
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
    logFile << "ReadCtsFile(): Unknown command \"" 
            << commandString << "\" encountered." << endl;
#endif

        AbortScript("ReadCtsFile(): Unknown command encountered.");
        return;
    }

    } // end while readNextCommand

} // end function ReadCtsFile

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::AbortScript()
//
////////////////////////////////////////////////////////////////////
void
CCheap3DView::AbortScript(LPCTSTR abortMsg)
{
#ifdef CHEAP3DVIEW_LOG_ERROR
    logFile << abortMsg << endl;
#endif
    KillTimer(DISPLAY_TIMER);
    displayTimerSet = FALSE;
    KillTimer(POINTS_PER_SECOND_TIMER);
    KillTimer(PLAYBACK_TIMER);
    playbackTime = 0;
    SetPlaybackTimerPane((long)playbackTime);
    if(ctsFilePtr)
    {
        fclose(ctsFilePtr);
        ctsFilePtr = 0;
    }
    loopCounter = 0;
    numberOfLoops = 0;
    scriptActive = M_FALSE;
    commandInProgress = M_FALSE;
    InitTrackVariables();
    CleanViewPort();

    DisplayParams tempParams;
    trackList.ResetIterator();
    trackPtr = trackList.GetCurrentTrack();

    for (M_UINT16 i=0 ; i<MAX_TRACKS ; ++i)
    {
        if (trackPtr)
        {
            tempParams = trackPtr->GetTrackParams();
            tempParams.displayMode &= ~DISPLAY_ON;
            tempParams.colorFactor = 0.0;
            trackPtr->SetTrackParams(tempParams);
        }

        trackPtr = trackList.GetNextTrack();
    }

    globalTrackDelay = defaultGlobalDelay;
    laserScale       = defaultLaserScale;
    verticalScale    = defaultVerticalScale;

    UpdateData(FALSE);

#ifdef ABORT_SCRIPT_MESSAGE_BOX
    if (strlen(abortMsg) > 0)
    {
        MessageBox(abortMsg);
        logFile << "CCheap3DView::AbortScript(): " << abortMsg << endl;
    }
#endif

}

