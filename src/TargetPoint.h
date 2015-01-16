//---------------------------------------------------------------------------
//File: TargetPoint.h
//Content: targetpoint attached to a RP
//Last update: 06/05/2014
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef TARGETPOINT_H
#define TARGETPOINT_H

#include "FiberMOSModel2.h"

//---------------------------------------------------------------------------

using namespace Lists;
using namespace Models;

//spacename for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//TTargetPoint:
//---------------------------------------------------------------------------

//class target point
class TTargetPoint {
        //STATIC PROPERTIES:

        //list of pointers to built target points
        static TItemsList<TTargetPoint*> Builts;

        //Allows to control the construction of asingletarget point by RP,
        //and the destruction of existing target point.

        //DYNAMIC PROPERTIES:

        TRoboticPositioner *__RoboticPositioner;

        //INTERFACE PROPERTIES:

public:
        //DYNAMIC PROPERTIES:

        //pointer to the attached RP
        TRoboticPositioner *getRoboticPositioner(void) const {return __RoboticPositioner;}

        //point where sall be positioned the point P3 of the attached RP
        TDoublePoint TargetP3;

        //GRAPHICAL PROPERTIES:

        //indicates if the target point is attached
        //default value: false
        bool Selected;

        //color of thetarget point
        //defaultcolor: clYellow
        //#QColor Color;

        //PROPERTIES IN TEXTFORMAT:

        //TargetP3 in text format
        AnsiString getTargetP3Text(void);
        void setTargetP3Text(const AnsiString&);

        //SETS OF PROPERTIES IN TEXT FORMAT:

        //target point in text format
        AnsiString getText(void); void setText(const AnsiString&);

        //------------------------------------------------------------------
        //STATIC METHODS:

        //compare the identifiers of the RPs attached to two target points
        static int  CompareIds(TTargetPoint *TPA1,
                TTargetPoint *TPA2);
        //this method shall be pointed in a pointer list
        //to allow the functioning of shorting and comparing methods

        //get the labels of the properties
        //in row text format
        static AnsiString GetIdTargetP3LabelsRow(void);

        //travel the labels of the properties
        //in a text string from the position i
        static void  TravelLabels(const AnsiString& S, int& i);

        //read the values of the properties
        //in a text string from the position i
        static void  ReadSeparated(int& Id, double& x, double& y, const AnsiString& S, int& i);

        //BUILDING AND DESTROYING METHODS:

        //build a target point attached a RP
        //if the RP already has an attached target point
        //  throw an exception EImproperArgument
        TTargetPoint(TRoboticPositioner *_RoboticPositioner,
                double x, double y);
        TTargetPoint(TRoboticPositioner *_RoboticPositioner,
                TDoublePoint _TargetP3);

        //destroy a targetpoint
        //if thereisn't a built target point
        //  throw an exception EImproperCall
        ~TTargetPoint();

        //CHECKING METHODS:

        //determines if the target point is out of the domain
        //of thepoint P3 of the attached RP
        bool IsOutDomainP3(void);
        //determines if the target point is in the secure area
        //of thepoint P3 of the attached RP
        bool IsInSafeAreaP3(void);

        //MOVING METHODS:

        //assign the point P3o of the attacheed RP to the point TargetP3
        void SetP3o(void) {TargetP3 = getRoboticPositioner()->getActuator()->getP3o();}
        //assign the point P3 of the attacheed RP to the point TargetP3
        void SetP3(void) {TargetP3 = getRoboticPositioner()->getActuator()->getArm()->getP3();}
        //randomize the point TargetP3 with uniform distribution
        //in the domain of the point P3 of its attached RP
        void Randomize(void);

        //There are three ways to randomize a point in the domain of a RP:
        //  - Randomizing the angular positions of their rotors in their
        //    respective domains;
        //  - Randomizing the point in polar coordinates respect S1 with
        //    uniform distribution in the radial interval. Iterating the
        //    process until the point is in the domain of the RP.
        //  - Randomizing the point in Cartesian coordinates respect S0,
        //    in the Cartesian insterval of the domain of the RP, until
        //    thepoint is inthedomain of the RP.
        //Themethod Ramize, implement the las way.

        //assign the point TargetP3 to the point P3 of its attached RP
        //and return the distance from the stable position to the target point
        //if the the point TargetP3 isn't on the domain of its attached RP:
        //  throw an exception EImpropercall
        double MoveToTargetP3(void);

        //NOTE: method MoveToTargetP3 will move the rotors of the RP to the
        //positions corresponding to the P3 is positioned on the TargetP3.
        //When the quantification of the rotors is enabled, will be quantified
        //first the rotor 1, and after the rotor 2, in any case, uncheking
        //which is the neares stable point.

        //GRAPHICAL METHODS:

        //indicates the part of the target point which can be grabbed
        //on the givenpoint
        //      1: point which is a circunference (TargetP3, 0.75)
        //      0: none;
        int Grab(TDoublePoint P);

        //print the target point in the canvas of the picture
        //of a ploter shapes
        //#void Paint(TPloterShapes *PS);
};

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
#endif // TARGETPOINT_H
