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
//File: Actuator.h
//Content: actuator model of a RP
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "Cilinder.h"
#include "Constants.h"
#include "adjacentitem.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TKnowledgeDegree
//---------------------------------------------------------------------------

//grado de conocimiento de las posiciones angulares de los rotores
//  kdPre: se conocen de forma precisa
//  kdApp: se conocen de forma aproximada
//  kdUnk: se desconoce la posición de algún eje
enum TKnowledgeDegree {kdPre, kdApp, kdUnk};

void  StrPrintKnowledgeDegree(AnsiString& S, TKnowledgeDegree kd);
void  StrReadKnowledgeDegree(TKnowledgeDegree& kd,
                             const AnsiString &S, int &i);
AnsiString KnowledgeDetgreeToStr(TKnowledgeDegree kd);
TKnowledgeDegree StrToKnowledgeDegree(const AnsiString& S);

//---------------------------------------------------------------------------
//TPurpose
//---------------------------------------------------------------------------

//Purpose of the SPM:
//  pAll: allocate RPs to projection points;
//  pGenPairPPDP: generate a pair (PP, DP);
//  pValDP: validate the generated DP;
//  pValPP: validate the generated PP;
//  pGenParPro: generate a parking program;
//  pValParPro: validate a parking program;
//  pExe: execute a generated MP.
enum TPurpose {pAll, pGenPairPPDP, pValDP, pValPP,
               pGenParPro, pValParPro, pExe};

void  strPrintPurpose(string& str, TPurpose p);
void  strReadPurpose(TPurpose& value, const string &str, unsigned int &i);
string purposeToStr(TPurpose value);
TPurpose strToPurpose(const string& str);

/*//---------------------------------------------------------------------------
//TPairEADmin
//---------------------------------------------------------------------------

class TPairEADmin {
    TExclusionArea *p_EA;
    double p_Dmin;

public:
    //pointer to EA attached to Dmin
    TExclusionArea *getEA(void) const {return p_EA;}
    void setEA(TExclusionArea*);

    //minimun distance with the actuator of the attached EA
    double getDmin(void) const {return p_Dmin;}
    void setDmin(double);

    //get the item in text format
    AnsiString getText(void) const;

    //STATIC METHODS FOR LISTS:

    //asigna un valor a un elemento
    static void  assign(TPairEADmin& item, double Dmin);
    //print the item in a string
    static void  print(AnsiString &S, TPairEADmin item);

    //CONTRUCTOR:

    //build an item by default
    TPairEADmin(void) : p_EA(NULL), p_Dmin(DBL_MAX) {;}
    //build an item with the specified values
    TPairEADmin(TExclusionArea *EA, double Dmin);

    //The default consturctor is necessary for can define a ItemsList.
};

//---------------------------------------------------------------------------
//TPairRPDmin
//---------------------------------------------------------------------------

class TPairRPDmin {
    TRoboticPositioner *p_RP;
    double p_Dmin;

public:
    //pointer to RP attached to Dmin
    TRoboticPositioner *getRP(void) const {return p_RP;}
    void setRP(TRoboticPositioner*);

    //minimun distance with the actuator of the attached RP
    double getDmin(void) const {return p_Dmin;}
    void setDmin(double);

    //get the item in text format
    AnsiString getText(void) const;

    //STATIC METHODS FOR LISTS:

    //asigna un valor a un elemento
    static void  assign(TPairRPDmin& item, double Dmin);
    //print the item in a string
    static void  print(AnsiString &S, TPairRPDmin RD);

    //CONTRUCTOR:

    //build an item by default
    TPairRPDmin(void) : p_RP(NULL), p_Dmin(DBL_MAX) {;}
    //build an item with the specified values
    TPairRPDmin(TRoboticPositioner *RP, double Dmin);

    //The default consturctor is necessary for can define a ItemsList.
};
*/
//---------------------------------------------------------------------------
//TActuator
//---------------------------------------------------------------------------

//clase actuador de un RP
class TActuator : public TCilinder {
protected:
    //COMPONENTES DE SPM:

    double p_SPMrec;

    double p_SPMsta;
    double p_SPMdyn;
    double p_SPMmin;
    double p_SPMsim;
    double p_SPMoff;

    //------------------------------------------------------------------
    //PROPIEDADES DE ESTADO:

    TKnowledgeDegree p_PAkd;
    TPurpose p_Purpose;

    int p_Id;

    //COTAS ÚTILES:

    double p_r_min;
    double p_r_saf;
    double p_r_2saf;
    double p_theta___2saf;
    double p_theta___3saf;
    double p_theta_2rad;

    //PROPIEDADES DE ÁREA:

    double p_r_3maxnom;

    double p_Sc;
    double p_Sw;
    double p_Sp;
    double p_Ss;
    double p_Se;
    double p_Re;

    double p_Spt;
    double p_Set;
    double p_Ret;

    //------------------------------------------------------------------
    //MÉTODOS DE ASIMILACIÓN:

    //asigna el SPM adecuado para (PAkd, Purpose)
    void AssignSPM(void);

    //------------------------------------------------------------------
    //MÉTODOS PRIVADOS PARA EL CÁLCULO DE DISTANCIAS CON BARRERAS:

    //determina la distancia mínima con una barrera
    double distanceMin(const TBarrier*) const;
    //determina si hay colisión con una barrera
    bool thereIsCollision(const TBarrier*) const;

public:
    //------------------------------------------------------------------
    //PROPIEDADES DE SEGURIDAD:

    //COMPONENTES DE SPM:

    //SPM para absorber el error de recuperación aproximada
    //debe ser no negativo
    //defeult value:
    //  (MEGARA_VMAXABS1*rbs*r_max + MEGARA_VMAXABS2*Arm->rbs*Arm->L1V)*
    //      MEGARA_Tstop
    double getSPMrec(void) const {return p_SPMrec;}
    void setSPMrec(double);

    //SPM para absorber el error mecánico y numérico
    //debe ser no negativo
    //defeult value:
    //  MEGARA_Eo*r_max + MEGARA_Ep
    double getSPMsta(void) const {return p_SPMsta;}
    void setSPMsta(double);
    //SPM para absorber el desfase temporal de los RPs
    //y  las variaciones de velocidad debidas a los motores paso a paso
    //debe ser no negativo
    //defeult value:
    //  (MEGARA_VMAXABS1*rbs*r_max + MEGARA_VMAXABS2*Arm->rbs*Arm->L1V)*
    //      MEGARA_Tshiff + MEGARA_SPMadd
    double getSPMdyn(void) const {return p_SPMdyn;}
    void setSPMdyn(double);
    //SPM para absorber la incertidumbre del salto mínimo
    //durante la programación
    //debe ser no negativo
    //defeult value:
    //  MEGARA_SPMmin
    double getSPMmin(void) const {return p_SPMmin;}
    void setSPMmin(double);
    //SPM para absorber la incertidumbre de la posición de P3
    //durante la simulación del movimiento
    //debe ser no negativo
    //defeult value:
    //  MEGARA_SPMsim
    double getSPMsim(void) const {return p_SPMsim;}
    void setSPMsim(double);
    //SPM para absorver las aproximaciones producidas
    //por corrección del offset
    //debe ser no negativo
    //defeult value:
    //  MEGARA_PAem*rmax + MEGARA_Pem
    double getSPMoff(void) const {return p_SPMoff;}
    void setSPMoff(double);

    //Calculating the SPM components in a RP:
    //  SPMrec = (CMF.vmaxabs1*Actuator->rbs*Actuator->r_max +
    //      CMF.vmaxabs2*Actuator->Arm->rbs*Actuator->Arm->L1V)*Tstop;
    //  SPMsta = Eo*Actuator->r_max + Ep;
    //  SPMdyn = (CMF.vmaxabs1*Actuator->rbs*Actuator->r_max +
    //      CMF.vmaxabs2*Actuator->Arm->rbs*Actuator->Arm->L1V)*Tshiff + SPMadd;
    //  SPMmin = SPMsta;
    //  SPMoff = PAem*Actuator->rmax + Pem;

    //Where are employed the following properties of a RP:
    //  CMF.vmaxabs1: velócidad máxima absoluta del rotor 1 en pasos/s.
    //  rbs: radianes por paso del rotor 1.
    //  r_max: radio apical del actuador.
    //  CMF.vmaxabs1: velócidad máxima absoluta del rotor 2 en pasos/s.
    //  Arm->rbs: radianes por paso del rotor 2.
    //  Arm->L1V: radio apical del brazo.
    //  rmax: coordenada radial máxima de P3 en S0.

    //Donde se emplean las siguientes propiedades, cuyos valores deben ser
    //proporcionados por el fabricante:
    //  Tstop: tiempo máximo entre el último almacenamiento
    //      de la posición y la detención de ambos rotores.
    //      Valor por defecto para MEGARA: 1 ms.
    //  Eo: margen de error de orientación por mecanizado.
    //      Valor por defecto para MEGARA: 0.001 rad.
    //  Ep: margen de error de posición por mecanizado.
    //      Valor por defecto para MEGARA: 0.01 mm.
    //  Tshif: desfase temporal entre RPs en movimiento.
    //      Valor por defecto para MEGARA: 1 ms.
    //  PAem: margen de error de orientación de S0.
    //      Valor por defecto para MEGARA: 0.001 rad.
    //  Pem: margen de error de apuntado de S0.
    //      Valor por defecto para MEGARA: 0.1 mm.

    //Nótese que CMF.vmaxabs1*rbs*r_max es la máxima velocidad
    //lineal del ápice del brazo debido a la rotación del rotor 1.

    //Nótese que CMF.vmaxabs2*Arm->rbs*Arm->Ra es la máxima velocidad
    //lineal del ápice del brazo debido a la rotación del rotor 2.

    //Nótese que SPMoff tiene dos componentes:
    //  * PAem*rmax: es el offset máximo producido por
    //    error de orientación de S0.
    //  * Pem:offset máximo producido por error de apuntado de S0.
    //La primera componente depende de la ubicación del actuador
    //en el Fiber MOS. No se debe confundir P0Vcon POV.
    //Como no hay valor por defecto para POVmax, se asigna MEGARA_P0Vmax,
    //como si hubiera un único actuador centrado en el origen.

    //VALORES DE SPM PARA CADA OCASIÓN:

    //Valores de SPM de precisión para el brazo del actuador,
    //cuando la posición de ambos rotores se conoce con precisión
    //(PAkd == kdPre):

    //SPM for execution:
    //  SPMsta + SPMdyn
    double getSPMexe_p(void) const;
    //SPM for validate a parking program:
    //  SPMexe_p + SPMmin + SPMsim
    double getSPMvalParPro_p(void) const;
    //SPM for generate a parking program:
    //  SPMvalParPro_p + SPMmin + SPMsim
    double getSPMgenParPro_p(void) const;
    //SPM for validate a PP:
    //  SPMgenParPro_p + SPMmin + SPMsim
    double getSPMvalPP_p(void) const;
    //SPM for validate a DP:
    //  SPMvalPP_p + SPMmin + SPMsim
    double getSPMvalDP_p(void) const;
    //SPM for generate a pair (PP, DP):
    //  SPMvalDP_p + SPMmin + SPMsim
    double getSPMgenPairPPDP_p(void) const;
    //SPM for allocation:
    //  SPMgenPairPPDP_p + SPMoff
    double getSPMall_p(void) const;

    //Valores de SPM de aproximación para el brazo del actuador,
    //cuando la posición de ambos rotores se conoce de forma aproximada
    //(PAkd == kdApp):

    //SPM for execution:
    //  SPMrec + SPMexe_p
    double getSPMexe_a(void) const;
    //SPM for validate a parking program:
    //  SPMexe_a + SPMmin + SPMsim
    double getSPMvalParPro_a(void) const;
    //SPM for generate a parking program:
    //  SPMvalParPro_a + SPMmin + SPMsim
    double getSPMgenParPro_a(void) const;
    //SPM for validate a PP:
    //  SPMgenParPro_a + SPMmin + SPMsim
    double getSPMvalPP_a(void) const;
    //SPM for validate a DP:
    //  SPMvalPP_a + SPMmin + SPMsim
    double getSPMvalDP_a(void) const;
    //SPM for generate a pair (PP, DP):
    //  SPMvalDP_a + SPMmin + SPMsim
    double getSPMgenPairPPDP_a(void) const;
    //SPM for allocation:
    //  SPMgenPairPPDP_a + SPMoff
    double getSPMall_a(void) const;

    //Valores de SPM de aproximación para la barrera del actuador,
    //cuando la posición de algún eje es desconocida
    //(PAkd == kdUnk):

    //SPM para todos los casos:
    //  SPMsta

    //------------------------------------------------------------------
    //PROPIEDADES DE ESTADO:

    //grado de conocimiento de las posiciones de los rotores
    //debe ser uno de los siguientes valores:
    //  kdPre: se conocen de forma precisa
    //  kdApp: se conocen de forma aproximada
    //  kdUnk: se desconoce la posición de algún eje
    //valor por defecto_ kdPrecise
    TKnowledgeDegree getPAkd(void) const {return p_PAkd;}
    void setPAkd(TKnowledgeDegree);
    //propósito con el que se va a usar el actuador
    //debe ser uno de los siguientes valores:
    //  pAll: allocate RPs to projection points;
    //  pGenPairPPDP: generate a pair (PP, DP);
    //  pValDP: validate the generated DP;
    //  pValPP: validate the generated PP;
    //  pGenParPro: generate a parking program;
    //  pValParPro: validate a parking program;
    //  pExe: execute a generated MP.
    //valor por defecto: pGenPairPPDP
    TPurpose getPurpose(void) const {return p_Purpose;}
    void setPurpose(TPurpose);

    //si PAkd == kdPre,
    //  si Porporse == pAll
    //      Arm->SPM = SPMall_p
    //  si Porporse == pPro
    //      Arm->SPM = SPMgen_p
    //  si Porporse == pVal
    //      Arm->SPM = SPMval_p
    //  si Porporse == pExe
    //      Arm->SPM = SPMexe_p
    //si PAkd == kdApp,
    //  si Porporse == pAll
    //      Arm->SPM = SPMall_a
    //  si Porporse == pPro
    //      Arm->SPM = SPMgen_a
    //  si Porporse == pVal
    //      Arm->SPM = SPMval_a
    //  si Porporse == pExe
    //      Arm->SPM = SPMexe_a
    //si PAkd == kdUnk,
    //  Barrier->SPM = SPMsta

    //identificador del actuador
    //debe ser no negativo
    //el número 0 indica indefinición
    int getId(void) const {return p_Id;}
    void setId(int);

    //El número de identificación debe ser una propiedad del actuador
    //para que los actuadores adyacentes puedan ser identificados.

    //lista de clusters (EA, Dmin, Dend) cuyo EA está lo bastante cerca
    //para que pueda colisionar con el brazo
    //valor por defecto: {}
    TAdjacentEAList AdjacentEAs;
    //lista de clusters (RP, Dmin, Dend) cuyo RP está lo bastante cerca
    //para que pueda colisionar con el brazo
    //valor por defecto: {}
    TAdjacentRPList AdjacentRPs;

    //Las EAs adyacentes y los RPs adyacentes serán determinados mediante
    //el método TRoboticPositionerList::determineAdyacents().

    //Las distancias mínimas serán determinadas durante la ejecución de validateMP.

    //indica si el brazo del actuador será tenido en cuenta
    //en la determinación de distancias y colisiones
    //valor por defecto: true
    bool Pending;

    //La propiedad Pending será usada en los métodos:
    //  thereIsCollisionWithPendingAdjacent
    //  searchCollindingPendingAdjacent
    //con objeto de evitar aplicar el método de determinación de colisión
    //más de una vez entre cada par de elementos.
    //Deberá determinarse el estado de colisión mediante el método:
    //  TActuator::thereIsCollision
    //cuando:
    //- la bandera de determinación de colisión pendiente (Pending)
    //  sea true.

    //estado de colisión del actuador con
    //alguno de sus adyacentes (EAs o RPs)
    //valor por defecto: false
//    bool Collision;

    //------------------------------------------------------------------
    //COTAS ÚTILES:

    //radio de la envolvente del contorno del brazo
    //cuando el eje 2 está en el origen
    //valor por  defecto: el mismo que r_max
    double getr_min(void) const {return p_r_min;}
    //Radio de la frontera segura.
    //Radio de la circunferencia descrita por el punto de Arm->Contour
    //más alejado del centro del actuador cuando
    //el brazo está retraido en el linde de la zona segura.
    //      r_saf = Min{D[i] - r_max[i] - SPM[i]} - SPM;
    //donde:
    //  D[i] es la distancia al adyacente i;
    //  r_max[i] es el radio del contorno del adyacente i;
    //  SPM[i] es el margen de seguridad del adyacente i.
    //  SPM es el margen de seguridad de este actuador.
    //valor por defecto: el mismo que r_max
    //donde Arm->SPM es el SPM de generación dado por defecto.
    double getr_saf(void) const {return p_r_saf;}

    //Radio seguro de P2.
    //Radio de la circunferencia descrita por P2 cuando
    //el brazo está retraido en el linde de la zona segura.
    //Valor por defecto: L02max
    double getr_2saf(void) const {return p_r_2saf;}
    //Posición angular de P2 respecto de S3 en radianes
    //a partir de la cual el brazo está dentro del área de seguridad.
    //Valor por defecto: M_PI rad
    double gettheta___2saf(void) const {return p_theta___2saf;}
    //Posición angular de P3 respecto de S3 en radianes
    //a partir de la cual el brazo está dentro del área de seguridad.
    //Valor por defecto: M_PI rad
    double gettheta___3saf(void) const {return p_theta___3saf;}

    //Posición angular de P2 respecto de S1 en radianes
    //a partir de la cual el brazo puede ser retraido
    //linealmente hasta la zona segura sin que theta_1
    //llegue a ser menor que max(0, theta_1min).
    //Valor por defecto: max(0, theta_1min) + M_PI/2 rad
    double gettheta_2rad(void) const {return p_theta_2rad;}

    //ADVERTENCIA: el controlador puede estar concebido para
    //trabajar solamente con valores no negativos de p_1,
    //de modo que theta_2rad es calculado pensando en que theta_1
    //no podrá rebasar max(0, theta_1min).

    //------------------------------------------------------------------
    //PROPIEDADES DE ÁREA:

    //radio del dominio de P3
    //debe ser un valor mayor que cero
    //valor por defecto: r_3max mm
    double getr_3maxnom(void) const {return p_r_3maxnom;}
    void setr_3maxnom(double);

    //Dado que las propiedades de área son calculadas
    //para el caso ideal, deberían tomarse los valores nominales
    //para el cálculo de las mismas. Como las propiedades de las que
    //dependen las propiedades de área son solamente:
    //  r = r_3maxnom y
    //  N = AdjacentRPs.Count.
    //el único valor nominal que debe darse es el de r_3max.

    //Los parámetros Sp, Se y Re son calculados analíticamente
    //para el caso en que los actuadores adyacentes cumplen:
    //  radio del dominio igual;
    //  equidistantes 4*L*cos(M_PI/6).

    //En el caso real, es posible que:
    //  - la distancia entre actuadores no es exactamente
    //    4*L*cos(PI/6);
    //  - el área de seguridad y el área no invasiva se vean ligeramente
    //    reducidas a causa del SPM.

    //Para facilitar la notación se sustituirá:
    //  N = AdjacentRPs.Count;
    //  r = r_3maxnom.

    //área del círculo de radio r
    //       Sc = M_PI*r*r
    double getSc(void) const {return p_Sc;}
    //área de una porción de seis del círculo de radio r
    //  Sw = Sc/6
    double getSw(void) const {return p_Sw;}

    //área participativa de una porción triangular
    //del exágono cinrcunscrito en la circunferencia de radio r
    //  Sp = r*r*aux/2
    double getSp(void) const {return p_Sp;}
    //área de uno de los segmentos de circunferencia
    //en las inmediaciones del exágono circunscrito
    //en la circunferencia de radio r
    // Ss = Sw - Sp
    double getSs(void) const {return p_Ss;}
    //área exclusiva de una porción exagonal
    //del círculode radio r
    //  Se = Sp - Ss
    double getSe(void) const {return p_Se;}
    //ratio entre área exclusiva y el área participativa
    //  Re = Se/Sp
    double getRe(void) const {return p_Re;}

    //calcula el área participativa total
    //  p_Spt = (6 - N)*Sw + N*Sp;
    //        = r*r*(M_PI + N*(sqrt(0.75) - M_PI/3)/2);
    double getSpt(void) const {return p_Spt;}
    //calcula el área exclusiva total
    //  p_Set = (6 - N)*Sw + N*Se;
    //        = r*r*(sqrt(0.75) - M_PI/6);
    double getSet(void) const {return p_Set;}
    //calcula el ratio entre Set y Spt
    //  p_Ret = Set/Spt;
    //        = 2 - M_PI/(3*sqrt(0.75))
    double getRet(void) const {return p_Ret;}

    //ADVERTENCIA: cuando cambia el radio del dominio del actuador
    //o de alguno de los adyacentes, o la distancia entre posicioandores
    //no es igual a 4*L*cos(M_PI/6), las propiedades de área pueden no
    //corresponderse con la realidad.

    //ADVERTENCIA: cuando r_3min!=0 se tiene que parte del área excusiva
    //no forma parte del dominio del actuador, con lo que habría que
    //distinguir dos tipos de áreas exclusivas:
    //  área exclusiva total;
    //  área exclusiva accesible.

    //------------------------------------------------------------------
    //PROPIEDADES DE SEGURIDAD EN FORMATO TEXTO:

    //COMPONENTES DE SPM EN FORMATO TEXTO:

    AnsiString getSPMrecText(void) const;
    void setSPMrecText(const AnsiString&);
    AnsiString getSPMstaText(void) const;
    void setSPMstaText(const AnsiString&);
    AnsiString getSPMdynText(void) const;
    void setSPMdynText(const AnsiString&);
    AnsiString getSPMminText(void) const;
    void setSPMminText(const AnsiString&);
    AnsiString getSPMsimText(void) const;
    void setSPMsimText(const AnsiString&);
    AnsiString getSPMoffText(void) const;
    void setSPMoffText(const AnsiString&);

    //VALORES DE SPM PARA CADA OCASIÓN EN FORMATO TEXTO:

    AnsiString getSPMexe_pText(void) const;
    AnsiString getSPMvalParPro_pText(void) const;
    AnsiString getSPMgenParPro_pText(void) const;
    AnsiString getSPMvalPP_pText(void) const;
    AnsiString getSPMvalDP_pText(void) const;
    AnsiString getSPMgenPairPPDP_pText(void) const;
    AnsiString getSPMall_pText(void) const;

    AnsiString getSPMexe_aText(void) const;
    AnsiString getSPMvalParPro_aText(void) const;
    AnsiString getSPMgenParPro_aText(void) const;
    AnsiString getSPMvalPP_aText(void) const;
    AnsiString getSPMvalDP_aText(void) const;
    AnsiString getSPMgenPairPPDP_aText(void) const;
    AnsiString getSPMall_aText(void) const;

    //------------------------------------------------------------------
    //PROPIEDADES DE ESTADO EN FORMATO TEXTO:

    AnsiString getPAkdText(void) const;
    void setPAkdText(const AnsiString&);
    AnsiString getPurposeText(void) const;
    void setPurposeText(const AnsiString&);

    AnsiString getIdText(void) const; void setIdText(const AnsiString&);

    //AdjacentRPs.Text
    //AdjacentEAs.Text

    AnsiString getPendingText(void) const;
    void setPendingText(const AnsiString&);

    //COTAS ÚTILES EN FORMATO TEXTO:

    AnsiString getr_minText(void) const;
    AnsiString getr_safText(void) const;
    AnsiString getr_2safText(void) const;
    AnsiString gettheta___2safText(void) const;
    AnsiString gettheta___3safText(void) const;
    AnsiString gettheta_2radText(void) const;

    //PROPIEDADES DE ÁREA EN FORMATO TEXTO:

    AnsiString getr_3maxnomText(void) const;
    void setr_3maxnomText(AnsiString&);

    AnsiString getScText(void) const;
    AnsiString getSwText(void) const;

    AnsiString getSpText(void) const;
    AnsiString getSsText(void) const;
    AnsiString getSeText(void) const;
    AnsiString getReText(void) const;

    AnsiString getSptText(void) const;
    AnsiString getSetText(void) const;
    AnsiString getRetText(void) const;

    //------------------------------------------------------------------
    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    //conjuntos de propiedades de seguridad
    //en formato asignaciones de texto
    AnsiString getSecurityText(void) const;
    //conjunto de propiedades de estado en formato texto
    //en formato asignaciones de texto
    AnsiString getStatusText(void) const;
    //conjunto de propiedades límite en formato texto
    //en formato asignaciones de texto
    AnsiString getLimitsText(void) const;
    //conjunto de propiedades de área en formato texto
    //en formato asignaciones de texto
    AnsiString getAreaText(void) const;

    //conjunto de todas propiedades en formato texto
    //en formato asignaciones de texto
    AnsiString getAllText(void) const;

    //conjunto de propiedades de instancia en formato texto
    //en formato asignaciones de texto
    AnsiString getInstanceText(void) const;
    void setInstanceText(const AnsiString&);

    //conjunto de propiedades de origen (Id, x0, y0, thetaO1)
    //en formato línea de texto
    AnsiString getOriginsRowText(void) const;
    //conjunto de propiedades de posición de P3 (Id, x3, y3)
    //en formato línea de texto
    AnsiString getPositionP3RowText(void) const;
    //conjunto de propiedades de posición de P3 (Id, x_3, y_3)
    //en formato línea de texto
    AnsiString getPositionP_3RowText(void) const;
    //conjunto de propiedades de posición de P3 (Id, p_1, p___3)
    //en formato línea de texto
    AnsiString getPositionAnglesRowText(void) const;

    //------------------------------------------------------------------
    //MÉTODOS ESTÁTICOS:

    //compara los identificadores de dos actuadores
    static int  CompareIds(TActuator *A1, TActuator *A2);

    //imprime el identificador de un actuador
    static void  PrintId(AnsiString &S, TActuator *A);

    //Los métodos estáticos:
    //  CompareIds
    //  PrintId
    //serán apuntados en la lista de RPs adyacentes
    //para permitir su ordenacíon en función de los identificadores
    //y la impresión de los mismos.

    //lee una instancia actuador en una cadena
    static void  readInstance(TActuator *A,
                              const AnsiString& S, int &i);

    //obtiene las etiquetas de las propiedades de origen
    //("Id", "x0", "y0", "thetaO1") al final de una cadena de texto
    //en formato fila de texto
    static AnsiString getOriginsLabelsRow(void);
    //atraviesa las etiquetas de las propiedades de origen
    //("Id", "x0", "y0", "thetaO1")
    //en formato fila de texto
    static void  travelOriginsLabelsRow(const AnsiString&, int&);

    //imprime los valores de las propiedades de orien de un actuador
    //(Id, x0, y0, thetaO1) al final de una cadena de texto
    //en formato fila de texto
    static void  printOriginsRow(AnsiString&, TActuator*);
    //lee los valores de las propiedades de orien de un actuador
    //(Id, x0, y0, thetaO1) desde la posición indicada de una cadena
    //de texto, en formato fila de texto
    static void  readOriginsRow(TActuator* &FP,
                                const AnsiString& S, int &i);

    //obtiene las etiquetas de las propiedades de posición
    //("Id", "x3", "y3") al final de una cadena de texto
    //en formato fila de texto
    static AnsiString getPositionP3LabelsRow(void);
    //obtiene las etiquetas de las propiedades de posición
    //("Id", "x_3", "y_3") al final de una cadena de texto
    //en formato fila de texto
    static AnsiString getPositionP_3LabelsRow(void);
    //obtiene las etiquetas de las propiedades de posición
    //("Id", "p_1", "p___3") al final de una cadena de texto
    //en formato fila de texto
    static AnsiString getPositionPPALabelsRow(void);
    //atraviesa las etiquetas de las propiedades de posición
    //("Id", "x3", "y3")
    //en formato fila de texto
    static void  travelPositionP3LabelsRow(const AnsiString&, int&);

    //imprime los valores de las propiedades de posición de un actuador
    //(Id, x3, y3) al final de una cadena de texto
    //en formato fila de texto
    static void  printPositionP3Row(AnsiString&, TActuator*);
    //lee los valores de las propiedades de posición de un actuador
    //(Id, x3, y3) desde la posición indicada de una cadena
    //de texto, en formato fila de texto
    static void  readPositionP3Row(TActuator* &FP,
                                   const AnsiString& S, int &i);
    //imprime los valores de las propiedades de posición de un actuador
    //(Id, x_3, y_3) al final de una cadena de texto
    //en formato fila de texto
    static void  printPositionP_3Row(AnsiString&, TActuator*);

    //imprime los valores de las propiedades de posición de un actuador
    //(Id, p_1, p___3) al final de una cadena de texto
    //en formato fila de texto
    static void  printPositionAnglesRow(AnsiString&, TActuator*);

    //------------------------------------------------------------------
    //MÉTODOS DE CONSTRUCCIÓN, COPIA Y DESTRUCCIÓN:

    //construye un actuador
    //con los valores indicados
    TActuator(int Id, TDoublePoint P0, double thetao_=MEGARA_thetaO1);

    //ADVERTENCIA: para poder contruir clones de actuadores
    //la duplicidad de números de identificación está permitida.
    //El control de duplicidad de identificadores debe llevarse
    //en todo caso mediante métodos. Al fin y al cabo en el mundo real
    //será posible configurar dos RPs para que tengan el mismo
    //identificador.

    //copia las propiedades de seguridad de un actuador
    void copySecurity(const TActuator*);
    //copia las propiedades de estado de un actuador
    void copyStatus(const TActuator*);
    //copia las propiedades límite de un actuador
    void copyLimits(const TActuator*);
    //copia las propiedades de área de un actuador
    void copyArea(const TActuator*);

    //clona todas las propiedades de un actuador
    void clone(const TActuator*);

    //clona un actuador
    TActuator(const TActuator*);

    //destruye un actuador
    ~TActuator();

    //------------------------------------------------------------------
    //MÉTODOS DE ASIMILACIÓN:

    //calcula las propiedades de seguridad:
    //      {r_min, r_saf, r_2saf, theta___2saf, theta___3saf, theta_2rad}
    void calculateSafeParameters(void);

    //Los métodos para:
    //      determinar la posición relativa;
    //      determinar si hay o no invasión;
    //requieren el cálculo previo de los parámetros de seguridad

    //-------------------------------------------------------------------
    //MÉTODOS DE ASIGNACIÓN CONJUNTA ATÓMICA:

    //asigna las propiedades angulares (theta_1min, theta_1max, theta_1)
    void setOrientationRadians(double theta_1min, double theta_1max,
                               double theta_1, double theta_O3o);
    //asigna las propiedades de cuantificación
    void setQuantification(double SB1);

    //asigna las propiedades de origen
    void setOrigins(int Id, double x0, double y0, double thetaO1);
    //asigna las propiedades de posición
    //si el punto (x3, y3) no está en el dominio del actuador
    //      lanza EImproperArgument
    void setPositionP3(double x3, double y3);
    /*    //assign a pair of position angles in radians
    //if the position angle theta_1 isn't in domain of theta_1,
    //or the position angle theta___3 isn't in domain of theta___3
    //  throw an exception EImproperArgument
    void setPositionPPARadians(double theta_1, double theta___3);
    //assign a pair of position angles in steps
    //if the position angle p_1 isn't in domain of p_1,
    //or the position angle p___3 isn't in domain of p___3
    //  throw an exception EImproperArgument
    void setPositionPPASteps(double p_1, double p___3);*/
    //asigna las propiedades de identificación y posición
    //si el punto (x3, y3) no está en el dominio del actuador
    //      lanza EImproperArgument
    void setPositionP3(int Id, double x3, double y3);

    //asigna las componentes del SPM
    void setSPMcomponents(double SPMrec, double SPMsta, double SPMdyn);

    //asigna las propiedades de estado (PAkd, Purpose)
    void setStatus(TKnowledgeDegree PAkd, TPurpose Purpose);

    //-------------------------------------------------------------------
    //MÉTODOS PARA ALMACENAR Y RECUPERAR POSICIONES DE LOS ROTORES:

    //apila las posiciones de los rotores
    void pushthetas(void);
    //restaura las últimas posiciones apuiladas de los rotores
    //si no hay una posisición almacenada para algún rotor:
    //      lanza EImproperCall
    void restorethetas(void);
    //determines if:
    //the rotors of the RP not coincide with the last stacked positions
    bool thetasNotCoincideWithStacked(void);
    //desempila las últimas posiciones apiladas de los rotores
    //si no hay una posisición almacenada para algún rotor:
    //      lanza EImproperCall
    void popthetas(void);
    //restaura y desempila las últimas posiciones apiladas de los rotores
    //si no hay una posisición almacenada para algún rotor:
    //      lanza EImproperCall
    void restoreAndPopthetas(void);

    //ADVERTENCIA: la recuperación de posiciones no estables,
    //cuando está la cuantificación activada, dará lugar
    //al desplazamiento a la posición estable más próxima de cada rotor.

    //-------------------------------------------------------------------
    //MÉTODOS PARA ALMACENAR Y RECUPERAR ESTADOS DE CUNATIFICACIÓN:

    //apila el estado de los cuantificadores
    void pushQuantifys(void);
    //restaura el último estado apilado de los cuantificadores
    //si no hay un estado almacenado para algún quantificador:
    //      lanza EImproperCall
    void restoreQuantifys(void);
    //desempila el último estado apilado de los cuantificadores
    //si no hay un estado almacenado para algún quantificador:
    //      lanza EImproperCall
    void popQuantifys(void);
    //restaura y desempila el último estado apilado de los cuantificadores
    //si no hay un estado almacenado para algún quantificador:
    //      lanza EImproperCall
    void restoreAndPopQuantifys(void);

    //------------------------------------------------------------------
    //METHODS TO DETERMINE THE RELATIVE POSITION OF THE ACTUATOR:

    //determina si un ángulo del rotor 2 en radianes
    //está fuera del área de seguridad
    bool theta___3IsOutSafeArea(double theta___3) const;
    //determina si un ángulo del rotor 2 en radianes
    //está dentro del área de seguridad
    bool theta___3IsInSafeArea(double theta___3) const;

    //determina si un ángulo del rotor 2 en pasos
    //está fuera del área de seguridad
    bool p___3IsOutSafeArea(double p___3) const;
    //determina si un ángulo del rotor 2 en pasos
    //está dentro del área de seguridad
    bool p___3IsInSafeArea(double p___3) const;

    //determina si un punto está fuera del área de seguridad
    bool P3IsOutSafeArea(TDoublePoint P) const;
    //determina si un punto está dentro del área de seguridad
    bool P3IsInSafeArea(TDoublePoint P) const;

    //determina si el brazo está fuera del área deseguridad
    bool ArmIsOutSafeArea(void) const;
    //determina si el brazo está dentro del área de seguridad
    bool ArmIsInSafeArea(void) const;

    //determines if any rotor of the actuator is out the origin
    bool isOutTheOrigin(void) const;

    //MÉTODOS PARA DETERMINAR EL PUNTO DE INFLEXIÓN PARA UN ÁNGULO DEL ROTOR 1:

    //Punto de inflexión de la trayectoria radial es
    //aquel en el que theta_1 == Max{theta_1min, 0}.

    //Cuando theta_2 >= Max{theta_1min, 0} + PI/2,
    //el punto de inflexión está en P0.

    //Coordenada radial del punto de inflexión para la pose dada en S1.
    //Pi es el punto de la trayectoria radial de P2 en que:
    //      theta_1 == Max{theta_1min, 0}.
    //si L01 != Arm->L12 lanza una excepción EImproperCall
    double getr_i(double &theta_1, double theta_2);
    //ángulo del rotor 2 respecto de S3 en radianes
    //para el punto de inflexión de la pose dada
    //si L01 != Arm->L12 lanza una excepción EImproperCall
    double gettheta___2i(double theta_2, double r_i);

    //MÉTODOS DE MOVIMIENTO:

    //mueve el brazo hasta a la posición de seguridad estable más próxima
    void moveArmToSafePosition(void);

    //------------------------------------------------------------------
    //MÉTODOS PARA CALCULAR DISTANCIAS
    //CON BRAZOS DE ACTUADORES ADYACENTES:

    //determina la distancia con otro actuador
    double distance(TActuator*);
    //determine the distance with a point
    double distance(TDoublePoint);
    //determina la distancia libre con otro actuador
    //      Df = D - SPM1 - SPM2
    //donde:
    //      Df: distancia libre
    //      SPM1: SPM de este actuador (según PAkd)
    //      SPM2: SPM del otro actuador (según PAkd)
    double distanceFree(TActuator*);

    //determina la distancia mínima entre el brazo del actuador
    //y los brazos de los actuadores adyacentes
    double distanceWithAdjacent(void);
    //determina la distancia mínima entre el punto P3 del actuador
    //y los puntos P3 de los actuadores adyacentes
    double distanceP3WithAdjacent(void);

    //MÉTODOS PARA DETERMINAR EN QUE ÁREA SE ENCUENTRA UN PUNTO:

    //determines if a point is in the security area
    bool pointIsInSecurityArea(TDoublePoint);

    //determina si un punto está fuera del área no invasiva
    bool P3IsOutNoninvasiveArea(TDoublePoint P);
    //determina si un punto está dentro del área invasiva
    bool P3IsInNoninvasiveArea(TDoublePoint P);

    //deterines if this actuator is invading the maneuvering domain
    //of other actuator A
    bool notInvadeManeuveringDomain(const TActuator *A) const;

    //------------------------------------------------------------------
    //MÉTODOS PARA DETERMINAR LAS COLISIONES
    //CON EAs y RPs ADYACENTES:

    //determina la distancia mínima con un EA
    double distanceMin(const TExclusionArea*);
    //determina la distancia mínima con un actuador
    double distanceMin(const TActuator*);

    //determina si hay colisión con un EA
    bool thereIsCollision(const TExclusionArea*);
    //determina si hay colisión con un actuador
    bool thereIsCollision(const TActuator*);

    //determina si hay colisión con una barrera o un actuador adyacente
    bool thereIsCollisionWithAdjacent(void);
    //determina si no hay colisión con una barrera o un actuador adyacente
    bool thereIsntCollisionWithAdjacent(void);

    //Busca los EAs adyacentes cuya
    //barrera colisiona con este actuador
    void searchCollindingAdjacent(TItemsList<TExclusionArea*>&);
    //Busca los RPs adyacentes cuyo
    //actuador colisiona con este actuador
    void searchCollindingAdjacent(TItemsList<TRoboticPositioner*>&);

    //Determina si hay colisión con una barrera o un actuador adyacente
    //con evaluación de colisión pendiente.
    bool thereIsCollisionWithPendingAdjacent(void);

    //Busca los EAs adyacentes
    //con evaluación de colisión pendiente
    //cuya barrera colisiona con este actuador.
    void searchCollindingPendingAdjacent(TItemsList<TExclusionArea*>&);
    //Busca los RPs adyacentes
    //con evaluación de colisión pendiente
    //cuyo actuador colisiona con este actuador.
    void searchCollindingPendingAdjacent(TItemsList<TRoboticPositioner*>&);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // ACTUATOR_H
