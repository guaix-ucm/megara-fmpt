//---------------------------------------------------------------------------
//Archivo: RoboticPositioner.h
//Contenido: posicionador básico (dimensión y posición)
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ROBOTICPOSITIONER_H
#define ROBOTICPOSITIONER_H

#include "Instruction.h"
#include "Actuator.h"
#include "ComposedMotionFunction.h"
#include "ItemsList.h"

//---------------------------------------------------------------------------

using namespace MotionFunctions;

//espacio de nombres de modelos
namespace Models {

//##########################################################################
//TControlMode
//##########################################################################

//Modo de funcionamiento del controlador
//      cmSinc: los gestos programados se ejecutarán inmediatamente;
//      cmAsinc: los gestos programados se ejecutarán al recibir el
//      comando start.
enum TControlMode {cmSinc, cmAsinc};

void  StrPrintControlMode(AnsiString& S,
        TControlMode cm);
void  StrReadControlMode(TControlMode& cm,
        const AnsiString &S, int &i);
AnsiString ControlModeToStr(TControlMode cm);
TControlMode StrToControlMode(const AnsiString& S);

//##########################################################################
//TFaultType
//##########################################################################

//Tipo de fallo del posicionador
//      ftUnk: desconocido;
//      ftSta: estático;
//      ftDyn: dinámico;
enum TFaultType {ftUnk, ftSta, ftDyn};

void  StrPrintFaultType(AnsiString& S,
        TFaultType ft);
void  StrReadFaultType(TFaultType& ft,
        const AnsiString &S, int &i);
AnsiString FaultTypeToStr(TFaultType ft);
TFaultType StrToFaultType(const AnsiString& S);

//##########################################################################
//TRoboticPositioner
//##########################################################################

//La clase TRoboticPositioner permite programar una función de movimiento para
//cada uno de los ejes que deben ser girados para que un posicionador
//realice un determinado gesto.
//Las funciones de movimiento serán programadas partiendo de la posición inicial
//para que p_1 y p___3 alcancen la posición final indicada.

//Cuando en el gesto interviene un único eje, es posible ajustar
//el intervalo temporal de desplazamiento de la función, así como
//el resto de parámetros libres de la función,
//y cuando en el gesto intervienen los dos ejes, es posible sincronizar
//los periodos de las funciones de movimeinto de los ejes.
//Las funciones de movimiento se programarán en pasos.

//Son importantes dos cuestiones:
//      1. Debe programar el movimiento de cada eje de manera independiente,
//      de modo que al mover un eje, el otro quede libre para ser desplazado
//      durante la programación;
//      de este modo no hace falta implementar una función Displace(t, qt).
//      2. Cada gesto se inicia en el instante t=0, de modo que se facilita
//      la realización aislada, lo cual simplifica el algoritmo de busqueda;
//      cuando haya que concatenar varios gestos, se sincronizarán con una
//      variable temproal externa que indique el instante en que debe iniciarse.

//Clase TRoboticPositioner; descripción:
//
//Un objeto de la clase TRoboticPositioner es contruido para un posicionador
//al que queda adscrito.
//Entonces un gesto puede ser programado para el posicionador, obteniendo
//la función de movimeinto compuesto para ambos ejes, mediante:
//      - los métodos que programan gestos;
//      - asignando el comando en formato de cadena de bytes;
//Una vez programado el gesto puede ser ejecutado mediante los métodos
//de movimeinto tantas veces como se desee.
//Finalmente debe indicarse que el gesto ha finalizado para que el controlador
//no sea sensible al comando start ("ST").
//
//Nótese que no es necesario cambiar las propiedades del gesto de manera
//independiente.
//
//Durante la ejecución del porgrama las instrucciones deben ser traducidas
//a cadenas de bytes que deberán ser entramadas para su envío, de modo que
//conviene poder programar los gestos a partir de dichas cadenas.
//
//Durante la programación habrá que programar gestos suyos comandos no
//serán entramados, de modo que conviene poder programarlos directamente.
//
//Por último la configuración del gesto puede ser editada mediante
//formularios, por lo que conviene poder acceder a la escritura de
//cada parámetro por separado para simplificar el tratamiento.

//Lista de gestos fundamentales:
//        None, //no gira ningún eje
//        M1, //gira el cilindro hasta p_1
//        M2, //gira el brazo hasta p___3
//        M, //gira el brazo y el cilindro hasta (p_1, p___3),

//Lista de gestos que requieren un cálculo previo:
//        TurnCilinderTotheta_1, //gira el cilindro hasta theta_1
//        TurnCilinderTotheta_2, //gira el cilindro hasta theta_2
//        TurnCilinderTotheta_3, //gira el cilindro hasta theta_3
//
//        TurnArmTotheta_2, //gira el brazo hasta theta_2
//        TurnArmTotheta_3, //gira el brazo hasta theta_3
//        TurnArmTor_2, //gira el brazo hasta r_2
//        TurnArmTor_3, //gira el brazo hasta r_3
//        TurnArmToSafeArea, //gira el brazo hasta el área de seguridad
//        TurnArmToOrigin; //gira el brazo hasta el origen de coordenadas

//        GoRadialTor_2, //desplaza P2 radialmente hasta r_2
//        GoRadialToSafeArea, //desplaza P2 radialmente hasta que el brazo
//                //quede dentro del área de seguridad
//                //o hasta donde se pueda
//        GoRadialToOrigin, //desplaza P2 radialmente hasta P0
//                //o hasta donde se pueda

//        GoDirectlyToPolarP_3, //va directamente a (r_3, theta_3)
//        GoDirectlyToCartesianP_3, //va directamente a (x_3, y_3)
//        GoDirectlyToCartesianP3, //va directamente a (x3, y3)

//ADVERTENCIA: para mantener el formalismo estricto se define TFibeRPositioner
//con una propiedad de la clase TActuator, evitándose intergar las propiedades
//de TActuator en la clase TRoboticPositioner. De este odo se obtienen algunas
//ventajas:
//- Las propiedades de inicialización de TActuator pueden ser filtradas antes de
//  cotnstruir el actuador.
//- El método Copy de TActuator no tiene que ser redefinido, manteniendose
//  la encapsulación de las propiedades del actuador.

//clase posicionador
class TRoboticPositioner {
protected:
        //PROPIEDADES MECÁNICAS:

        TActuator *__Actuator;

        //TOLERANCIAS:

        double __Eo;
        double __Ep;

        double __Tstop;
        double __Tshiff;

        //PROPIEDADES DE ESTADO:

        double __FaultProbability;

        //------------------------------------------------------------------
        //PROPIEDADES EN FORMATO TEXTO:

        //PROPIEDADES MECÁNICAS EN FORMATO TEXTO:

        //PROPIEDADES DE CONTROL EN FORMATO TEXTO:

        //TOLERANCIAS EN FORMATO TEXTO:

        //PROPIEDADES DE ESTADO EN FORMATO TEXTO:

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

public:
        //PROPIEDADES ESTÁTICAS:

        //lista de objetos construidos
        static TItemsList<TRoboticPositioner*> Builts;

        //La lista de objetos contruidos es útil para:
        //- determinar los identificadores que se encuentran repetidos;
        //- obtener identificadores unívocos;
        //- controlar la integridad de memoria.

        //El objeto estático Builts debe ser contruido
        //antes de invocar al contructor mediante:
        //      TItemList<TRoboticPositioner*> TRoboticPositioner::Builts;
        //para ello esta sentencia deberá ser insertada en el mismo
        //archivo donde se invoca al constructor, antes de la invocación.
        //Si se insertase en este archivo o en su correspondiente .cpp,
        //sería invocado después de llamar al constructor.

        //PROPIEDADES MECÁNICAS:

        //actuador del posicionador
        TActuator *getActuator(void) const {return __Actuator;}

        //PROPIEDADES DE CONTROL:

        //función de movimiento compuesta
        TComposedMotionFunction CMF;

        //TOLERANCIAS:

        //margen de error de orientación en S0
        //      Valor por defecto: MEGARA_Eo rad
        double getEo(void) const {return __Eo;}
        void setEo(double);
        //margen de error de posición en S0
        //      Valor por defecto: MEGARA_Ep mm
        double getEp(void) const {return __Ep;}
        void setEp(double);

        //tiempo máximo entre el último almacenamiento
        //de la posición y la detención de ambos ejes.
        //      Valor por defecto: MEGARA_Tstop ms
        double getTstop(void) const {return __Tstop;}
        void setTstop(double);
        //desfase temporal entre posicionadores en movimeinto.
        //      Valor por defecto: MEGARA_Tshiff ms
        double getTshiff(void) const {return __Tshiff;}
        void setTshiff(double);

        //PROPIEDADES DE ESTADO:

        //estado de deshabilitación del posicionador
        //valor por defecto: false
        bool Disabled;
        //probabilidad de que el posicionador esté anomalo
        //debe estar en [0, 1]
        //valor por defecto: 0
        double getFaultProbability(void) const {return __FaultProbability;}
        void setFaultProbability(double);
        //tipo de fallo
        //      ftUnk: desconoido
        //      ftSta: estático
        //      ftDyn: dinámico
        //valor por defecto: ftUnk
        TFaultType FaultType;
        //Modo de funcionamiento del controlador
        //      cmSinc: los gestos programados se ejecutarán inmediatamente;
        //      cmAsinc: los gestos programados se ejecutarán al recibir el
        //      comando start.
        //Valor por defecto: cmSinc.
        TControlMode ControlMode;

        //indica si el posicionador está operativo
        //      FaultProbability<=0 && !Disabled
        bool getOperative(void) const {
            return getFaultProbability()<=0 && !Disabled;}

        //NOTA: la clase TRoboticPositioner no tiene capacidad de ejecución.
        //La ejecución de gestos se efectuará en TMultifiberPositioner
        //de modo que ControlMode tiene aquí valor testamental.

        //NOTA: el valor de ControlMode puede ser cambiado directamente
        //o mediante los comandos:
        //      "MS": asigna el modo síncrono;
        //      "MA": asigna el modo asíncrono.

/*        //indica que el posicionador no puede ser destruido
        //porque está adscrito a un punto objetivo
        //valor por defecto: false
        bool Latch;

        //El cerrojo deberá mantenerse activado mientras
        //al posicionador tenga asignado un punto objetivo.
        //Solamente la clase TTargetPoint debería cambiar el estado de Latch.
*/

        //------------------------------------------------------------------
        //PROPIEDADES EN FORMATO TEXTO:

        //PROPIEDADES MECÁNICAS EN FORMATO TEXTO:

        //dirección en memoria del actuador
        AnsiString getActuatorAddressText(void) const {
                return IntToHex(reinterpret_cast<intptr_t>(getActuator()), 8);}

        //PROPIEDADES DE CONTROL EN FORMATO TEXTO:

        //dirección en memoria de la propiedad CMF
        AnsiString getCMFAddressText(void) const {
                return IntToHex(reinterpret_cast<intptr_t>(&CMF), 8);}

        //TOLERANCIAS EN FORMATO TEXTO:

        AnsiString getEoText(void) const;
        void setEoText(const AnsiString&);
        AnsiString getEpText(void) const;
        void setEpText(const AnsiString&);

        AnsiString getTstopText(void) const;
        void setTstopText(const AnsiString&);
        AnsiString getTshiffText(void) const;
        void setTshiffText(const AnsiString&);

        //PROPIEDADES DE ESTADO EN FORMATO TEXTO:

        AnsiString getDisabledText(void) const;
        void setDisabledText(const AnsiString&);
        AnsiString getFaultProbabilityText(void) const;
        void setFaultProbabilityText(const AnsiString&);
        AnsiString getFaultTypeText(void) const;
        void setFaultTypeText(const AnsiString&);
        AnsiString getControlModeText(void) const;
        void setControlModeText(AnsiString&);

        AnsiString getOperativeText(void) const;

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //conjuntos de todas las propiedades de seguridad
        //en formato asignaciones de texto
        AnsiString getToleranceText(void) const;
        //conjunto de propiedades de estado
        //en formato asignaciones de texto
        AnsiString getStatusText(void) const;

        //conjunto de todas las propiedades
        //en formato asignaciones de texto
        AnsiString getAllText(void) const;

        //instancia del posicionador
        //en formato asignaciones de texto
        AnsiString getInstanceText(void) const;
        void setInstanceText(const AnsiString&);

        //------------------------------------------------------------------
        //MÉTODOS ESTÁTICOS:

        //compara los identificadores de dos posicionadores
        static int  CompareIds(TRoboticPositioner *FP1,
                TRoboticPositioner *FP2);

        //imprime el identificador de un posicionador
        static void  PrintId(AnsiString &S, TRoboticPositioner *FP);

        //Los métodos estáticos:
        //      CompareIds
        //      PrintId
        //serán apuntados en la lista de posicionadores adyacentes
        //para permitir su ordenacíon en función de los identificadores
        //y la impresión de los mismos.

        //imprime los valores de las propiedades de origen de un posicionador
        //(Id, x0, y0, thetaO1) al final de una cadena de texto
        //en formato fila de texto
        static void  PrintOriginsRow(AnsiString& S,
                TRoboticPositioner *FP);
        //lee los valores de las propiedades de origen de un posicionador
        //(Id, x0, y0, thetaO1) desde la posición indicada de una cadena
        //de texto, en formato fila de texto
        static void  ReadOriginsRow(TRoboticPositioner *FP,
                const AnsiString& S, int &i);

        //imprime los valores de las propiedades de posición de un posicionador
        //(Id, x3, y3) al final de una cadena de texto
        //en formato fila de texto
        static void  PrintPositionP3Row(AnsiString& S,
                                        TRoboticPositioner *FP);
        //lee los valores de las propiedades de posición de un posicionador
        //(Id, x3, y3) desde la posición indicada de una cadena
        //de texto, en formato fila de texto
        static void  ReadPositionP3Row(TRoboticPositioner* &FP,
                                       const AnsiString& S, int &i);
        //imprime los valores de las propiedades de posición de un posicionador
        //(Id, p_1, p___3) al final de una cadena de texto
        //en formato fila de texto
        static void  PrintPositionPAPRow(AnsiString& S,
                                        TRoboticPositioner *FP);

        //lee una instancia en una cadena
        static void  ReadInstance(TRoboticPositioner* &FP,
                const AnsiString& S, int &i);

        //------------------------------------------------------------------
        //MÉTODOS DE CONSTRUCCIÓN, COPIA, CLONACIÓN Y DESTRUCCIÓN:

        //construye un posicionador
        //con los valores por defecto
        TRoboticPositioner(void);
        //construye un posicionador
        //con los valores indicados
        //si el número de identificación es menor que uno
        //lanza una execepción EimproperArgument
        TRoboticPositioner(int _Id, TDoublePoint _P0,
                double _thetaO1=MEGARA_thetaO1);

        //ADVERTENCIA: para poder contruir clones de posicionadores
        //la duplicidad de números de identificación está permitida.
        //El control de duplicidad de identificadores debe llevarse
        //en todo caso mediante métodos. Al fin y al cabo en el mundo real
        //será posible configurar dos objetos para que tengan elmismo
        //identificador.

        //copia las propiedades de control de un posicionador
        void CopyControl(const TRoboticPositioner*);
        //copia las propiedades de seguridad de un posicionador
        void CopyTolerance(const TRoboticPositioner*);
        //copia las propiedades de estado de un posicionador
        void CopyStatus(const TRoboticPositioner*);

        //copia todas las propiedades de un posicionador
        void Clone(const TRoboticPositioner*);
        TRoboticPositioner& operator=(const TRoboticPositioner&);

        //contruye un clon de un posicionador
        TRoboticPositioner(const TRoboticPositioner*);

        //libera la memoria dinámica y borra el objeto de Builts
        //si el objeto no está en Builts lanza EImproperCall
        ~TRoboticPositioner();

        //-------------------------------------------------------------------
        //MÉTODOS PARA CALCULAR LOS VALORES RECOMENDADOS DE
        //LAS COMPONENTES DE SPM:

        //calcula la componente de SPM para absorber el error de recuperación:
        //      SPMrec = (CMF.vmaxabs1*Actuator->rbs*Actuator->r_max +
        //              CMF.vmaxabs2*Actuator->Arm->rbs*Actuator->Arm->Ra)*
        //              Tstop mm
        double SPMrec(void) const;

        //Calcula el márgen perimetral de seguridad para evitar:
        //- el error mecánico debido al mecanizado del actuador;
        //- el error mecánico de orientación en el anclaje del posicionador;
        //- el error mecánico de posicionamiento en el anclaje del posicionador;
        //- el error numérico de las operaciones matemáticas del modelo;
        //- el error de posición debido a variaciones de velocidad;
        //  al desplazarse de un paso a otro.
        //- el error de posición debido a la falta de correspondencia de
        //  F(theta_1) y Arm->F(theta___3) con al realidad.
        //Todos los errores serán absorbidos por Eo y Ep
        //en la siguiente fórmula:
        //      SPMsta = Eo*r_max + Ep
        //Donde:
        //      Eo: margen de error de orientación en rad;
        //      Ep: margen de error de posición en mm.
        double SPMsta(void) const;

        //ADVERENCIA: no confundir (Eo, Ep) con (PAem, Pem); (Eo, Ep)
        //se refiere al error de posición y orientación al colocar
        //el posiciondor en el posicionador multifibra, tras la calibración.

        //NOTA: la alinealidad de las funciones G(p_1) y Arm->G(p___3)
        //no tiene por que implicar un eror de posición a lo largo de
        //la trayectoria de P3, ya que los ángulos p_1 y p___3 son calculados
        //teniendo en cuenta dicha alinealidad.
        //De este modo solo habrá que tener en cuenta el salto más grande
        //de cada eje para determinar la contribución de F en Ep.

        //calcula la componente de SPM para absorber el error dinámico
        //      SPMdyn = (CMF.vmaxabs1*Actuator->rbs*Actuator->r_max +
        //              CMF.vmaxabs2*Actuator->Arm->rbs*Actuator->Arm->Ra)*
        //              Tshiff + (Actuator->rbs*Actuator->r_max +
        //              Actuator->Arm->rbs*Actuator->Arm->Ra) mm
        double SPMdyn(void) const;

/*        //Calcula el margen perimetral de seguridad para
        //validación o programación:
        //      SPMtop = r_max*(8/M_PI*w1 + 4/M_PI*w2)
        //Donde:
        //      r_max: radio apical del posicionador;
        //      w1: velocidad angular máxima del eje 1 en rad/ms;
        //      w2: velocidad angular máxima del eje 2 en rad/ms.
        double GetSPMtop(double w1=0.3, double w2=0.3) const;

        //w1 y w2 se determinan a partir de CMF del siguiente modo:
        //      w1 = CMF.vmaxabs1/SB1*M_2PI;
        //      w2 = CMF.vmaxabs2/Arm->SB2*M_2PI.
        //Los valores de w1 y w2 deberían ser corregidos
        //en función de las derivadas de G(p_1) y Arm->G(p___3).
        //Pero si las funciones G son aproximadamente lineales,
        //realmente no hace falta por que el valor devuelto por SPMtop
        //solo tiene que ser orientativo, pudiendo elegir el valor
        //que se desee.

        //asigna un valor a SPM según el propósito
        //      SPM = SPMerr + SPMtop + SPMtop + SPMpro; cuando Popose=sPro
        //      SPM = SPMerr + SPMtop;                   cuando Popose=sVal
        //      SPM = SPMerr;                            cuando Popose=sExe
        void SetSPM(TPorpose Porpose,
                double Eo=0.002, double Ep=0.002,
                double w1=0.3, double w2=0.3);

        //ADVERTENCIA: el valor devuelto por SPMtop solo es orientativo
        //pudiendo elegir un valor diferente para SPMval y/o SPMpro.
        //Por ello w1maxabs y w2maxabs son calculados mediante:
        //      double w1 = CMF.vmaxabs1/SB1*M_2PI;
        //      double w2 = CMF.vmaxabs2/Arm->SB2*M_2PI;
        //sin tener en cuenta el efecto de la alinealidad de las funciones
        //G(p_1) y Arm->G(p___3).
  */
        //-------------------------------------------------------------------
        //MÉTODOS DE ASIGNACIÓN CONJUNTA:

        //asigna conjuntamente las tolerancias
        //      {Eo, Ep, Tstop, Tshiff}
        void SetTolerances(double _Eo, double _Ep,
                double _Tstop, double _Tshiff);

        //------------------------------------------------------------------
        //MÉTODOS DE ASIMILACIÓN:

        //A partir de:
        //      (Eo, Ep, Tstop, Tshiff)
        //      rmax
        //Determina:
        //      (SPMrec, SPMsta, SPMdyn, SPMmin)
        void CalculateSPMComponents(void);

        //asigna la componente de SPM para absorber el desplazamiento
        //por corrección del offset
        //      SPMoff = PAem*Actuator->rmax + Pem mm
        void SetSPMoff(double PAem, double Pem);

        //------------------------------------------------------------------
        //MÉTODOS DE INSTRUCCIÓN:

        //Los gestos se programarán según el tipo de función seleccionada
        //y el tipo de sincronismo seleccionado para el tipo de función.

        //Establecer modo asíncrono:
        //      ControllerMode = cmAsinc;
        //Establecer modo síncrono:
        //      ControllerMode = cmSinc;

        //Restricciones semánticas de las instrucciones:
        //      los argumentos deben estar en el dominio de los ejes

        //gira el cilindro hasta p_1
        //si hay un comando programado lanza una excepción EImproperCall
        //si la posición es inalcanzable devuelve falso
        void M1(double _p_1);
        //gira el brazo hasta p___3
        //si no hay un comando programado lanza una excepción EImproperCall
        //si la posición es inalcanzable devuelve falso
        void M2(double _p___3);
        //gira ambos ejes hasta (p_1, p___3)
        //si no hay un comando programado lanza una excepción EImproperCall
        //si la posición es inalcanzable devuelve falso
        void MM(double _p_1, double _p___3);

        //gira el cilindro desde p_1sta hasta p_1fin
        //si hay un comando programado lanza una excepción EImproperCall
        //si la posición es inalcanzable devuelve falso
        void M1(double _p_1sta, double _p_1fin);
        //gira el brazo desde p___3sta hasta p___3fin
        //si no hay un comando programado lanza una excepción EImproperCall
        //si la posición es inalcanzable devuelve falso
        void M2(double _p___3sta, double _p___3fin);
        //gira ambos ejes desde (p_1sta, p___3sta) hasta (p_1fin, p___3fin)
        //si no hay un comando programado lanza una excepción EImproperCall
        //si la posición es inalcanzable devuelve falso
        void MM(double _p_1sta, double _p___3sta,
                double _p_1fin, double _p___3fin);

        //detiene el desplazamiento borrando el gesto programado
        //      si no hay un gesto programado
        //              lanza una excepción EImproperCall
        void SP(void);

        //asigna una instrucción al controlador
        //si la instrucción está vacía lanza una excepción EImproperAgument
        void SetInstruction(TInstruction&);
        //lee una instrucción del controlador
        //si el controlador no tiene ningún movimiento programado
        //      lanza una excepción EImproperCall
        void GetInstruction(TInstruction&);

        //MÉTODOS DE PROGRAMACIÓN DE GESTOS:

        //programa el abatimiento del brazo hasta
        //la posición de seguridad estable más próxima
        void TurnArmToSafeArea(void);
        //programa la retracción del brazo hasta
        //la posición de seguridad estable más próxima
        void RetractArmToSafeArea(void);

        //gira el cilindro hasta theta_1
        //si la posición es inalcanzable devuelve falso
        void TurnCilinderTotheta_1(double _theta_1);
        //gira el cilindro hasta theta_2
//        void TurnCilinderTotheta_2(double theta_2);
        //gira el cilindro hasta theta_3
//        void TurnCilinderTotheta_3(double theta_3);

        //gira el brazo hasta theta_2
//        void TurnArmTotheta_2(double theta_2);
        //gira el brazo hasta theta_3
//        void TurnArmTotheta_3(double theta_3);
        //gira el brazo hasta r_2
//        void TurnArmTor_2(double r_2);
        //gira el brazo hasta r_3
//        void TurnArmTor_3(double r_3);
        //gira el brazo hasta el área de seguridad
        //si el brazo ya está en el área de seguridad devuelve falso
//        void TurnArmToSafeArea(void);
        //gira el brazo hasta el origen de coordenadas
//        void TurnArmToOrigin(void);

        //desplaza P2 radialmente hasta r_2
//        void GoRadialTor_2(double r_2);
        //desplaza P2 radialmente hasta que el brazo quede dentro
        //del área de seguridad o hasta donde se pueda
//        void GoRadialToSafeArea(void);
        //desplaza P2 radialmente hasta P0 o hasta donde se pueda
//        void GoRadialToOrigin(void);

        //va directamente a (r_3, theta_3)
//        void GoDirectlyToPolarP_3(double r_3, double theta_3);
        //va directamente a (x_3, y_3)
        //si la posición es inalcanzable devuelve falso
        void GoDirectlyToCartesianP_3(double x_3, double y_3);
        //si la posición es inalcanzable devuelve falso
        void GoDirectlyToCartesianP3(double x3, double y3);

        //MÉTODOS PARA MODIFICAR EL GESTO PROGRAMADO:

        //invierte el gesto en el dominio del tiempo
        void InvertTime(void);

        //MÉTODOS PARA DETERMINAR LA DISTANCIA MÁXIMA RECORRIDA
        //POR UN PUNTO DEL BRAZO EN UN INTERVALO DE TIEMPO:

        //determina el valor absoluto de
        //el ángulo máximo recorrido al moverse el eje 1 durante un tiempo T
        //      theta_MaxAbs(T) = Actuator->G(CMF.vmaxabs1)*T;
        double theta_MaxAbs(double T);
        //determina el valor absoluto de
        //la distancia máxima recorrida por un punto del brazo
        //al moverse el eje 1 durante un tiempo T
        //      d_MaxAbs(T) = theta_MaxAbs(T)*Actuator->r_max;
        double d_MaxAbs(double T);

        //determina el valor absoluto de
        //el ángulo máximo recorrido al moverse el eje 2 durante un tiempo T
        //      theta___MaxAbs(T) = Actuator->Arm->G.Image(CMF.vmaxabs2)*T;
        double theta___MaxAbs(double T);
        //determina el valor absoluto de
        //la distancia máxima recorrida por un punto del brazo
        //al moverse el eje 2 durante un tiempo T
        //      d___MaxAbs(T) = theta___MaxAbs(T)*Actuator->Arm->Ra;
        double d___MaxAbs(double T);

        //determina una cota superior para la distancia máxima recorrida
        //por un punto del brazo del posicionador, al moverse ambos ejes
        //durante un intervalo de tiempo T
        //      dMaxAbs = Max(d_MaxAbs(T), d___MaxAbs(T))
        double dMaxAbs(double T);

        //MÉTODOS DE MOVIMIENTO A INSTANTES DE TIEMPO DEL GESTO PROGRAMADO:

        //ADVERTENCIA: Para simular el movimeinto de los ejes correctamente
        //debe desactivarse la cuantificación de los ejes que se van a mover.
        //Esto queda bajo responsabilidad del programador.
        //Lo normal es que se desactiven ambos ejes, ya que el eje
        //que no se va a desplazar debe quedar libre para su desplazameinto.

        //lleva los ejes del posicionador adscrito a
        //las posiciones correspondientes al instante t
        //sila cuantificación de los ejes que se van amover está activada
        //lanza una excepción EImproperCall
        void Move(double t);
        //lleva los ejes del posicionador adscrito a
        //las posiciones correspondientes al instante 0
        //(MF1->psta, MF2->psta)
        void MoveSta(void);
        //lleva los ejes del posicionador adscrito a
        //las posiciones correspondientes al instante CMF->tendmax
        //(MF1->pfin, MF2->pfin)
        void MoveFin(void);

        //desplaza los ejes hasta sus posiciones correspondientes
        //al instante t+qt, donde:
        //      t: instante de tiempo previo al actual
        //      qt: escalón de cuantificación
        //void Displace(double t, double qt);

        //si se invoca el método Displace(t, qt) reiteradamente
        //se acumulará el error de suscesivos desplazamientos en los ejes
        //para minimizar este error este método debería invocarse
        //con la cuantificación de los ejes del posicionador desactivada

        //Como el gesto mueve ambos ejes de manera independiente,
        //se puede prescindir del método Displace(t, qt)
        //P.e: cuando el brazo es abatido, es posible cambiar
        //la orientación del cilindro, y al invocar al método Move(t),
        //este conservará su dirección; cosa que no ocurriría si
        //el gesto hubiera definido una función constante para el eje
        //que no se mueve, en cuyo caso volvería a su posición inicial.
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // ROBOTICPOSITIONER_H
