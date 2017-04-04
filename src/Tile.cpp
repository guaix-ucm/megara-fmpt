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
//File: Tile.cpp
//Content: tile list
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Tile.h"
#include "TextFile.h"
#include "Strings.h"

//---------------------------------------------------------------------------

using namespace Strings;

//espacio de nombres de modelos
namespace Models {

//--------------------------------------------------------------------------
//TTile:
//--------------------------------------------------------------------------

//INICIALIZA LAS PROPIEDADES ESTÁTICAS:

AnsiString TTile::IdLabel = "Id";
AnsiString TTile::RALabel = "RA";
AnsiString TTile::DECLabel = "DEC";
AnsiString TTile::RLabel = "R";

//PROPIEDADES DEFINITORIAS:

void TTile::setId(int Id)
{
    if(Id < 0)
        throw EImproperArgument("identifier number Id should be nonnegative");

    //asigna el nuevo valor
    p_Id = Id;
}

void TTile::setDEC(double DEC)
{
    if(DEC<-M_PI/2 || M_PI/2<DEC)
        throw EImproperArgument(AnsiString("declination '")+FloatToStr(DEC)+AnsiString("' should be in [-pi/2, pi/2]"));

    //asigna el nuevo valor
    p_DEC = DEC;
}
void TTile::setR_(double R_)
{
    if(R_ <= 0)
        throw EImproperArgument(AnsiString("radio '")+FloatToStr(R_)+AnsiString("' should be upper zero"));

    //asigna el nuevo valor
    p_R_ = R_;
}

//PROPIEDADES EN FORMATO TEXTO:

AnsiString TTile::getIdText(void) const
{
    return IntToStr(getId());
}
void TTile::setIdText(const AnsiString &S)
{
    try {
        setId(StrToInt(S));
    } catch(...) {
        throw;
    }
}
AnsiString TTile::getRAText(void) const
{
    return FloatToStr(RA);
}
void TTile::setRAText(const AnsiString &S)
{
    try {
        RA = StrToFloat(S);
    } catch(...) {
        throw;
    }
}
AnsiString TTile::getDECText(void) const
{
    return FloatToStr(getDEC());
}
void TTile::setDECText(const AnsiString &S)
{
    try {
        setDEC(StrToFloat(S));
    } catch(...) {
        throw;
    }
}
AnsiString TTile::getR_Text(void) const
{
    return FloatToStr(getR_());
}
void TTile::setR_Text(const AnsiString &S)
{
    try {
        setR_(StrToFloat(S));
    } catch(...) {
        throw;
    }
}

//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TTile::getRowText(void) const
{
    AnsiString S;

    S = getId();
    StrFill(S, 12, ' ');
    S += getRAText();
    StrFill(S, 12+24, ' ');
    S += getDECText();
    StrFill(S, 12+24*2, ' ');
    S += getR_Text();

    return S;
}
void TTile::setRowText(const AnsiString &S)
{
    try {
        //read the tile form the first position
        int i = 1;
        ReadRow(this, S, i);

        //search unexpected text
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("setting tile in row text format");
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting tile in row text format: ");
        throw;
    }
}
AnsiString TTile::getColText(void) const
{
    AnsiString S;

    S = getIdText()+AnsiString("\r\n");
    S += getRAText()+AnsiString("\r\n");
    S += getDECText()+AnsiString("\r\n");
    S += getR_Text();

    return S;
}
void TTile::setColText(const AnsiString &S)
{
    try {
        //read the tile from the first position
        int i = 1;
        ReadCol(this, S, i);

        //search unexpected text
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("setting tile in column text format");
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting tile in column text format: ");
        throw;
    }
}
AnsiString TTile::getAssignsText(void) const
{
    AnsiString S;

    S = AnsiString("Id = ")+getIdText()+AnsiString("\r\n");
    S += AnsiString("phi' = ")+getRAText()+AnsiString("\r\n");
    S += AnsiString("theta' = ")+getDECText()+AnsiString("\r\n");
    S += AnsiString("R' = ")+getR_Text();

    return S;
}
void TTile::setAssignsText(const AnsiString &S)
{
    try {
        //read the tile from the first position
        int i = 1;
        ReadAssigns(this, S, i);

        //search unexpected text
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("setting tile in assign text format");
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting tile in assign text format: ");
        throw;
    }
}

//MÉTODOS ESTÁTICOS:

//compara una de las propiedades de dos objetos
int  TTile::CompareIds(const TTile *T1, const TTile *T2)
{
    //el puntero T1 debería apuntar a un objeto contruido
    if(T1 == NULL)
        throw EImproperArgument("pointer T1 should point to built object");

    //el puntero T2 debería apuntar a un objeto contruido
    if(T2 == NULL)
        throw EImproperArgument("pointer T2 should point to built object");

    if(T1->getId() < T2->getId())
        return -1;
    if(T1->getId() > T2->getId())
        return -1;
    return 0;
}
int  TTile::CompareRAs(const TTile *T1, const TTile *T2)
{
    //el puntero T1 debería apuntar a un objeto contruido
    if(T1 == NULL)
        throw EImproperArgument("pointer T1 should point to built object");

    //el puntero T2 debería apuntar a un objeto contruido
    if(T2 == NULL)
        throw EImproperArgument("pointer T2 should point to built object");

    if(T1->RA < T2->RA)
        return -1;
    if(T1->RA > T2->RA)
        return -1;
    return 0;
}
int  TTile::CompareDECs(const TTile *T1, const TTile *T2)
{
    //el puntero T1 debería apuntar a un objeto contruido
    if(T1 == NULL)
        throw EImproperArgument("pointer T1 should point to built object");

    //el puntero T2 debería apuntar a un objeto contruido
    if(T2 == NULL)
        throw EImproperArgument("pointer T2 should point to built object");

    if(T1->getDEC() < T2->getDEC())
        return -1;
    if(T1->getDEC() > T2->getDEC())
        return -1;
    return 0;
}
int  TTile::CompareR_s(const TTile *T1, const TTile *T2)
{
    //el puntero T1 debería apuntar a un objeto contruido
    if(T1 == NULL)
        throw EImproperArgument("pointer T1 should point to built object");

    //el puntero T2 debería apuntar a un objeto contruido
    if(T2 == NULL)
        throw EImproperArgument("pointer T2 should point to built object");

    if(T1->getR_() < T2->getR_())
        return -1;
    if(T1->getR_() > T2->getR_())
        return -1;
    return 0;
}
int  TTile::CompareQ_s(const TTile *T1, const TTile *T2)
{
    //el puntero T1 debería apuntar a un objeto contruido
    if(T1 == NULL)
        throw EImproperArgument("pointer T1 should point to built object");

    //el puntero T2 debería apuntar a un objeto contruido
    if(T2 == NULL)
        throw EImproperArgument("pointer T2 should point to built object");

    if(T1->getDEC()<T2->getDEC() || (T1->getDEC()==T2->getDEC() && T1->RA<T2->RA))
        return -1;
    if(T1->getDEC()>T2->getDEC() || (T1->getDEC()==T2->getDEC() && T1->RA>T2->RA))
        return -1;
    return 0;
}

//obtiene las etiquetas de las propiedades en una cadena
AnsiString TTile::GetLabels(void)
{
    return AnsiString("(")+IdLabel+AnsiString(", ")+RALabel+AnsiString(", ")+DECLabel+AnsiString(", ")+RLabel+AnsiString(")");
}
AnsiString TTile::GetLabelsRow(void)
{
    return IdLabel+AnsiString("\t")+RALabel+AnsiString("\t")+DECLabel+AnsiString("\t")+RLabel;
}

//imprime las propiedades de un objeto en una cadena
void  TTile::PrintRow(AnsiString &S, const TTile *T)
{
    //el puntero T debería apuntar a un objeto contruido
    if(T == NULL)
        throw EImproperArgument("pointer T should point to built object");

    S += T->getRowText();
}
void  TTile::PrintCol(AnsiString &S, const TTile *T)
{
    //el puntero T debería apuntar a un objeto contruido
    if(T == NULL)
        throw EImproperArgument("pointer T should point to built object");

    S += T->getColText();
}
void  TTile::PrintAssigns(AnsiString &S, const TTile *T)
{
    //el puntero T debería apuntar a un objeto contruido
    if(T == NULL)
        throw EImproperArgument("pointer T should point to built object");

    S += T->getAssignsText();
}

//lee las propiedades de un objeto en una cadena
void  TTile::ReadRow(TTile *T, const AnsiString &S, int &i)
{
    if(T == NULL)
        throw EImproperArgument("pointer T shoult point to built object");
    if(i<1 || S.Length()<i)
        throw EImproperArgument("tile not found");

    try {
        int Id;
        StrReadInt(Id, S, i);
        double RA;
        StrReadFloat(RA, S, i);
        double DEC;
        StrReadFloat(DEC, S, i);
        double R_;
        StrReadFloat(R_, S, i);

        T->Set(Id, RA, DEC, R_);
    }
    catch(Exception &E) {
        E.Message.Insert(1, "reading tile in row text format: ");
        throw;
    }
}
void  TTile::ReadCol(TTile *T, const AnsiString &S, int &i)
{
    if(T == NULL)
        throw EImproperArgument("pointer T shoult point to built object");
    if(i<1 || S.Length()<i)
        throw EImproperArgument("tile not found");

    try {
        int Id;
        StrReadInt(Id, S, i);
        double RA;
        StrReadFloat(RA, S, i);
        double DEC;
        StrReadFloat(DEC, S, i);
        double R_;
        StrReadFloat(R_, S, i);

        T->Set(Id, RA, DEC, R_);
    }
    catch(Exception &E) {
        E.Message.Insert(1, "reading tile in row text format: ");
        throw;
    }
}
void  TTile::ReadAssigns(TTile *T, const AnsiString &S, int &i)
{
    if(T == NULL)
        throw EImproperArgument("pointer T shoult point to built object");
    if(i<1 || S.Length()<i)
        throw EImproperArgument("tile not found");

    try {
        StrTravelLabel("Id", S, i);
        StrTravelLabel("=", S, i);
        int Id;
        StrReadInt(Id, S, i);

        StrTravelLabel("phi'", S, i);
        StrTravelLabel("=", S, i);
        double RA;
        StrReadFloat(RA, S, i);

        StrTravelLabel("theta'", S, i);
        StrTravelLabel("=", S, i);
        double DEC;
        StrReadFloat(DEC, S, i);

        StrTravelLabel("R'", S, i);
        StrTravelLabel("=", S, i);
        double R_;
        StrReadFloat(R_, S, i);

        T->Set(Id, RA, DEC, R_);
    }
    catch(Exception &E) {
        E.Message.Insert(1, "reading tile in row text format: ");
        throw;
    }
}

//MÉTODOS PÚBLICOS:

//construye un alista de azulejos
TTile::TTile(void) :
    p_Id(0), p_DEC(0), p_R_(2.9088902912790199E-3),
    RA(0), SkyPointList()
{
}

//contruye un clon de un azulejo
TTile::TTile(TTile *T)
{
    try {
        Copy(T);
    } catch(...) {
        throw;
    }
}

//MÉTODOS DE COPIA

//clona un axulejo
void TTile::Copy(TTile *T)
{
    //el puntero T debería apuntar a un azulejo contruido
    if(T == NULL)
        throw EImproperArgument("pointer T should point to built tail");

    //copia las propiedades
    SkyPointList = T->SkyPointList;
    p_Id = T->p_Id;
    RA = T->RA;
    p_DEC = T->p_DEC;
    p_R_ = T->p_R_;
}
TTile &TTile::operator=(const TTile &T)
{
    //copia las propiedades
    SkyPointList = T.SkyPointList;
    p_Id = T.p_Id;
    RA = T.RA;
    p_DEC = T.p_DEC;
    p_R_ = T.p_R_;

    return *this;
}

//MÉTODOS:

//asigna conjuntamente
void TTile::Set(int Id, double RA, double DEC, double R_)
{
    if(Id < 0)
        throw EImproperArgument("identifier number Id should be nonnegative");
    if(DEC<-M_PI/2 || M_PI/2<DEC)
        throw EImproperArgument(AnsiString("declination '")+FloatToStr(DEC)+AnsiString("' should be in [-pi/2, pi/2]"));
    if(R_ <= 0)
        throw EImproperArgument(AnsiString("radio '")+FloatToStr(R_)+AnsiString("' should be upper zero"));

    p_Id = Id;
    RA = RA;
    p_DEC = DEC;
    p_R_ = R_;
}

//mueve el azulejo al punto indicado
void TTile::Move(double t_RA, double DEC)
{
    //el azimut theta debe estar en [-pi/2, pi/2]
    if(DEC<-M_PI/2 || M_PI/2<DEC)
        throw EImproperArgument(AnsiString("azimut '")+FloatToStr(DEC)+AnsiString("' should be in [-pi/2, pi/2]"));

    //asigna los nuevos valores
    RA = t_RA;
    p_DEC = DEC;
}

//segrega los puntos de cielo del catálogo en el azulejo incluyendo
//aquellos que estén en el interior del círculo (RA, DEC, 1.2*R_)
void TTile::Segregate(TSkyPointList *SPL)
{
    //el putero SPL debería apuntar a una lista de puntos de cielo contruida
    if(SPL == NULL)
        throw EImproperArgument("pointer SPL should point to built sky point list");

    TSkyPoint *SP;
    TVector3D C, P;

    //inicializa la lista de segregados
    SkyPointList.Clear();

    //traduce el centro del zulejo a coordenadas cartesianas
    SphericToCartesian(C.x, C.y, C.z, getDEC(), RA, 1);

    //por cada punto de cielo de la lista
    for(int i=0; i<SPL->getCount(); i++) {
        //apunta el punto de cielo indicado para facilitar su acceso
        SP = SPL->Get(i);
        //traduce el punto de cielo a coordenadas cartesianas
        SphericToCartesian(P.x, P.y, P.z, SP->getDEC(), SP->getRA(), 1);
        //si el punto de cielo está en el tail
        if(Angle(C, P) <= getR_()*1.2)
            SkyPointList.Add(SP); //añade el punto de cielo
    }
}

//--------------------------------------------------------------------------
//TTileList
//--------------------------------------------------------------------------

//PROPIEDADES EN FORMATO TEXTO:

AnsiString TTileList::getTableText(void)
{
    AnsiString S;

    //añade la cabecera
    S = TTile::GetLabelsRow()+AnsiString("\r\n");
    for(int i=0; i<getCount(); i++)
        S += Items[i]->getRowText()+AnsiString("\r\n");

    return S;
}

void TTileList::setTableText(const AnsiString &S)
{
    //descompone la cadena en lineas
    TStringList *SL = new TStringList();
    SL->setText(S);

    //la lista de azulejos debería contener la linea de identificadores
    if(SL->getCount() < 1)
        throw EImproperArgument("row identifiers not found");

    //la primera linea debería tener los identificadores de los campos
    //de la lista de azulejos
    if(StrNotHasSameWords(SL->Strings[0], TTile::GetLabelsRow()))
        throw EImproperArgument("row identifiers not found");

    //variable tampón
    TItemsList<TTile*> LT;
    //variables auxiliares
    TTile *T;

    //COMPRUEBA LA SINTAXIS DE LAS LÍNEAS DE TEXTO:

    try {
        //por cada linea de texto a partir de la segunda
        for(int i=1; i<SL->getCount(); i++) {
            //contruye un azulejo
            T = new TTile();
            //asigna la linea de texto al azulejo
            T->setRowText(SL->Strings[i]);
            //añade el azulejo a la lista
            LT.Add(T);
        }
    } catch(...) {
        throw;
    }

    //ASIGNA LOS NUEVOS VALORES:

    //destruye los objetos originales
    Clear();
    //traslada los azulejos a la lista
    for(int i=0; i<LT.getCount(); i++)
        Add(LT[i]);

    //libera la memoria dinámica
    delete SL;
}

//MÉTODOS PÚBLICOS:

//construye una lista de azulejos
TTileList::TTileList(int Capacity) :
    TPointersList<TTile>(Capacity,
                         TTile::CompareQ_s,
                         NULL, NULL,
                         TTile::PrintRow, NULL)//, TTile::ReadRow)
{
}

//construye un número de azulejos
void TTileList::Build(int N)
{
    //el número de azulejos N debe ser mayor que cero
    if(N < 1)
        throw EImproperArgument("tails number N should be upper zero");

    //repite N veces
    for(int i=1; i<=N; i++)
        //añade un nuevo azulejo
        Add(new TTile());
}

//MÉTODOS:

//segrega los puntos de cielo del catálogo en los azulejos de una lista
void TTileList::Segregate(TSkyPointList *SPL)
{
    //el putero SPL debería apuntar a una lista de puntos de cielo contruida
    if(SPL == NULL)
        throw EImproperArgument("pointer SPL should point to built sky point list");

    for(int i=0; i<getCount(); i++)
        Items[i]->Segregate(SPL);
}

//numera los azulejos
void TTileList::Reidentify(void)
{
    //por cada azulejo de la lista
    for(int k=0; k<getCount(); k++)
        Items[k]->setId(k + 1);
}
/*
//guarda los azulejos en el directorio correspondiente
void TTileList::SaveToFiles(AnsiString FolderName)
{
    AnsiString FileName;

    //contruye el directorio en caso necesario
    mkdir(FolderName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    //por cada azulejo de la lista
    for(int k=0; k<getCount(); k++) {
        //contruye el nombre del archivo
        FileName = FolderName+AnsiString("/tile ")+IntToStr(k+1)+AnsiString(".txt");
        //guarda las propiedades del azulejo
        StrWriteToFile(FileName, Items[k]->getAssignsText());
    }
}*/

//MÉTODOS DE INTERFAZ:
/*#
//imprime los azulejos en una caja de lista
void TTileList::Print(QListWidget *LB)
{
    //el puntero LB debería apuntar a una caja de lista construida
    if(LB == NULL)
        throw EImproperArgument("pointer LB should point to built list box");

    LB->clear();
    QString QS;
    for(int i=0; i<getCount(); i++) {
        QS = Items[i]->getRowText().c_str();
        LB->addItem(QS);
    }
}
//imprime los azulejos en un memorando
void TTileList::Print(QTextEdit *M)
{
    //el puntero M debería apuntar a un memorando construido
    if(M == NULL)
        throw EImproperArgument("pointer M should point to built memo");

    M->clear();
    QString QS = TTile::GetLabelsRow().c_str();
    M->append(QS);
    for(int i=0; i<getCount(); i++) {
        QS = Items[i]->getRowText().c_str();
        M->append(QS);
    }
}
*/
//---------------------------------------------------------------------------

} //namespace Models

//--------------------------------------------------------------------------

