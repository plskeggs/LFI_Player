#if !defined(AFX_PREFPROPPAGEBEAMVIEW_H__68765CC4_1E37_4949_960F_D0A8CAB7FBD8__INCLUDED_)
#define AFX_PREFPROPPAGEBEAMVIEW_H__68765CC4_1E37_4949_960F_D0A8CAB7FBD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: PrefProppageBeamView.h                     *
// * Description: This file contains the interface for     *
// *              CPrefProppageBeamView class, which is    *
// *              the Beam View preferences property page. *
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

class CPrefProppageBeamView : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefProppageBeamView)

// Construction
public:
	CPrefProppageBeamView();
	~CPrefProppageBeamView();

    BOOL OnSetActive();
	CCheap3DView* parentPtr;

// Dialog Data
	//{{AFX_DATA(CPrefProppageBeamView)
	enum { IDD = IDD_PREF_PROPPAGE_BEAMVIEW };
	CSliderCtrl	m_SliderBeamOriginX1;
	CSliderCtrl	m_SliderBeamOriginX2;
	CSliderCtrl	m_SliderBeamOriginY1;
	CSliderCtrl	m_SliderBeamOriginY2;
	CSliderCtrl	m_SliderBeamHeight1;
	CSliderCtrl	m_SliderBeamHeight2;
	UINT	    m_BeamOriginX1;
	UINT	    m_BeamOriginX2;
	UINT	    m_BeamOriginY1;
	UINT	    m_BeamOriginY2;
	UINT	    m_BeamHeight1;
	UINT	    m_BeamHeight2;
    BOOL        m_CheckBeamOn1;
    BOOL        m_CheckBeamOn2;
    BOOL        m_CheckBeamInvertX1;
    BOOL        m_CheckBeamInvertX2;
    BOOL        m_CheckBeamYagView;
    int         m_RadioBeamMode;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPrefProppageBeamView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPrefProppageBeamView)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckBeamOn1();
	afx_msg void OnCheckBeamOn2();
	afx_msg void OnCheckBeamInvertX1();
	afx_msg void OnCheckBeamInvertX2();
	afx_msg void OnCheckBeamYagView();
	afx_msg void OnBeamModeNormal();
	afx_msg void OnBeamModeShowPoints();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFPROPPAGEBEAMVIEW_H__68765CC4_1E37_4949_960F_D0A8CAB7FBD8__INCLUDED_)