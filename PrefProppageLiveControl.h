#if !defined(AFX_PREFPROPPAGELIVECONTROL_H__866AA275_20B7_45BC_A15A_B0623A474318__INCLUDED_)
#define AFX_PREFPROPPAGELIVECONTROL_H__866AA275_20B7_45BC_A15A_B0623A474318__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCheap3DView;

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: PrefProppageLiveControl.h                  *
// * Description: This file contains the interface for     *
// *              CPrefProppageLiveControl class, which    *
// *              is the Live Control preferences          *
// *              property page.                           *
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

class CPrefProppageLiveControl : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefProppageLiveControl)

// Construction
public:
	CPrefProppageLiveControl();
	~CPrefProppageLiveControl();
    BOOL OnSetActive();

	CCheap3DView* parentPtr;

// Dialog Data
	//{{AFX_DATA(CPrefProppageLiveControl)
	enum { IDD = IDD_PREF_PROPPAGE_LIVE_CONTROL };
	CSliderCtrl	m_SliderFadeOut;
	CSliderCtrl	m_SliderFadeIn;
	CSliderCtrl	m_SliderZoomIncrement;
	CSliderCtrl	m_SliderRotateIncrement;
	CSliderCtrl	m_SliderMoveIncrement;
	CSliderCtrl	m_SliderDragInertia;
    CSpinButtonCtrl m_SpinCtrlDragSpeed;
    CString     m_EditContentRootDirectory;
	UINT	m_FadeInFrames;
	UINT	m_FadeOutFrames;
	UINT	m_MoveIncrement;
	UINT	m_RotateIncrement;
	UINT	m_ZoomIncrement;
    UINT    m_DragInertia;
    UINT    m_DragSpeed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPrefProppageLiveControl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPrefProppageLiveControl)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnUpdateEditContentRootDirectory();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFPROPPAGELIVECONTROL_H__866AA275_20B7_45BC_A15A_B0623A474318__INCLUDED_)
