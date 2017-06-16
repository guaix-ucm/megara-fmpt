// Copyright (c) 2012-2017 Isaac Morales Durán. All rights reserved.
// Institute of Astrophysics of Andalusia, IAA-CSIC
//
// This file is part of FMPT (Fiber MOS Positioning Tools)
//
// FMPT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//---------------------------------------------------------------------------
/// @file FMOSA.h
/// @brief structure FMOSA
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FMOSA_H
#define FMOSA_H

#include "AllocationList.h"
#include "FiberMOSModel.h"
#include "MotionProgram.h"
#include "PointersList.h"

//#include <jsoncpp/json/json.h>

//---------------------------------------------------------------------------

//namespace for models
namespace Models {

//---------------------------------------------------------------------------
//class TObservingSource:

class TObservingSource {
public:
    //SP properties:
    string Name;        ///< name ("")                 (can be empty)
    double RA;          ///< rect ascension (0)
    double Dec;         ///< declination (0)
    double Mag;         ///< magnitude (0)             (can be empty)
    TPointType Type;    ///< type (ptUNKNOWN)

    //Allocation properties
    unsigned int Pr;    ///< allocation priority (0)   (can be empty)
    unsigned int Bid;   ///< Id of the CB (0)          (can be empty)
    unsigned int Pid;   ///< Id of the RP (0)

    //Projection point properties:
    double X;           ///< abcise (0)
    double Y;           ///< ordinate (0)
    double Angle;       ///< angle of the microlens (0)
    bool Enabled;       ///< indicates if the point is allocated to the RP (false)

    //Allocation properties:
    //bool notAllocated;  //indicates if the point is not allocated in other CB
    //                    //(true)    (could be empty)
    //bool allocateInAll; //indicates if the point must be allocated in all CBs
    //                    //(false)   (could be empty)
    string Comment;     ///< coment ("")               (can be empty)

    //Used to generate a pair (PP, DP):
    //  Type, [Bid], Pid, X, Y, Enabled

    //If field Bid is not empty, all fields (except Name and Comment)
    //shall be filled.
    //If field Bid is not empty, the OS correspond to a projection point.
    //If field Enabled is true, the projection point is allocated to the RP.

    //Used to determine if an allocation is of must type
    //when attempt regenerate a pair (PP, DP):
    //  Type, [Pr], [Bid], Pid, X, Y, Enabled, [notAllocated], [allocateInAll]

    //flags indicating fields which can be empty (except Name and Comment):
    //default value: false
    bool there_is_Mag;  ///< field Mag is not empty
    bool there_is_Pr;   ///< field Pr is not empty
    bool there_is_Bid;  ///< field Bid is not empty
    //bool there_is_notAllocated;
    //bool there_is_allocateInAll;

    //SETS OF PROPERTIES IN TEXT FORMAT:

    /// get the structure in text format
    AnsiString getRowText(void) const;

    /// get the structure in JSON format
    Json::Value getJSON(void) const;

    /// set the structure in text format
    void setText(const string& str);

    /// @brief print the properties of an OS in a string
    /// in row format
    static void  printRow(AnsiString &S, const TObservingSource *OS);

    //PUBLIC METHODS:

    /// build a structure by default
    TObservingSource(void);

    /// copy all properties of an object of the same type
    TObservingSource& operator=(const TObservingSource&);

    /// build a clon
    TObservingSource(TObservingSource *OS);

    /// compare all properties of an object of the same type
    bool operator!=(const TObservingSource&);
};

//---------------------------------------------------------------------------
//class TFMOSA:

class TFMOSA : public TPointersList<TObservingSource> {
    /// read the OB section in tampon variables
    void readOBText(int& _Id, double& _Ra, double& _Dec, double& _Pos,
                    const string& str);

public:
    /// @brief last valid setted FMOSA in text format including comments
    /// - default value: ""
    string str_original;

    /// comments before the OB
    TStrings comments;

    //OB properties
    unsigned int Id;    ///< the block identification
    double Ra;          ///< the rect ascension of the block
    double Dec;         ///< the declination of the block
    double Pos;         ///< position angle

    /// set a FMOSA in text format
    void setTableText(unsigned int& Bid, const string& str);

    /// get the FMOSA in text format
    void getTableText(string& str) const;

    /// get the FMOSA in format JSON
    Json::Value getJSON(void) const;

    /// @brief get the Pids of the OSs which accomplish:
    /// - there_is_Bid
    /// - Pid is not found in the FMM
    void searchMissingPids(TVector<int>& Pids,
                           const TRoboticPositionerList2 *RPL);

    /// @brief get the Pids of the OSs which accomplish:
    /// - there_is_Bid
    /// - Enabled don't match with Disbaled in the FMM
    /// @brief If there are missing Pids:
    /// - throw EImproperCall
    void searchDontMatchEnabled(TVector<int>& Pids,
                                const TRoboticPositionerList2 *RPL);

    /// @brief get the allocations which accomplish: there_is_Bid && Enabled
    /// @pre there aren't missing Pids;
    /// @pre there aren't don't match Enabled.
    /// @exception EImproperCall: If not meet the pre-conditions
    /// @brief Allocations to not operative (although enabled) RPs, can be got,
    /// but them motion programs can will be generated only by force.
    void getAllocations(TAllocationList& AL);

    /// build a FMOSA by default
    TFMOSA(void);

    /// clone a FMOSA
    void Clone(TFMOSA&);

    /// copy a FMOSA
    TFMOSA& operator=(const TFMOSA&);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

#endif // FMOSA_H
