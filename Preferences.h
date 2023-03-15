#if !defined(AFX_PREFERENCES_H__CE9D932D_143A_43FC_B5E6_18EDBFDBA6ED__INCLUDED_)
#define AFX_PREFERENCES_H__CE9D932D_143A_43FC_B5E6_18EDBFDBA6ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Preferences.h                              *
// * Description: This file contains the interface for the *
// *              CPreferences dialog class.               *
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

class CCheap3DView;

#include "PrefProppageDisplayTimer.h"
#include "PrefProppageColor.h"
#include "PrefProppageIldaRecording.h"
#include "PrefProppageLiveControl.h"
#include "PrefProppageBeamView.h"
#include "PrefProppageOutput.h"
#include "PageComSettings.h"

/////////////////////////////////////////////////////////////////////////////
// CPreferences dialog

class CPreferences : public CPropertySheet
{
	friend CCheap3DView;

// Construction
public:
	CPreferences(CWnd* pParent = NULL);   // standard constructor

// Attributes
	CPrefProppageColor          colorPropertyPage;
	CPrefProppageDisplayTimer   displayTimerPropertyPage;
	CPrefProppageIldaRecording  ildaRecordingPropertyPage;
	CPrefProppageLiveControl    liveControlPropertyPage;
	CPrefProppageBeamView       beamViewPropertyPage;
	CPrefProppageOutput         outputPropertyPage;
	CPageComSettings            comSettingsPropertyPage;

// Dialog Data
	//{{AFX_DATA(CPreferences)
	enum { IDD = IDD_PREFERENCES_DIALOG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreferences)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CCheap3DView* parentPtr;

	// Generated message map functions
	//{{AFX_MSG(CPreferences)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCES_H__CE9D932D_143A_43FC_B5E6_18EDBFDBA6ED__INCLUDED_)
