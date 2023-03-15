// ColoredStatusBarCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "dlgviewtest.h"
#include "ColoredStatusBarCtrl.h"

#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std; 

extern ofstream logFile;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColoredStatusBarCtrl

CColoredStatusBarCtrl::CColoredStatusBarCtrl()
{
	for (UINT i=0; i<NUM_PANES; ++i)
	{
        m_StatusBarPaneInfoArray[i].backColor = RGB(255,255,255);
        m_StatusBarPaneInfoArray[i].fontColor = RGB(0,0,0);
        m_StatusBarPaneInfoArray[i].messageString = "";
	}
	// useCustomId = -1;
}

CColoredStatusBarCtrl::~CColoredStatusBarCtrl()
{
}


BEGIN_MESSAGE_MAP(CColoredStatusBarCtrl, CStatusBar)
	//{{AFX_MSG_MAP(CColoredStatusBarCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CColoredStatusBarCtrl::SetPaneTextClr(UINT    id,
		                                   CString statusStr,
		                                   COLORREF backClr,
		                                   COLORREF fontClr)
{
    // useCustomId = id;
    m_StatusBarPaneInfoArray[id].backColor = backClr;
    m_StatusBarPaneInfoArray[id].fontColor = fontClr;
    m_StatusBarPaneInfoArray[id].messageString = statusStr;

	// statusString = statusStr;
    // newBackClr = backClr;
    /// newFontClr = fontClr;

}

/////////////////////////////////////////////////////////////////////////////
// CColoredStatusBarCtrl message handlers

void CColoredStatusBarCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	UINT id = lpDrawItemStruct->itemID;

	// if (useCustomId >= 0 &&
    //    (UINT)useCustomId == id)
	{
#ifdef STATUS_BAR_LOG_INFO
           logFile << "CColoredStatusBarCtrl::DrawItem: ID = " << dec << id << endl;
#endif
		// Attach to a CDC object
		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);

		// dc.SetBkMode(TRANSPARENT);

		// Get the pane rectangle and calculate text coordinates
		CRect rect(&lpDrawItemStruct->rcItem);

        // COLORREF crOldTextColor = dc.GetTextColor();
        // COLORREF crOldBkColor = dc.GetBkColor();

		dc.SetTextColor(m_StatusBarPaneInfoArray[id].fontColor);
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, m_StatusBarPaneInfoArray[id].backColor);
        dc.TextOut(rect.left+2, rect.top, m_StatusBarPaneInfoArray[id].messageString);

        // dc.SetTextColor(crOldTextColor);
        // dc.SetBkColor(crOldBkColor);

		// Detach from the CDC object, otherwise the hDC will be
		// destroyed when the CDC object goes out of scope
		dc.Detach();

		// useCustomId = -1;
	}

}

/*
void CColoredStatusBarCtrl::DrawItem()
{
	if (useCustomId >= 0 &&
		(UINT)useCustomId == lpDrawItemStruct->itemID)
	{
#ifdef STATUS_BAR_LOG_INFO
           logFile << "CColoredStatusBarCtrl::DrawItem: ID = " << dec << lpDrawItemStruct->itemID << endl;
#endif
		// Attach to a CDC object
		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);

		// dc.SetBkMode(TRANSPARENT);

		// Get the pane rectangle and calculate text coordinates
		CRect rect(&lpDrawItemStruct->rcItem);

        COLORREF crOldTextColor = dc.GetTextColor();
        COLORREF crOldBkColor = dc.GetBkColor();

		dc.SetTextColor(newFontClr);
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, newBackClr);
        dc.TextOut(rect.left+2, rect.top, statusString);

        dc.SetTextColor(crOldTextColor);
        dc.SetBkColor(crOldBkColor);

		// Detach from the CDC object, otherwise the hDC will be
		// destroyed when the CDC object goes out of scope
		dc.Detach();

		useCustomId = -1;
	}

} */
