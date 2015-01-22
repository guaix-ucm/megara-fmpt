// Copyright (c) 2014-2015 Isaac Morales Durán. All rights reserved.
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
//Archivo: RoboticPositionerList3.h
//Contenido: lista de posicionadores con funciones de programación y ejecución
//Última actualización: 12/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "RoboticPositionerList3.h"
#include "Strings.h"

//#include <QTimer>

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//PROPIEDADES DE EJECUCIÓN:

double TRoboticPositionerList::getTdis(void) const
{
        const TRoboticPositioner *RP;
        double Tdis = 0;

        //por cada controlador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el controlador indicado para facilitar su acceso
                RP = Items[i];

                //si el controlador indicado tiene un periodo mayor
                if(RP->CMF.gettendmax() > Tdis)
                        //actualiza la variable
                        Tdis = RP->CMF.gettendmax();
        }

        return Tdis;
}

void TRoboticPositionerList::setTit(const double _Tit)
{
        //el periodo de iteración Tit debería ser mayor que cero
        if(_Tit <= 0)
                throw EImproperArgument("itration period Tit should be upper zero");

        //asigna el nuevo valor
//        for(int i=0; i<getCount(); i++)
  //              RoboticPositioners[i]->Tit = _Tit;
        __Tit = _Tit;
}

//PROPIEDADES DE EJECUCIÓN
//EN FORMATO TEXTO:

AnsiString TRoboticPositionerList::getTdisText(void) const
{
        return FloatToStr(getTdis());
}

AnsiString TRoboticPositionerList::getTitText(void) const
{
        return FloatToStr(getTit());
}
void TRoboticPositionerList::setTitText(const AnsiString &S)
{
        try {
                t = StrToFloat_(S);
        } catch(...) {
                throw;
        }
}
/*AnsiString TRoboticPositionerList::getTimerEnabledText(void) const
{
        return BoolToStr_(getTimer()->isActive());
}
void TRoboticPositionerList::setTimerEnabledText(const AnsiString &S)
{
        try {
                t = StrToFloat_(S);
        } catch(...) {
                throw;
        }
}
AnsiString TRoboticPositionerList::getTimerIntervalText(void) const
{
        return FloatToStr(getTimer()->interval());
}
void TRoboticPositionerList::setTimerIntervalText(const AnsiString &S)
{
        try {
                t = StrToFloat_(S);
        } catch(...) {
                throw;
        }
}*/
AnsiString TRoboticPositionerList::gettText(void) const
{
        return FloatToStr(t);
}
void TRoboticPositionerList::settText(const AnsiString &S)
{
        try {
                t = StrToFloat_(S);
        } catch(...) {
                throw;
        }
}

//CONJUNTOS DE PROPIEDADES EN MODO TEXTO:

AnsiString TRoboticPositionerList::getExecutionText(void) const
{
        AnsiString S;

        S += AnsiString("Tdis = ")+getTdisText()+AnsiString("\r\n");
        S += AnsiString("Tit = ")+getTitText()+AnsiString("\r\n");
        S += "Timer:\r\n";
        //#S += AnsiString("    Enabled = ")+getTimerEnabledText()+AnsiString("\r\n");
        //#S += AnsiString("    Interval = ")+getTimerIntervalText()+AnsiString("\r\n");
        S += AnsiString("t = ")+gettText();

        return S;
}
/*AnsiString TRoboticPositionerList::getInstanceText(void) const
{
        AnsiString S;

        return S;
}
void TRoboticPositionerList::setInstanceText(const AnsiString& S)
{
        try {
                //contruye un clon de un posicionador multifibra
                TRoboticPositionerList aux(this);
                TRoboticPositionerList *RPL = &aux;
                //lee la instancia mediante el clon
                int i = 1;
                ReadInstance(RPL, S, i);

                //avanza el índice hasta el próximo caracterno separador
                //o hasta el final de la cadena
                StrTravelSeparatorsIfAny(S, i);
                //siel índice no indica a la posúltima posición de la cadena
                if(i<=S.Length())
                        //indica que la cadena S debería contener solamente una instancia de lista de posicionadores
                        throw EImproperArgument("string S should containan instance of robotic positioner list only");

                //asigna la variable tampón
                Clone(RPL);

        } catch(...) {
                throw;
        }
}
  */
/*AnsiString TRoboticPositionerList::getControllersText(void)
{
        AnsiString S;

        S += "Tdis = "+FloatToStr(Tdis)+";\r\n";

        return S;
} */

//###########################################################################
//MÉTODOS PÚBLICOS:
//###########################################################################

/*//------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:

//lee una instancia de posicionador multifibra
//en una cadena de texto en formato de asignaciones
void  TRoboticPositionerList::ReadInstance(TRoboticPositionerList *RPL,
        const AnsiString &S, int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el puntero RPL debería apuntar a ua lista de posicionadores construida
        if(RPL == NULL)
                throw EImproperArgument("pointer RPL shoult point to built robotic positioner list");

        //el índice i debería indicar una posición de la cadena de texto S
        if(i<1 || S.Length()<i)
                throw EImproperArgument("index i should indicate a position in the string text S");

        //estado de lectura
        //      0: intancia leida con éxito
        int status = 0;

        //variables tampón
        TRoboticPositionerList _RPL(RPL);

        //ADVERTENCIA: las variables tampón con propiedades interdependientes
        //deben ser clones de las variables que se pretenden modificar.

        //NOTA: adviertase que las propiedades enteras son leidas como
        //valores en punto flotante para detectar errores en los cuales
        //sea especificado un valor en punto flotante en vez de un valor entero.

        do {
                switch(status) {
                }
        } while(status < 0);

        //asigna las variables tampón
        try {
                RPL->Clone(&_RPL);
        } catch(...) {
                throw;
        }
}
*/
//---------------------------------------------------------------------------
//MÉTODOS DE CONSTRUCCIÓN, COPIA, CLONACIÓN Y DESTRUCCIÓN:

//construye una lista de posicionadores de fibra
TRoboticPositionerList::TRoboticPositionerList(void) :
        TRoboticPositionerList2(),
        __Tit(40),
        PaintAll(NULL),
        t(0)
{
        //contruye el temporizador
        //#__Timer = new QTimer(this);

        //configura el temporizador
        //getTimer()->stop(); //esto no es necesario porque está desactivado por defecto
        //#connect(__Timer, SIGNAL(timeout()), this, SLOT(TimerTimer(void)));
        //#getTimer()->setInterval(40);
}

//copia las propiedades de ejecución de una lista de posicionadores de fibra
void TRoboticPositionerList::CopyExecution(const TRoboticPositionerList *RPL)
{
        //el puntero RPL debería apuntar a una lista de posicionadores construida
        if(RPL == NULL)
                throw EImproperArgument("pointer RPL should point to built robotic positiner list");

        //copia las propiedades de ejecución

        __Tit = RPL->getTit();
        t = RPL->t;
/*#
        if(RPL->getTimer()->isActive())
            getTimer()->start();
        else
            getTimer()->stop();
        getTimer()->setInterval(RPL->getTimer()->interval());*/
}

//clona una lista de posicionadores de fibra
void TRoboticPositionerList::Clone(const TRoboticPositionerList *RPL)
{
        //el puntero RPL debe apuntar a una lista de posicionadores construida
        if(RPL == NULL)
                throw EImproperArgument("pointers RPL should point to built robotic positioner list");

        //inicializa la lista de posicionadores de fibra
        Destroy();
        //clona los posicionadores de la lista
//        Items.Clone(RPL->Items);
        for(int i=0; i<RPL->getCount(); i++)
                Add(new TRoboticPositioner(RPL->Get(i)));

        //apunta a las mismas funciones externas
        Compare = RPL->Compare;
        Evaluate = RPL->Evaluate;
        Assign = RPL->Assign;
        Print = RPL->Print;
        Read = RPL->Read;

        //copia las propiedades de seguridad
        CopyTolerance(RPL);
        //copia las propiedades de dimensionamiento:
        CopySizing(RPL);
        //copia las propiedades de área
        CopyArea(RPL);
        //copia laspropiedades gráficas
        CopyGraphics(RPL);
        //copia las propiedades del mapa
        CopyMap(RPL);

        //copia las propiedades de ejecución
        CopyExecution(RPL);
}

//construye un clon de una lista de posicionadores de fibra
TRoboticPositionerList::TRoboticPositionerList(const TRoboticPositionerList *RPL) :
        TRoboticPositionerList2(RPL)
{
        //el puntero RPL debería apuntar a una lista de posicionadores construida
        if(RPL == NULL)
                throw EImproperArgument("pointer RPL should point to built robotic positioner list");

        //contruye el temporizador
        //#__Timer = new QTimer(RPL->__Timer);
        //clona todas las propiedades
        Clone(RPL);
}

//destruye una lista de posicionadores de fibra
TRoboticPositionerList::~TRoboticPositionerList()
{
        //destruye el temporizador
        //#delete __Timer;
}

/*//--------------------------------------------------------------------------
//MARGEN PERIMETRAL DE SEGURIDAD:

//asigna un periodo de iteración a todos los posicionadores
void TInstrumentCarrier::SetTi(double _Tit)
{
        //el periodo de iteración Tit debería ser mayor que cero
        if(_Tit <= 0)
                throw EImproperArgument("iteration period Tit should be upper zero");

        //po cada posicionador de la lista
        for(int i=0; i<getCount(); i++)
                RoboticPositioners[i]->Tit = _Tit; //asigna el nuevo valor
}
//asigna un estado de validación a todos los posicionadores
void TInstrumentCarrier::SetValidating(bool _Validating)
{
        //po cada posicionador de la lista
        for(int i=0; i<getCount(); i++)
                RoboticPositioners[i]->Validating = _Validating; //asigna el nuevo valor
}
//asigna un estado de ejecución a todos los posicionadores
void TInstrumentCarrier::SetExecuting(bool _Executing)
{
        //po cada posicionador de la lista
        for(int i=0; i<getCount(); i++)
                RoboticPositioners[i]->Executing = _Executing; //asigna el nuevo valor
}
               */
//MÉTODOS DE CONFIGURACIÓN:

//obtiene la tabla de identificadores (Id, Id1, Id2)
void TRoboticPositionerList::GetIdTable(TPointersList<TTernIntegers>& IdTable) const
{
        //inicializa la lista
        IdTable.Clear();
        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                TRoboticPositioner *RP = Items[i];
                //construye una terna con los identificadores del posicionador
                TTernIntegers *TI = new TTernIntegers(RP->getActuator()->getId(), RP->CMF.getId1(), RP->CMF.getId2());
                //añade la terna a la lista
                IdTable.Add(TI);
        }
}
//asigna la tabla de identificadores (Id, Id1, Id2)
//si IdTable no contiene una terna por cada posicionador:
//      lanza una excepción EImproperArgument
//si algún identificador en IdTable no es unívoco:
//      lanza una excepción EImproperArgument
//si algún Id en IdTable no se refiere a un posicionador de la lista:
//      lanza una excepción EImproperArgument
void TRoboticPositionerList::SetIdTable(TPointersList<TTernIntegers>& IdTable)
{
        //COMPRUEBA LAS PRECONDICIONES:

        //IdTable debe contener una terna por cada posicionador
        if(IdTable.getCount() != getCount())
                throw EImproperArgument("IdTable should contain one tern for each positioner");

        //IdTable no debe contener ningún identificador no unívoco
        for(int i=0; i<IdTable.getCount(); i++) {
                //apunta la terna indicada para facilitar su acceso
                TTernIntegers *TIi = IdTable.GetPointer(i);

                //si Id2 es igual que Id1
                if(TIi->z == TIi->y)
                        throw EImproperArgument("IdTable not should contain a non univocal identifiers");

                //para cada una de las ternas previas
                for(int j=0; j<i; j++) {
                        //apunta la terna indicada para facilitar su acceso
                        TTernIntegers *TIj = IdTable.GetPointer(j);

                        //si el Id está repetido
                        if(TIj->x == TIi->x)
                                throw EImproperArgument("IdTable not should contain a non univocal identifiers");

                        //si el Id1 ó Id2 está repetido
                        if(TIj->y==TIi->y || TIj->z==TIi->y ||
                                TIj->y==TIi->z || TIj->z==TIi->z || TIj->y==TIj->z)
                                throw EImproperArgument("IdTable not should contain a non univocal identifier");
                }
                //para cada una de las ternas posteriores
                for(int j=i+1; j<IdTable.getCount(); j++) {
                        //apunta la terna indicada para facilitar su acceso
                        TTernIntegers *TIj = IdTable.GetPointer(j);

                        //si el Id está repetido
                        if(TIj->x == TIi->x)
                                throw EImproperArgument("IdTable not should contain a non univocal identifiers");

                        //si el Id1 ó Id2 está repetido
                        if(TIj->y==TIi->y || TIj->z==TIi->y ||
                                TIj->y==TIi->z || TIj->z==TIi->z || TIj->y==TIj->z)
                                throw EImproperArgument("IdTable not should contain a non univocal identifier");
                }

                //todos los Ids en IdTable deben referirse a posicionadores de la lista
                if(SearchId(TIi->x) >= getCount())
                        throw EImproperArgument("all Ids in IdTable should do refer to positioners of the list");
        }

        //ASIGNA LOS IDENTIFICADORES Id1 E Id2 A CADA POSICIONADOR Id

        //por cada terna (Id, Id1, Id2)
        for(int i=0; i<IdTable.getCount(); i++) {
                //apunta la terna indicada para facilitar su acceso
                TTernIntegers *TI = IdTable.GetPointer(i);

                //busca el posicionador en la lista
                int j = SearchId(TI->x);
                //si no lo ha encontrado
                if(j >= getCount())
                        //indica que se ha producido un efecto lateral
                        throw Exception("lateral efect happened");
                //apunta el posicionador indicado para facilitar su acceso
                TRoboticPositioner *RP = Items[j];
                //asigna los identificadores Id1 e Id2
                RP->CMF.setId1(TI->y);
                RP->CMF.setId2(TI->z);
        }
}

//prepara el posicionador multifibra según un propósito
void TRoboticPositionerList::SetPurpose(TPurpose Purpose)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->getActuator()->setPurpose(Purpose);
}

//configura el estado de habilitación del cuantificador de rot 1
//de todos los posicionadores de la lista
void TRoboticPositionerList::SetQuantify_s(bool Quantify_)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->getActuator()->setQuantify_(Quantify_);
}
//configura el estado de habilitación del cuantificador de rot 2
//de todos los posicionadores de la lista
void TRoboticPositionerList::SetArmQuantify___s(bool Quantify___)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->getActuator()->getArm()->setQuantify___(Quantify___);
}
//configura el estado de habilitación de los cuantificadores
//de todos los posicionadores de la lista
void TRoboticPositionerList::SetQuantifys(bool Quantify_, bool Quantify___)
{
        TRoboticPositioner *RP;

        for(int i=0; i<getCount(); i++) {
                RP = Items[i];
                RP->getActuator()->setQuantify_(Quantify_);
                RP->getActuator()->getArm()->setQuantify___(Quantify___);
        }
}

//configura el estado de colisión
//de todos los posicionadores de la lista
void TRoboticPositionerList::SetCollisions(bool Collision)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->getActuator()->Collision = Collision;
}

//------------------------------------------------------------------
//MÉTODOS PARACOMPROBAR LAS PRECONDICIONES:

//determina si todos los posicionadores operativos en
//posiciones de inseguridad están en la lista Abatibles
bool TRoboticPositionerList::NotAllOperativeInUnsafetyPositionsAreInTheList(
        TRoboticPositionerList *Abatibles) const
{
        //por cada posicionador de fibra de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                TRoboticPositioner *RP = Items[i];

                //si el posicionador está operativo en posición de inseguridad
                if(RP->getOperative() && RP->getActuator()->ArmIsOutSafeArea()) {
                        //busca el posicionador en la lista Abatibles
                        int j = Abatibles->Search(RP);
                        //si el posicionador no está en la lista
                        if(Abatibles==NULL || j>=Abatibles->getCount())
                                //indica que no todos los operativos en posiciones de inseguridad están en la lista Abatibles
                                return true;
                }
        }

        //indica que todos los operativos en posiciones de inseguridad están en la lista Abatibles
        return false;
}

//determina si todos los posicionadores de la lista
//están operativos en posiciones de inseguridad
bool TRoboticPositionerList::NotAllPositionersAreOperativesInUnsafetyPositions(void) const
{
        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                TRoboticPositioner *RP = Items[i];

                //si el posicionador no está operativo o en posición de seguridad
                if(!RP->getOperative() || RP->getActuator()->ArmIsInSafeArea())
                        //indica que no todos los posicionadores de la lista están operativos en positiones de inseguridad
                        return true;
        }

        //indica que todos los posicionadores de la lista están operativos en positiones de inseguridad
        return false;
}

//---------------------------------------------------------------------------
//MÉTODOS DE PROGRAMACIÓN:

//borra las instrucciones de movimeinto de los posicionadores
void TRoboticPositionerList::ClearInstructions(void)
{
        //por cada posicionador de fibra de la lista
        for(int i=0; i<getCount(); i++)
                //borra el programa de su función de movimiento
                Items[i]->CMF.ClearProgram();
}

//asigna una instrucción
//si el identificador Id es negativo
//      lanza una excepción EImproperArgument
//si el posicionador Id no está en la lista
//      lanza una excepción EImproperArgument
//si Id==0 asigna la instrucción a todos los controldaores
void TRoboticPositionerList::SetInstruction(const int Id, const TInstruction &Instruction)
{
        //el identificador Id debería ser no negativo
        if(Id < 0)
                throw EImproperArgument("identifier Id should be nonnegative");

        try {
                //si la instrucción va dirigida a un solo posicionador
                if(Id != 0) {
                        //busca el posicionador en la lista
                        int i = SearchId(Id);
                        //si ha encontrado el posicioandor
                        if(i < getCount())
                                //asigna la instrucción al posicionador identificado
                                Items[i]->setInstruction(Instruction);
                } else { //si la instrucción va dirigida a todos los posicinadores
                        //si es la instrucción 'ST'
                        if(Instruction.getName() == "ST") {
                                //inicia el desplazamiento
                                //#getTimer()->start();
                        } else {
                                //por cada controlador de la lista
                                for(int i=0; i<getCount(); i++)
                                        //asigna la instrucción al controlador indicado
                                        Items[i]->setInstruction(Instruction);
                        }
                }
        } catch(...) {
                throw;
        }
}

//añade gestos de abatimiento para los posicionadores de fibras de la lista
void TRoboticPositionerList::programTurnArmsToSafeAreas(void)
{
        TRoboticPositioner *RP;

        //por cada controlador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el controlador indicado para facilitar su acceso
                RP = Items[i];
                //si el brazo del posicionador adscrito está fuera del área de seguridad
                if(RP->getActuator()->ArmIsOutSafeArea())
                        //programa el abatimiento desde su posición actual
                        RP->programTurnArmToSafeArea();
        }
}
//sincroniza todos los gestos de la lista
void TRoboticPositionerList::SinchronizeArrivals(void)
{
        //determina el periodo de desplazameinto máximo
        double Tdis = getTdis();

        //para cada posicionador de fibra de la lista
        for(int i=0; i<getCount(); i++)
                //sincriniza la llegada con el periodo de desplazamiento máximo
                Items[i]->CMF.SinchronizeArrival(Tdis);
}

//invierte los gestos en el dominio del tiempo
void TRoboticPositionerList::InvertTime(void)
{
        //para cada controlador de la lista
        for(int i=0; i<getCount(); i++)
                //invierte el gesto en el dominio del tiempo
                Items[i]->InvertTime();
}

//MÉTODOS DE MOVIMIENTO:

//mueve los RPs a la posición indicada
//por las funciones de movimiento en el instante t
//si el puntero PainAll no apunta a una función
//      lanza EImproperCall
//no actualiza k
void TRoboticPositionerList::Move(double t)
{
        //por cada RP de la lista
        for(int i=0; i<getCount(); i++)
                //mueve el RP a su posición correspondiente al instante t
                Items[i]->Move(t);

        //actualiza la interfaz
//        PaintAll();
}

//mueve los posicionadores de fibra a sus posiciones inciiales
//si el puntero PainAll no apunta a una función
//      lanza EImproperCall
//no actualiza k
void TRoboticPositionerList::MoveSta(void)
{
        //por cada RP de la lista
        for(int i=0; i<getCount(); i++)
                //mueve el RP a su posición inicial
                Items[i]->MoveSta();

        //actualiza la interfaz
//        PaintAll();
}
//mueve los RPs a sus posiciones finales
//si el puntero PainAll no apunta a una función
//      lanza EImproperCall
//no actualiza k
void TRoboticPositionerList::MoveFin(void)
{
        //por cada RP de la lista
        for(int i=0; i<getCount(); i++)
                //mueve el RP a su posición final
                Items[i]->MoveFin();

        //actualiza la interfaz
//        PaintAll();
}

/*#//realiza una iteración
//si el puntero PainAll no apunta a una función
//      lanza EImproperCall
void TRoboticPositionerList::TimerTimer(void)
{
        //si el tiempo acumulado ha alcanzado al tiempo de desplazamiento
        while(t >= getTdis()) {
                //detiene el temporizador
                getTimer()->stop();
                return; //no hace nada más
        }

        //actualiza el contador
        t += getTit();

        //mueve los posicionadores al instante t
        Move(t);

        //si debe mostrar el proceso
        if(PaintAll != NULL)
                PaintAll(); //actualiza la interfaz

        //si detecta una colisión
        if(ThereIsCollision()) {
                getTimer()->stop(); //detiene la ejecución
//                Collision = true; //indica que ha detectado una colisión
        }
}
*/
//METHODS TO TRANSLATE MOTION PROGRAMS:

//Translate the motion progam to the format stablished for
//the interface FMPT-MCS.
//Preconditions:
//  All message of instructions of the motion program
//  shall be addressed to an existing RP.
void TRoboticPositionerList::TranslateMotionProgram(AnsiString& S, int CBId,
    const TPairPositionAnglesList& IPL, const TMotionProgram& MP)
{
    //VERIFIES THE PRECONDITIONS:

    //all message of instructions of the motion program shall be addressed to an existing RP
    for(int i=0; i<MP.getCount(); i++) {
        const TMessageList *ML = MP.GetPointer(i);
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);
            int k = SearchId(MI->getId());
            if(k >= getCount())
                throw EImproperArgument("invalid motion program because contains a message of instrucction addressed to a nonexistent RP");
        }
    }

    //the initial position list must has a PPA for each RP
    if(IPL.getCount() != getCount())
        throw EImproperArgument("the initialposition list must have a PPA for each RP");

    //TRANSLATE THE MOTION PROGRAM:

    //stores the initial status of the RPs
//    TRoboticPositionerList1 RPL(this);

    //for each RP of the Fiber MOS Model
    SetPositions(IPL);

    //print the star delimiter of motion program cluster
    S = AnsiString("obs depos_")+IntToStr(CBId)+AnsiString(" {");

    //for each list of message of instructions of the motion program
    for(int i=0; i<MP.getCount(); i++) {
        //points the indicated message list to facilitate its access
        const TMessageList *ML = MP.GetPointer(i);

        //initialices the CMF of all RPs
        this->ClearInstructions();

        //PRINT THE GROUP CORRESPONDING TO THE INDICATED MESSAGE LIST:

        //print the start delimiter of goup i
        S += AnsiString("\r\n\tgroup_");
        S += strInsertChar(IntToStr(i+1), 2);
        S += AnsiString(" {");

        //for each MI of the list
        for(int j=0; j<ML->getCount(); j++) {
            //point the indicated MI to facilitate its access
            const TMessageInstruction *MI = ML->GetPointer(j);

            //search the RP to which the message is addressed
            int k = SearchId(MI->getId());

            //point the found RP to facilitate its access
            TRoboticPositioner *RP = Items[k];

            //set the instruction to the RP
            RP->setInstruction(MI->Instruction);

            //if there is a gesture programmedfor the RP
            if(RP->CMF.getMF1()!=NULL || RP->CMF.getMF2()!=NULL) {
                //print the programmed a motion for rotor 1
                if(RP->CMF.getMF1()!=NULL && RP->CMF.getMF1()->getpfinText()!=RP->getActuator()->getp_1Text()) {
                    //print the identifier of RP and the identifier of the rotor
                    S += AnsiString("\r\n\trp")+strInsertChar(RP->getActuator()->getIdText(), 2)+AnsiString(" r1");
                    //print the new final position
                    S += AnsiString(" ")+RP->CMF.getMF1()->getpfinText();
                }
//                else
                    //print the same final position
//                    S += AnsiString(" ")+RP->getActuator()->getp_1Text();

                //print the programmed a motion for rotor 2
                if(RP->CMF.getMF2()!=NULL && RP->CMF.getMF2()->getpfinText()!=RP->getActuator()->getArm()->getp___3Text()) {
                    //print the identifier of RP and the identifier of the rotor
                    S += AnsiString("\r\n\trp")+strInsertChar(RP->getActuator()->getIdText(), 2)+AnsiString(" r2");
                    //print the new final position
                    S += AnsiString(" ")+RP->CMF.getMF2()->getpfinText();
                }
//                else
                    //print the same final position
//                    S += AnsiString(" ")+RP->getActuator()->getArm()->getp___3Text();
            }
        }

        //print the end delimiter of group i
        S += AnsiString("\r\n\t}");

        //MOVE ALL RPs TO ITS PROGRAMMED FINAL POSITIONS:

        MoveFin();
/*        //for each RP of the Fiber MOS Model
        for(int i=0; i<getCount(); i++)
            //move the RP to its programmed final positions
            Items[i]->MoveFin();*/
    }
    //print the end delimiter of motion program cluster
    S += AnsiString("\r\n}");

    //recovers the initial status of the RPs
//    Clone(RPL);
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

