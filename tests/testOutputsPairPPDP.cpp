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
//File: TestOutputsPairPPDP.cpp
//Content: test for the module Outputs
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testOutputsPairPPDP.h"
#include "../src/OutputsPairPPDP.h"
//#include "cppunit_assert_emulator.h" //Qt only.

#include <iostream>
#include <limits.h> //INT_MAX
#include <float.h> //DBL_MAX

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestOutputsPairPPDP);

//overide setUp(), init data, etc
void TestOutputsPairPPDP::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestOutputsPairPPDP::tearDown() {
}

//---------------------------------------------------------------------------

void TestOutputsPairPPDP::test_()
{
/*    //build a value for an output structure in text format
    string str;                                     //0     0       ""
    str += "#Positioning program\r\n";              //22    22      "#Positioning program\r\n"          22
    str += "@@SPP@@\r\n";                           //9     31      "...@@SPP@" / "...@@SPP@@" / "...@@SPP@@\r\n"      28 / 29 / 31
    str += "pos_0 {\r\n";                           //9     40
    str += "	group_1 {\r\n";                     //12    52
    str += "	rp1 r1 13838\r\n";                  //15    67
    str += "	rp1 r2 0\r\n";                      //11    78
    str += "	}\r\n";                             //4     82
    str += "	group_2 {\r\n";                     //12    94
    str += "	rp1 r1 14692\r\n";                  //15    109
    str += "	rp1 r2 269\r\n";                    //13    122
    str += "	}\r\n";                             //4     126
    str += "}\r\n";                                 //3     129
    str += "@@EPP@@\r\n";                           //9     138     "...@@EPP@" / "...@@EPP@@" / "...@@EPP@@\r\n"      135 / 136 / 138

    str += "#Depositioning program\r\n";            //24    162     "#Depositioning program\r\n"        162
    str += "@@SDP@@\r\n";                           //9     171      "...@@SDP@" / "...@@SDP@@" / "...@@SDP@@\r\n"     168 / 169 / 171
    str += "depos_0 {\r\n";                         //11    182
    str += "	group_1 {\r\n";                     //12    194
    str += "	rp1 r1 13838\r\n";                  //15    209
    str += "	rp1 r2 0\r\n";                      //11    220
    str += "	}\r\n";                             //4     224
    str += "	group_2 {\r\n";                     //12    236
    str += "	rp1 r1 0\r\n";                      //11    247
    str += "	rp1 r2 0\r\n";                      //11    258
    str += "	}\r\n";                             //4     262
    str += "}\r\n";                                 //3     265
    str += "@@EDP@@\r\n";                           //9     274     "...@@EDP@" / "...@@EDP@@" / "...@@EDP@@\r\n"      271 / 272 / 274

    str += "# Id| Ra| Dec| Pos\r\n";                //20    294     "# Id| Ra| Dec| Pos\r\n"            294
    str += "@@SOB@@\r\n";                           //9     303     "...@@SOB@" / "...@@SOB@@" / "...@@SOB@@\r\n"      300 / 301 / 303
    str += "0| 15.027879| 45.000422| 0.060858\r\n"; //35    338     "0| 15.027879| 45.000422" / "0| 15.027879| 45.000422| 0.060858\r\n"    326 / 338
    str += "@@EOB@@\r\n";                           //9     347     "...@@SOB@" / "...@@SOB@@" / "...@@SOB@@\r\n"      344 / 345 / 347

    str += "#      Name             RA         Dec    Mag        Type         Pr  Bid Pid   X(mm)     Y(mm)  Enabled      Comment\r\n";     //119   466     "...Enabled      Comment\r\n"                   466
    str += "@@SOS@@\r\n";                                                                                                                   //9     475     "...@@SOS@" / "...@@SOS@@" / "...@@SOS@@\r\n"   472 / 473 / 475
    str += "s:751               |15.027787|45.017925|20.67|SOURCE          |6 |0  |1  |-0.137909 |51.932865 |1|foo comment         \r\n";   //121   596
    str += "                    |15.066176|45.029778|     |UNKNOWN         |  |   |97 |80.400000 |87.035553 |1|                    \r\n";   //121   717     "...                     \r\n"                  717
    str += "@@EOS@@";                                                                                                                       //9     726     "...@@EOS@" / "...@@EOS@@" / "...@@EOS@@\r\n"   723 / 724 / 726

    for(int i=0; i<100; i++) {
        //build a outputs structure and set the value in text format
        Outputs *outputs = new Outputs();
        unsigned int Bid;
        outputs->setText(Bid, str);

        //check the Bid
        if(Bid != 0) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //build the corresponding initial position list
        TPairPositionAnglesList IPL;
        TRoboticPositioner RP1(1, TDoublePoint(0, 0));
        IPL.Add(new TPairPositionAngles(&RP1));

        //build the corresponding observing position list
        TPairPositionAnglesList OPL;
        OPL.Add(new TPairPositionAngles(&RP1));
        OPL[0].setPPA(14692, 269);

        //get the reconstructed outputs in text format
        string str1;
        outputs->getOutputsText(str1, Bid, OPL, IPL, false);

        //check the reconstructed string
        if(str1 != str) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //-------------------------------------------------------------------
        //TRY SET SUBSTRINGS INCLUDING PP:

        //""                                        0
        try {
            outputs->setText(Bid, str.substr(0, 0));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: label @@SPP@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"#Positioning program\r\n"                22
        try {
            outputs->setText(Bid, str.substr(0, 22));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: label @@SPP@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@SPP@"                               28
        try {
            outputs->setText(Bid, str.substr(0, 28));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: label @@SPP@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@SPP@@"                               29
        try {
            outputs->setText(Bid, str.substr(0, 29));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: should to have a positioning program") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@SPP@@\r\n"                          31
        try {
            outputs->setText(Bid, str.substr(0, 31));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: label @@EPP@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@EPP@"                               135
        try {
            outputs->setText(Bid, str.substr(0, 135));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: label @@EPP@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@EPP@@"                               136
        try {
            outputs->setText(Bid, str.substr(0, 136));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: should to have a depositioning program") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@EPP@@\r\n"                          138
        try {
            outputs->setText(Bid, str.substr(0, 138));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: label @@SDP@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //-------------------------------------------------------------------
        //TRY SET SUBSTRINGS INCLUDING PP AND DP:

        //"#Depositioning program\r\n"              162
        try {
            outputs->setText(Bid, str.substr(0, 162));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: label @@SDP@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@SDP@"                               168
        try {
            outputs->setText(Bid, str.substr(0, 168));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: label @@SDP@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@SDP@@"                               169
        try {
            outputs->setText(Bid, str.substr(0, 169));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: should to have a depositioning program") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@SDP@@\r\n"                          171
        try {
            outputs->setText(Bid, str.substr(0, 171));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: label @@EDP@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@EDP@"                               271
        try {
            outputs->setText(Bid, str.substr(0, 271));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: label @@EDP@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@EDP@@"                          272
        try {
            outputs->setText(Bid, str.substr(0, 272));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: should to have a FMOSA table") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@EDP@@\r\n"                          274
        try {
            outputs->setText(Bid, str.substr(0, 274));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: setting FMOSA table in text format in row 2: label @@SOB@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //-------------------------------------------------------------------
        //SETTING FMOSA TABLE IN TEXT FORMAT:

        //"# Id| Ra| Dec| Pos\r\n"                  294
        try {
            outputs->setText(Bid, str.substr(0, 294));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: setting FMOSA table in text format in row 3: label @@SOB@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@SOB@"                               300
        try {
            outputs->setText(Bid, str.substr(0, 300));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: setting FMOSA table in text format in row 2: label @@SOB@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@SOB@@"                               301
        try {
            outputs->setText(Bid, str.substr(0, 301));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: setting FMOSA table in text format in row 3: OB parameters not found: <Id> | <Ra> | <Dec> | <Pos>") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@SOB@@\r\n"                          303
        try {
            outputs->setText(Bid, str.substr(0, 303));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: setting FMOSA table in text format in row 4: OB parameters not found: <Id> | <Ra> | <Dec> | <Pos>") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"0| 15.027879| 45.000422"                 326
        try {
            outputs->setText(Bid, str.substr(0, 326));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: setting FMOSA table in text format in row 3: reading OB section: should have 4 fields separated by '|': \"<Id> | <Ra> | <Dec> | <Pos>\"") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"0| 15.027879| 45.000422| 0.060858\r\n"   338
        try {
            outputs->setText(Bid, str.substr(0, 338));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: setting FMOSA table in text format in row 5: label @@EOB@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@SOB@"                               344
        try {
            outputs->setText(Bid, str.substr(0, 344));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: setting FMOSA table in text format in row 4: label @@EOB@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@SOB@@"                               345
        try {
            outputs->setText(Bid, str.substr(0, 345));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: setting FMOSA table in text format in row 5: label @@SOS@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@SOB@@\r\n"                          347
        try {
            outputs->setText(Bid, str.substr(0, 347));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: setting FMOSA table in text format in row 6: label @@SOS@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...Enabled      Comment\r\n"             466
        try {
            outputs->setText(Bid, str.substr(0, 466));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: setting FMOSA table in text format in row 7: label @@SOS@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@SOS@"                               472
        try {
            outputs->setText(Bid, str.substr(0, 472));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: setting FMOSA table in text format in row 6: label @@SOS@@ not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@SOS@@"                               473
        try {
            outputs->setText(Bid, str.substr(0, 473));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: setting FMOSA table in text format in row 7: OS parameters not found: \"<Name> | <RA> | <Dec> | <Mag> | <Type> | <Pr> | <Bid> | <Pid> | <X(mm)> | <Y(mm)> | <Enabled> | <Comment>\"") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@SOS@@\r\n"                          475
        try {
            outputs->setText(Bid, str.substr(0, 475));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: setting FMOSA table in text format in row 8: OS parameters not found: \"<Name> | <RA> | <Dec> | <Mag> | <Type> | <Pr> | <Bid> | <Pid> | <X(mm)> | <Y(mm)> | <Enabled> | <Comment>\"") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...                     \r\n"            717
        try {
            outputs->setText(Bid, str.substr(0, 717));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(E.Message.str != "setting text in Outputs structure: setting FMOSA table in text format in row 9: setting OS section: setting an OS: should have 12 fields separated by '|': \"<Name> | <RA> | <Dec> | <Mag> | <Type> | <Pr> | <Bid> | <Pid> | <X(mm)> | <Y(mm)> | <Enabled> | <Comment>\"") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@EOS@"                               723
        try {
            outputs->setText(Bid, str.substr(0, 723));
        }
        catch(Exception& E) {
            if(typeid(E) != typeid(EImproperArgument)) {
                CPPUNIT_ASSERT(false);
                return;
            }
            string aux = E.Message.str.substr(60, E.Message.str.length()-1);
            if(E.Message.str != "setting text in Outputs structure: setting FMOSA table in text format in row 9: setting OS section: setting an OS: should have 12 fields separated by '|': \"<Name> | <RA> | <Dec> | <Mag> | <Type> | <Pr> | <Bid> | <Pid> | <X(mm)> | <Y(mm)> | <Enabled> | <Comment>\"") {
                CPPUNIT_ASSERT(false);
                return;
            }
        }

        //"...@@EOS@@"                               724
        try {
            outputs->setText(Bid, str.substr(0, 724));
        }
        catch(Exception& E) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //"...@@EOS@@\r\n"                          726
        try {
            outputs->setText(Bid, str.substr(0, 726));
        }
        catch(Exception& E) {
            CPPUNIT_ASSERT(false);
            return;
        }

        delete outputs;
    }
*/
    CPPUNIT_ASSERT(true);
}

