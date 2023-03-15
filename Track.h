#ifndef TRACK_H
#define TRACK_H

//
// *********************************************************
// *                                                       *
// * Copyright 1998-2022, Lazerus Laser Systems            *
// * Author: Michael G. Svob                               *
// *                                                       *
// * File Name: Track.h                                    *
// * Description: This file contains the interface for the *
// *              Track class.  Note that any kind of      *
// *              object which can be stored in the track  *
// *              list inherits from this class.           *
// *                                                       *
// * This code is distributed under the GNU Public License *
// * which can be found at...                              *
// *                                                       *
// * http://www.gnu.org/licenses/gpl.txt                   *
// *                                                       *
// *********************************************************
//

#ifndef GLOBALS_H
#include "globals.h"
#endif

#ifndef GEM_PC_H
#include "Gem_pc.h"
#endif

#ifndef JOBBLOCK_H
#include "jobblock.h"
#endif

#ifndef PALETTE_H
#include "Palette.h"
#endif

#ifndef POINTBUFF_H
#include "PointBuff.h"
#endif

const M_UINT16 DISPLAY_OFF = 0x0000;
const M_UINT16 DISPLAY_ON  = 0x0001;
const M_UINT16 POINTS_ONLY = 0x0010;

const enum DisplayMode{NO_DISPLAY          = 0,
                       NORMAL_DISPLAY      = 1,
                       POINTS_ONLY_DISPLAY = 2};

const enum RotationAxis{X_AXIS, Y_AXIS, Z_AXIS};

const enum ZoomDirection{ZOOM_IN, ZOOM_OUT};

const enum FadeDirection{FADE_IN, FADE_OUT};

const enum ScriptMode{SINGLE = 0, LOOP = 1};

typedef struct DisplayParams
{
    M_INT32        delayValue;
    M_UINT16       blankingDelay;
    M_UINT16       displayMode;
    M_UINT16       displayReverse;
    M_UINT16       displayBeamView;
    double        colorFactor;
    double        redLevel;
    double        greenLevel;
    double        blueLevel;
    M_UINT16       fadeFramesIn;
    M_UINT16       fadeFramesOut;
    M_INT32        frameRepeat;
    M_UINT16       repeatCount;
    M_UINT16       trackScale;
    M_INT32        startFrameNumber;
    M_UINT16       stopFrameNumber;
    M_UINT16       currentFrameNumber;
    M_UINT16       zAxisGain;
    M_UINT16       zAxisOffset;
    M_UINT16       zAxisFormula;
    double        zAxisPeriod;
    PALETTE_PTR_T palettePtr;
}DisplayParams;

class Track
{
public:
//    virtual M_UINT16 Ok(void) const {return(okFlag);}
    virtual M_UINT16 Ok(void){return M_FALSE;}
    virtual M_UINT16 SetPattern(const M_UINT16 newPattern){return M_FALSE;}

    typedef enum
    {
        INVALID  = 0,
        ILDA     = 1,
        ABSTRACT = 2,
        TEXT     = 3
    }TrackType;

    Track();
    ~Track() {return;}

    TrackType GetTrackType()
    {
        return trackType;
    }
    void SetTrackType(const TrackType newTrackType)
    {
        trackType = newTrackType;
    }
    ViewType GetTrackView()
    {
        return trackView;
    }
    void SetTrackView(ViewType newView)
    {
        trackView = newView;
    }
    DisplayParams GetTrackParams()
    {
        return trackParams;
    }
    void SetTrackParams(DisplayParams newParams)
    {
        trackParams = newParams;
    }
    void SetTrackDevice(M_UINT8 newDevice)
    {
        outputDevice = newDevice;
    }
    M_UINT8 GetTrackDevice(void)
    {
        return outputDevice;
    }
    void AddBufferPoint(OutputPoint_T newBuffPoint)
    {
        trackPointBuff.AddPoint(newBuffPoint);        
    }
    OutputPoint_T GetBufferPoint(M_UINT8 offset)
    {
        return trackPointBuff.GetPoint(offset);
    }

    M_UINT16 GetNumberOfPoints();

    JobBlock    jobBlock;
    JobBlock    saveJobBlock;
    M_UINT16      ZAxisGain;
    M_UINT16      ZAxisOffset;
    M_UINT16      ZAxisFormula;
    double       ZAxisPeriod; 
    double      IncrementValue;

protected:
    M_UINT16  okFlag;

private:
    TrackType      trackType;
    ViewType       trackView;
    DisplayParams  trackParams;
    PointBuff      trackPointBuff;
    M_UINT8        outputDevice;
};

#endif
