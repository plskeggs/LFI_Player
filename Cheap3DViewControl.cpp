//
// *********************************************************
// *                                                       *
// * Copyright 1998-2008, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Cheap3DViewControl.cpp                     *
// * Description: Contains the implementation of those     *
// * methods on the Cheap3DView class which pertain to the *
// * handling of control events for the main control panel *
// * window.                                               *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

////////////////////////////////////////////////////////////////////
//
// CCheap3DView Control Methods
//
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//
// CCheap3DView ProcessMidiMessage
//
////////////////////////////////////////////////////////////////////

M_UINT16 CCheap3DView::ProcessMidiEvent()
{
    MidiEventPtr_T tempEventPtr;
    M_UINT16 retVal = M_TRUE;

    if (!midiPtr)
    {
        return M_FALSE;
    }

    tempEventPtr = midiPtr->GetEvent();

    while (tempEventPtr)
    {
        if ((tempEventPtr->status == SB_NOTEON || tempEventPtr->status == SB_NOTEOFF) &&
            (tempEventPtr->data1 >= 0x54 && tempEventPtr->data1 <= 0x60))
        {
            if (midiPtr->WaitingForEvent())
            {
                midiPtr->CheckContinuationEventReceived(tempEventPtr);
                return M_TRUE;
            }
        }

        //	char tempString[100];
        //    midiPtr->GetDisplayText(&tempString[0], tempEventPtr);
        //	logFile << "MIDI Event is: " << tempString << endl;

        // Will leave this for now, but likely will want to not
        // have to have Live Control active for CS-10 to work.
        if (liveDialog)
        {
            switch (tempEventPtr->status)
            {
            case SB_NOTEON:
            {
#ifdef LOG_MIDI_DATA
                logFile << "MIDI NOTEON: data1=" << tempEventPtr->data1
                    << ", data2=" << tempEventPtr->data2 << endl;
#endif
                // The first 48 notes on the keyboard (starting at C)
                // are cue events
                if ((tempEventPtr->data1 >= 0x24) &&
                    (tempEventPtr->data1 <= 0x53))
                {
                    liveDialog->DoButton(tempEventPtr->data1 - 0x23);
                    ++(midiPtr->midiNotesOn);
                }
                // The next 13 keys are reserved for effects.  However,
                // only the white keys are currently assigned to do anything.
                else if ((tempEventPtr->data1 >= 0x54) &&
                    (tempEventPtr->data1 <= 0x60))
                {
                    midiPtr->SetContinuationEvent(tempEventPtr);
                }

            } // end case NOTEON
            break;

            case SB_NOTEOFF:
            {
#ifdef LOG_MIDI_DATA
                logFile << "MIDI NOTEOFF: data1=" << tempEventPtr->data1
                    << ", data2=" << tempEventPtr->data2 << endl;
#endif
                if ((tempEventPtr->data1 >= 0x24) &&
                    (tempEventPtr->data1 <= 0x53))
                {
                    if (midiPtr->midiNotesOn > 0)
                        --midiPtr->midiNotesOn;

                    if (midiPtr->midiNotesOn == 0)
                    {
                        liveDialog->DoButton(1);
                    }
                }

            } // end case NOTEOFF
            break;

            case SB_PROGRAMCHANGE:
            {
#ifdef LOG_MIDI_DATA
                logFile << "MIDI PROGRAMCHANGE: data1=" << tempEventPtr->data1
                    << ", data2=" << tempEventPtr->data2 << endl;
#endif
                if ((tempEventPtr->data1 >= 1) &&
                    (tempEventPtr->data1 <= 12))
                {
                    liveDialog->SetPage(tempEventPtr->data1);
                }

            } // end case PROGRAMCHANGE
            break;

            case SB_CONTROLCHANGE00:
            case SB_CONTROLCHANGE15:
            {
#ifdef LOG_MIDI_DATA
                logFile << "MIDI CONTROL_CHANGE: data1=" << tempEventPtr->data1
                    << ", data2=" << tempEventPtr->data2 << endl;
#endif
                switch (tempEventPtr->data1)
                {
                case CC_VOLUME:
                case CC_MODWHEEL:
                    liveDialog->SetColorIntensity((UINT)tempEventPtr->data2);
                    break;

                case CC_DECRTRACK:
                    if (tempEventPtr->data2 == MIDIVALUEMAX)
                        liveDialog->LivePagePrevious();
                    break;

                case CC_INCRTRACK:
                    if (tempEventPtr->data2 == MIDIVALUEMAX)
                        liveDialog->LivePageNext();
                    break;

                default:
                    break;
                }

            } // end case CONTROLCHANGE
            break;

            default:
            {
                retVal = M_FALSE;
            }
            break;

            } // end switch on MIDI status

        } // end if liveDialog
        else
        {
            retVal = M_FALSE;
        }

        tempEventPtr = midiPtr->GetEvent();
    }

    return retVal;
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView OnCheckArm Methods
//
////////////////////////////////////////////////////////////////////

M_UINT16 CCheap3DView::ProcessMidiWaitForContinuation()
{
    M_UINT16 tempVal = M_TRUE;

    switch (midiPtr->GetContinuationData())
    {
        case 0x54:
            OnDisplayZoomIn();
            break;
        case 0x56:
            OnDisplayZoomOut();
            break;
        case 0x58:
            OnDisplayRotatePlusX();
            break;
        case 0x59:
            OnDisplayRotateMinusX();
            break;
        case 0x5B:
            OnDisplayRotatePlusY();
            break;
        case 0x5D:
            OnDisplayRotateMinusY();
            break;
        case 0x5F:
            OnDisplayRotatePlusZ();
            break;
        case 0x60:
            OnDisplayRotateMinusZ();
            break;
        default:
            tempVal = M_FALSE;
            break;                
    } // end switch

    return tempVal;
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView OnCheckArm Methods
//
////////////////////////////////////////////////////////////////////

void CCheap3DView::SetTrackArmState(USHORT trackNum,
                                    EnumArmState newState)
{
    Track* tempTrack = trackList.GetTrack(trackNum);

    if (!tempTrack)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "SetTrackArmState(): NULL Track pointer track "
                << trackNum << endl;
#endif
        return;
    }

    BOOLEAN currentState;

    switch (trackNum)
    {
        case 1:
            currentState = m_ArmTrack1;
            break;
        case 2:
            currentState = m_ArmTrack2;
            break;
        case 3:
            currentState = m_ArmTrack3;
            break;
        case 4:
            currentState = m_ArmTrack4;
            break;
        default:
        {
#ifdef CHEAP3DVIEW_LOG_ERROR
            logFile << "SetTrackArmState(): Invalid track number "
                    << trackNum << endl;
#endif
            return;
        }
        break;
    }

    DisplayParams tempParams = tempTrack->GetTrackParams();

    if ( (newState == TOGGLE_TRACK_ARM && currentState == TRUE) ||
         newState == DISARM_TRACK)
    {
        tempParams.displayMode = tempParams.displayMode & ~DISPLAY_ON;
        SetTrackArmValue(trackNum, FALSE);
    } 
    else if ( (newState == TOGGLE_TRACK_ARM && currentState == FALSE) ||
         newState == ARM_TRACK)
    {
        tempParams.displayMode = tempParams.displayMode | DISPLAY_ON;
        SetTrackArmValue(trackNum, TRUE);
    }

    tempTrack->SetTrackParams(tempParams);
    UpdateData(FALSE);
}

////////////////////////////////////////////////////////////////////

void CCheap3DView::OnCheckArm1() 
{
    SetTrackArmState(1, TOGGLE_TRACK_ARM);
}

////////////////////////////////////////////////////////////////////

void CCheap3DView::OnCheckArm2() 
{
    SetTrackArmState(2, TOGGLE_TRACK_ARM);
}

////////////////////////////////////////////////////////////////////

void CCheap3DView::OnCheckArm3() 
{
    SetTrackArmState(3, TOGGLE_TRACK_ARM);
}

////////////////////////////////////////////////////////////////////

void CCheap3DView::OnCheckArm4() 
{
    SetTrackArmState(4, TOGGLE_TRACK_ARM);
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView OnCheckDisplay Methods
//
////////////////////////////////////////////////////////////////////

void CCheap3DView::OnCheckDisplay1() 
{
    jobBlock.numOfFrames = fadeInFrames;
    jobBlock.reentry = M_FALSE;
    jobBlock.functionType = FADE;

    if (m_DisplayTrack1 == FALSE)
    {
        jobBlock.fadeDirection = FADE_IN;
        m_DisplayTrack1 = TRUE;
    }
    else
    {
        jobBlock.fadeDirection = FADE_OUT;
        m_DisplayTrack1 = FALSE;
    }

    SetTrackJobBlock(jobBlock, 1);
    UpdateData(FALSE);
//	DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////

void CCheap3DView::OnCheckDisplay2() 
{
    jobBlock.numOfFrames = fadeInFrames;
    jobBlock.reentry = M_FALSE;
    jobBlock.functionType = FADE;

    if (m_DisplayTrack2 == FALSE)
    {
        jobBlock.fadeDirection = FADE_IN;
        m_DisplayTrack2 = TRUE;
    }
    else
    {
        jobBlock.fadeDirection = FADE_OUT;
        m_DisplayTrack2 = FALSE;
    }

    SetTrackJobBlock(jobBlock, 2);
    UpdateData(FALSE);
//	DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////

void CCheap3DView::OnCheckDisplay3() 
{
    jobBlock.numOfFrames = fadeInFrames;
    jobBlock.reentry = M_FALSE;
    jobBlock.functionType = FADE;

    if (m_DisplayTrack3 == FALSE)
    {
        jobBlock.fadeDirection = FADE_IN;
        m_DisplayTrack3 = TRUE;
    }
    else
    {
        jobBlock.fadeDirection = FADE_OUT;
        m_DisplayTrack3 = FALSE;
    }

    SetTrackJobBlock(jobBlock, 3);
    UpdateData(FALSE);
//	DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////

void CCheap3DView::OnCheckDisplay4() 
{
    jobBlock.numOfFrames = fadeInFrames;
    jobBlock.reentry = M_FALSE;
    jobBlock.functionType = FADE;

    if (m_DisplayTrack4 == FALSE)
    {
        jobBlock.fadeDirection = FADE_IN;
        m_DisplayTrack4 = TRUE;
    }
    else
    {
        jobBlock.fadeDirection = FADE_OUT;
        m_DisplayTrack4 = FALSE;
    }

    SetTrackJobBlock(jobBlock, 4);
    UpdateData(FALSE);
//	DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView OnRadioFocus Methods
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::SetTrackFocus(int focusTrack) 
{
    // Remember, the radio buttons start at 0 so assign
    // value accordingly.  m_TrackFocus is actually a
    // macro for m_TrackFocusButton + 1.
    m_TrackFocusButton = focusTrack - 1;

    Track* tempTrack = trackList.GetTrack(m_TrackFocus);
    if (tempTrack)
    {
        if (tempTrack->GetTrackType() == Track::ABSTRACT)
        {
            if (!abstractDialog)
            {
#ifndef CHEAP3DVIEW_LOG_INFO
                logFile << "SetTrackFocus(): Creating abstract dialog" << endl;
#endif
                abstractDialog = new CAbstractControl(this);
                abstractDialog->Create(IDD_ABS_CONTROL_DIALOG, this);

                // GChangeNumber (aka envelope speed)
                abstractDialog->m_SliderGChangeNumber.SetRange(0, 3600);
                abstractDialog->m_SliderGChangeNumber.SetTicFreq(360);

                // GDepth (aka envelope depth)
                abstractDialog->m_SliderGDepth.SetRange(0, 100);
                abstractDialog->m_SliderGDepth.SetTicFreq(10);

                // ZChangeNumber (Channel 1 Delta)
                abstractDialog->m_SliderZChangeNumber.SetRange(0, 360);
                abstractDialog->m_SliderZChangeNumber.SetTicFreq(36);
                // Gain Z (Channel 1 Gain)
                abstractDialog->m_SliderFactor1.SetRange(0, 100);
                abstractDialog->m_SliderFactor1.SetTicFreq(10);
                // Offset Z (Channel 1 Offset)
                abstractDialog->m_SliderFactor5.SetRange(0, 3600);
                abstractDialog->m_SliderFactor5.SetTicFreq(360);

                // ZzChangeNumber (Channel 2 Delta)
                abstractDialog->m_SliderZzChangeNumber.SetRange(0, 360);
                abstractDialog->m_SliderZzChangeNumber.SetTicFreq(36);
                // Gain Zz (Channel 2 Gain)
                abstractDialog->m_SliderFactor2.SetRange(0, 100);
                abstractDialog->m_SliderFactor2.SetTicFreq(10);
                // Offset Zz (Channel 2 Offset)
                abstractDialog->m_SliderFactor6.SetRange(0, 3600);
                abstractDialog->m_SliderFactor6.SetTicFreq(360);

                // ZzzChangeNumber (Channel 3 Delta)
                abstractDialog->m_SliderFactor7.SetRange(0, 360);
                abstractDialog->m_SliderFactor7.SetTicFreq(36);
                // Gain Zzz (Channel 3 Gain)
                abstractDialog->m_SliderFactor3.SetRange(0, 100);
                abstractDialog->m_SliderFactor3.SetTicFreq(10);
                // Offset Zzz (Channel 3 Offset)
                abstractDialog->m_SliderFactor4.SetRange(0, 3600);
                abstractDialog->m_SliderFactor4.SetTicFreq(360);

                // Color change speed
                abstractDialog->m_SliderColorSpeed.SetRange(0, 3600);
                abstractDialog->m_SliderColorSpeed.SetTicFreq(360);

                // Blanking speed
                abstractDialog->m_SliderBlankSpeed.SetRange(0, 3600);
                abstractDialog->m_SliderBlankSpeed.SetTicFreq(360);

                // Blanking spot
                abstractDialog->m_SliderBlankSpot.SetRange(0, 200);
                abstractDialog->m_SliderBlankSpot.SetTicFreq(20);

                // Red offset
                abstractDialog->m_SliderRedOffset.SetRange(0, 360);
                abstractDialog->m_SliderRedOffset.SetTicFreq(36);

                // Green offset
                abstractDialog->m_SliderGreenOffset.SetRange(0, 360);
                abstractDialog->m_SliderGreenOffset.SetTicFreq(36);

                // Blue offset
                abstractDialog->m_SliderBlueOffset.SetRange(0, 360);
                abstractDialog->m_SliderBlueOffset.SetTicFreq(36);

                // Z-Axis offset
                abstractDialog->m_SliderZAxisOffset.SetRange(0, 360);
                abstractDialog->m_SliderZAxisOffset.SetTicFreq(36);

                // Z-Axis gain
                abstractDialog->m_SliderZAxisGain.SetRange(0, 100);
                abstractDialog->m_SliderZAxisGain.SetTicFreq(10);

                // Number of abstract points displayed per frame
                abstractDialog->m_SliderAbsPtsPerFrame.SetRange(1, 2000);
                abstractDialog->m_SliderAbsPtsPerFrame.SetTicFreq(200);
            
            } // end if abstract but dialog doesn't exist
            
            abstractDialog->abstractPtr = (Abstract*)tempTrack;
            // GChangeNumber
            abstractDialog->m_SliderGChangeNumber.SetPos(3600 - abstractDialog->abstractPtr->GChngNum);
            abstractDialog->m_EditGChangeNumber = abstractDialog->abstractPtr->GChngNum;

            // envelopeDepth
            abstractDialog->m_SliderGDepth.SetPos(100 - abstractDialog->abstractPtr->envelopeDepth);
            abstractDialog->m_EditGDepth = abstractDialog->abstractPtr->envelopeDepth;

            // envelopeShape
            abstractDialog->m_RadioGShape = (int)(abstractDialog->abstractPtr->envelopeShape);

            // ZChangeNumber
            abstractDialog->m_SliderZChangeNumber.SetPos(360 - abstractDialog->abstractPtr->ZChngNum);
            abstractDialog->m_EditZChangeNumber = abstractDialog->abstractPtr->ZChngNum;
            // Gain Z
            abstractDialog->m_SliderFactor1.SetPos(100 - abstractDialog->abstractPtr->GainZ);
            abstractDialog->m_EditFactor1 = abstractDialog->abstractPtr->GainZ;
            // Offset Z
            abstractDialog->m_SliderFactor5.SetPos(3600 - abstractDialog->abstractPtr->OffsetZ);
            abstractDialog->m_EditFactor5 = abstractDialog->abstractPtr->OffsetZ;

            // ZzChangeNumber
            abstractDialog->m_SliderZzChangeNumber.SetPos(360 - abstractDialog->abstractPtr->ZzChngNum);
            abstractDialog->m_EditZzChangeNumber = abstractDialog->abstractPtr->ZzChngNum;
            // Gain Zz
            abstractDialog->m_SliderFactor2.SetPos(100 - abstractDialog->abstractPtr->GainZz);
            abstractDialog->m_EditFactor2 = abstractDialog->abstractPtr->GainZz;
            // Offset Zz
            abstractDialog->m_SliderFactor6.SetPos(3600 - abstractDialog->abstractPtr->OffsetZz);
            abstractDialog->m_EditFactor6 = abstractDialog->abstractPtr->OffsetZz;

            // ZzzChangeNumber
            abstractDialog->m_SliderFactor7.SetPos(360 - abstractDialog->abstractPtr->ZzzChngNum);
            abstractDialog->m_EditFactor7 = abstractDialog->abstractPtr->ZzzChngNum;
            // Gain Zzz
            abstractDialog->m_SliderFactor3.SetPos(100 - abstractDialog->abstractPtr->GainZzz);
            abstractDialog->m_EditFactor3 = abstractDialog->abstractPtr->GainZzz;
            // Offset Zzz
            abstractDialog->m_SliderFactor4.SetPos(3600 - abstractDialog->abstractPtr->OffsetZzz);
            abstractDialog->m_EditFactor4 = abstractDialog->abstractPtr->OffsetZzz;

            abstractDialog->m_SliderColorSpeed.SetPos(3600 - abstractDialog->abstractPtr->ColorSpd);
            abstractDialog->m_EditColorSpeed = abstractDialog->abstractPtr->ColorSpd;
            abstractDialog->m_SliderBlankSpeed.SetPos(3600 - abstractDialog->abstractPtr->BlankSpd);
            abstractDialog->m_EditBlankSpeed = abstractDialog->abstractPtr->BlankSpd;
            abstractDialog->m_SliderBlankSpot.SetPos(200 - abstractDialog->abstractPtr->BlankSpot);
            abstractDialog->m_EditBlankSpot = abstractDialog->abstractPtr->BlankSpot;
            abstractDialog->m_SliderRedOffset.SetPos(360 - abstractDialog->abstractPtr->redOffset);
            abstractDialog->m_EditRedOffset = abstractDialog->abstractPtr->redOffset;
            abstractDialog->m_SliderGreenOffset.SetPos(360 - abstractDialog->abstractPtr->greenOffset);
            abstractDialog->m_EditGreenOffset = abstractDialog->abstractPtr->greenOffset;
            abstractDialog->m_SliderBlueOffset.SetPos(360 - abstractDialog->abstractPtr->blueOffset);
            abstractDialog->m_EditBlueOffset = abstractDialog->abstractPtr->blueOffset;
            abstractDialog->m_SliderZAxisOffset.SetPos(360 - abstractDialog->abstractPtr->ZAxisOffset);
            abstractDialog->m_EditZAxisOffset = abstractDialog->abstractPtr->ZAxisOffset;
            abstractDialog->m_SliderZAxisGain.SetPos(100 - abstractDialog->abstractPtr->ZAxisGain);
            abstractDialog->m_EditZAxisGain = abstractDialog->abstractPtr->ZAxisGain;
            abstractDialog->m_SliderAbsPtsPerFrame.SetPos(2000 - abstractDialog->abstractPtr->AbsPtsPerFrame);
            abstractDialog->m_EditAbsPtsPerFrame = abstractDialog->abstractPtr->AbsPtsPerFrame;

            abstractDialog->m_AbstractPattern = abstractDialog->abstractPtr->Formula - 1;
            abstractDialog->m_ZAxis = abstractDialog->abstractPtr->ZAxisFormula - 1;
            abstractDialog->m_ColorShift = abstractDialog->abstractPtr->ColorPtn - 1;
            if (abstractDialog->abstractPtr->usePalette == M_TRUE)
            {
                abstractDialog->m_UsePalette = TRUE;
            }
            else
            {
                abstractDialog->m_UsePalette = FALSE;
            }

            abstractDialog->UpdateData(FALSE);

        } // end if abstract

        if ( abstractDialog && 
            (tempTrack->GetTrackType() != Track::ABSTRACT))
        {
            abstractDialog->InitControlSettings();
        }

        DisplayParams tempParams = tempTrack->GetTrackParams();
        ViewType tempView        = tempTrack->GetTrackView();
        m_FocusTrackDelay = tempParams.delayValue;
        m_SliderFocusTrackDelay.SetPos(m_FocusTrackDelay);
        m_FocusTrackRepeat = tempParams.frameRepeat;
        m_SliderFocusTrackRepeat.SetPos(m_FocusTrackRepeat);
        m_BlankingDelay = tempParams.blankingDelay;
        m_SliderBlankDelay.SetPos(m_BlankingDelay);
        m_TrackScale = tempParams.trackScale;
        m_SliderTrackScale.SetPos(m_TrackScale);
        m_TrackRedBalance = (UINT)(tempParams.redLevel * 100);
        m_SliderTrackRedBalance.SetPos(m_TrackRedBalance);
        m_TrackGreenBalance = (UINT)(tempParams.greenLevel * 100);
        m_SliderTrackGreenBalance.SetPos(m_TrackGreenBalance);
        m_TrackBlueBalance = (UINT)(tempParams.blueLevel * 100);
        m_SliderTrackBlueBalance.SetPos(m_TrackBlueBalance);
        m_TrackCameraAngle = (UINT)(tempView.view_angle);
        m_SliderTrackCameraAngle.SetPos(m_TrackCameraAngle);

        if (tempParams.palettePtr == &LaserIllusionsColorPalette[0])
        {
            m_ILDA_Palette_Button = (int)PaletteType::LASER_ILLUSIONS;
        }
        else if (tempParams.palettePtr == &X29ColorPalette[0])
        {
            m_ILDA_Palette_Button = (int)PaletteType::X29;
        }
        else
        {
            m_ILDA_Palette_Button = (int)PaletteType::PANGOLIN;
        }

        if (tempParams.displayMode & POINTS_ONLY)
        {
            m_PointsOnly = TRUE;
        }
        else
        {
            m_PointsOnly = FALSE;
        }

        if (tempParams.displayReverse == TRUE)
        {
            m_Reverse = TRUE;
        }
        else
        {
            m_Reverse = FALSE;
        }

        if (tempParams.displayBeamView == TRUE)
        {
            m_DisplayBeamView = TRUE;
        }
        else
        {
            m_DisplayBeamView = FALSE;
        }

    }

    UpdateData(FALSE);
}

////////////////////////////////////////////////////////////////////

void CCheap3DView::OnRadioFocus1() 
{
    SetTrackFocus(1);
}

////////////////////////////////////////////////////////////////////

void CCheap3DView::OnRadioFocus2() 
{
    SetTrackFocus(2);
}

////////////////////////////////////////////////////////////////////

void CCheap3DView::OnRadioFocus3() 
{
    SetTrackFocus(3);
}

////////////////////////////////////////////////////////////////////

void CCheap3DView::OnRadioFocus4() 
{
    SetTrackFocus(4);
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView Slider Handler Methods
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    m_BlankingDelay = m_SliderBlankDelay.GetPos();
    m_TrackScale        = m_SliderTrackScale.GetPos();

    m_FocusTrackRepeat = m_SliderFocusTrackRepeat.GetPos();
    m_FocusTrackDelay  = m_SliderFocusTrackDelay.GetPos();
    m_TrackRedBalance  = m_SliderTrackRedBalance.GetPos();
    m_TrackGreenBalance  = m_SliderTrackGreenBalance.GetPos();
    m_TrackBlueBalance = m_SliderTrackBlueBalance.GetPos();
    m_TrackCameraAngle = m_SliderTrackCameraAngle.GetPos();
    Track* tempTrack   = trackList.GetTrack(m_TrackFocus);

    if (tempTrack)
    {
        DisplayParams tempParams = tempTrack->GetTrackParams();
        ViewType tempView        = tempTrack->GetTrackView();
        tempParams.delayValue    = m_FocusTrackDelay;
        tempParams.frameRepeat   = m_FocusTrackRepeat;
        tempParams.blankingDelay = m_BlankingDelay;
        tempParams.trackScale    = m_TrackScale;
        tempParams.redLevel   = (float)m_TrackRedBalance   / 100.0;
        tempParams.greenLevel = (float)m_TrackGreenBalance / 100.0;
        tempParams.blueLevel  = (float)m_TrackBlueBalance  / 100.0;
        tempView.view_angle   = m_TrackCameraAngle;
        tempTrack->SetTrackParams(tempParams);
        tempTrack->SetTrackView(tempView);
    }

    UpdateData(FALSE);
}

//void CCheap3DView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
//{
//	UpdateData(FALSE);
//}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView OnCheckPointsOnly Method
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnCheckPointsOnly() 
{
    Track* tempTrack = trackList.GetTrack(m_TrackFocus);

    if (tempTrack)
    {
        DisplayParams tempParams = tempTrack->GetTrackParams();

        if (tempParams.displayMode & POINTS_ONLY)
        {
            tempParams.displayMode &= ~POINTS_ONLY;
            m_PointsOnly = FALSE;
        }
        else
        {
            tempParams.displayMode |= POINTS_ONLY;
            m_PointsOnly = TRUE;
        }

        tempTrack->SetTrackParams(tempParams);

    }
    
    UpdateData(FALSE);
}


////////////////////////////////////////////////////////////////////
//
// CCheap3DView OnCheckBeamView Method
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnCheckBeamview() 
{
    Track* tempTrack = trackList.GetTrack(m_TrackFocus);

    if (tempTrack)
    {
        DisplayParams tempParams = tempTrack->GetTrackParams();

        if (tempParams.displayBeamView == TRUE)
        {
            tempParams.displayBeamView = M_FALSE;
            m_DisplayBeamView = FALSE;
        }
        else
        {
            tempParams.displayBeamView = TRUE;
            m_DisplayBeamView = TRUE;
        }

        tempTrack->SetTrackParams(tempParams);

    }
    
    UpdateData(FALSE);	
    
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView OnCheckReverse Method
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnCheckReverse() 
{
    Track* tempTrack = trackList.GetTrack(m_TrackFocus);

    if (tempTrack)
    {
        DisplayParams tempParams = tempTrack->GetTrackParams();

        if (tempParams.displayReverse == TRUE)
        {
            tempParams.displayReverse = M_FALSE;
            m_Reverse = FALSE;
        }
        else
        {
            tempParams.displayReverse = TRUE;
            m_Reverse = TRUE;
        }

        tempTrack->SetTrackParams(tempParams);

    }
    
    UpdateData(FALSE);	
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView Live Console Accelerator Buttons
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnLiveButton1() 
{
    if(liveDialog)
        liveDialog->OnLiveButton1();	
}

void CCheap3DView::OnLiveButton2() 
{
    if(liveDialog)
        liveDialog->OnLiveButton2();	
}

void CCheap3DView::OnLiveButton3() 
{
    if(liveDialog)
        liveDialog->OnLiveButton3();	
}

void CCheap3DView::OnLiveButton4() 
{
    if(liveDialog)
        liveDialog->OnLiveButton4();	
}

void CCheap3DView::OnLiveButton5() 
{
    if(liveDialog)
        liveDialog->OnLiveButton5();	
}

void CCheap3DView::OnLiveButton6() 
{
    if(liveDialog)
        liveDialog->OnLiveButton6();	
}

void CCheap3DView::OnLiveButton7() 
{
    if(liveDialog)
        liveDialog->OnLiveButton7();	
}

void CCheap3DView::OnLiveButton8() 
{
    if(liveDialog)
        liveDialog->OnLiveButton8();	
}

void CCheap3DView::OnLiveButton9() 
{
    if(liveDialog)
        liveDialog->OnLiveButton9();	
}

void CCheap3DView::OnLiveButton10() 
{
    if(liveDialog)
        liveDialog->OnLiveButton10();	
}

void CCheap3DView::OnLiveButton11() 
{
    if(liveDialog)
        liveDialog->OnLiveButton11();	
}

void CCheap3DView::OnLiveButton12() 
{
    if(liveDialog)
        liveDialog->OnLiveButton12();	
}

void CCheap3DView::OnLiveButton13() 
{
    if(liveDialog)
        liveDialog->OnLiveButton13();	
}

void CCheap3DView::OnLiveButton14() 
{
    if(liveDialog)
        liveDialog->OnLiveButton14();	
}

void CCheap3DView::OnLiveButton15() 
{
    if(liveDialog)
        liveDialog->OnLiveButton15();	
}

void CCheap3DView::OnLiveButton16() 
{
    if(liveDialog)
        liveDialog->OnLiveButton16();	
}

void CCheap3DView::OnLiveButton17() 
{
    if(liveDialog)
        liveDialog->OnLiveButton17();	
}

void CCheap3DView::OnLiveButton18() 
{
    if(liveDialog)
        liveDialog->OnLiveButton18();	
}

void CCheap3DView::OnLiveButton19() 
{
    if(liveDialog)
        liveDialog->OnLiveButton19();	
}

void CCheap3DView::OnLiveButton20() 
{
    if(liveDialog)
        liveDialog->OnLiveButton20();	
}

void CCheap3DView::OnLiveButton21() 
{
    if(liveDialog)
        liveDialog->OnLiveButton21();	
}

void CCheap3DView::OnLiveButton22() 
{
    if(liveDialog)
        liveDialog->OnLiveButton22();	
}

void CCheap3DView::OnLiveButton23() 
{
    if(liveDialog)
        liveDialog->OnLiveButton23();	
}

void CCheap3DView::OnLiveButton24() 
{
    if(liveDialog)
        liveDialog->OnLiveButton24();	
}

void CCheap3DView::OnLiveButton25() 
{
    if(liveDialog)
        liveDialog->OnLiveButton25();	
}

void CCheap3DView::OnLiveButton26() 
{
    if(liveDialog)
        liveDialog->OnLiveButton26();	
}

void CCheap3DView::OnLiveButton27() 
{
    if(liveDialog)
        liveDialog->OnLiveButton27();	
}

void CCheap3DView::OnLiveButton28() 
{
    if(liveDialog)
        liveDialog->OnLiveButton28();	
}

void CCheap3DView::OnLiveButton29() 
{
    if(liveDialog)
        liveDialog->OnLiveButton29();	
}

void CCheap3DView::OnLiveButton30() 
{
    if(liveDialog)
        liveDialog->OnLiveButton30();	
}

void CCheap3DView::OnLiveButton31() 
{
    if(liveDialog)
        liveDialog->OnLiveButton31();	
}

void CCheap3DView::OnLiveButton32() 
{
    if(liveDialog)
        liveDialog->OnLiveButton32();	
}

void CCheap3DView::OnLiveButton33() 
{
    if(liveDialog)
        liveDialog->OnLiveButton33();	
}

void CCheap3DView::OnLiveButton34() 
{
    if(liveDialog)
        liveDialog->OnLiveButton34();	
}

void CCheap3DView::OnLiveButton35() 
{
    if(liveDialog)
        liveDialog->OnLiveButton35();	
}

void CCheap3DView::OnLiveButton36() 
{
    if(liveDialog)
        liveDialog->OnLiveButton36();	
}

void CCheap3DView::OnLiveButton37() 
{
    if(liveDialog)
        liveDialog->OnLiveButton37();	
}

void CCheap3DView::OnLiveButton38() 
{
    if(liveDialog)
        liveDialog->OnLiveButton38();	
}

void CCheap3DView::OnLiveButton39() 
{
    if(liveDialog)
        liveDialog->OnLiveButton39();	
}

void CCheap3DView::OnLiveButton40() 
{
    if(liveDialog)
        liveDialog->OnLiveButton40();	
}

void CCheap3DView::OnLiveButton41() 
{
    if(liveDialog)
        liveDialog->OnLiveButton41();	
}

void CCheap3DView::OnLiveButton42() 
{
    if(liveDialog)
        liveDialog->OnLiveButton42();	
}

void CCheap3DView::OnLiveButton43() 
{
    if(liveDialog)
        liveDialog->OnLiveButton43();	
}

void CCheap3DView::OnLiveButton44() 
{
    if(liveDialog)
        liveDialog->OnLiveButton44();	
}

void CCheap3DView::OnLiveButton45() 
{
    if(liveDialog)
        liveDialog->OnLiveButton45();	
}

void CCheap3DView::OnLiveButton46() 
{
    if(liveDialog)
        liveDialog->OnLiveButton46();	
}

void CCheap3DView::OnLiveButton47() 
{
    if(liveDialog)
        liveDialog->OnLiveButton47();	
}

void CCheap3DView::OnLiveButton48() 
{
    if(liveDialog)
        liveDialog->OnLiveButton48();	
}

void CCheap3DView::OnLivePageNext() 
{
    if(liveDialog)
        liveDialog->LivePageNext();	
}

void CCheap3DView::OnLivePagePrevious() 
{
    if(liveDialog)
        liveDialog->LivePagePrevious();	
}
////////////////////////////////////////////////////////////////////
//
// CCheap3DView Global Delay Accelerator Buttons
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayIncreaseGlobalDelay()
{
    globalTrackDelay += DELAYINCR;

    if (globalTrackDelay >= MAX_GLOBAL_DELAY/2)
    {
        globalTrackDelay = MAX_GLOBAL_DELAY/2;
    }

}

void CCheap3DView::OnDisplayDecreaseGlobalDelay()
{
    globalTrackDelay -= DELAYINCR;

    if (globalTrackDelay <= MIN_GLOBAL_DELAY)
    {
        globalTrackDelay = MIN_GLOBAL_DELAY;
    }

}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView Focus Track Repeat Accelerator Buttons
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayIncreaseFocusRepeat()
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayIncreaseFocusRepeat()" << endl;
#endif

    Track* tempTrack;
    tempTrack = trackList.GetTrack(m_TrackFocus);

    if (!tempTrack)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "OnDisplayIncreaseFocusRepeat(): NULL Track pointer." 
                << endl;
#endif
        return;
    }

    DisplayParams tempParams = tempTrack->GetTrackParams();

    ++m_FocusTrackRepeat;

    if (m_FocusTrackRepeat >= MAX_FOCUS_REPEAT)
    {
        m_FocusTrackRepeat = MAX_FOCUS_REPEAT;
    }

    tempParams.frameRepeat = m_FocusTrackRepeat;
    tempTrack->SetTrackParams(tempParams);
    m_SliderFocusTrackRepeat.SetPos(m_FocusTrackRepeat);
    UpdateData(FALSE);
}

void CCheap3DView::OnDisplayDecreaseFocusRepeat()
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayDecreaseFocusRepeat()" << endl;
#endif

    Track* tempTrack;
    tempTrack = trackList.GetTrack(m_TrackFocus);

    if (!tempTrack)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "OnDisplayDecreaseFocusRepeat(): NULL Track pointer." 
                << endl;
#endif
        return;
    }

    DisplayParams tempParams = tempTrack->GetTrackParams();

    --m_FocusTrackRepeat;

    if ((int)m_FocusTrackRepeat <= 0)
    {
        m_FocusTrackRepeat = 0;
    }

    tempParams.frameRepeat = m_FocusTrackRepeat;
    tempTrack->SetTrackParams(tempParams);
    m_SliderFocusTrackRepeat.SetPos(m_FocusTrackRepeat);
    UpdateData(FALSE);

}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView Focus Track Blanking Delay Accelerator Buttons
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayIncreaseBlankingDelay()
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayIncreaseBlankingDelay()" << endl;
#endif

    Track* tempTrack;
    tempTrack = trackList.GetTrack(m_TrackFocus);

    if (!tempTrack)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "OnDisplayIncreaseBlankingDelay(): NULL Track pointer." 
                << endl;
#endif
        return;
    }

    DisplayParams tempParams = tempTrack->GetTrackParams();

    ++m_BlankingDelay;

    if (m_BlankingDelay >= 100)
    {
        m_BlankingDelay = 100;
    }

    tempParams.blankingDelay = m_BlankingDelay;
    tempTrack->SetTrackParams(tempParams);
    m_SliderBlankDelay.SetPos(m_BlankingDelay);
    UpdateData(FALSE);
}

void CCheap3DView::OnDisplayDecreaseBlankingDelay()
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayDecreaseBlankingDelay()" << endl;
#endif

    Track* tempTrack;
    tempTrack = trackList.GetTrack(m_TrackFocus);

    if (!tempTrack)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "OnDisplayDecreaseBlankingDelay(): NULL Track pointer." 
                << endl;
#endif
        return;
    }

    DisplayParams tempParams = tempTrack->GetTrackParams();

    --m_BlankingDelay;

    if ((int)m_BlankingDelay <= 0)
    {
        m_BlankingDelay = 0;
    }

    tempParams.blankingDelay = m_BlankingDelay;
    tempTrack->SetTrackParams(tempParams);
    m_SliderBlankDelay.SetPos(m_BlankingDelay);
    UpdateData(FALSE);
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView Focus Track Scale Accelerator Buttons
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayIncreaseTrackScale()
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayIncreaseTrackScale()" << endl;
#endif

    Track* tempTrack;
    tempTrack = trackList.GetTrack(m_TrackFocus);

    if (!tempTrack)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "OnDisplayIncreaseTrackScale(): NULL Track pointer." 
                << endl;
#endif
        return;
    }

    DisplayParams tempParams = tempTrack->GetTrackParams();

    ++m_TrackScale;

    if (m_TrackScale >= MAX_PERCENT)
    {
        m_TrackScale = MAX_PERCENT;
    }

    tempParams.trackScale = m_TrackScale;
    tempTrack->SetTrackParams(tempParams);
    m_SliderTrackScale.SetPos(m_TrackScale);
    UpdateData(FALSE);
}

void CCheap3DView::OnDisplayDecreaseTrackScale()
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayDecreaseTrackScale()" << endl;
#endif

    Track* tempTrack;
    tempTrack = trackList.GetTrack(m_TrackFocus);

    if (!tempTrack)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "OnDisplayDecreaseTrackScale(): NULL Track pointer." 
                << endl;
#endif
        return;
    }

    DisplayParams tempParams = tempTrack->GetTrackParams();

    --m_TrackScale;
    
    if ((int)(m_TrackScale) <= MIN_PERCENT)
    {
        m_TrackScale = MIN_PERCENT;
    }

    tempParams.trackScale = m_TrackScale;
    tempTrack->SetTrackParams(tempParams);
    m_SliderTrackScale.SetPos(m_TrackScale);
    UpdateData(FALSE);
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView Laser Scale Accelerator Buttons
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayIncreaseLaserScale()
{
    if (++laserScale >= MAX_PERCENT)
    {
        laserScale = MAX_PERCENT;
    }

}

void CCheap3DView::OnDisplayDecreaseLaserScale()
{
    if ((int)(--laserScale) <= MIN_PERCENT)
    {
        laserScale = MIN_PERCENT;
    }

}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView Move Increment Accelerator Buttons
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayIncreaseMoveIncrement()
{
    moveIncrement += 100;

    if (moveIncrement >= MAX_MOVE_INCREMENT)
    {
        moveIncrement = MAX_MOVE_INCREMENT;
    }

}

void CCheap3DView::OnDisplayDecreaseMoveIncrement()
{
    moveIncrement -= 100;

    if ((int)moveIncrement <= 1)
    {
        moveIncrement = 1;
    }

}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView Zoom Increment Accelerator Buttons
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayIncreaseZoomIncrement()
{
    ++zoomIncrement;

    if (zoomIncrement >= MAX_ZOOM_INCREMENT)
    {
        zoomIncrement = MAX_ZOOM_INCREMENT;
    }

}

void CCheap3DView::OnDisplayDecreaseZoomIncrement()
{
    --zoomIncrement;

    if ((int)zoomIncrement <= 1)
    {
        zoomIncrement = 1;
    }

}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView Rotate Increment Accelerator Buttons
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayIncreaseRotateIncrement()
{
    ++rotateIncrement;

    if (rotateIncrement >= MAX_ROTATE_INCREMENT)
    {
        rotateIncrement = MAX_ROTATE_INCREMENT;
    }

}

void CCheap3DView::OnDisplayDecreaseRotateIncrement()
{
    --rotateIncrement;

    if ((int)rotateIncrement <= 1)
    {
        rotateIncrement = 1;
    }

}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView Move Both Point Accelerator Buttons
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnMoveBothPlusX() 
{
    jobBlock.xIncrement = ((float)moveIncrement);
    jobBlock.yIncrement = 0.0;
    jobBlock.zIncrement = 0.0;
    jobBlock.functionType = MOVE_BOTH;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveBothMinusX() 
{
    jobBlock.xIncrement = -((float)moveIncrement);
    jobBlock.yIncrement = 0.0;
    jobBlock.zIncrement = 0.0;
    jobBlock.functionType = MOVE_BOTH;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveBothPlusY() 
{
    jobBlock.xIncrement = 0.0;
    jobBlock.yIncrement = ((float)moveIncrement);
    jobBlock.zIncrement = 0.0;
    jobBlock.functionType = MOVE_BOTH;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveBothMinusY() 
{
    jobBlock.xIncrement = 0.0;
    jobBlock.yIncrement = -((float)moveIncrement);
    jobBlock.zIncrement = 0.0;
    jobBlock.functionType = MOVE_BOTH;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveBothPlusZ() 
{
    jobBlock.xIncrement = 0.0;
    jobBlock.yIncrement = 0.0;
    jobBlock.zIncrement = ((float)moveIncrement);
    jobBlock.functionType = MOVE_BOTH;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveBothMinusZ() 
{
    jobBlock.xIncrement = 0.0;
    jobBlock.yIncrement = 0.0;
    jobBlock.zIncrement = -((float)moveIncrement);
    jobBlock.functionType = MOVE_BOTH;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView Move From Point Accelerator Buttons
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnMoveFromPlusX() 
{
    jobBlock.xIncrement = ((float)moveIncrement);
    jobBlock.yIncrement = 0.0;
    jobBlock.zIncrement = 0.0;
    jobBlock.functionType = MOVE_FROM;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveFromMinusX() 
{
    jobBlock.xIncrement = -((float)moveIncrement);
    jobBlock.yIncrement = 0.0;
    jobBlock.zIncrement = 0.0;
    jobBlock.functionType = MOVE_FROM;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveFromPlusY() 
{
    jobBlock.xIncrement = 0.0;
    jobBlock.yIncrement = ((float)moveIncrement);
    jobBlock.zIncrement = 0.0;
    jobBlock.functionType = MOVE_FROM;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveFromMinusY() 
{
    jobBlock.xIncrement = 0.0;
    jobBlock.yIncrement = -((float)moveIncrement);
    jobBlock.zIncrement = 0.0;
    jobBlock.functionType = MOVE_FROM;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveFromPlusZ() 
{
    jobBlock.xIncrement = 0.0;
    jobBlock.yIncrement = 0.0;
    jobBlock.zIncrement = ((float)moveIncrement);
    jobBlock.functionType = MOVE_FROM;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveFromMinusZ() 
{
    jobBlock.xIncrement = 0.0;
    jobBlock.yIncrement = 0.0;
    jobBlock.zIncrement = -((float)moveIncrement);
    jobBlock.functionType = MOVE_FROM;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView Move At Point Accelerator Buttons
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnMoveAtPlusX() 
{
    jobBlock.xIncrement = ((float)moveIncrement);
    jobBlock.yIncrement = 0.0;
    jobBlock.zIncrement = 0.0;
    jobBlock.functionType = MOVE_AT;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveAtMinusX() 
{
    jobBlock.xIncrement = -((float)moveIncrement);
    jobBlock.yIncrement = 0.0;
    jobBlock.zIncrement = 0.0;
    jobBlock.functionType = MOVE_AT;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveAtPlusY() 
{
    jobBlock.xIncrement = 0.0;
    jobBlock.yIncrement = ((float)moveIncrement);
    jobBlock.zIncrement = 0.0;
    jobBlock.functionType = MOVE_AT;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveAtMinusY() 
{
    jobBlock.xIncrement = 0.0;
    jobBlock.yIncrement = -((float)moveIncrement);
    jobBlock.zIncrement = 0.0;
    jobBlock.functionType = MOVE_AT;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveAtPlusZ() 
{
    jobBlock.xIncrement = 0.0;
    jobBlock.yIncrement = 0.0;
    jobBlock.zIncrement = ((float)moveIncrement);
    jobBlock.functionType = MOVE_AT;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveAtMinusZ() 
{
    jobBlock.xIncrement = 0.0;
    jobBlock.yIncrement = 0.0;
    jobBlock.zIncrement = -((float)moveIncrement);
    jobBlock.functionType = MOVE_AT;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveForward() 
{
    jobBlock.xIncrement = ((float)moveIncrement);
    jobBlock.yIncrement = 0.0;
    jobBlock.zIncrement = 0.0;
    jobBlock.functionType = MOVE_FORWARD;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

void CCheap3DView::OnMoveBackward() 
{
    jobBlock.xIncrement = -((float)moveIncrement);
    jobBlock.yIncrement = 0.0;
    jobBlock.zIncrement = 0.0;
    jobBlock.functionType = MOVE_FORWARD;
    jobBlock.numOfFrames = 1;
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}


////////////////////////////////////////////////////////////////////
//
// CCheap3DView Effects Check Box Accelerator Buttons
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnCheckEffect1() 
{
    if (m_EffectTrack1 == TRUE)
    {
        m_EffectTrack1 = FALSE;
    }
    else
    {
        m_EffectTrack1 = TRUE;
    }
    UpdateData(FALSE);
}

void CCheap3DView::OnCheckEffect2() 
{
    if (m_EffectTrack2 == TRUE)
    {
        m_EffectTrack2 = FALSE;
    }
    else
    {
        m_EffectTrack2 = TRUE;
    }
    UpdateData(FALSE);
}

void CCheap3DView::OnCheckEffect3() 
{
    if (m_EffectTrack3 == TRUE)
    {
        m_EffectTrack3 = FALSE;
    }
    else
    {
        m_EffectTrack3 = TRUE;
    }
    UpdateData(FALSE);
}

void CCheap3DView::OnCheckEffect4() 
{
    if (m_EffectTrack4 == TRUE)
    {
        m_EffectTrack4 = FALSE;
    }
    else
    {
        m_EffectTrack4 = TRUE;
    }
    UpdateData(FALSE);
}


////////////////////////////////////////////////////////////////////
//
// CCheap3DView Rotate About Zero Check Box Accelerator Buttons
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnCheckRotAbt01() 
{
    UpdateData(TRUE);
}

void CCheap3DView::OnCheckRotAbt02() 
{
    UpdateData(TRUE);
}

void CCheap3DView::OnCheckRotAbt03() 
{
    UpdateData(TRUE);
}

void CCheap3DView::OnCheckRotAbt04() 
{
    UpdateData(TRUE);
}


////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayRotateBothPlusX()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayRotateBothPlusX() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayRotateBothPlusX()" << endl;
#endif
    jobBlock.functionType = ROTATE_BOTH;
    jobBlock.numOfFrames = 1;
    jobBlock.rotationAxis = X_AXIS;
    jobBlock.degreeAngle = ((float)rotateIncrement);
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayRotateBothMinusX()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayRotateBothMinusX() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayRotateBothMinusX()" << endl;
#endif
    jobBlock.functionType = ROTATE_BOTH;
    jobBlock.numOfFrames = 1;
    jobBlock.rotationAxis = X_AXIS;
    jobBlock.degreeAngle = -(((float)rotateIncrement));
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayRotateBothPlusY()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayRotateBothPlusY() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayRotateBothPlusY()" << endl;
#endif
    jobBlock.functionType = ROTATE_BOTH;
    jobBlock.numOfFrames = 1;
    jobBlock.rotationAxis = Y_AXIS;
    jobBlock.degreeAngle = ((float)rotateIncrement);
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayRotateBothMinusY()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayRotateBothMinusY() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayRotateBothMinusY()" << endl;
#endif
    jobBlock.functionType = ROTATE_BOTH;
    jobBlock.numOfFrames = 1;
    jobBlock.rotationAxis = Y_AXIS;
    jobBlock.degreeAngle = -(((float)rotateIncrement));
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayRotateBothPlusZ()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayRotateBothPlusZ() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayRotateBothPlusZ()" << endl;
#endif
    jobBlock.functionType = ROTATE_BOTH;
    jobBlock.numOfFrames = 1;
    jobBlock.rotationAxis = Z_AXIS;
    jobBlock.degreeAngle = ((float)rotateIncrement);
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayRotateBothMinusZ()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayRotateBothMinusZ() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayRotateBothMinusZ()" << endl;
#endif
    jobBlock.functionType = ROTATE_BOTH;
    jobBlock.numOfFrames = 1;
    jobBlock.rotationAxis = Z_AXIS;
    jobBlock.degreeAngle = -(((float)rotateIncrement));
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
    DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnCheckDisplayCoordinates()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnCheckDisplayCoordinates() 
{
    if (m_DisplayCoordinates == TRUE)
    {
        m_DisplayCoordinates = FALSE;
        m_FromPointX = 0.0;
        m_FromPointY = 0.0;
        m_FromPointZ = 0.0;
        m_AtPointX   = 0.0;
        m_AtPointY   = 0.0;
        m_AtPointZ   = 0.0;
    }
    else
    {
        m_DisplayCoordinates = TRUE;
    }

    UpdateData(FALSE);
    
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnRadioPangolin()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnRadioPangolin() 
{
    Track* tempTrack = trackList.GetTrack(m_TrackFocus);
    if (tempTrack)
    {
            DisplayParams tempParams = tempTrack->GetTrackParams();
            tempParams.palettePtr = &PangolinColorPalette[0];
            tempTrack->SetTrackParams(tempParams);

    } // end if track is valid

    m_ILDA_Palette_Button = (int)PaletteType::PANGOLIN;
    UpdateData(FALSE);

}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnRadioLaserIllusions()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnRadioLaserIllusions() 
{
    // TODO: Add your control notification handler code here
    Track* tempTrack = trackList.GetTrack(m_TrackFocus);
    if (tempTrack)
    {
            DisplayParams tempParams = tempTrack->GetTrackParams();
            tempParams.palettePtr = &LaserIllusionsColorPalette[0];
            tempTrack->SetTrackParams(tempParams);

    } // end if track is valid

    m_ILDA_Palette_Button = (int)PaletteType::LASER_ILLUSIONS;
    UpdateData(FALSE);

}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnRadioIlda()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnRadioIlda() 
{
    // TODO: Add your control notification handler code here
    Track* tempTrack = trackList.GetTrack(m_TrackFocus);
    if (tempTrack)
    {
            DisplayParams tempParams = tempTrack->GetTrackParams();
            tempParams.palettePtr = &IldaColorPalette[0];
            tempTrack->SetTrackParams(tempParams);

    } // end if track is valid

    m_ILDA_Palette_Button = (int)PaletteType::ILDA;
    UpdateData(FALSE);

}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnRadioX29()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnRadioX29() 
{
    // TODO: Add your control notification handler code here
    Track* tempTrack = trackList.GetTrack(m_TrackFocus);
    if (tempTrack)
    {
            DisplayParams tempParams = tempTrack->GetTrackParams();
            tempParams.palettePtr = &X29ColorPalette[0];
            tempTrack->SetTrackParams(tempParams);

    } // end if track is valid

    m_ILDA_Palette_Button = (int)PaletteType::X29;
    UpdateData(FALSE);

}
