//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: PrefProppageColor.cpp                      *
// * Description: Implementation of the                    *
// * CPrefProppageColor class, which is the Color property *
// * page.                                                 *
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
#include "PrefProppageColor.h"
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
// CPrefProppageColor property page

IMPLEMENT_DYNCREATE(CPrefProppageColor, CPropertyPage)

CPrefProppageColor::CPrefProppageColor() : CPropertyPage(CPrefProppageColor::IDD)
{
    //{{AFX_DATA_INIT(CPrefProppageColor)
    m_RadioDisplayColorMode = -1;
    //}}AFX_DATA_INIT

}

BOOL CPrefProppageColor::OnSetActive()
{
    CPropertyPage::OnSetActive();

    m_RadioDisplayColorMode = (int)(parentPtr->colorMode);

    m_SliderMasterRed.SetRange(1, 100);
    m_SliderMasterRed.SetTicFreq(10);
    m_EditMasterRed = (M_UINT16)((parentPtr->masterRed) * 100);
    m_SliderMasterRed.SetPos(m_EditMasterRed);

    m_SliderMasterGreen.SetRange(1, 100);
    m_SliderMasterGreen.SetTicFreq(10);
    m_EditMasterGreen = (M_UINT16)((parentPtr->masterGreen) * 100);
    m_SliderMasterGreen.SetPos(m_EditMasterGreen);

    m_SliderMasterBlue.SetRange(1, 100);
    m_SliderMasterBlue.SetTicFreq(10);
    m_EditMasterBlue = (M_UINT16)((parentPtr->masterBlue) * 100);
    m_SliderMasterBlue.SetPos(m_EditMasterBlue);

    m_SliderTTLThresholdRed.SetRange(1, 100);
    m_SliderTTLThresholdRed.SetTicFreq(10);
    m_EditTTLThresholdRed = (M_UINT16)((parentPtr->ttlThresholdRed) * 100);
    m_SliderTTLThresholdRed.SetPos(m_EditTTLThresholdRed);

    m_SliderTTLThresholdGreen.SetRange(1, 100);
    m_SliderTTLThresholdGreen.SetTicFreq(10);
    m_EditTTLThresholdGreen = (M_UINT16)((parentPtr->ttlThresholdGreen) * 100);
    m_SliderTTLThresholdGreen.SetPos(m_EditTTLThresholdGreen);

    m_SliderTTLThresholdBlue.SetRange(1, 100);
    m_SliderTTLThresholdBlue.SetTicFreq(10);
    m_EditTTLThresholdBlue = (M_UINT16)((parentPtr->ttlThresholdBlue) * 100);
    m_SliderTTLThresholdBlue.SetPos(m_EditTTLThresholdBlue);

    UpdateData(FALSE);

    return true;
}

CPrefProppageColor::~CPrefProppageColor()
{
}

void CPrefProppageColor::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPrefProppageColor)
    DDX_Radio(pDX, IDC_PREF_COLOR_NORM, m_RadioDisplayColorMode);
    DDX_Control(pDX, IDC_SLIDER_MASTER_RED, m_SliderMasterRed);
    DDX_Control(pDX, IDC_SLIDER_MASTER_GREEN, m_SliderMasterGreen);
    DDX_Control(pDX, IDC_SLIDER_MASTER_BLUE, m_SliderMasterBlue);
    DDX_Text(pDX, IDC_EDIT_MASTER_RED, m_EditMasterRed);
    DDX_Text(pDX, IDC_EDIT_MASTER_GREEN, m_EditMasterGreen);
    DDX_Text(pDX, IDC_EDIT_MASTER_BLUE, m_EditMasterBlue);
    DDX_Control(pDX, IDC_SLIDER_TTL_THRESHOLD_RED, m_SliderTTLThresholdRed);
    DDX_Control(pDX, IDC_SLIDER_TTL_THRESHOLD_GREEN, m_SliderTTLThresholdGreen);
    DDX_Control(pDX, IDC_SLIDER_TTL_THRESHOLD_BLUE, m_SliderTTLThresholdBlue);
    DDX_Text(pDX, IDC_EDIT_TTL_THRESHOLD_RED, m_EditTTLThresholdRed);
    DDX_Text(pDX, IDC_EDIT_TTL_THRESHOLD_GREEN, m_EditTTLThresholdGreen);
    DDX_Text(pDX, IDC_EDIT_TTL_THRESHOLD_BLUE, m_EditTTLThresholdBlue);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefProppageColor, CPropertyPage)
    //{{AFX_MSG_MAP(CPrefProppageColor)
    ON_BN_CLICKED(IDC_PREF_COLOR_NORM, OnPrefColorNorm)
    ON_BN_CLICKED(IDC_PREF_COLOR_MONO, OnPrefColorMono)
    ON_BN_CLICKED(IDC_PREF_COLOR_INV, OnPrefColorInv)
    ON_BN_CLICKED(IDC_PREF_COLOR_GRAY, OnPrefColorGray)
    ON_BN_CLICKED(IDC_PREF_COLOR_CVL, OnPrefColorCVL)
    ON_BN_CLICKED(IDC_PREF_COLOR_TTL, OnPrefColorTTL)
    ON_BN_CLICKED(IDC_PREF_RG_INV_TTL, OnPrefColorRGInvTTL)
    ON_WM_HSCROLL()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefProppageColor message handlers

void CPrefProppageColor::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    m_EditMasterRed = m_SliderMasterRed.GetPos();
    parentPtr->masterRed = (double)m_EditMasterRed / (double)100.0;

    m_EditMasterGreen = m_SliderMasterGreen.GetPos();
    parentPtr->masterGreen = (double)m_EditMasterGreen / (double)100.0;

    m_EditMasterBlue = m_SliderMasterBlue.GetPos();
    parentPtr->masterBlue = (double)m_EditMasterBlue / (double)100.0;

    m_EditTTLThresholdRed = m_SliderTTLThresholdRed.GetPos();
    parentPtr->ttlThresholdRed = (double)m_EditTTLThresholdRed / (double)100.0;

    m_EditTTLThresholdGreen = m_SliderTTLThresholdGreen.GetPos();
    parentPtr->ttlThresholdGreen = (double)m_EditTTLThresholdGreen / (double)100.0;

    m_EditTTLThresholdBlue = m_SliderTTLThresholdBlue.GetPos();
    parentPtr->ttlThresholdBlue = (double)m_EditTTLThresholdBlue / (double)100.0;

    m_RadioDisplayColorMode = (int)(parentPtr->colorMode);

    UpdateData(FALSE);

}

void CPrefProppageColor::OnPrefColorNorm() 
{
    parentPtr->colorMode = DisplayColorMode::COLOR_NORMAL;
}

void CPrefProppageColor::OnPrefColorMono() 
{
    parentPtr->colorMode = DisplayColorMode::COLOR_MONOCHROME;
}

void CPrefProppageColor::OnPrefColorInv() 
{
    parentPtr->colorMode = DisplayColorMode::COLOR_INVERT;
}

void CPrefProppageColor::OnPrefColorGray() 
{
    parentPtr->colorMode = DisplayColorMode::COLOR_GRAYSCALE;
}

void CPrefProppageColor::OnPrefColorCVL() 
{
    parentPtr->colorMode = DisplayColorMode::COLOR_CVL;
}

void CPrefProppageColor::OnPrefColorTTL() 
{
    parentPtr->colorMode = DisplayColorMode::COLOR_TTL;
}

void CPrefProppageColor::OnPrefColorRGInvTTL() 
{
    parentPtr->colorMode = DisplayColorMode::COLOR_RG_INV_TTL;
}

