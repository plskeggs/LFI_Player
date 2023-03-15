//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: TrackLst.cpp                               *
// * Description: Implementation of the TrackList          *
// * class.                                                *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#include "stdafx.h"

#ifndef TRACKLIST_H
#include "tracklst.h"
#endif

#ifndef TRACK_H
#include "track.h"
#endif

#ifndef GLOBALS_H
#include "globals.h"
#endif

#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std; 

extern ofstream logFile;

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: TrackList::TrackList
//
// Input Parameters:
//  None
//
// Returns: N/A
//
// Description:
// This is the constructor for the TrackList class.  It will initialize
// all of the Track pointers in the list to NULL, and zero the length
// and current track member attributes.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

TrackList::TrackList():
    currentTrack(FIRST_TRACK),
    numOfTracks(0),
    okFlag(M_TRUE)
{

    // The zero element of the array is not used, but we'll
    // initialize it first pass anyway just to be safe.
    for (M_UINT16 i=0 ; i<=MAX_TRACKS ; ++i)
    {
        trackList[i] = 0;
    }
    return;
}

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: TrackList::~TrackList
//
// Input Parameters:
//  None
//
// Returns: N/A
//
// Description:
// This is the destructor for the TrackList class.  It will delete all
// the tracks in the list, and zero the length and current track
// member attributes by calling DeleteTrackList().
//
///////////////////////////PROLOGUE_END/////////////////////////////////

TrackList::~TrackList()
{
    DeleteTrackList();
}

/////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: TrackList::AddTrack
//
// Input Parameters:
//  Track* trackPtr  - pointer to track to be added to the track list
//
// Returns: M_UINT16
//
// Description:
// This is function will add the track, passed in as an argument, to
// the track list.  It will return M_SUCCESS if the the track was
// successfully added, and M_FAILURE otherwise.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

M_UINT16
TrackList::AddTrack(Track* trackPtr)
{
    M_UINT16 nextTrackNumber;

    if (numOfTracks >= MAX_TRACKS)
    {
#ifdef TRACK_LOG_ERROR
        logFile << "AddTrack(): Track List already full." << endl;
#endif
        okFlag = M_FALSE;
        return(M_FAILURE);
    }
    else
    {
        nextTrackNumber = numOfTracks+1;
    }

    if (trackList[nextTrackNumber])
    {
        // Track is currently occupied.  Give the new track the same
        // view and parameters as the existing track before blowing
        // the existing track away.
        trackPtr->SetTrackParams((trackList[nextTrackNumber])->GetTrackParams());
        trackPtr->SetTrackView((trackList[nextTrackNumber])->GetTrackView());

        if ((trackList[nextTrackNumber])->GetTrackType() == Track::ILDA)
        {
            delete (FrameSequence*)(trackList[nextTrackNumber]);
        }
        else
        {
            delete (Abstract*)(trackList[nextTrackNumber]);
        }

        trackList[nextTrackNumber] = 0;
    }

    trackList[nextTrackNumber] = trackPtr;
    currentTrack = nextTrackNumber;
    ++numOfTracks;

    return(M_SUCCESS);

} // end TrackList::AddTrack

/////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: TrackList::DeleteTrack
//
// Input Parameters:
//  Track* theTrack   - pointer to the track in the list to be
//                      deleted
//
// Returns: M_UINT16
//
// Description:
// This is function will delete the track, passed in as a pointer,
// from the track list.  It will return M_SUCCESS if the the track was
// successfully deleted, and M_FAILURE otherwise.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

M_UINT16
TrackList::DeleteTrack(Track* theTrack)
{
    if(numOfTracks == 0) // list is already empty
    {
#ifdef TRACK_LOG_ERROR
        logFile << "DeleteTrack(): Track List is empty, "
                << "cannot delete track." << endl;
#endif
        return(M_FAILURE);
    }

    M_UINT16 i = FIRST_TRACK;
    M_UINT16 found = M_FALSE;

    while (!found && i <= numOfTracks)
    {
        if (theTrack == trackList[i])
        {
            found = M_TRUE;
        }
        else
        {
            ++i;
        }
    }

    if (found)
    {
        switch ((trackList[i])->GetTrackType())
        {
        case Track::ILDA:
            delete (FrameSequence*)(trackList[i]);
            break;

        case Track::ABSTRACT:
            delete (Abstract*)(trackList[i]);
            break;

        case Track::TEXT:
            delete (TextScroll*)(trackList[i]);
            break;

        default:
            break;
        }

        trackList[i] = 0;
        M_UINT16 j=0;
        for (j=i+1 ; j<numOfTracks ; j++)
        {
            trackList[j-1] = trackList[j];
        }

        trackList[j-1] = 0;
        --numOfTracks;
    }
    else
    {
#ifdef TRACK_LOG_ERROR
        logFile << "DeleteTrack(): Track not found; unable to delete." 
                << endl;
#endif
        return(M_FAILURE);
    }

    return(M_SUCCESS);

} // end TrackList::DeleteTrack

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: TrackList::GetNextTrack
//
// Input Parameters:
//  N/A
//
// Returns: Track*
//
// Description:
//  This function returns the next frame (pointer) in the list and
//  increments the currentPtr accordingly.  Returns NULL pointer if
//  iterator already at end of list.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

Track* 
TrackList::GetNextTrack() 
{
    if (++currentTrack > MAX_TRACKS)
    {
#ifdef TRACK_LOG_ERROR
        logFile << "GetNextTrack(): Can't get track past end of list." 
                << endl;
#endif
        currentTrack = MAX_TRACKS;
        return 0;
    }
    else
    {
        return trackList[currentTrack];
    }

} // end TrackList::GetNextTrack

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Track::GetTrack
//
// Input Parameters:
//  const M_UINT16 trackIndex  - Index of the track to be retrieved from
//                             the list.
//
// Returns: Track*
//
// Description:
//  This function returns the track indicated by the index, and leaves
// the value of currentTrack unchanged.  Returns NULL track pointer
//  if invalid index or track not found.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

Track* 
TrackList::GetTrack(const M_UINT16 trackIndex) 
{
    // unsigned value so only check greater or equal
    if (trackIndex == 0 || trackIndex > MAX_TRACKS)
    {
#ifdef TRACK_LOG_ERROR
        logFile << "GetTrack(): Invalid track index specified." 
                << endl;
#endif
        return 0;
    }

    return trackList[trackIndex];

} // end TrackList::GetTrack

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: Track::SetTrack
//
// Input Parameters:
//  const M_UINT16 trackIndex  - Index of the track to be retrieved from
//                             the list.
//  Track* newTrack          - The track to be placed in the list.
//
// Returns: M_UINT16
//
// Description:
//  This function adds the track passed in to the location indicated
//  by the index.  Returns M_SUCCESS if added okay, and M_FAILURE if
//  the index was invalid.  This function does not affect the value
//  of currentTrack.  And since numOfTracks is sort of a high water
//  mark which is only reduced by DeleteTrack() or DeleteTrackList(),
//  it doesn't get modified either.  It also leaves the track alone
//  if the old and new track pointers are the same.
//
//  Note that because it cleans up before setting the new track, this
//  method can be used to "remove" a track without changing the rest
//  of the list by simply passing in a NULL pointer for the appropriate
//  track number.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

M_UINT16 
TrackList::SetTrack(const M_UINT16 trackIndex, Track* newTrack) 
{
    if (trackIndex == 0 || trackIndex > MAX_TRACKS)
    {
#ifdef TRACK_LOG_ERROR
        logFile << "SetTrack(): Invalid track index specified." 
                << endl;
        return M_FAILURE;
#endif
    }

    if (trackList[trackIndex])
    {
        if (trackList[trackIndex] == newTrack)
        {
            return M_SUCCESS;
        }
        else
        {
            if (newTrack)
            {
                newTrack->SetTrackParams((trackList[trackIndex])->GetTrackParams());
                newTrack->SetTrackView((trackList[trackIndex])->GetTrackView());
            }

            if ((trackList[trackIndex])->GetTrackType() == Track::ILDA)
            {
                delete (FrameSequence*)(trackList[trackIndex]);
            }
            else
            {
                delete (Abstract*)(trackList[trackIndex]);
            }
        }
    }

    trackList[trackIndex] = newTrack;
    return M_SUCCESS;

} // end TrackList::SetTrack

///////////////////////////PROLOGUE_START/////////////////////////////////
//
// Copyright Lazerus Laser Systems 2000
//
// Function: TrackList::DeleteTrackList
//
// Input Parameters:
//  None
//
// Returns: void
//
// Description:
//  This function deletes all of the tracks in the list and re-
//  initializes the number of tracks and current track to zero.
//
///////////////////////////PROLOGUE_END/////////////////////////////////

void
TrackList::DeleteTrackList()
{
    if (numOfTracks && !trackList[FIRST_TRACK]) // list empty but index is 
    {								            // non-zero.
#ifdef TRACKLIST_LOG_ERROR
        logFile << "DeleteTrackList(): Track List is empty, but "
                << "number of entries is not zero." << endl;
#endif
    }
    else // list is not empty
    {
        for (M_UINT16 i=FIRST_TRACK ; i<=MAX_TRACKS ; ++i)
        {
            if (trackList[i])
            {
                if ((trackList[i])->GetTrackType() == Track::ILDA)
                {
                    delete (FrameSequence*)(trackList[i]);
                }
                else
                {
                    delete (Abstract*)(trackList[i]);
                }

                trackList[i] = 0;
            }
        }

        currentTrack = 1;
        numOfTracks = 0;
    }

   return;

} // end TrackList::DeleteTrackList
