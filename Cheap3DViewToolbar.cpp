//
// *********************************************************
// *                                                       *
// * Copyright 1998-2005, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Cheap3DViewLaser.cpp                       *
// * Description: Contains the implementation of those     *
// * methods on the Cheap3DView class which pertain to the *
// * handling of application toolbar events.               *
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
// CCheap3DView::OnDisplayStop()
//
////////////////////////////////////////////////////////////////////
//
// There is no OnDisplayStop() method.  Instead, the method
// CCheap3DView::OnFileClosetracklist() is called (see file
// Cheap3DView.cpp).
//

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayRewind()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayRewind() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayRewind()" << endl;
#endif
    if (ctsFilePtr && (scriptActive == M_TRUE))
    {
        rewind(ctsFilePtr);
        commandInProgress = M_FALSE;
        firstTime = M_TRUE;
    }
    
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayPlay()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayPlay() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayPlay()" << endl;
#endif

    if (midiPtr)
    {
        midiPtr->StartMidiProcessing();
    }
    
    // Install system timers
    if (!SetTimer(DISPLAY_TIMER, displayTimerValue, NULL))
    {
        MessageBox("OnDisplayPlay(): Can't install DISPLAY_TIMER.");
        return;
    }
    else
    {
        displayTimerSet = TRUE;
    }

    if (!SetTimer(POINTS_PER_SECOND_TIMER, PPS_TIMER_VALUE, NULL))
    {
        MessageBox("OnDisplayPlay(): Can't install POINTS_PER_SECOND_TIMER.");
        return;
    }

    if (!SetTimer(PLAYBACK_TIMER, 10, NULL))
    {
        MessageBox("OnDisplayPlay(): Can't install PLAYBACK_TIMER.");
    }

}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayPause()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayPause() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayPause()" << endl;
#endif

    if (midiPtr)
    {
        midiPtr->StopMidiProcessing();
    }

    KillTimer(DISPLAY_TIMER);
    displayTimerSet = FALSE;
    KillTimer(POINTS_PER_SECOND_TIMER);
    SetPointsPerSecondPane(0);
    KillTimer(PLAYBACK_TIMER);

    for(M_UINT8 s=0 ; s<MAX_TRACKS ; ++s)
    {
        if(outputDeviceArray[s])
        {
            (outputDeviceArray[s])->DisplayOff(invertBlanking, colorMode);
        }
    }

//    UpdateData(FALSE);
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayRecord()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayRecord() 
{
    char fileTitle[MAX_FILETITLE_SIZE];
    char fileName[MAX_FILENAME_SIZE];

    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.hInstance = NULL;

    ofn.lpstrFilter =
    TEXT("ILDA laser display files *.ild\0*.ILD\0All Files *.*\0*.*\0\0");

    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = 500;
    ofn.lpstrFileTitle = fileTitle;
    ofn.nMaxFileTitle = 99;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = "Save ILDA File";
    ofn.Flags = OFN_FILEMUSTEXIST;
    ofn.lpstrDefExt = "ILD";
    ofn.lCustData = NULL;
    ofn.lpfnHook = NULL;
    ofn.lpTemplateName = NULL;

    fileName[0] = '\0';

    GetSaveFileName(&ofn);
    errno_t forv = 1;

    if (fileName[0] == '\0')
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "OnDisplayRecord(): GetSaveFileName() failed." << endl;
#endif
    return;
    }
    else
    {

        forv = fopen_s(&outFile, &fileName[0],"wb");
        logFile << "Saving ILDA file: " << &fileName[0] << endl;
    }
    
#ifdef CHEAP3DVIEW_LOG_ERROR
    if (forv != 0)
    {
        logFile << "OnDisplayRecord(): Unable to open file " 
                << & fileName[0] << endl;
    }
#endif

    frameNumPosArray = new M_INT32[1000];

    // msvob 4/13/2022: What in the name of Sam Hill is this?!?!?!?!
logFile << "Writing 69 to array position 69..." << endl;
frameNumPosArray[69] = 69;
logFile << "Reading " << frameNumPosArray[69] << " from array position 69." << endl;

}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayRotatePlusX()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayRotatePlusX() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayRotatePlusX()" << endl;
#endif
    jobBlock.functionType = ROTATE_FROM;
    jobBlock.numOfFrames = 1;
    jobBlock.rotationAxis = X_AXIS;
    jobBlock.degreeAngle = ((float)rotateIncrement);
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
//	DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayRotateMinusX()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayRotateMinusX() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayRotateMinusX()" << endl;
#endif
    jobBlock.functionType = ROTATE_FROM;
    jobBlock.numOfFrames = 1;
    jobBlock.rotationAxis = X_AXIS;
    jobBlock.degreeAngle = -(((float)rotateIncrement));
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
//	DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayRotatePlusY()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayRotatePlusY() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayRotatePlusY()" << endl;
#endif
    jobBlock.functionType = ROTATE_FROM;
    jobBlock.numOfFrames = 1;
    jobBlock.rotationAxis = Y_AXIS;
    jobBlock.degreeAngle = ((float)rotateIncrement);
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
//	DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayRotateMinusY()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayRotateMinusY() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayRotateMinusY()" << endl;
#endif
    jobBlock.functionType = ROTATE_FROM;
    jobBlock.numOfFrames = 1;
    jobBlock.rotationAxis = Y_AXIS;
    jobBlock.degreeAngle = -(((float)rotateIncrement));
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
//	DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayRotatePlusZ()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayRotatePlusZ() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayRotatePlusZ()" << endl;
#endif
    jobBlock.functionType = ROTATE_FROM;
    jobBlock.numOfFrames = 1;
    jobBlock.rotationAxis = Z_AXIS;
    jobBlock.degreeAngle = ((float)rotateIncrement);
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
//	DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayRotateMinusZ()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayRotateMinusZ() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayRotateMinusZ()" << endl;
#endif
    jobBlock.functionType = ROTATE_FROM;
    jobBlock.numOfFrames = 1;
    jobBlock.rotationAxis = Z_AXIS;
    jobBlock.degreeAngle = -(((float)rotateIncrement));
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
//	DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayZoomIn()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayZoomIn() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayZoomIn()" << endl;
#endif
    jobBlock.functionType = ZOOM;
    jobBlock.numOfFrames = 1;
    jobBlock.zoomDirection = ZOOM_IN;
    jobBlock.zoomIncrement = ((float)zoomIncrement);
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
//	DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayZoomOut()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayZoomOut() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayZoomIn()" << endl;
#endif
    jobBlock.functionType = ZOOM;
    jobBlock.numOfFrames = 1;
    jobBlock.zoomDirection = ZOOM_OUT;
    jobBlock.zoomIncrement = ((float)zoomIncrement);
    jobBlock.skipManipulation = M_FALSE;
    SetTracksJobBlocks(jobBlock);
//	DisplayAllTracks();
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayIncreaseDelay()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayIncreaseDelay() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayIncreaseDelay()" << endl;
#endif

    Track* tempTrack;
    tempTrack = trackList.GetTrack(m_TrackFocus);

    if (!tempTrack)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "OnDisplayIncreaseDelay(): NULL Track pointer." 
                << endl;
#endif
        return;
    }

    DisplayParams tempParams = tempTrack->GetTrackParams();

    if (tempParams.delayValue <= (MAX_TRACK_DELAY - DELAYINCR))
    {
        tempParams.delayValue = tempParams.delayValue + DELAYINCR;
        tempTrack->SetTrackParams(tempParams);
        m_FocusTrackDelay = tempParams.delayValue;
        m_SliderFocusTrackDelay.SetPos(m_FocusTrackDelay);
        UpdateData(FALSE);
    }
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnDisplayDecreaseDelay()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnDisplayDecreaseDelay() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnDisplayDecreaseDelay()" << endl;
#endif

    Track* tempTrack;
    tempTrack = trackList.GetTrack(m_TrackFocus);

    if (!tempTrack)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "OnDisplayDecreaseDelay(): NULL Track pointer." 
                << endl;
#endif
        return;
    }

    DisplayParams tempParams = tempTrack->GetTrackParams();

    if (tempParams.delayValue >= (MIN_TRACK_DELAY + DELAYINCR))
    {
        tempParams.delayValue = tempParams.delayValue - DELAYINCR;
        tempTrack->SetTrackParams(tempParams);
        m_FocusTrackDelay = tempParams.delayValue;
        m_SliderFocusTrackDelay.SetPos(m_FocusTrackDelay);
        UpdateData(FALSE);
    }
}

////////////////////////////////////////////////////////////////////
//
// CCheap3DView::OnButtonReset()
//
////////////////////////////////////////////////////////////////////
void CCheap3DView::OnButtonReset() 
{
#ifdef CHEAP3DVIEW_LOG_INFO
    logFile << "Entering OnButtonReset()" << endl;
#endif

    Track* tempTrack;
    tempTrack = trackList.GetTrack(m_TrackFocus);

    if (!tempTrack)
    {
#ifdef CHEAP3DVIEW_LOG_ERROR
        logFile << "OnButtonReset(): NULL Track pointer." 
                << endl;
#endif
        return;
    }

    ViewType theView = tempTrack->GetTrackView();
    InitializeCurrentView(theView);
    m_SliderTrackCameraAngle.SetPos((int)(theView.view_angle));
    m_TrackCameraAngle = (UINT)(theView.view_angle);
    tempTrack->SetTrackView(theView);

    UpdateData(FALSE);
}
