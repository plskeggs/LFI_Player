//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: PrefProppageOutput.cpp                     *
// * Description: Implementation of the                    *
// * CPrefProppageOutput class, which is the laser Output  *
// * property page.                                        *
// *                                                       *
// * NOTE: This file was generated by MS Visual Studio,    *
// *       and modified by the author as necessary for     *
// *       the LFI Player application.
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#include "stdafx.h"
#include "Cheap3D.h"
#include "PrefProppageOutput.h"
#include "Cheap3DView.h"
#include <iostream>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std; 

extern ofstream logFile;

/////////////////////////////////////////////////////////////////////////////
// CPrefProppageOutput property page

IMPLEMENT_DYNCREATE(CPrefProppageOutput, CPropertyPage)

CPrefProppageOutput::CPrefProppageOutput() : CPropertyPage(CPrefProppageOutput::IDD)
{
	//{{AFX_DATA_INIT(CPrefProppageOutput)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPrefProppageOutput::~CPrefProppageOutput()
{
}

BOOL CPrefProppageOutput::OnSetActive()
{
	m_CheckInvertX = FALSE;
	m_CheckInvertY = FALSE;
    m_CheckScreenInvert = FALSE;

	m_DisplayLaser = parentPtr->displayLaser;
	m_DisplayScreen = parentPtr->displayScreen;
	m_ScriptRepeat = parentPtr->scriptRepeat;
    m_FlickerFree = parentPtr->flickerFree;
    m_InvertBlanking = parentPtr->invertBlanking;
	m_BlankClipView = parentPtr->blankClipView;

    if (parentPtr->invertBitmap & INVERT_X)
    	m_CheckInvertX = TRUE;

    if (parentPtr->invertBitmap & INVERT_Y)
    	m_CheckInvertY = TRUE;

    if (parentPtr->invertBitmap & SCREEN_INVERT)
    	m_CheckScreenInvert = TRUE;

	m_SliderLaserScale.SetRange(0, 100);
	m_SliderLaserScale.SetTicFreq(10);
	m_LaserScale = parentPtr->defaultLaserScale;
//	parentPtr->laserScale = m_LaserScale;
	m_SliderLaserScale.SetPos(m_LaserScale);

	m_SliderVerticalScale.SetRange(0, 100);
	m_SliderVerticalScale.SetTicFreq(10);
	m_VerticalScale = parentPtr->defaultVerticalScale;
//    parentPtr->verticalScale = m_VerticalScale;
	m_SliderVerticalScale.SetPos(m_VerticalScale);

	m_SliderGlobalTrackDelay.SetRange(MIN_GLOBAL_DELAY, MAX_GLOBAL_DELAY);
	m_SliderGlobalTrackDelay.SetTicFreq(MAX_GLOBAL_DELAY/10);
	m_GlobalTrackDelay = parentPtr->defaultGlobalDelay;
//	parentPtr->globalTrackDelay = m_GlobalTrackDelay;
	m_SliderGlobalTrackDelay.SetPos(m_GlobalTrackDelay);

	m_SliderBlankingShift.SetRange(MIN_BLANKING_SHIFT, MAX_BLANKING_SHIFT);
	m_SliderBlankingShift.SetTicFreq(10);
	m_BlankingShift = parentPtr->blankingShift;
//	parentPtr->blankingShift = m_BlankingShift;
	m_SliderBlankingShift.SetPos(m_BlankingShift);

	UpdateData(FALSE);

	return true;
}

void CPrefProppageOutput::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefProppageOutput)
	DDX_Check(pDX, IDC_PREF_INVERTX, m_CheckInvertX);
	DDX_Check(pDX, IDC_PREF_INVERTY, m_CheckInvertY);
	DDX_Check(pDX, IDC_PREF_SCRN_INVERT, m_CheckScreenInvert);
	DDX_Control(pDX, IDC_SLIDER_VERT_SCALE, m_SliderVerticalScale);
	DDX_Control(pDX, IDC_SLIDER_GLOBAL_TRACK_DELAY, m_SliderGlobalTrackDelay);
	DDX_Control(pDX, IDC_SLIDER_LASER_SCALE, m_SliderLaserScale);
	DDX_Control(pDX, IDC_SLIDER_BLANKING_SHIFT, m_SliderBlankingShift);
	DDX_Check(pDX, IDC_CHECK_DISPLAY_LASER, m_DisplayLaser);
	DDX_Check(pDX, IDC_CHECK_DISPLAY_SCREEN, m_DisplayScreen);
	DDX_Check(pDX, IDC_CHECK_SCRIPT_REPEAT, m_ScriptRepeat);
	DDX_Check(pDX, IDC_CHECK_FLICKER_FREE, m_FlickerFree);
	DDX_Check(pDX, IDC_CHECK_INVERT_BLANKING, m_InvertBlanking);
	DDX_Check(pDX, IDC_CHECK_BLANKCLIP_VIEW, m_BlankClipView);
	DDX_Text(pDX, IDC_EDIT_GLOBAL_TRACK_DELAY, m_GlobalTrackDelay);
	DDX_Text(pDX, IDC_EDIT_LASER_SCALE, m_LaserScale);
	DDX_Text(pDX, IDC_EDIT_VERT_SCALE, m_VerticalScale);
	DDX_Text(pDX, IDC_EDIT_BLANKING_SHIFT, m_BlankingShift);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefProppageOutput, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefProppageOutput)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_PREF_INVERTX, OnPrefInvertx)
	ON_BN_CLICKED(IDC_PREF_INVERTY, OnPrefInverty)
	ON_BN_CLICKED(IDC_PREF_SCRN_INVERT, OnPrefScrnInvert)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY_LASER, OnCheckDisplayLaser)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY_SCREEN, OnCheckDisplayScreen)
	ON_BN_CLICKED(IDC_CHECK_SCRIPT_REPEAT, OnCheckScriptRepeat)
    ON_BN_CLICKED(IDC_CHECK_FLICKER_FREE, OnCheckFlickerFree)
	ON_BN_CLICKED(IDC_CHECK_BLANKCLIP_VIEW, OnCheckBlankClipView)
    ON_BN_CLICKED(IDC_CHECK_INVERT_BLANKING, OnCheckInvertBlanking)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefProppageOutput message handlers

void CPrefProppageOutput::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_GlobalTrackDelay = m_SliderGlobalTrackDelay.GetPos();
	parentPtr->globalTrackDelay   = m_GlobalTrackDelay;
	parentPtr->defaultGlobalDelay   = m_GlobalTrackDelay;

	m_LaserScale = m_SliderLaserScale.GetPos();
    parentPtr->laserScale        = m_LaserScale;
	parentPtr->defaultLaserScale = m_LaserScale;

	m_VerticalScale = m_SliderVerticalScale.GetPos();
	parentPtr->verticalScale        = m_VerticalScale;
	parentPtr->defaultVerticalScale = m_VerticalScale;

	m_BlankingShift = m_SliderBlankingShift.GetPos();
	parentPtr->blankingShift        = m_BlankingShift;

	UpdateData(FALSE);
}

void CPrefProppageOutput::OnCheckDisplayLaser() 
{
    if (m_DisplayLaser == TRUE)
	{
        m_DisplayLaser = FALSE;
	    parentPtr->displayLaser = m_DisplayLaser;

        for(M_UINT8 s=0 ; s<MAX_TRACKS ; ++s)
        {
            if(parentPtr->outputDeviceArray[s])
            {
                (parentPtr->outputDeviceArray[s])->DisplayOff(parentPtr->invertBlanking, parentPtr->colorMode);
            }
        }
	}
	else
	{
	    m_DisplayLaser = TRUE;
		parentPtr->displayLaser = m_DisplayLaser;
	}

	UpdateData(FALSE);
}

void CPrefProppageOutput::OnCheckDisplayScreen() 
{
	UpdateData(TRUE);
	parentPtr->displayScreen = m_DisplayScreen;
}

void CPrefProppageOutput::OnCheckFlickerFree() 
{
	UpdateData(TRUE);
	parentPtr->flickerFree = m_FlickerFree;
}

void CPrefProppageOutput::OnCheckBlankClipView() 
{
	UpdateData(TRUE);
	parentPtr->blankClipView = m_BlankClipView;
}

void CPrefProppageOutput::OnCheckInvertBlanking() 
{
	UpdateData(TRUE);
	parentPtr->invertBlanking = (M_UINT16)m_InvertBlanking;
}

void CPrefProppageOutput::OnPrefInvertx() 
{
    parentPtr->invertBitmap ^= INVERT_X;
	UpdateData(TRUE);
}

void CPrefProppageOutput::OnPrefInverty() 
{
    parentPtr->invertBitmap ^= INVERT_Y;
	UpdateData(TRUE);
}

void CPrefProppageOutput::OnPrefScrnInvert() 
{
    parentPtr->invertBitmap ^= SCREEN_INVERT;
	UpdateData(TRUE);
}

void CPrefProppageOutput::OnCheckScriptRepeat() 
{
	UpdateData(TRUE);
	parentPtr->scriptRepeat = m_ScriptRepeat;
}


