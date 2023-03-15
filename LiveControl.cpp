//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: LiveControl.cpp                            *
// * Description: Implementation of the CLiveControl       *
// * dialog class (i.e. the "Live" control console).       *
// *                                                       *
// * NOTE: Some or all of the code contained in this file  *
// *       was generated by the Visual Studio MFC          *
// *       framework, and modified by the author as        *
// *       necessary for this application.                 *
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
#include <string.h>

#include "stdafx.h"
#include "Cheap3D.h"
#include "LiveControl.h"
#include "Cheap3DView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std; 

extern ofstream logFile;

/////////////////////////////////////////////////////////////////////////////
// CLiveControl dialog


////////////////////////////////////////////////////////////////////
//
// CLiveControl::CLiveControl()
//
////////////////////////////////////////////////////////////////////
CLiveControl::CLiveControl(CWnd* pParent /*=NULL*/)
    : CDialog(CLiveControl::IDD, pParent),
    currentPage(0),
    queButtonStatus(M_FALSE)
{
    parentPtr = (CCheap3DView*)pParent;
    //{{AFX_DATA_INIT(CLiveControl)
    m_LiveDescription = _T("");
    m_EditLiveFadeIn = 1;
    m_EditLiveFadeOut = 1;
    m_EditLiveFrameRepeat = 0;
    //}}AFX_DATA_INIT

    currentPage   = 0;
    currentButton = 0;

    for (M_UINT16 h=0 ; h<MAX_LIVE_PAGES ; ++h)
    {
        for (M_UINT16 i=0 ; i<MAX_LIVE_TABLE_ENTRIES ; ++i)
        {
            livePages[h].tableEntries[i] = 0;
        }

        livePages[h].currentTableEntry = 0;
    }

    currentPage  = 0;

}


////////////////////////////////////////////////////////////////////
//
// CLiveControl::OnInitDialog()
//
////////////////////////////////////////////////////////////////////
BOOL CLiveControl::OnInitDialog()
{
    CDialog::OnInitDialog();

    VERIFY(pageButtonArray[0].SubclassDlgItem(IDC_LIVE_PAGE_1, this));
    VERIFY(pageButtonArray[1].SubclassDlgItem(IDC_LIVE_PAGE_2, this));
    VERIFY(pageButtonArray[2].SubclassDlgItem(IDC_LIVE_PAGE_3, this));
    VERIFY(pageButtonArray[3].SubclassDlgItem(IDC_LIVE_PAGE_4, this));
    VERIFY(pageButtonArray[4].SubclassDlgItem(IDC_LIVE_PAGE_5, this));
    VERIFY(pageButtonArray[5].SubclassDlgItem(IDC_LIVE_PAGE_6, this));
    VERIFY(pageButtonArray[6].SubclassDlgItem(IDC_LIVE_PAGE_7, this));
    VERIFY(pageButtonArray[7].SubclassDlgItem(IDC_LIVE_PAGE_8, this));
    VERIFY(pageButtonArray[8].SubclassDlgItem(IDC_LIVE_PAGE_9, this));
    VERIFY(pageButtonArray[9].SubclassDlgItem(IDC_LIVE_PAGE_10, this));
    VERIFY(pageButtonArray[10].SubclassDlgItem(IDC_LIVE_PAGE_11, this));
    VERIFY(pageButtonArray[11].SubclassDlgItem(IDC_LIVE_PAGE_12, this));

    VERIFY(liveButtonArray[0].SubclassDlgItem(IDC_LIVE_BUTTON1, this));
    VERIFY(liveButtonArray[1].SubclassDlgItem(IDC_LIVE_BUTTON2, this));
    VERIFY(liveButtonArray[2].SubclassDlgItem(IDC_LIVE_BUTTON3, this));
    VERIFY(liveButtonArray[3].SubclassDlgItem(IDC_LIVE_BUTTON4, this));
    VERIFY(liveButtonArray[4].SubclassDlgItem(IDC_LIVE_BUTTON5, this));
    VERIFY(liveButtonArray[5].SubclassDlgItem(IDC_LIVE_BUTTON6, this));
    VERIFY(liveButtonArray[6].SubclassDlgItem(IDC_LIVE_BUTTON7, this));
    VERIFY(liveButtonArray[7].SubclassDlgItem(IDC_LIVE_BUTTON8, this));
    VERIFY(liveButtonArray[8].SubclassDlgItem(IDC_LIVE_BUTTON9, this));
    VERIFY(liveButtonArray[9].SubclassDlgItem(IDC_LIVE_BUTTON10, this));
    VERIFY(liveButtonArray[10].SubclassDlgItem(IDC_LIVE_BUTTON11, this));
    VERIFY(liveButtonArray[11].SubclassDlgItem(IDC_LIVE_BUTTON12, this));

    VERIFY(liveButtonArray[12].SubclassDlgItem(IDC_LIVE_BUTTON13, this));
    VERIFY(liveButtonArray[13].SubclassDlgItem(IDC_LIVE_BUTTON14, this));
    VERIFY(liveButtonArray[14].SubclassDlgItem(IDC_LIVE_BUTTON15, this));
    VERIFY(liveButtonArray[15].SubclassDlgItem(IDC_LIVE_BUTTON16, this));
    VERIFY(liveButtonArray[16].SubclassDlgItem(IDC_LIVE_BUTTON17, this));
    VERIFY(liveButtonArray[17].SubclassDlgItem(IDC_LIVE_BUTTON18, this));
    VERIFY(liveButtonArray[18].SubclassDlgItem(IDC_LIVE_BUTTON19, this));
    VERIFY(liveButtonArray[19].SubclassDlgItem(IDC_LIVE_BUTTON20, this));
    VERIFY(liveButtonArray[20].SubclassDlgItem(IDC_LIVE_BUTTON21, this));
    VERIFY(liveButtonArray[21].SubclassDlgItem(IDC_LIVE_BUTTON22, this));
    VERIFY(liveButtonArray[22].SubclassDlgItem(IDC_LIVE_BUTTON23, this));
    VERIFY(liveButtonArray[23].SubclassDlgItem(IDC_LIVE_BUTTON24, this));

    VERIFY(liveButtonArray[24].SubclassDlgItem(IDC_LIVE_BUTTON25, this));
    VERIFY(liveButtonArray[25].SubclassDlgItem(IDC_LIVE_BUTTON26, this));
    VERIFY(liveButtonArray[26].SubclassDlgItem(IDC_LIVE_BUTTON27, this));
    VERIFY(liveButtonArray[27].SubclassDlgItem(IDC_LIVE_BUTTON28, this));
    VERIFY(liveButtonArray[28].SubclassDlgItem(IDC_LIVE_BUTTON29, this));
    VERIFY(liveButtonArray[29].SubclassDlgItem(IDC_LIVE_BUTTON30, this));
    VERIFY(liveButtonArray[30].SubclassDlgItem(IDC_LIVE_BUTTON31, this));
    VERIFY(liveButtonArray[31].SubclassDlgItem(IDC_LIVE_BUTTON32, this));
    VERIFY(liveButtonArray[32].SubclassDlgItem(IDC_LIVE_BUTTON33, this));
    VERIFY(liveButtonArray[33].SubclassDlgItem(IDC_LIVE_BUTTON34, this));
    VERIFY(liveButtonArray[34].SubclassDlgItem(IDC_LIVE_BUTTON35, this));
    VERIFY(liveButtonArray[35].SubclassDlgItem(IDC_LIVE_BUTTON36, this));

    VERIFY(liveButtonArray[36].SubclassDlgItem(IDC_LIVE_BUTTON37, this));
    VERIFY(liveButtonArray[37].SubclassDlgItem(IDC_LIVE_BUTTON38, this));
    VERIFY(liveButtonArray[38].SubclassDlgItem(IDC_LIVE_BUTTON39, this));
    VERIFY(liveButtonArray[39].SubclassDlgItem(IDC_LIVE_BUTTON40, this));
    VERIFY(liveButtonArray[40].SubclassDlgItem(IDC_LIVE_BUTTON41, this));
    VERIFY(liveButtonArray[41].SubclassDlgItem(IDC_LIVE_BUTTON42, this));
    VERIFY(liveButtonArray[42].SubclassDlgItem(IDC_LIVE_BUTTON43, this));
    VERIFY(liveButtonArray[43].SubclassDlgItem(IDC_LIVE_BUTTON44, this));
    VERIFY(liveButtonArray[44].SubclassDlgItem(IDC_LIVE_BUTTON45, this));
    VERIFY(liveButtonArray[45].SubclassDlgItem(IDC_LIVE_BUTTON46, this));
    VERIFY(liveButtonArray[46].SubclassDlgItem(IDC_LIVE_BUTTON47, this));
    VERIFY(liveButtonArray[47].SubclassDlgItem(IDC_LIVE_BUTTON48, this));

    m_SliderLiveFadeIn.SetRange(MIN_LIVE_FADE_IN_FRAMES,
                                MAX_LIVE_FADE_IN_FRAMES);
    m_SliderLiveFadeIn.SetTicFreq(10);
    m_EditLiveFadeIn = parentPtr->fadeInFrames;
    m_SliderLiveFadeIn.SetPos((MAX_LIVE_FADE_IN_FRAMES + 1) - m_EditLiveFadeIn);

    m_SliderLiveFadeOut.SetRange(MIN_LIVE_FADE_OUT_FRAMES,
                                 MAX_LIVE_FADE_OUT_FRAMES);
    m_SliderLiveFadeOut.SetTicFreq(10);
    m_EditLiveFadeOut = parentPtr->fadeOutFrames;
    m_SliderLiveFadeOut.SetPos((MAX_LIVE_FADE_OUT_FRAMES + 1) - m_EditLiveFadeOut);

    m_SliderLiveColorIntensity.SetRange(MIN_LIVE_COLOR_INTENSITY,
                                        MAX_LIVE_COLOR_INTENSITY);
    m_SliderLiveColorIntensity.SetTicFreq(10);
    m_EditLiveColorIntensity = parentPtr->colorIntensity;
    m_SliderLiveColorIntensity.SetPos((MAX_LIVE_COLOR_INTENSITY) - m_EditLiveColorIntensity);

    // In the following, we need to consider a value of -1,
    // which indicates the global frame repeat should be set
    // to the maximum value possible (65535).  Note that this
    // is meant to be sort of a pseudo "infinite repeat",
    // where the affected tracks are "frozen" on the current
    // frame.  This differs from the regular MAX_LIVE_FRAME_REPEAT
    // value, in that the later is not intended to freeze the
    // animation, but does slow the progression way down.
    m_SliderLiveFrameRepeat.SetRange(MIN_LIVE_FRAME_REPEAT,
                                     MAX_LIVE_FRAME_REPEAT + 1);
    m_SliderLiveFrameRepeat.SetTicFreq(10);

    if (parentPtr->globalFrameRepeat > MAX_LIVE_FRAME_REPEAT)
    {
        m_EditLiveFrameRepeat = -1;
    }
    else
    {
        m_EditLiveFrameRepeat = parentPtr->globalFrameRepeat;
    }
    m_SliderLiveFrameRepeat.SetPos(MAX_LIVE_FRAME_REPEAT - m_EditLiveFrameRepeat);

    UpdateData(FALSE);

    return TRUE;
}

////////////////////////////////////////////////////////////////////
//
// CLiveControl::CLiveControl()
//
////////////////////////////////////////////////////////////////////
void CLiveControl::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CLiveControl)
    DDX_Control(pDX, IDC_SLIDER_LIVE_FRAME_REPEAT, m_SliderLiveFrameRepeat);
    DDX_Control(pDX, IDC_SLIDER_LIVE_FADE_OUT, m_SliderLiveFadeOut);
    DDX_Control(pDX, IDC_SLIDER_LIVE_FADE_IN, m_SliderLiveFadeIn);
    DDX_Control(pDX, IDC_SLIDER_LIVE_COLOR_INTENSITY, m_SliderLiveColorIntensity);
    DDX_Text(pDX, IDC_EDIT_LIVE_DESC, m_LiveDescription);
    DDX_Text(pDX, IDC_EDIT_LIVE_FADE_IN, m_EditLiveFadeIn);
    DDX_Text(pDX, IDC_EDIT_LIVE_FADE_OUT, m_EditLiveFadeOut);
    DDX_Text(pDX, IDC_EDIT_LIVE_FRAME_REPEAT, m_EditLiveFrameRepeat);
    DDX_Text(pDX, IDC_EDIT_LIVE_COLOR_INTENSITY, m_EditLiveColorIntensity);
    //}}AFX_DATA_MAP
}


////////////////////////////////////////////////////////////////////
//
// CLiveControl Message Map
//
////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CLiveControl, CDialog)
    //{{AFX_MSG_MAP(CLiveControl)
    ON_BN_CLICKED(IDC_LIVE_BUTTON1, OnLiveButton1)
    ON_BN_CLICKED(IDC_LIVE_BUTTON2, OnLiveButton2)
    ON_BN_CLICKED(IDC_LIVE_BUTTON3, OnLiveButton3)
    ON_BN_CLICKED(IDC_LIVE_BUTTON4, OnLiveButton4)
    ON_BN_CLICKED(IDC_LIVE_BUTTON5, OnLiveButton5)
    ON_BN_CLICKED(IDC_LIVE_BUTTON6, OnLiveButton6)
    ON_BN_CLICKED(IDC_LIVE_BUTTON7, OnLiveButton7)
    ON_BN_CLICKED(IDC_LIVE_BUTTON8, OnLiveButton8)
    ON_BN_CLICKED(IDC_LIVE_BUTTON9, OnLiveButton9)
    ON_BN_CLICKED(IDC_LIVE_BUTTON10, OnLiveButton10)
    ON_BN_CLICKED(IDC_LIVE_BUTTON11, OnLiveButton11)
    ON_BN_CLICKED(IDC_LIVE_BUTTON12, OnLiveButton12)
    ON_BN_CLICKED(IDC_LIVE_BUTTON13, OnLiveButton13)
    ON_BN_CLICKED(IDC_LIVE_BUTTON14, OnLiveButton14)
    ON_BN_CLICKED(IDC_LIVE_BUTTON15, OnLiveButton15)
    ON_BN_CLICKED(IDC_LIVE_BUTTON16, OnLiveButton16)
    ON_BN_CLICKED(IDC_LIVE_BUTTON17, OnLiveButton17)
    ON_BN_CLICKED(IDC_LIVE_BUTTON18, OnLiveButton18)
    ON_BN_CLICKED(IDC_LIVE_BUTTON19, OnLiveButton19)
    ON_BN_CLICKED(IDC_LIVE_BUTTON20, OnLiveButton20)
    ON_BN_CLICKED(IDC_LIVE_BUTTON21, OnLiveButton21)
    ON_BN_CLICKED(IDC_LIVE_BUTTON22, OnLiveButton22)
    ON_BN_CLICKED(IDC_LIVE_BUTTON23, OnLiveButton23)
    ON_BN_CLICKED(IDC_LIVE_BUTTON24, OnLiveButton24)
    ON_BN_CLICKED(IDC_LIVE_BUTTON25, OnLiveButton25)
    ON_BN_CLICKED(IDC_LIVE_BUTTON26, OnLiveButton26)
    ON_BN_CLICKED(IDC_LIVE_BUTTON27, OnLiveButton27)
    ON_BN_CLICKED(IDC_LIVE_BUTTON28, OnLiveButton28)
    ON_BN_CLICKED(IDC_LIVE_BUTTON29, OnLiveButton29)
    ON_BN_CLICKED(IDC_LIVE_BUTTON30, OnLiveButton30)
    ON_BN_CLICKED(IDC_LIVE_BUTTON31, OnLiveButton31)
    ON_BN_CLICKED(IDC_LIVE_BUTTON32, OnLiveButton32)
    ON_BN_CLICKED(IDC_LIVE_BUTTON33, OnLiveButton33)
    ON_BN_CLICKED(IDC_LIVE_BUTTON34, OnLiveButton34)
    ON_BN_CLICKED(IDC_LIVE_BUTTON35, OnLiveButton35)
    ON_BN_CLICKED(IDC_LIVE_BUTTON36, OnLiveButton36)
    ON_BN_CLICKED(IDC_LIVE_BUTTON37, OnLiveButton37)
    ON_BN_CLICKED(IDC_LIVE_BUTTON38, OnLiveButton38)
    ON_BN_CLICKED(IDC_LIVE_BUTTON39, OnLiveButton39)
    ON_BN_CLICKED(IDC_LIVE_BUTTON40, OnLiveButton40)
    ON_BN_CLICKED(IDC_LIVE_BUTTON41, OnLiveButton41)
    ON_BN_CLICKED(IDC_LIVE_BUTTON42, OnLiveButton42)
    ON_BN_CLICKED(IDC_LIVE_BUTTON43, OnLiveButton43)
    ON_BN_CLICKED(IDC_LIVE_BUTTON44, OnLiveButton44)
    ON_BN_CLICKED(IDC_LIVE_BUTTON45, OnLiveButton45)
    ON_BN_CLICKED(IDC_LIVE_BUTTON46, OnLiveButton46)
    ON_BN_CLICKED(IDC_LIVE_BUTTON47, OnLiveButton47)
    ON_BN_CLICKED(IDC_LIVE_BUTTON48, OnLiveButton48)
    ON_WM_CLOSE()
    ON_BN_CLICKED(ID_LIVE_CLOSE, OnCloseButton)
    ON_BN_CLICKED(IDC_LIVE_PAGE_1, OnLivePage1)
    ON_BN_CLICKED(IDC_LIVE_PAGE_2, OnLivePage2)
    ON_BN_CLICKED(IDC_LIVE_PAGE_3, OnLivePage3)
    ON_BN_CLICKED(IDC_LIVE_PAGE_4, OnLivePage4)
    ON_BN_CLICKED(IDC_LIVE_PAGE_5, OnLivePage5)
    ON_BN_CLICKED(IDC_LIVE_PAGE_6, OnLivePage6)
    ON_BN_CLICKED(IDC_LIVE_PAGE_7, OnLivePage7)
    ON_BN_CLICKED(IDC_LIVE_PAGE_8, OnLivePage8)
    ON_BN_CLICKED(IDC_LIVE_PAGE_9, OnLivePage9)
    ON_BN_CLICKED(IDC_LIVE_PAGE_10, OnLivePage10)
    ON_BN_CLICKED(IDC_LIVE_PAGE_11, OnLivePage11)
    ON_BN_CLICKED(IDC_LIVE_PAGE_12, OnLivePage12)
    ON_WM_NCRBUTTONDOWN()
    ON_WM_VSCROLL()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////
//
// CLiveControl::ReadCLVFile()
//
////////////////////////////////////////////////////////////////////
M_UINT16
CLiveControl::ReadCLVFile(const char* fileName)
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::ReadCLVFile()" << endl;
#endif
    
    FILE* clvFilePtr;
    fopen_s(&clvFilePtr, fileName, "rb");

    M_UINT16 buttonNumber;
    M_UINT16 nextAvailableButton = 1;
    char scriptFileName[300];
    char description[MAX_DESCRIPTION_LENGTH];

    if (!clvFilePtr)
    {
#ifdef LIVECONTROL_LOG_ERROR
    logFile << "ReadCLVFile(): Unable to open file "
            << fileName << endl;
#endif 
        return M_FAILURE;
    }

    if (!fscanf(clvFilePtr,"%s", &description))
    {
#ifdef LIVECONTROL_LOG_ERROR
    logFile << "ReadCLVFile(): Unable to read header for " 
            << fileName << endl;
#endif 
        return M_FAILURE;
    }

    if (strcmp(description, "LFI_PLAYER_LIVE_CONTROL") != 0 &&
        strcmp(description, "LAZERUS_3D_LIVE_CONTROL") != 0)
    {
#ifdef LIVECONTROL_LOG_ERROR
    logFile << "ReadCLVFile(): Invalid header info for " 
            << fileName << endl;
#endif 
        return M_FAILURE;
    }

    int returnCode;
    char tempString[20];

    returnCode = fscanf(clvFilePtr,"%s", &tempString);

    if (returnCode == 0 || returnCode == EOF)
    {
#ifdef LIVECONTROL_LOG_ERROR
    logFile << "ReadCLVFile(): Error reading info from " 
            << fileName << endl;
#endif 
        return M_FAILURE;
    }

    for (M_UINT16 n=0; n<MAX_LIVE_PAGES; ++n)
        pageButtonEnabledArray[n] = FALSE;
    
    while ((returnCode !=0) && (returnCode != EOF))
    {
        if(strcmp(tempString,"PAGE") == 0)
        {
            returnCode = fscanf(clvFilePtr,"%d", &currentPage);
            if (returnCode == 0)
            {
                break;
            }
            else
            {
                pageButtonArray[currentPage-1].EnableWindow(TRUE);
                pageButtonEnabledArray[currentPage-1] = TRUE;
                nextAvailableButton = 1;
            }
        }
        // ********** Comment Line **********
        else if (strcmp(&tempString[0], "//") == 0)
        {
            char tempChar;

            do
            {
                tempChar = (char)fgetc(clvFilePtr);
            }
            while (tempChar != '\n');
        }
        else if (strlen(&tempString[0]) > 0)
        {
            char tempString2[300];

            buttonNumber = atoi(&tempString[0]);
            returnCode = fscanf(clvFilePtr,"%s %s",
                                  &tempString2,
                                  &description);

            strcpy(scriptFileName, parentPtr->contentDirectoryName);
            strcat(scriptFileName, tempString2);

            if (returnCode == 0)
            {
                break;
            }

#ifdef LIVECONTROL_LOG_INFO
            logFile << "ButtonNumber is: " << buttonNumber << "\n"
                    << "scriptFileName is :" << scriptFileName << "\n"
                    << "desription is: " << description << endl;
#endif 

//            if ((buttonNumber < 1) || 
//			    (buttonNumber > MAX_LIVE_TABLE_ENTRIES))
            if ( buttonNumber > MAX_LIVE_TABLE_ENTRIES )
            {
#ifdef LIVECONTROL_LOG_ERROR
                logFile << "ReadCLVFile(): Invalid button number "
                        << buttonNumber << endl;
#endif
                fclose(clvFilePtr);
                return M_FAILURE;
            }

            if ( buttonNumber == 0)
            {
                buttonNumber = nextAvailableButton;
            }
            
            nextAvailableButton = buttonNumber + 1;

            if ( nextAvailableButton > MAX_LIVE_TABLE_ENTRIES)
            {
                nextAvailableButton = MAX_LIVE_TABLE_ENTRIES;
            }

            livePages[currentPage-1].tableEntries[buttonNumber-1] = new LIVE_TABLE_ENTRY_T;

            if (!livePages[currentPage-1].tableEntries[buttonNumber-1])
            {
#ifdef LIVECONTROL_LOG_ERROR
                logFile << "ReadCLVFile(): Unable to allocate table "
                        << "memory for button " << buttonNumber << endl;
#endif
                fclose(clvFilePtr);
                return M_FAILURE;
            }

            // fopen_s(&(livePages[currentPage-1].tableEntries[buttonNumber-1]->filePointer), scriptFileName, "rb");
            livePages[currentPage - 1].tableEntries[buttonNumber - 1]->filePointer = fopen(scriptFileName, "rb");

            if (!(livePages[currentPage-1].tableEntries[buttonNumber-1]->filePointer))
            {
#ifdef LIVECONTROL_LOG_ERROR
                logFile << "ReadCLVFile(): Unable to open CTS file "
                        << scriptFileName << endl;
#endif
                fclose(clvFilePtr);
                delete livePages[currentPage-1].tableEntries[buttonNumber-1];
                livePages[currentPage-1].tableEntries[buttonNumber-1] = 0;
                return M_FAILURE;
            }

            strcpy((livePages[currentPage-1].tableEntries[buttonNumber-1]->fileDescription), description);

        } // end else next entry is a button number

        returnCode = fscanf(clvFilePtr,"%s",
                            &tempString[0]);

    } // end while

    fclose(clvFilePtr);

    if (returnCode == 0)
    {
#ifdef LIVECONTROL_LOG_ERROR
        logFile << "ReadCLVFile(): Error while reading " 
                << fileName << endl;
#endif
        return M_FAILURE;
    }
    else
    {
        SetPage(1);
        return M_SUCCESS;
    }

}

////////////////////////////////////////////////////////////////////
//
// CLiveControl::EmptyTables()
//
////////////////////////////////////////////////////////////////////
void
CLiveControl::EmptyTables(void)
{
    for (M_UINT16 h=0 ; h<MAX_LIVE_PAGES ; ++h)
    {
        for (M_UINT16 i=0 ; i<MAX_LIVE_TABLE_ENTRIES ; ++i)
        {
            if (livePages[h].tableEntries[i])
            {
                if ((livePages[h].tableEntries[i])->filePointer)
                {
                    fclose((livePages[h].tableEntries[i])->filePointer);
                }
                delete(livePages[h].tableEntries[i]);
                livePages[h].tableEntries[i] = 0;
            }

            liveButtonArray[i].SetState(FALSE);
            liveButtonArray[i].EnableWindow(FALSE);

        }
        
        pageButtonArray[h].SetState(FALSE);
        pageButtonArray[h].EnableWindow(FALSE);
    }

} // end method CLiveControl::EmptyTables

////////////////////////////////////////////////////////////////////
//
// CLiveControl::DoButton() 
//
////////////////////////////////////////////////////////////////////
void
CLiveControl::DoButton(const M_UINT16 buttonNumber)
{
    if(livePages[currentPage-1].tableEntries[buttonNumber-1])
    {
        if (parentPtr->ctsFilePtr == \
                livePages[currentPage-1].tableEntries[buttonNumber-1]->filePointer)
        {
            return;
        }

        liveButtonArray[(livePages[currentPage-1].currentTableEntry)-1].SetState(FALSE);
        liveButtonArray[buttonNumber-1].SetState(TRUE);
        livePages[currentPage-1].currentTableEntry = buttonNumber;
        parentPtr->CleanViewPort();
        for(M_UINT8 s=0 ; s<MAX_TRACKS ; ++s)
        {
            if(parentPtr->outputDeviceArray[s])
            {
                (parentPtr->outputDeviceArray[s])->DisplayOff(parentPtr->invertBlanking, parentPtr->colorMode);
            }
        }

        // Rewind the previous CTS file pointer before loading the current one (clean up)
        if(parentPtr->ctsFilePtr)
        {
            rewind(parentPtr->ctsFilePtr);
        }
        parentPtr->ctsFilePtr = \
            livePages[currentPage-1].tableEntries[buttonNumber-1]->filePointer;
        parentPtr->scriptActive = M_TRUE;
        parentPtr->commandInProgress = M_FALSE;
        parentPtr->firstTime = M_TRUE;
        m_LiveDescription = &(livePages[currentPage-1].tableEntries[buttonNumber-1]->fileDescription[0]);
        queButtonStatus = M_TRUE;

        currentButton = buttonNumber;

        // Set document title 
        CCheap3DDoc* pDoc = parentPtr->GetDocument();
        pDoc->SetTitle(m_LiveDescription);

        UpdateData(FALSE);
    }
}

////////////////////////////////////////////////////////////////////
//
// CLiveControl::SetColorIntensity() 
//
////////////////////////////////////////////////////////////////////
void CLiveControl::SetColorIntensity(UINT intensity)
{
    m_EditLiveColorIntensity = intensity;
    m_SliderLiveColorIntensity.SetPos(MAX_LIVE_COLOR_INTENSITY - intensity);
    parentPtr->colorIntensity = m_EditLiveColorIntensity;

    UpdateData(FALSE);
}

////////////////////////////////////////////////////////////////////
//
// CLiveControl::SetPage() 
//
////////////////////////////////////////////////////////////////////
void CLiveControl::SetPage(M_UINT16 pageNumber)
{
    pageButtonArray[currentPage-1].SetState(FALSE);
    currentPage = pageNumber;
    pageButtonArray[currentPage-1].SetState(TRUE);

    for (M_UINT16 i=0 ; i<MAX_LIVE_TABLE_ENTRIES ; ++i)
    {
        if (livePages[currentPage-1].tableEntries[i])
        {
            liveButtonArray[i].EnableWindow(TRUE);
        }
        else
        {
            liveButtonArray[i].EnableWindow(FALSE);
        }

        liveButtonArray[i].SetState(FALSE);
    }

}


/////////////////////////////////////////////////////////////////////////////
// CLiveControl message handlers

////////////////////////////////////////////////////////////////////
//
// CLiveControl::OnCloseButton()
//
////////////////////////////////////////////////////////////////////
void CLiveControl::OnCloseButton() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnCloseButton()" << endl;
#endif
    parentPtr->AbortScript();
    EmptyTables();
    parentPtr->liveDialog = 0;
    // Set document title 
    CCheap3DDoc* pDoc = parentPtr->GetDocument();
    pDoc->SetTitle("");

    DestroyWindow();
    delete(this);
}


////////////////////////////////////////////////////////////////////
//
// CLiveControl Page Next/Previous V_KEY Handlers
//
////////////////////////////////////////////////////////////////////
void CLiveControl::LivePageNext()
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::LivePageNext()" << endl;
#endif

    M_UINT16 pageIndex = currentPage;
    M_UINT16 tryCount = 0;
    BOOL found = false;

    // if (++pageIndex > MAX_LIVE_PAGES)
    //    pageIndex = 1;

    do
    {
        if (++pageIndex > MAX_LIVE_PAGES)
            pageIndex = 1;

        if (pageButtonEnabledArray[pageIndex-1] == TRUE)
            found = TRUE;

    } while (found == FALSE &&
             tryCount++ < MAX_LIVE_PAGES);

    if (found == TRUE)
        SetPage(pageIndex);

}

void CLiveControl::LivePagePrevious()
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::LivePagePrevious()" << endl;
#endif

    M_UINT16 pageIndex = currentPage;
    M_UINT16 tryCount = 0;
    BOOL found = false;

//    if (--pageIndex == 0)
//	    pageIndex = MAX_LIVE_PAGES;

    do
    {
        if (--pageIndex == 0)
            pageIndex = MAX_LIVE_PAGES;

        if (pageButtonEnabledArray[pageIndex-1] == TRUE)
            found = TRUE;

    } while (found == FALSE &&
             tryCount++ < MAX_LIVE_PAGES);

    if (found == TRUE)
        SetPage(pageIndex);

}


////////////////////////////////////////////////////////////////////
//
// CLiveControl Live Button Handlers
//
////////////////////////////////////////////////////////////////////
void CLiveControl::OnLiveButton1() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton1()" << endl;
#endif
    DoButton(1);
}

void CLiveControl::OnLiveButton2() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton2()" << endl;
#endif
    DoButton(2);
}

void CLiveControl::OnLiveButton3() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton3()" << endl;
#endif
    DoButton(3);	
}

void CLiveControl::OnLiveButton4() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton4()" << endl;
#endif
    DoButton(4);	
}

void CLiveControl::OnLiveButton5() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton5()" << endl;
#endif
    DoButton(5);	
}

void CLiveControl::OnLiveButton6() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton6()" << endl;
#endif
    DoButton(6);	
}

void CLiveControl::OnLiveButton7() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton7()" << endl;
#endif
    DoButton(7);	
}

void CLiveControl::OnLiveButton8() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton8()" << endl;
#endif
    DoButton(8);	
}

void CLiveControl::OnLiveButton9() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton9()" << endl;
#endif
    DoButton(9);	
}

void CLiveControl::OnLiveButton10() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton10()" << endl;
#endif
    DoButton(10);	
}

void CLiveControl::OnLiveButton11() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton11()" << endl;
#endif
    DoButton(11);	
}

void CLiveControl::OnLiveButton12() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton12()" << endl;
#endif
    DoButton(12);	
}

void CLiveControl::OnLiveButton13() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton13()" << endl;
#endif
    DoButton(13);	
}

void CLiveControl::OnLiveButton14() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton14()" << endl;
#endif
    DoButton(14);	
}

void CLiveControl::OnLiveButton15() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton15()" << endl;
#endif
    DoButton(15);	
}

void CLiveControl::OnLiveButton16() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton16()" << endl;
#endif
    DoButton(16);	
}

void CLiveControl::OnLiveButton17() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton17()" << endl;
#endif
    DoButton(17);	
}

void CLiveControl::OnLiveButton18() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton18()" << endl;
#endif
    DoButton(18);	
}

void CLiveControl::OnLiveButton19() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton19()" << endl;
#endif
    DoButton(19);	
}

void CLiveControl::OnLiveButton20() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton20()" << endl;
#endif
    DoButton(20);	
}

void CLiveControl::OnLiveButton21() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton21()" << endl;
#endif
    DoButton(21);	
}

void CLiveControl::OnLiveButton22() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton22()" << endl;
#endif
    DoButton(22);	
}

void CLiveControl::OnLiveButton23() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton23()" << endl;
#endif
    DoButton(23);	
}

void CLiveControl::OnLiveButton24() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton24()" << endl;
#endif
    DoButton(24);	
}

void CLiveControl::OnLiveButton25() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton25()" << endl;
#endif
    DoButton(25);	
}

void CLiveControl::OnLiveButton26() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton26()" << endl;
#endif
    DoButton(26);	
}

void CLiveControl::OnLiveButton27() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton27()" << endl;
#endif
    DoButton(27);	
}

void CLiveControl::OnLiveButton28() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton28()" << endl;
#endif
    DoButton(28);	
}

void CLiveControl::OnLiveButton29() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton29()" << endl;
#endif
    DoButton(29);	
}

void CLiveControl::OnLiveButton30() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton30()" << endl;
#endif
    DoButton(30);	
}

void CLiveControl::OnLiveButton31() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton31()" << endl;
#endif
    DoButton(31);	
}

void CLiveControl::OnLiveButton32() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton32()" << endl;
#endif
    DoButton(32);	
}

void CLiveControl::OnLiveButton33() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton33()" << endl;
#endif
    DoButton(33);	
}

void CLiveControl::OnLiveButton34() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton34()" << endl;
#endif
    DoButton(34);	
}

void CLiveControl::OnLiveButton35() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton35()" << endl;
#endif
    DoButton(35);	
}

void CLiveControl::OnLiveButton36() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton36()" << endl;
#endif
    DoButton(36);	
}

void CLiveControl::OnLiveButton37() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton37()" << endl;
#endif
    DoButton(37);	
}

void CLiveControl::OnLiveButton38() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton38()" << endl;
#endif
    DoButton(38);	
}

void CLiveControl::OnLiveButton39() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton39()" << endl;
#endif
    DoButton(39);	
}

void CLiveControl::OnLiveButton40() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton40()" << endl;
#endif
    DoButton(40);	
}

void CLiveControl::OnLiveButton41() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton41()" << endl;
#endif
    DoButton(41);	
}

void CLiveControl::OnLiveButton42() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton42()" << endl;
#endif
    DoButton(42);	
}

void CLiveControl::OnLiveButton43() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton43()" << endl;
#endif
    DoButton(43);	
}

void CLiveControl::OnLiveButton44() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton44()" << endl;
#endif
    DoButton(44);	
}

void CLiveControl::OnLiveButton45() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton45()" << endl;
#endif
    DoButton(45);	
}

void CLiveControl::OnLiveButton46() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton46()" << endl;
#endif
    DoButton(46);	
}

void CLiveControl::OnLiveButton47() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton47()" << endl;
#endif
    DoButton(47);	
}

void CLiveControl::OnLiveButton48() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnLiveButton48()" << endl;
#endif
    DoButton(48);	
}

void CLiveControl::OnClose() 
{
#ifdef LIVECONTROL_LOG_INFO
    logFile << "Entering CLiveControl::OnClose()" << endl;
#endif
    OnCloseButton();
//	CDialog::OnClose();
}

void CLiveControl::OnLivePage1() 
{
    SetPage(1);
    
}

void CLiveControl::OnLivePage2() 
{
    SetPage(2);
    
}

void CLiveControl::OnLivePage3() 
{
    SetPage(3);
    
}

void CLiveControl::OnLivePage4() 
{
    SetPage(4);
    
}

void CLiveControl::OnLivePage5() 
{
    SetPage(5);
    
}

void CLiveControl::OnLivePage6() 
{
    SetPage(6);
    
}

void CLiveControl::OnLivePage7() 
{
    SetPage(7);
    
}

void CLiveControl::OnLivePage8() 
{
    SetPage(8);
    
}

void CLiveControl::OnLivePage9() 
{
    SetPage(9);
    
}

void CLiveControl::OnLivePage10() 
{
    SetPage(10);
    
}

void CLiveControl::OnLivePage11() 
{
    SetPage(11);
    
}

void CLiveControl::OnLivePage12() 
{
    SetPage(12);
    
}

// void CLiveControl::OnRButtonDown(UINT nFlags, CPoint point)
void CLiveControl::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    logFile << "CLiveControl::OnNcRButtonDown() - Que button " << currentButton << " currently selected."<< endl;
    
//	CDialog::OnRButtonDown(nFlags, point);
    CWnd::OnNcRButtonDown(nHitTest, point);
}

void CLiveControl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    m_EditLiveFadeIn = (MAX_LIVE_FADE_IN_FRAMES + 1) - m_SliderLiveFadeIn.GetPos();
    parentPtr->fadeInFrames = m_EditLiveFadeIn;

    m_EditLiveFadeOut = (MAX_LIVE_FADE_OUT_FRAMES + 1) - m_SliderLiveFadeOut.GetPos();
    parentPtr->fadeOutFrames = m_EditLiveFadeOut;

    m_EditLiveColorIntensity = (MAX_LIVE_COLOR_INTENSITY) - m_SliderLiveColorIntensity.GetPos();
    parentPtr->colorIntensity = m_EditLiveColorIntensity;

    m_EditLiveFrameRepeat = MAX_LIVE_FRAME_REPEAT - m_SliderLiveFrameRepeat.GetPos();

    if(m_EditLiveFrameRepeat >= 0)
    {
        parentPtr->globalFrameRepeat = m_EditLiveFrameRepeat;
    }
    else
    {
        parentPtr->globalFrameRepeat = MAX_UINT_16_VALUE;
    }
    
    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);

    UpdateData(FALSE);

}
