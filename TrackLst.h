#ifndef TRACKLST_H
#define TRACKLST_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: TrackLst.h                                 *
// * Description: This file contains the interface for the *
// *              TrackList class.                         *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#ifndef TRACK_H
#include "track.h"
#endif

#ifndef GLOBALS_H
#include "globals.h"
#endif

#ifndef FRAMESEQUENCE_H
#include "frameseq.h"
#endif

#ifndef ABSTRACT_H
#include "abstract.h"
#endif

#ifndef TEXTSCROLL_H
#include "textscroll.h"
#endif

const M_UINT16 MAX_TRACKS = 4;
const M_UINT16 FIRST_TRACK = 1;

class TrackList
{
public:
    short Ok(void) const {return(okFlag);}

    TrackList();
    ~TrackList();

    // accessor methods
    Track* GetTrack(const M_UINT16 trackIndex);
	M_UINT16 SetTrack(const M_UINT16 trackIndex, Track* newTrack);

    void   ResetIterator() {currentTrack = FIRST_TRACK;}
	Track* GetNextTrack();
	Track* GetCurrentTrack() {return trackList[currentTrack];}
	M_UINT16 GetNumberOfTracks() {return numOfTracks;}
	M_UINT16 GetCurrentTrackNumber() {return currentTrack;}

    M_UINT16 AddTrack(Track*);
    M_UINT16 DeleteTrack(Track*);
	void   DeleteTrackList();
	M_UINT16 NumPointsArray[MAX_TRACKS+1];

//    void   PrintTrackList(ostream&, const M_UINT16);

private:
    Track*  trackList[MAX_TRACKS+1];
    M_UINT16	numOfTracks;
    M_UINT16  currentTrack;

    short   okFlag;
};

#endif
