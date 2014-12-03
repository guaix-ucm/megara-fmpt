//---------------------------------------------------------------------------
//Archivo: Actuator.h
//Contenido: actuador de un posicionador
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "Constants.h"
#include "Cilinder.h"

//##include <QColor>

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//###########################################################################
//TKnowledgeDegree
//###########################################################################

//grado de conocimiento de las posiciones angulares de los ejes
//      kdPre: se conocen de forma precisa
//      kdApp: se conocen de forma aproximada
//      kdUnk: se desconoce la posición de algún eje
enum TKnowledgeDegree {kdPre, kdApp, kdUnk};

void  StrPrintKnowledgeDegree(AnsiString& S, TKnowledgeDegree kd);
void  StrReadKnowledgeDegree(TKnowledgeDegree& kd,
                                       const AnsiString &S, int &i);
AnsiString KnowledgeDetgreeToStr(TKnowledgeDegree kd);
TKnowledgeDegree StrToKnowledgeDegree(const AnsiString& S);

//###########################################################################
//TPorpose
//###########################################################################

//propósito de SPM
//      pAll: asignación de posicionadores a puntos a posicionadores;
//      pGen: generación de un programa de movimiento;
//      pVal: validación de un programa de movimineto;
//      pExe: ejecución de un programa de movimineto.
enum TPorpose {pAll, pGen, pVal, pExe};

void  StrPrintPorpose(AnsiString& S, TPorpose p);
void  StrReadPorpose(TPorpose& p, const AnsiString &S, int &i);
AnsiString PorposeToStr(TPorpose p);
TPorpose StrToPorpose(const AnsiString& S);

//###########################################################################
//TActuator
//###########################################################################

//predeclara la clase TRoboticPositioner para poder definir la lista de punteros
//a posicionadores adyacentes. Conviene definir la lista de posicionadores
//adyacentes y no de actuadores para facilitar el acceso a los posicionadores
//y no solo a los actuadores.
class TRoboticPositioner;

//clase actuador de posicionador
class TActuator : public TCilinder {
protected:
    //COMPONENTES DE SPM:

    double __SPMrec;

    double __SPMsta;
    double __SPMdyn;
    double __SPMmin;
    double __SPMoff;

    //VALOR DE SPM PARA CADA OCASIÓN:

    //------------------------------------------------------------------
    //PROPIEDADES DE ESTADO:

    TKnowledgeDegree __PAkd;
    TPorpose __Porpose;

    int __Id;

    //COTAS ÚTILES:

    double __r_min;
    double __r_saf;
    double __r_2saf;
    double __theta___2saf;
    double __theta___3saf;
    double __theta_2rad;

    //PROPIEDADES DE ÁREA:

    double __r_3maxnom;

    double __Sc;
    double __Sw;
    double __Sp;
    double __Ss;
    double __Se;
    double __Re;

    double __Spt;
    double __Set;
    double __Ret;

    //##################################################################
    //PROPIEDADES EN FORMATO TEXTO:
    //##################################################################

    //COMPONENTES DE SPM EN FORMATO TEXTO:

    //VALOR DE SPM PARA CADA OCASIÓN EN FORMATO TEXTO:

    //------------------------------------------------------------------
    //PROPIEDADES DE ESTADO EN FORMATO TEXTO:

    //------------------------------------------------------------------
    //COTAS ÚTILES EN FORMATO TEXTO:

    //------------------------------------------------------------------
    //PROPIEDADES DE ÁREA EN FORMATO TEXTO:

    //------------------------------------------------------------------
    //PROPIEDADES GRÁFICAS EN FORMATO TEXTO:

    AnsiString __Label;

    //------------------------------------------------------------------
    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    //##################################################################
    //MÉTODOS PRIVADOS:
    //##################################################################

    //------------------------------------------------------------------
    //MÉTODOS DE ASIMILACIÓN:

    //asigna el SPM adecuado para (PAkd, Porpose)
    void AssignSPM(void);

public:
    //##################################################################
    //PROPIEDADES PÚBLICAS:
    //##################################################################

    //------------------------------------------------------------------
    //PROPIEDADES DE SEGURIDAD:

    //COMPONENTES DE SPM:

    //SPM para absorber el error de recuperación aproximada
    //debe ser no negativo
    //valor por defecto: (0.3*rbs*r_max + 0.3*Arm->rbs*Arm->Ra)*1 mm
    double getSPMrec(void) const {return __SPMrec;}
    void setSPMrec(double);

    //SPM para absorber el error mecánico y numérico
    //debe ser no negativo
    //valor por defecto: MEGARA_SPMsta mm
    double getSPMsta(void) const {return __SPMsta;}
    void setSPMsta(double);
    //SPM para absorber el desfase temporal de los posicionadores
    //y  las variaciones de velocidad debidas a los motores paso a paso
    //debe ser no negativo
    //valor por defecto: (0.3*rbs*r_max + 0.3*Arm->rbs*Arm->Ra)*1 +
    //                   + (rbs*r_max + Arm->rbs*Arm->Ra) mm
    double getSPMdyn(void) const {return __SPMdyn;}
    void setSPMdyn(double);
    //SPM para absorber la incertidumbre del salto mínimo
    //durante la programación
    //debe ser no negativo
    //valor por defecto: MEGARA_SPMsta mm
    double getSPMmin(void) const {return __SPMmin;}
    void setSPMmin(double);
    //SPM para absorver las aproximaciones producidas
    //por corrección del offset
    //debe ser no negativo
    //valor por defecto: 0.001*MEGARA_rmax + 0.1 mm
    double getSPMoff(void) const {return __SPMoff;}
    void setSPMoff(double);


    //Valores recomendados para las componentes de SPM
    //(de un actuador determinado):
    //      SPMrec = (CMF.vmaxabs1*rbs*r_max + CMF.vmaxabs2*Arm->rbs*Arm->Ra)*
    //              *Tstop_em mm
    //      SPMsta = Eo*r_max + Ep mm
    //      SPMdyn = (CMF.vmaxabs1*rbs*r_max + CMF.vmaxabs2*Arm->rbs*Arm->Ra)*
    //              *Tshif_em +
    //              + (rbs*r_max + Arm->rbs*Arm->Ra) mm
    //      SPMmin = SPMsta mm
    //      SPMoff = PAem*rmax + Pem mm

    //Donde se emplean las siguientes propiedades del posicionador:
    //      CMF.vmaxabs1: velócidad máxima absoluta del eje 1 en pasos/s.
    //      rbs: radianes por paso del eje 1.
    //      r_max: radio apical del actuador.
    //      CMF.vmaxabs1: velócidad máxima absoluta del eje 2 en pasos/s.
    //      Arm->rbs: radianes por paso del eje 2.
    //      Arm->Ra: radio apical del brazo.
    //      rmax: coordenada radial máxima de P3 en S0.
    //      L03max: distancia máxima entre P0 y P3.

    //Donde se emplean las siguientes propiedades, cuyos valores deben ser
    //proporcionados por el fabricante:
    //      Tstop_em: tiempo máximo entre el último almacenamiento
    //              de la posición y la detención de ambos ejes.
    //              Valor por defecto para MEGARA: 1 ms.
    //      Eo: margen de error de orientación por mecanizado.
    //              Valor por defecto para MEGARA: 0.001 rad.
    //      Ep: margen de error de posición por mecanizado.
    //              Valor por defecto para MEGARA: 0.01 mm.
    //      Tshif_em: desfase temporal entre posicionadores en movimeinto.
    //              Valor por defecto para MEGARA: 1 ms.
    //      PAem: margen de error de orientación de S0.
    //              Valor por defecto para MEGARA: 0.001 rad.
    //      Pem: margen de error de apuntado de S0.
    //              Valor por defecto para MEGARA: 0.1 mm.

    //Nótese que CMF.vmaxabs1*rbs*r_max es la máxima velocidad
    //lineal del ápice del brazo debido a la rotación del eje 1.

    //Nótese que CMF.vmaxabs2*Arm->rbs*Arm->Ra es la máxima velocidad
    //lineal del ápice del brazo debido a la rotación del eje 2.

    //Nótese que SPMoff tiene dos componentes:
    //    * PAem*rmax: es el offset máximo producido por
    //      error de orientación de S0.
    //    * Pem:offset máximo producido por error de apuntado de S0.
    //La primera componente depende de la ubicación del posicionador
    //en el Fiber MOS. No se debe confundir P0Vcon POV.
    //Como no hay valor por defecto para POVmax, se asigna MEGARA_P0Vmax,
    //como si hbiera un único posicionador centrado en el origen.

    //VALORES DE SPM PARA CADA OCASIÓN:

    //Valores de SPM de precisión para el brazo del posicionador,
    //cuando la posición de ambos ejes se conoce con precisión
    //(PAkd == kdPre):

    //SPM para ejecución:
    //      SPMexe_p = SPMsta + SPMdyn
    double getSPMexe_p(void) const;
    //SPM para validación:
    //      SPMval_p = SPMexe_p + SPMmin
    double getSPMval_p(void) const;
    //SPM para programación:
    //      SPMgen_p = SPMval_p + SPMmin
    double getSPMgen_p(void) const;
    //SPM para asignación:
    //      SPMall_p = SPMgen_p + SPMoff
    double getSPMall_p(void) const;

    //Valores de SPM de aproximación para el brazo del posicionador,
    //cuando la posición de ambos ejes se conoce de forma aproximada
    //(PAkd == kdApp):

    //SPM para ejecución:
    //      SPMexe_a = SPMrec + SPMexe_p
    double getSPMexe_a(void) const;
    //SPM para validación:
    //      SPMval_a = SPMexe_a + SPMmin
    double getSPMval_a(void) const;
    //SPM para programación:
    //      SPMgen_a = SPMval_a + SPMmin
    double getSPMgen_a(void) const;
    //SPM para asignación:
    //      SPMall_a = SPMgen_a + SPMoff
    double getSPMall_a(void) const;

    //Valores de SPM de aproximación para el muro del posicionador,
    //cuando la posición de algún eje es desconocida
    //(PAkd == kdUnk):

    //SPM para todos los casos:
    //      SPMsta

    //------------------------------------------------------------------
    //PROPIEDADES DE ESTADO:

    //grado de conocimiento de las posiciones de los ejes
    //debe ser uno de los siguientes valores:
    //      kdPre: se conocen de forma precisa
    //      kdApp: se conocen de forma aproximada
    //      kdUnk: se desconoce la posición de algún eje
    //valor por defecto_ kdPrecise
    TKnowledgeDegree getPAkd(void) const {return __PAkd;}
    void setPAkd(TKnowledgeDegree);
    //propósito con el que se va a usar el posicionador
    //debe ser uno de los siguientes valores:
    //      pAll: asignación
    //      pPro: programación
    //      pVal: validación
    //      pExe: ejecución
    //valor por defecto: pAll
    TPorpose getPorpose(void) const {return __Porpose;}
    void setPorpose(TPorpose);

    //si PAkd == kdPre,
    //      si Porporse == pAll
    //              Arm->SPM = SPMall_p
    //      si Porporse == pPro
    //              Arm->SPM = SPMgen_p
    //      si Porporse == pVal
    //              Arm->SPM = SPMval_p
    //      si Porporse == pexe
    //              Arm->SPM = SPMexe_p
    //si PAkd == kdApp,
    //      si Porporse == pAll
    //              Arm->SPM = SPMall_a
    //      si Porporse == pPro
    //              Arm->SPM = SPMgen_a
    //      si Porporse == pVal
    //              Arm->SPM = SPMval_a
    //      si Porporse == pexe
    //              Arm->SPM = SPMexe_a
    //si PAkd == kdUnk,
    //      Barrier->SPM = SPMsta

    //identificador del posicionador
    //debe ser no negativo
    //el número 0 indica indefinición
    int getId(void) const {return __Id;}
    void setId(int);

    //El número de identificación debe ser una propiedad del actuador
    //para que los actuadores adyacentes puedan ser identificados.

    //lista de posicionadores de RPs lo bastante cerca
    //para que puedan colisionar con el brazo
    //valor por defecto:
    //      Adjacents.Capacity = 6;
    //      Adjacents.Compare = TRoboticPositioner::CompareIds;
    //      Adjacents.Print = TRoboticPositioner::PrintId;
    TItemsList<TRoboticPositioner*> Adjacents;

    //los actuadores adyacentes serán determinados mediante
    //el método TLisTRoboticPositioners::DetermineAdyacents()

    //indica si el brazo del actuador será tenido en cuenta
    //en la determinación de distancias y colisiones
    //valor por defecto: true
    bool Pending;

    //La propiedad Pending será usada en los métodos:
    //      ThereIsCollisionWithPendingAdjacent
    //      SearchCollindingPendingAdjacent
    //con objeto de evitar aplicar el método de determinación de colisión
    //más de una vez entre cada par de actuadores.
    //Deberá determinarse el estado de colisión mediante el método:
    //      TActuator::ThereIsCollision
    //cuando:
    //    - la bandera de determinación de colisión pendiente (Pending)
    //      sea true.

    //estado de colisión del actuador con alguno de sus adyacentes
    //valor por defecto: false
    bool Collision;

    //------------------------------------------------------------------
    //COTAS ÚTILES:

    //radio de la envolvente del contorno del brazo
    //cuando el eje2 está en el origen
    //valor por  defecto: el mismo que r_max
    double getr_min(void) const {return __r_min;}
    //Radio de la frontera segura.
    //Radio de la circunferencia descrita por el punto de Arm->Contour
    //más alejado del centro del posicionador cuando
    //el brazo está retraido en el linde de la zona segura.
    //      r_saf = Min{D[i] - r_max[i] - SPM[i]} - SPM;
    //donde:
    //  D[i] es la distancia al adyacente i;
    //  r_max[i] es el radio del contorno del adyacente i;
    //  SPM[i] es el margen de seguridad del adyacente i.
    //  SPM es el margen de seguridad de este posicionador.
    //valor por defecto: el mismo que r_max
    //donde Arm->SPM es el SPM de generación dado por defecto.
    double getr_saf(void) const {return __r_saf;}

    //Radio seguro de P2.
    //Radio de la circunferencia descrita por P2 cuando
    //el brazo está retraido en el linde de la zona segura.
    //Valor por defecto: L02max
    double getr_2saf(void) const {return __r_2saf;}
    //Posición angular de P2 respecto de S3 en radianes
    //a partir de la cual el brazo está dentro del área de seguridad.
    //Valor por defecto: M_PI rad
    double gettheta___2saf(void) const {return __theta___2saf;}
    //Posición angular de P3 respecto de S3 en radianes
    //a partir de la cual el brazo está dentro del área de seguridad.
    //Valor por defecto: M_PI rad
    double gettheta___3saf(void) const {return __theta___3saf;}

    //Posición angular de P2 respecto de S1 en radianes
    //a partir de la cual el brazo puede ser retraido
    //linealmente hasta la zona segura sin que theta_1
    //llegue a ser menor que Max(0, theta_1min).
    //Valor por defecto: Max(0, theta_1min) + M_PI/2 rad
    double gettheta_2rad(void) const {return __theta_2rad;}

    //ADVERTENCIA: el controlador puede estar concebido para
    //trabajar solamente con valores no negativos de p_1,
    //de modo que theta_2rad es calculado pensando en que theta_1
    //no podrá rebasar Max(0, theta_1min).

    //------------------------------------------------------------------
    //PROPIEDADES DE ÁREA:

    //radio del dominio de P3
    //debe ser un valor mayor que cero
    //valor por defecto: r_3max mm
    double getr_3maxnom(void) const {return __r_3maxnom;}
    void setr_3maxnom(double);

    //Dado que las propiedades de área son calculadas
    //para el caso ideal, deberían tomarse los valores nominales
    //para el cálculo de las mismas. Como las propiedades de las que
    //dependen las propiedades de área son solamente:
    //      r = r_3maxnom y
    //      N = Adjacents.Count.
    //el único valor nominal que debe darse es el de r_3max.

    //Los parámetros Sp, Se y Re son calculados analíticamente
    //para el caso en que los posicionadores adyacentes cumplen:
    //       radio del dominio igual;
    //       equidistantes 4*L*cos(M_PI/6).

    //En el caso real, es posible que:
    //       - la distancia entre posicionadores no es exactamente
    //       4*L*cos(PI/6);
    //       - el área de seguridad y el área no invasiva se vean ligeramente
    //       reducidas a causa del SPM.

    //Para facilitar la notación se sustituirá:
    //       N = Adjacents.Count;
    //       r = r_3maxnom.

    //área del círculo de radio r
    //       Sc = M_PI*r*r
    double getSc(void) const {return __Sc;}
    //área de una porción de seis del círculo de radio r
    //       Sw = Sc/6
    double getSw(void) const {return __Sw;}

    //área participativa de una porción triangular
    //del exágono cinrcunscrito en la circunferencia de radio r
    //       Sp = r*r*aux/2
    double getSp(void) const {return __Sp;}
    //área de uno de los segmentos de circunferencia
    //en las inmediaciones del exágono circunscrito
    //en la circunferencia de radio r
    //      Ss = Sw - Sp
    double getSs(void) const {return __Ss;}
    //área exclusiva de una porción exagonal
    //del círculode radio r
    //      Se = Sp - Ss
    double getSe(void) const {return __Se;}
    //ratio entre área exclusiva y el área participativa
    //      Re = Se/Sp
    double getRe(void) const {return __Re;}

    //calcula el área participativa total
    //      __Spt = (6 - N)*Sw + N*Sp;
    //            = r*r*(M_PI + N*(sqrt(0.75) - M_PI/3)/2);
    double getSpt(void) const {return __Spt;}
    //calcula el área exclusiva total
    //      __Set = (6 - N)*Sw + N*Se;
    //            = r*r*(sqrt(0.75) - M_PI/6);
    double getSet(void) const {return __Set;}
    //calcula el ratio entre Set y Spt
    //      __Ret = Set/Spt;
    //            = 2 - M_PI/(3*sqrt(0.75))
    double getRet(void) const {return __Ret;}

    //ADVERTENCIA: cuando cambia el radio del dominio del posicionador
    //o de alguno de los adyacentes, o la distancia entre posicioandores
    //no es igual a 4*L*cos(M_PI/6), las propiedades de área pueden no
    //corresponderse con la realidad.

    //ADVERTENCIA: cuando r_3min!=0 se tiene que parte del área excusiva
    //no forma parte del dominio del posicionador, con lo que habría que
    //distinguir dos tipos de áreas exclusivas:
    //      área exclusiva total;
    //      área exclusiva accesible.

    //------------------------------------------------------------------
    //PROPIEDADES GRÁFICAS:
/*#
    //color con que son contruidos los posicionadores
    //valor por defecto: clGray
    static QColor DefaultColor;

    //color del cilindro del posicionador
    //valor por defecto: DefaultColor
    QColor ColorCilinder;
    //color del brazo del posicionador
    //valor por defecto: DefaultColor
    QColor ColorArm;
    //color del límite del dominio del punto P3
    //valor por defecto: DefaultColor
    QColor ColorLimitDomainP3;
    //color del límite del dominio de maniobra
    //valor por defecto: DefaultColor
    QColor ColorLimitDomainManeuvering;

    //color delas barreras en colisión
    //valor por defecto: clRed
    QColor ColorCollision;
*/
    //indica si debe dibujarse el cuerpo del posicionador
    //valor por defecto: true
    bool PaintBody;
    //indica si debe dibujarse el límite del dominio del punto P3
    //valor por defecto: false
    bool PaintLimitDomainP3;
    //indica si debe dibujarse el límite del dominio de maniobra
    //valor por defecto: false
    bool PaintLimitDomainManeuvering;

    //indica si el posicionador está seleccionado
    //valor por defecto: false
    bool Selected;

    //##################################################################
    //PROPIEDADES EN FORMATO TEXTO:
    //##################################################################

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
    AnsiString getSPMoffText(void) const;
    void setSPMoffText(const AnsiString&);

    //VALORES DE SPM PARA CADA OCASIÓN EN FORMATO TEXTO:

    AnsiString getSPMexe_pText(void) const;
    AnsiString getSPMval_pText(void) const;
    AnsiString getSPMgen_pText(void) const;
    AnsiString getSPMall_pText(void) const;

    AnsiString getSPMexe_aText(void) const;
    AnsiString getSPMval_aText(void) const;
    AnsiString getSPMgen_aText(void) const;
    AnsiString getSPMall_aText(void) const;

    //------------------------------------------------------------------
    //PROPIEDADES DE ESTADO EN FORMATO TEXTO:

    AnsiString getPAkdText(void) const;
    void setPAkdText(const AnsiString&);
    AnsiString getPorposeText(void) const;
    void setPorposeText(const AnsiString&);

    AnsiString getIdText(void) const; void setIdText(const AnsiString&);

    //Adjacents.Text

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

    //PROPIEDADES GRÁFICAS EN FORMATO TEXTO:

    //etiqueta de identificación del objeto
    //debe ser una cadena imprimible
    //valor por defecto: ""
    AnsiString getLabel(void) const {return __Label;}
    void setLabel(const AnsiString&);

    AnsiString getDefaultColorText(void) const;
    void setDefaultColorText(const AnsiString&);

    AnsiString getColorCilinderText(void) const;
    void setColorCilinderText(const AnsiString&);
    AnsiString getColorArmText(void) const;
    void setColorArmText(const AnsiString&);
    AnsiString getColorLimitDomainP3Text(void) const;
    void setColorLimitDomainP3Text(const AnsiString&);
    AnsiString getColorLimitDomainManeuveringText(void) const;
    void setColorLimitDomainManeuveringText(const AnsiString&);

    AnsiString getPaintBodyText(void) const;
    void setPaintBodyText(const AnsiString&);
    AnsiString getPaintLimitDomainP3Text(void) const;
    void setPaintLimitDomainP3Text(const AnsiString&);
    AnsiString getPaintLimitDomainManeuveringText(void) const;
    void setPaintLimitDomainManeuveringText(const AnsiString&);

    AnsiString getSelectedText(void) const;
    void setSelectedText(const AnsiString&);

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
    //conjunto de propiedades gráficas en formato texto
    //en formato asignaciones de texto
    AnsiString getGraphicsText(void) const;

    //conjunto de otras propiedades en formato texto
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

    //##################################################################
    //MÉTODOS PÚBLICOS:
    //##################################################################

    //------------------------------------------------------------------
    //MÉTODOS ESTÁTICOS:

    //compara los identificadores de dos actuadores
    static int  CompareIds(TActuator *A1, TActuator *A2);

    //imprime el identificador de un actuador
    static void  PrintId(AnsiString &S, TActuator *A);

    //Los métodos estáticos:
    //      CompareIds
    //      PrintId
    //serán apuntados en la lista de posicionadores adyacentes
    //para permitir su ordenacíon en función de los identificadores
    //y la impresión de los mismos.

    //lee una instancia actuador en una cadena
    static void  ReadInstance(TActuator* &A,
                                        const AnsiString& S, int &i);

    //obtiene las etiquetas de las propiedades de origen
    //("Id", "x0", "y0", "thetaO1") al final de una cadena de texto
    //en formato fila de texto
    static AnsiString GetOriginsLabelsRow(void);
    //atraviesa las etiquetas de las propiedades de origen
    //("Id", "x0", "y0", "thetaO1")
    //en formato fila de texto
    static void  TravelOriginsLabelsRow(const AnsiString&, int&);

    //imprime los valores de las propiedades de orien de un posicionador
    //(Id, x0, y0, thetaO1) al final de una cadena de texto
    //en formato fila de texto
    static void  PrintOriginsRow(AnsiString&, TActuator*);
    //lee los valores de las propiedades de orien de un posicionador
    //(Id, x0, y0, thetaO1) desde la posición indicada de una cadena
    //de texto, en formato fila de texto
    static void  ReadOriginsRow(TActuator* &FP,
                                          const AnsiString& S, int &i);

    //obtiene las etiquetas de las propiedades de posición
    //("Id", "x3", "y3") al final de una cadena de texto
    //en formato fila de texto
    static AnsiString GetPositionP3LabelsRow(void);
    //atraviesa las etiquetas de las propiedades de posición
    //("Id", "x3", "y3")
    //en formato fila de texto
    static void  TravelPositionP3LabelsRow(const AnsiString&, int&);

    //imprime los valores de las propiedades de posición de un posicionador
    //(Id, x3, y3) al final de una cadena de texto
    //en formato fila de texto
    static void  PrintPositionP3Row(AnsiString&, TActuator*);
    //lee los valores de las propiedades de posición de un posicionador
    //(Id, x3, y3) desde la posición indicada de una cadena
    //de texto, en formato fila de texto
    static void  ReadPositionP3Row(TActuator* &FP,
                                             const AnsiString& S, int &i);

    //------------------------------------------------------------------
    //MÉTODOS DE CONSTRUCCIÓN, COPIA Y DESTRUCCIÓN:

    //construye un actuador
    //con los valores indicados
    TActuator(int _Id, TDoublePoint _P0, double _thetao_=MEGARA_thetaO1);

    //ADVERTENCIA: para poder contruir clones de actuadores
    //la duplicidad de números de identificación está permitida.
    //El control de duplicidad de identificadores debe llevarse
    //en todo caso mediante métodos. Al fin y al cabo en el mundo real
    //será posible configurar dos posicionadores para que tengan la misma
    //dirección.

    //copia las propiedades de seguridad de un actuador
    void CopySecurity(const TActuator*);
    //copia las propiedades de estado de un actuador
    void CopyStatus(const TActuator*);
    //copia las propiedades límite de un actuador
    void CopyLimits(const TActuator*);
    //copia las propiedades de área de un actuador
    void CopyArea(const TActuator*);
    //copia las propiedades gráficas de un posicioandor
    void CopyGraphics(const TActuator*);

    //copia todas las propiedades de un actuador
    void Clone(const TActuator*);

    //clona un actuador
    TActuator(const TActuator*);

    //destruye un actuador
    ~TActuator();

    //------------------------------------------------------------------
    //MÉTODOS DE ASIMILACIÓN:

    //calcula las propiedades de seguridad:
    //      {r_min, r_saf, r_2saf, theta___2saf, theta___3saf, theta_2rad}
    void CalculateSafeParameters(void);

    //Los métodos para:
    //      determinar la posición relativa;
    //      determinar si hay o no invasión;
    //requieren el cálculo previo de los parámetros de seguridad

    //-------------------------------------------------------------------
    //MÉTODOS DE ASIGNACIÓN CONJUNTA ATÓMICA:

    //asigna las propiedades angulares (theta_1min, theta_1max, theta_1)
    void SetOrientationRadians(double _theta_1min, double _theta_1max,
                               double _theta_1, double _theta_O3o);
    //asigna las propiedades de cuantificación
    void SetQuantification(double _SB1);

    //asigna las propiedades de origen
    void SetOrigins(int _Id, double _x0, double _y0, double _thetaO1);
    //asigna las propiedades de posición
    //si el punto (x3, y3) no está en el dominio del actuador
    //      lanza EImproperArgument
    void SetPositionP3(double x3, double y3);
    //asigna las propiedades de identificación y posición
    //si el punto (x3, y3) no está en el dominio del actuador
    //      lanza EImproperArgument
    void SetPositionP3(int _Id, double x3, double y3);

    //asigna las componentes del SPM
    void SetSPMComponents(double _SPMrec, double _SPMsta,
                          double _SPMdyn, double _SPMmin);//, double _SPMoff);

    //asigna las propiedades de estado (PAkd, Porpose)
    void SetStatus(TKnowledgeDegree _PAkd, TPorpose _Porpose);

    //-------------------------------------------------------------------
    //MÉTODOS PARA ALMACENAR Y RECUPERAR POSICIONES DE LOS ROTORES:

    //apila las posiciones de los rotores
    void Pushthetas(void);
    //restaura las últimas posiciones apuiladas de los rotores
    //si no hay una posisición almacenada para algún rotor:
    //      lanza EImproperCall
    void Restorethetas(void);
    //desempila las últimas posiciones apiladas de los rotores
    //si no hay una posisición almacenada para algún rotor:
    //      lanza EImproperCall
    void Popthetas(void);
    //restaura y desempila las últimas posiciones apiladas de los rotores
    //si no hay una posisición almacenada para algún rotor:
    //      lanza EImproperCall
    void RestoreAndPopthetas(void);

    //ADVERTENCIA: la recuperación de posiciones no estables,
    //cuando está la cuantificación activada, dará lugar
    //al desplazamiento a la posición estable más próxima de cada rotor.

    //-------------------------------------------------------------------
    //MÉTODOS PARA ALMACENAR Y RECUPERAR ESTADOS DE CUNATIFICACIÓN:

    //apila el estado de los cuantificadores
    void PushQuantifys(void);
    //restaura el último estado apilado de los cuantificadores
    //si no hay un estado almacenado para algún quantificador:
    //      lanza EImproperCall
    void RestoreQuantifys(void);
    //desempila el último estado apilado de los cuantificadores
    //si no hay un estado almacenado para algún quantificador:
    //      lanza EImproperCall
    void PopQuantifys(void);
    //restaura y desempila el último estado apilado de los cuantificadores
    //si no hay un estado almacenado para algún quantificador:
    //      lanza EImproperCall
    void RestoreAndPopQuantifys(void);

    //------------------------------------------------------------------
    //MÉTODOS PARA DETERMINAR LA POSICIÓN RELATIVA DEL BRAZO:

    //determina si un ángulo del eje 2 en radianes
    //está fuera del área de seguridad
    bool theta___3IsOutSafeArea(double theta___3);
    //determina si un ángulo del eje 2 en radianes
    //está dentro del área de seguridad
    bool theta___3IsInSafeArea(double theta___3);

    //determina si un ángulo del eje 2 en pasos
    //está fuera del área de seguridad
    bool p___3IsOutSafeArea(double p___3);
    //determina si un ángulo del eje 2 en pasos
    //está dentro del área de seguridad
    bool p___3IsInSafeArea(double p___3);

    //determina si un punto está fuera del área de seguridad
    bool P3IsOutSafeArea(TDoublePoint P);
    //determina si un punto está dentro del área de seguridad
    bool P3IsInSafeArea(TDoublePoint P);

    //determina si el brazo está fuera del área deseguridad
    bool ArmIsOutSafeArea(void);
    //determina si el brazo está dentro del área de seguridad
    bool ArmIsInSafeArea(void);

    //MÉTODOS PARA DETERMINAR EL PUNTO DE INFLEXIÓN PARA UN ÁNGULO DEL EJE 1:

    //Punto de inflexión de la trayectoria radial es
    //aquel en el que theta_1 == Max{theta_1min, 0}.

    //Cuando theta_2 >= Max{theta_1min, 0} + PI/2,
    //el punto de inflexión está en P0.

    //Coordenada radial del punto de inflexión para la pose dada en S1.
    //Pi es el punto de la trayectoria radial de P2 en que:
    //      theta_1 == Max{theta_1min, 0}.
    //si L01 != Arm->L12 lanza una excepción EImproperCall
    double Getr_i(double &theta_1, double theta_2);
    //ángulo del eje 2 respecto de S3 en radianes
    //para el punto de inflexión de la pose dada
    //si L01 != Arm->L12 lanza una excepción EImproperCall
    double Gettheta___2i(double theta_2, double r_i);

    //MÉTODOS DE MOVIMIENTO:

    //mueve el brazo hasta a la posición de seguridad estable más próxima
    void MoveArmToSafePosition(void);

    //------------------------------------------------------------------
    //MÉTODOS PARA CALCULAR DISTANCIAS
    //CON BRAZOS DE POSICIONADORES ADYACENTES:

    //determina la distancia con otro actuador
    double Distance(TActuator*);
    //determina la distancia libre con otro actuador
    //      Df = D - SPM1 - SPM2
    //donde:
    //      Df: distancia libre
    //      SPM1: SPM de este actuador (según PAkd)
    //      SPM2: SPM del otro actuador (según PAkd)
    double DistanceFree(TActuator*);

    //determina la distancia mínima entre el brazo del posicionador
    //y los brazos de los posicionadores adyacentes
    double DistanceWithAdjacent(void);
    //determina la distancia mínima entre el punto P3 del posicionador
    //y los puntos P3 de los posicionadores adyacentes
    double DistanceP3WithAdjacent(void);

    //MÉTODOS PARA DETERMINAR EN QUE ÁREA SE ENCUENTRA UN PUNTO:

    //determina si un punto está fuera del área no invasiva
    bool P3IsOutNoninvasiveArea(TDoublePoint P);
    //determina si un punto está dentro del área invasiva
    bool P3IsInNoninvasiveArea(TDoublePoint P);

    //------------------------------------------------------------------
    //MÉTODOS PARA DETERMINAR LAS COLISIONES
    //CON POSICIONADORES ADYACENTES:

    //determina si hay colisión con un actuador adyacente
    bool ThereIsCollision(const TActuator*);
    //determina si hay colisión con un actuador adyacente
    bool ThereIsCollisionWithAdjacent(void);
    //determina si no hay colisión con un actuador adyacente
    bool ThereIsntCollisionWithAdjacent(void);
    //Busca los posicionadores adyacentes cuyo
    //brazo colisiona con el de este posicionador.
    //        void SearchCollindingAdjacent(TVector<int> &Ids);
    //Busca los posicionadores adyacentes cuyo
    //brazo colisiona con el de este posicionador.
    void SearchCollindingAdjacent(
            TItemsList<TRoboticPositioner*> &Collindings);

    //Determina si hay colisión con un actuador adyacente
    //con evaluación de colisión pendiente.
    bool ThereIsCollisionWithPendingAdjacent(void);
    //Busca los posicionadores adyacentes
    //con evaluación de colisión pendiente
    //cuyo actuador colisiona con el de este posicionador.
    void SearchCollindingPendingAdjacent(
            TItemsList<TRoboticPositioner*> &Collindings);

    /*        //MÉTODOS PARA EL CÁLCULO DE ÁNGULOS DE GIRO PARA RESOLVER COLISIONES:

        //determina los ángulos hay que rotar este posicionador,
        //para que su brazo quede adyacente al segmento indicado
        void TurnSegment(TVector<double> &dts,
                TDoublePoint Pa, TDoublePoint Pb);

        //determina los ángulos hay que rotar este posicionador,
        //para que su brazo quede adyacente al arco indicado
        void TurnArc(TVector<double> &dts,
                TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R);

        //determina los ángulos hay que rotar este posicionador,
        //para que su brazo quede adyacente al brazo indicado
        void TurnArm(TVector<double> &dts, TArmAbstract *Arm_);
*/
    //-------------------------------------------------------------------
    //MÉTODOS GRÁFICOS:

    //indica que parte del posicionador puede ser agarrado
    //en el punto indicado:
    //      3: botón (P2, R2)
    //      2: brazo (PA...)
    //      1: cilindro (P0, L01)
    //      0: niguna;
    int Grab(TDoublePoint P);

    //resalta un color si el posicionador está seleccionado
//#    QColor HighlightIfSelected(QColor);

    //asigna un color al posicionador
//#    void SetAllColors(QColor Color);

    //traza el posicionador con los colores indicados
    //en un trazador de formas
    //#void Paint(TPloterShapes*);
    //traza el posicionador con los colores indicados
    //en un trazador de formas
    //en el modelo simplificado (dos segmentos de P0 a P1 y de P1 a P2)
    //#void PaintSimplified(TPloterShapes*);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // ACTUATOR_H
