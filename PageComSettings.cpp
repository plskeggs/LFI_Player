#include "StdAfx.h"
//#include "SerialTestMFC.h"
#include "PageComSettings.h"
#include "Serial.h"
#include "Cheap3DView.h"
#include "Routines.h"

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
// CPageComSettings property page

BEGIN_MESSAGE_MAP(CPageComSettings, CPropertyPage)
	//{{AFX_MSG_MAP(CPageComSettings)
	ON_BN_CLICKED(IDC_BUTTON_COM_APPLY, OnButtonComApply)
	ON_BN_CLICKED(IDC_CHECK_COM_ENABLED, OnCheckComEnabled)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPageComSettings methods

// CPageComSettings::CPageComSettings(CSerial* pSerial)
CPageComSettings::CPageComSettings()
	: CPropertyPage(CPageComSettings::IDD)
//	, m_pSerial(pSerial)
{
	//{{AFX_DATA_INIT(CPrefProppageDisplayTimer)
	m_nComIndex = 0;
	//}}AFX_DATA_INIT
}

void CPageComSettings::OnCheckComEnabled() 
{
	UpdateData(TRUE);
	parentPtr->useSerialPort = m_CheckComEnabled;
}

BOOL CPageComSettings::OnSetActive() 
{
	m_nComIndex = comStringToIndex(parentPtr->strComPort);

	// Enable only existing COM-ports
    if(CSerial::CheckPort(_T("COM1")) == CSerial::EPortAvailable ||
	   (parentPtr->serialPort.IsOpen() == true &&  m_nComIndex == 0))
	{
        GetDlgItem(IDC_COM1)->EnableWindow(true);
	}

    if(CSerial::CheckPort(_T("COM2")) == CSerial::EPortAvailable ||
	   (parentPtr->serialPort.IsOpen() == true &&  m_nComIndex == 1))
	{
        GetDlgItem(IDC_COM2)->EnableWindow(true);
	}

    if(CSerial::CheckPort(_T("COM3")) == CSerial::EPortAvailable ||
	   (parentPtr->serialPort.IsOpen() == true &&  m_nComIndex == 2))
	{
        GetDlgItem(IDC_COM3)->EnableWindow(true);
	}

    if(CSerial::CheckPort(_T("COM4")) == CSerial::EPortAvailable ||
	   (parentPtr->serialPort.IsOpen() == true &&  m_nComIndex == 3))
	{
        GetDlgItem(IDC_COM4)->EnableWindow(true);
	}

//	GetDlgItem(IDC_COM1)->EnableWindow(CSerial::CheckPort(_T("COM1")) == CSerial::EPortAvailable);
//	GetDlgItem(IDC_COM2)->EnableWindow(CSerial::CheckPort(_T("COM2")) == CSerial::EPortAvailable);
//	GetDlgItem(IDC_COM3)->EnableWindow(CSerial::CheckPort(_T("COM3")) == CSerial::EPortAvailable);
//	GetDlgItem(IDC_COM4)->EnableWindow(CSerial::CheckPort(_T("COM4")) == CSerial::EPortAvailable);

	// Obtain all settings from the serial port
//	CSerial::EBaudrate  eBaudrate  = m_pSerial->GetBaudrate();
//	CSerial::EDataBits  eDataBits  = m_pSerial->GetDataBits();
//	CSerial::EParity    eParity    = m_pSerial->GetParity();
//	CSerial::EStopBits  eStopBits  = m_pSerial->GetStopBits();
//	CSerial::EHandshake eHandshake = m_pSerial->GetHandshaking();

	m_nBaudrateIndex = baudrateToIndex(parentPtr->serialPort.GetBaudrate());
	m_nDataIndex = databitsToIndex(parentPtr->serialPort.GetDataBits());
    m_nParityIndex = parityToIndex(parentPtr->serialPort.GetParity());
	m_nStopIndex = stopbitsToIndex(parentPtr->serialPort.GetStopBits());
	m_nHandshakingIndex = handshakeToIndex(parentPtr->serialPort.GetHandshaking());

	m_CheckComEnabled = parentPtr->useSerialPort;

	UpdateData(false);
	return true;
}

void CPageComSettings::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageComSettings)
	DDX_Radio(pDX, IDC_BAUDRATE_1200, m_nBaudrateIndex);
	DDX_Radio(pDX, IDC_DATA_5, m_nDataIndex);
	DDX_Radio(pDX, IDC_HANDSHAKING_OFF, m_nHandshakingIndex);
	DDX_Radio(pDX, IDC_PARITY_NONE, m_nParityIndex);
	DDX_Radio(pDX, IDC_STOP_1, m_nStopIndex);
	DDX_Radio(pDX, IDC_COM1, m_nComIndex);
	DDX_Check(pDX, IDC_CHECK_COM_ENABLED, m_CheckComEnabled);
	//}}AFX_DATA_MAP
}

void CPageComSettings::OnButtonComApply() 
{
	// Retrieve the settings from the page
	UpdateData(true);

	CSerial::EBaudrate  eBaudrate  = indexToBaudrate(m_nBaudrateIndex);
	CSerial::EDataBits  eDataBits  = indexToDatabits(m_nDataIndex);
	CSerial::EParity    eParity    = indexToParity(m_nParityIndex);
	CSerial::EStopBits  eStopBits  = indexToStopbits(m_nStopIndex);
	CSerial::EHandshake eHandshake = indexToHandshake(m_nHandshakingIndex);

    // Generate the COM port string
	parentPtr->strComPort.Format(_T("COM%d"), m_nComIndex+1);

    if (parentPtr->serialPort.IsOpen() == true)
        parentPtr->serialPort.Close();

	if (parentPtr->useSerialPort == M_TRUE)
	{
	    // Open the serial port
	    if (parentPtr->serialPort.Open(parentPtr->strComPort,parentPtr) != ERROR_SUCCESS)
		{
		    AfxMessageBox(_T("Unable to open COM-port"),MB_ICONSTOP|MB_OK);
            logFile << "OnButtonComApply(): Unable to open COM-port." << endl;
		    return;
		}
	}

	// Setup the COM port
	if (parentPtr->serialPort.Setup(eBaudrate,eDataBits,eParity,eStopBits))
	{
		logFile << "OnButtonComApply(): COM Setup() failed." << endl;
		return;
	}

	// Setup handshaking
	if (parentPtr->serialPort.SetupHandshaking(eHandshake))
	{
		logFile << "OnButtonComApply(): COM SetupHandshaking() failed." << endl;
		return;
	}

//	// Call base class
//	return CPropertyPage::OnApply();
}
