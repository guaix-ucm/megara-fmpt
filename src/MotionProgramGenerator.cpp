//---------------------------------------------------------------------------
//Archivo: MotionProgramGenerator.cpp
//Contenido: generador de programas de movimiento
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "MotionProgramGenerator.h"

//#include <values.h>
#include <unistd.h>
//#include "Windows.h" //Sleep

//---------------------------------------------------------------------------

//espacio de nombres del exposicionamiento
namespace Positioning {

//###########################################################################
//TMotionProgramGenerator:
//###########################################################################

//PROPIEDADES GRÁFICAS:

void TMotionProgramGenerator::setSleepingTime(int _SleepingTime)
{
        //el intervalo de tiempo SleepingTime debe ser no negativo
        if(_SleepingTime < 0)
                throw EImproperArgument("time interval SleepingTime should be nonnegative");

        __SleepingTime = _SleepingTime; //asigna el nuevo valor
}

//---------------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//construye un generador de programas de movimiento
//adscrito a una lista de RPs
TMotionProgramGenerator::TMotionProgramGenerator(TRoboticPositionerList *_RoboticPositionerList) :
        TTargetPointList(_RoboticPositionerList),
        __dt_jmp(-M_PI/36), __SleepingTime(0),
        PaintAll(NULL)
{
}
//destruye los puntos objetivo y destruye el programador
TMotionProgramGenerator::~TMotionProgramGenerator(void)
{
}

//---------------------------------------------------------------------------
//MÉTODOS BÁSICOS:

//calcula el tiempo libre de dos posicionadores acotado inferiormente
//de modo que cuando Df > SPMmin
//      el tiempo libre se calcula de modo que se consuma la Df
//      sin llegar a rebasarla
//y cuando Df <= SPMmin
//      el tiempo libre se calcula de modo que se consuma el SPMmin
//      sin llegar a rebasarlo
double TMotionProgramGenerator::CalculateTf(TRoboticPositioner *RP1, TRoboticPositioner *RP2)
{
        //el puntero RP1 debería apuntar a un RP construido
        if(RP1 == NULL)
                throw EImproperArgument("pointer RP1 should point to built fiber positioner");

        //el puntero RP2 debería apuntar a un RP construido
        if(RP2 == NULL)
                throw EImproperArgument("pointer RP2 should point to built fiber positioner");

        //calcula la distancia libre entre los posicionadores
        double Df = RP1->getActuator()->DistanceFree(RP2->getActuator()); //<-----------------Hacer que se calcule en ambos sentidos

    //La distancia libre es calculada en ambos sentidos,
    //para garantizar que es la mínima.

        //si los posicionadores no guardan la distancia de seguridad
        if(Df < 0)
                return 0; //indica que el tiempo libre es cero

        //-------------------------------------------------------------------
        //CALCULA UNA COTA SUPERIOR PARA
        //LA VELOCIDAD LINEAL ABSOLUTA DEL ÁPICE DEL POSICIONADOR 1:

        double vtop1; //velocidad lineal máxima absoluta

        //si no se mueve ningún eje
        if(RP1->CMF.getMF1()==NULL && RP1->CMF.getMF2()==NULL)
                vtop1 = 0; //la velocidad es cero

        else { //si se mueve algún eje
                //calcula la velocidad lineal máxima en mm/ms del ápice del brazo
                if(RP1->CMF.getMF1()!=NULL && RP1->CMF.getMF2()==NULL) { //si solo se mueve el eje 1
                        //calcula la velocidad angular del eje 1 en rad/ms
                        double w1 = RP1->CMF.getMF1()->getvmaxabs()/RP1->getActuator()->getSB1()*M_2PI;
                        //calcula lacota superior para la velocidad lineal en mm/ms
                        vtop1 = w1*RP1->getActuator()->getr_max();
                }
                else if(RP1->CMF.getMF1()==NULL && RP1->CMF.getMF2()!=NULL) { //si solo se mueve el eje 2
                        //calcula la velocidad angular del eje 2 en rad/ms
                        double w2 = RP1->CMF.getMF2()->getvmaxabs()/RP1->getActuator()->getArm()->getSB2()*M_2PI;
                        //calcula lacota superior para la velocidad lineal en mm/ms
                        vtop1 = w2*RP1->getActuator()->getArm()->getL1V();
                }
                else { //si se mueven ambos ejes
                        //calcula la velocidad angular del eje 1 en rad/ms
                        double w1 = RP1->CMF.getMF1()->getvmaxabs()/RP1->getActuator()->getSB1()*M_2PI;
                        //calcula la velocidad angular del eje 2 en rad/ms
                        double w2 = RP1->CMF.getMF2()->getvmaxabs()/RP1->getActuator()->getArm()->getSB2()*M_2PI;
                        //calcula lacota superior para la velocidad lineal en mm/ms
                        vtop1 = w1*RP1->getActuator()->getr_max() + w2*RP1->getActuator()->getArm()->getL1V();
                }
        }

        //-------------------------------------------------------------------
        //CALCULA UNA COTA SUPERIOR PARA
        //LA VELOCIDAD LINEAL ABSOLUTA DEL ÁPICE DEL POSICIONADOR 2:

        double vtop2; //velocidad lineal máxima del ápice del posicionador 2

        //si no se mueve ningún eje
        if(RP2->CMF.getMF1()==NULL && RP2->CMF.getMF2()==NULL)
                vtop2 = 0; //la velocidad es cero

        else { //si se mueve algún eje
                //calcula la velocidad lineal máxima en mm/ms del ápice del brazo
                if(RP2->CMF.getMF1()!=NULL && RP2->CMF.getMF2()==NULL) { //si solo se mueve el eje 1
                        //calcula la velocidad angular del eje 1 en rad/ms
                        double w1 = RP2->CMF.getMF1()->getvmaxabs()/RP2->getActuator()->getSB1()*M_2PI;
                        //calcula lacota superior para la velocidad lineal en mm/ms
                        vtop2 = w1*RP2->getActuator()->getr_max();
                }
                else if(RP2->CMF.getMF1()==NULL && RP2->CMF.getMF2()!=NULL) { //si solo se mueve el eje 2
                        //calcula la velocidad angular del eje 2 en rad/ms
                        double w2 = RP2->CMF.getMF2()->getvmaxabs()/RP2->getActuator()->getArm()->getSB2()*M_2PI;
                        //calcula lacota superior para la velocidad lineal en mm/ms
                        vtop2 = w2*RP2->getActuator()->getArm()->getL1V();
                }
                else { //si se mueven ambos ejes
                        //calcula la velocidad angular del eje 1 en rad/ms
                        double w1 = RP2->CMF.getMF1()->getvmaxabs()/RP2->getActuator()->getSB1()*M_2PI;
                        //calcula la velocidad angular del eje 2 en rad/ms
                        double w2 = RP2->CMF.getMF2()->getvmaxabs()/RP2->getActuator()->getArm()->getSB2()*M_2PI;
                        //calcula lacota superior para la velocidad lineal en mm/ms
                        vtop2 = w1*RP2->getActuator()->getr_max() + w2*RP2->getActuator()->getArm()->getL1V();
                }
        }


        //-------------------------------------------------------------------
        //CALCULA EL TIEMPO LIBRE:

        //calcula la cota superior para la velocidad lineal máxima absoluta conjunta
        double vtop = vtop1 + vtop2;

        //calcula el tiempo libre
        double Tf;
        if(vtop == 0) //si la velocidad máxima absoluta es cero
                Tf = MAXDOUBLE; //el tiempo libre es infinito
        else { //si la velocidad es distinta de cero
                //calcula el SPM de salto mínimo conjunto
                double SPMmin = RP1->getActuator()->getSPMmin() + RP2->getActuator()->getSPMmin();

                //si la distancia libre libre es mayor que el SPM de salto mínimo conjunto
                if(Df > SPMmin)
                        //calcula el tiempo libre de modo que se consuma la distancia libre sin llegar a rebasarla
                        Tf = Df/vtop;
                else //Df <= SPMmin
                        //calcula el tiempo libre de modo que se consuma el SPM de salto mínimo sin llegar a rebasarlo
                        Tf = SPMmin/vtop;
        }

        //devuelve el tiempo libre
        return Tf;
}

//Por el método de salto,
//determina el desplazamiento angular en sentido positivo o negativo
//del rotor 1 de un posicionador, en pasos, para que no colisione
//con sus adyacentes, y determina el tiempo libre respecto de sus
//adyacentes, en la nueva posición estable.
//Si no es posible encontrar una solución lanza ECantFindSolution.
double TMotionProgramGenerator::DisplacementToAvoidInvasion_JumpingMethod(double &Tfmin,
        TRoboticPositioner *RP)
{
        //COMPRUEBA LAS PRECONDICIONES:

        //el puntero RP debería apuntar a un posicionador contruido
        if(RP == NULL)
                throw EImproperArgument("pointer RP should point to built positioner");

        //el cuantificador del rotor 1 del posicionador RP debe estar desactivado
        if(RP->getActuator()->getQuantify_())
                throw EImproperCall("the rot 1 quantificator of the positioner RP should be disabled");

        //El cuantificador del rotor 1 debe estar activado para que
        //las posiciones a las que se desplace sean estables.

        //COMPRUEBA EL CASO EN QUE NO TIENE QUE DESPLAZARSE:

        TRoboticPositioner *RPA; //puntero a posicionador adyacente
        double Tf; //tiempo libre con un posicionador adyacente
        Tfmin; //tiempo libre mínimo con los posicionadores adyacentes

        //calcula el Tfmin y determina si hay colisión
        Tfmin = MAXDOUBLE; //inicializa el tiempo libre mínimo a su valor máximo posible
        for(int i=0; i<RP->getActuator()->Adjacents.getCount(); i++) { //por cada posicionador adyacente
                //apunta el posicionador indicado para facilitar su acceso
                RPA = RP->getActuator()->Adjacents[i];

                //calcula el tiempo libre del central con el adyacente
                Tf = CalculateTf(RP, RPA);
                //actualiza el tiempo libre mínimo
                if(Tf < Tfmin)
                        Tfmin = Tf;
        }

        //si no hay colisión
        if(Tfmin > 0)
                //indica que no es requerido desplazamiento angular
                return 0;

        //a partir de aquí hay al menos un adyacente invadido

        //GUARDA LA POSICIÓN ORIGINAL E INICIALIZA EL PROCESO DE BÚSQUEDA:

        //guarda la posición original del rotor 1 en radianes y en pasos
        double theta_1bak = RP->getActuator()->gettheta_1();
        double p_1bak = RP->getActuator()->getp_1();
        //inicializa los interruptores de iteración
        bool iterate_down = true;
        bool iterate_up = true;
        //inicializa el desplazamiento angular
        double dt_ = 0;

        //El proceso de búsqueda se iterará hasta que se resuelvan
        //las colisiones o se agote el espacio de búsqueda.

        //REALIZA UNA BÚSQUEDA A SALTOS:

        double theta_1new; //nueva posición de rot 1

        do {
                //incrementa el desplazamiento angular
                dt_ += getdt_jmp();

        //si el dominio en sentido negativo no se ha agotado
        if(iterate_down) {
                    //calcula la nueva posición de salto en sentido negativo
                    //desde la posición inicial
                    theta_1new = theta_1bak + dt_;

                    //si la posición de salto no está  en el dominio de rot 1
                    if(RP->getActuator()->IsntInDomaintheta_1(theta_1new))
                            iterate_down = false; //indica que el dominio en sentido negativo se ha agotado

                    else { //si está en el dominio
                //--------------
                            //desplaza rot 1 a la nueva posición de salto
                            RP->getActuator()->settheta_1(theta_1new);

/*                        	//si el posicionador no colisiona con los adyacentes
                            if(!RP->getActuator()->ThereIsCollisionWithAdjacent()) {
                                    //restaura la posición de rot 1
                                    RP->getActuator()->theta_1 = theta_1bak;
                                    //devuelve el desplazamiento
                                    return dt;
                            } */

                            //calcula el Tfmin y determina si hay colisión
                            Tfmin = MAXDOUBLE; //inicializa el tiempo libre mínimo a su valor máximo
                            for(int i=0; i<RP->getActuator()->Adjacents.getCount(); i++) { //por cada posicionador adyacente
                                    //apunta el posicionador indicado para facilitar su acceso
                                    TRoboticPositioner *RPA = RP->getActuator()->Adjacents[i];

                                    //calcula el tiempo libre entre el central y el adyacente
                                    Tf = CalculateTf(RP, RPA);
                                    //actualiza el tiempo libre mínimo
                                    if(Tf < Tfmin)
                                                Tfmin = Tf;
                            }

                            //si el posicionador no colisiona con los adyacentes
                            if(Tfmin > 0) {
                                        //calcula el desplazamiento angular en pasos
                                        double dp_ = RP->getActuator()->getp_1() - p_1bak;
                                        //corrige el error numérico
                                        dp_ = Round(dp_);
                                    //restaura la posición de rot 1
                                    RP->getActuator()->settheta_1(theta_1bak);
                                    //devuelve el desplazamiento angular
                                    return dp_;
                            }

                                //Dado el estado de activación del cuantificador
                                //de rot 1, el error numérico com etido en:
                                //      double dp_ = RP->getActuator()->getp_1() - p_1bak;
                                //es susceptible de ser nulo.
                //--------------
            }

                }

        //si el dominio en sentido positivo no se ha agotado
        if(iterate_up) {
                    //calcula la nueva posición de salto en sentido positivo
                    //desde la posición inicial
                    theta_1new = theta_1bak - dt_;

                    //si la posición de salto no está  en el dominio de rot 1
                    if(RP->getActuator()->IsntInDomaintheta_1(theta_1new))
                            iterate_up = false; //indica que el dominio en sentido positivo se ha agotado

                    else { //si está en el dominio
                //--------------
                            //desplaza rot 1 a la nueva posición de salto
                            RP->getActuator()->settheta_1(theta_1new);

/*                        	//si el posicionador no colisiona con los adyacentes
                            if(!RP->getActuator()->ThereIsCollisionWithAdjacent()) {
                                    //restaura la posición de rot 1
                                    RP->getActuator()->theta_1 = theta_1bak;
                                    //devuelve el desplazamiento
                                    return dt;
                            } */

                            //calcula el Tfmin y determina si hay colisión
                            Tfmin = MAXDOUBLE; //inicializa el tiempo libre mínimo a su valor máximo
                            for(int i=0; i<RP->getActuator()->Adjacents.getCount(); i++) { //por cada posicionador adyacente
                                    //apunta el posicionador indicado para facilitar su acceso
                                    TRoboticPositioner *RPA = RP->getActuator()->Adjacents[i];

                                    //calcula el tiempo libre entre el central y el adyacente
                                    Tf = CalculateTf(RP, RPA);
                                    //actualiza el tiempo libre mínimo
                                    if(Tf < Tfmin)
                                            Tfmin = Tf;
                            }

                            //si el posicionador no colisiona con los adyacentes
                            if(Tfmin > 0) {
                                        //calcula el desplazamiento angular en pasos
                                        double dp_ = RP->getActuator()->getp_1() - p_1bak;
                                        //corrige el error numérico
                                        dp_ = Round(dp_);
                                    //restaura la posición de rot 1
                                    RP->getActuator()->settheta_1(theta_1bak);
                                    //devuelve el desplazamiento angular
                                    return dp_;
                            }

                                //Dado el estado de activación del cuantificador
                                //de rot 1, el error numérico com etido en:
                                //      double dp_ = RP->getActuator()->getp_1() - p_1bak;
                                //es susceptible de ser nulo.
                //--------------
            }

                }


        //mientras el dominio no se haya agotado en ambos sentidos
        } while(iterate_down || iterate_up);

        //restaura la posición de rot 1
        RP->getActuator()->settheta_1(theta_1bak);
        //indica que no ha encontrado solución
        throw ECantFindSolution("cant find a theta_1 displacement without collision");

    //DEFECTO: el código demarcado es igual, y la función para determinar si hay colisión con los adyacentes,
    //debería determinar el Tfmin. Comprobar y sustituir.
}
/*//Por el método de búsqueda binaria,
//determina el desplazamiento angular en sentido negativo
//del eje 1 de un posicionador, en radianes, para que
//no colisione con sus adyacentes,
//y determina el tiempo libre respecto de sus adyacentes.
//Si no es posible encontrar una solución lanza ECantFindSolution.
double TMotionProgramGenerator::DisplacementToAvoidInvasion_BinarySearch(double &Tfmin,
        TRoboticPositioner *RP)
{
        //COMPRUEBA LAS PRECONDICIONES:

        //el puntero RP debería apuntar a un RP contruido
        if(RP == NULL)
                throw EImproperArgument("pointer RP should point to built fiber positioner");

        //el cuantificador del eje 1 del RP RP debe estar activado
        if(!RP->getActuator()->getQuantify_())
                throw EImproperCall("the axis 1 quantificator of the fiber positioner RP should be enabled");

        //El cuantificador del eje 1 debe estar activado para que
        //las posiciones a las que se desplace sean estables.

**//*        //GUARDA EL SPM Y LO INCREMENTA:

        //guarda el SPM original
        double SPMbak = RP->getActuator()->getArm()->SPM;
        //incrementa el SPM según el margen de error numérico
        RP->getActuator()->getArm()->SPM += ERR_NUM;

        //El SPM debe ser incrementado para evitar que en la determinación
        //de colisión se obtenga un falso positivo, ya que la determinación
        //puede ser recíproca.
        //El margen de error numérico está subredimensionado, de modo que
        //se garantiza que el estado de colisión se determina correctamente.
  *//**
        //COMPRUEBA EL CASO EN QUE NO TIENE QUE DESPLAZARSE:

        //calcula el Tfmin y determina si hay colisión
        Tfmin = MAXDOUBLE; //inicializa el tiempo libre mínimo a su valor máximo
        for(int i=0; i<RP->getActuator()->Adjacents.getCount(); i++) { //por cada posicionador adyacente
                //apunta el posicionador indicado para facilitar su acceso
                TRoboticPositioner *RPA = RP->getActuator()->Adjacents[i];

                //calcula el tiempo libre entre el central y el adyacente
                double Tf = CalculateTf(RP, RPA);
                //actualiza el tiempo libre mínimo
                if(Tf < Tfmin)
                        Tfmin = Tf;

                //calcula el tiempo libre entre el adyacente y el central
                Tf = CalculateTf(RPA, RP);
                //actualiza el tiempo libre mínimo
                if(Tf < Tfmin)
                        Tfmin = Tf;
        }

        //si no hay colisión
        if(Tfmin > 0) {
                //restaura el SPM original
//                RP->getActuator()->getArm()->SPM = SPMbak;
                //indica que no es requerido desplazamiento angular
                return 0;
        }

        //GUARDA LA POSICIÓN ORIGINAL E INICIALIZA EL PROCESO DE BÚSQUEDA:

        //guarda la posición original del eje 1
        double theta_1bak = RP->getActuator()->gettheta_1();

        //inicializa los márgenes del espacio de búsqueda
        double theta_1min = 0;
        double theta_1max = theta_1bak;

        //inicializa el interruptor de iteración
        bool iterate = true;

        //El proceso de búsqueda se iterará hasta que la nueva posición
        //salga igual que la anterior, en virtud del estado habilitado
        //del cuantificador del eje 1.

        //REALIZA UNA BÚSQUEDA BINARIA:

        do { //repite

                //guarda la posición actual
                double theta_1prev = RP->getActuator()->gettheta_1();

                //calcula la nueva posición  de salto
                double theta_1new = (theta_1min + theta_1max)/2;
                //mueve el eje 1 del posicionador a la nueva posición de salto
                RP->getActuator()->theta_1 = theta_1new;

                //si la nueva posición es igual que la anterior
                if(RP->getActuator()->theta_1 == theta_1prev)
                        //indica que no debe seguir iterando
                        iterate = false;

                //si la nueva posición difiere de la anterior
                else {
                        //calcula el Tfmin y determina si hay colisión
                        Tfmin = MAXDOUBLE; //inicializa el tiempo libre mínimo a su valor máximo
                        for(int i=0; i<RP->getActuator()->Adjacents.getCount(); i++) { //por cada posicionador adyacente
                                //apunta el posicionador indicado para facilitar su acceso
                                TRoboticPositioner *RPA = RP->getActuator()->Adjacents[i];

                                //calcula el tiempo libre con el adyacente
                                double Tf = CalculateTf(RP, RPA);
                                //actualiza el tiempo libre mínimo
                                if(Tf < Tfmin)
                                        Tfmin = Tf;
                        }

                        //si hay colisión
                        if(Tfmin <= 0)
                                //reduce el espacio de búsqueda por arriba
                                theta_1max = RP->getActuator()->gettheta_1();
                        else //si no hay colisión
                                //reduce el espacio de búsqueda por abajo
                                theta_1min = RP->getActuator()->gettheta_1();
                }

        //mientras encuentre una nueva posición de salto
        } while(iterate);

        //si hay colisión
        if(Tfmin <= 0) {
                //restaura el estado inicial del posicionador
                RP->getActuator()->theta_1 = theta_1bak;
//                RP->getActuator()->getArm()->SPM = SPMbak;
                //indica que no hay solución
                throw ECantFindSolution("there isn't axis 1 angular position without collision");
        }

        //calcula el salto angular
        double dt = RP->getActuator()->theta_1 - theta_1bak;
        //restaura el estado inicial del posicionador
        RP->getActuator()->theta_1 = theta_1bak;
//        RP->getActuator()->getArm()->SPM = SPMbak;
        //devuelve el desplazamiento angular
        return dt;
}   */

//Resuelve la colisiones girando el rotor 1 de los posicionadores
//operativos de la lista, en orden de menor a mayor desplazamiento
//angular absoluto, e indica si se ha encontrado alguna colisión.
//Todos los posicionadores de la lista Invaders:
//      - deben tener el cuantificador de rot 1 deshabilitado;
//      - deben tener un gesto de retracción programado;
//      - deben estar operativos;
//      - pueden quedar con rot 1 desplazado;
//      - quedarán con la función de movimiento de rot 1 desplazada
//        la misma distancia que rot 1.
//Si para encontrar la solución ha tenido que girar algún posicionador:
//	devuelve verdadero.
//Si no es posible encontrar una solución lanza ECantFindSolution.
bool TMotionProgramGenerator::TurnToSolveCollisions(double& Tfmin,
        TRoboticPositionerList& Invaders)
{
        //todos los posicionadores de la lista Invaders...
        for(int i=0; i<Invaders.getCount(); i++) {
                //apunta el posicionador para facilitar su acceso
                TRoboticPositioner *RP = Invaders.Get(i);

                //...deben tener deshabilitado el cuantificador del rotor 1
                if(RP->getActuator()->getQuantify_())
                        throw EImproperArgument("all positioners in the list Invaders should has disabled the rot 1 quantifier");

                //...deben tener una función de retracción programada
                if(RP->CMF.getMF1()==NULL ||RP->CMF.getMF2()==NULL)
                        throw EImproperArgument("all positioners in the list Invaders should has programmed a retraction function");

                //...deben estar operativos
                if(!RP->getOperative())
                        throw EImproperArgument("all positioners in the list Invaders should be operative");
        }

        //Los cuantificadores de los rotores 1 deben estar deshabilitados
        //para que las posiciones iniciales puedan ser inestables.

        //------------------------------------------------------------------
        //DETERMINA EL DESPLAZAMIENTO ANGULAR REQUERIDO DEL ROTOR 1
        //DE CADA UNO DE LOS POSICIONADORES INVASORES,
        //Y ALISTA LOS POSICIONADORES QUE REQUIEREN DESPLAZAMIENTO:

        double dp_; //desplazamiento angular requerido
        double Tf; //tiempo libre

        //inicializa el tiempo libre mínimo a su valor máximo posible
        Tfmin = MAXDOUBLE;

        //contruye una lista de pares de desplazamientos angulares
        //que puede ser ordenada en función del desplazamiento absoluto
        TPointersList<TPairTD<TRoboticPositioner> > Pairs(Invaders.getCount(),
                TPairTD<TRoboticPositioner>::CompareAbsx);

        TRoboticPositioner *RP;

        //por cada posicionador invasor
        for(int i=0; i<Invaders.getCount(); i++) {
                try {
                        //apunta el posicionador indicado para facilitar su acceso
                        RP = Invaders.Get(i);
                        //calcula el desplazamiento requerido en pasos
                        //y el tiempo libre respecto a los adyacentes
                        dp_ = DisplacementToAvoidInvasion_JumpingMethod(Tf, RP);

                        //actualiza el tiempo libre mínimo
                        if(Tf < Tfmin)
                                Tfmin = Tf;
                        //si requiere desplazamiento
                        if(dp_ != 0)
                                //añade el par de desplazamiento requerido a
                                //la lista de los que requieren desplazamiento
                                Pairs.Add(new TPairTD<TRoboticPositioner>(RP, dp_));
                }
                //si DisplacementToAvoidInvasion no ha podido encontrar una solución
                //este caso se da cuando rot 1 choca con el tope
                catch(ECantFindSolution &E) {
                        //no hace nada más
                }
                //atrapa y relanza las demás excepciones
                catch (...) {
                        throw;
                }
        }

        //RESUELVE LAS COLISIONES CONSUMIENDO LA LISTA DE DESPLAZAMIENTOS
        //Y ACTUALIZÁNDOLA EN CADA ITERACIÓN:

        //si la lista de los que requieren desplazamiento está vacía
        if(Pairs.getCount() < 1)
                //indica que no se ha encontrado invasión
                return false;

        //si la lista de los que requieren desplazamiento tiene más de un elemento
        if(Pairs.getCount() > 1)
                //ordena la lista de mayor a menor desplazamiento requerido
                Pairs.SortDec(0, Pairs.getCount()-1);

        //el orden decreciente permite manipular la lista por el final
        //facilitando el borrado de elementos

        TPairTD<TRoboticPositioner> *Pair; //puntero a par de desplazamiento requerido
        TRoboticPositioner *RPA; //puntero a posicionador adyacente

        //mientras haya algún posicionador que requiere ser desplazado
        while(Pairs.getCount() > 0) {
                //apunta el último objeto de la lista
                //para facilitar su acceso
                Pair = Pairs.getLastPointer();
                RP = Pair->P;

                //desplaza el rotor 1 del posicionador
                RP->getActuator()->setp_1(RP->getActuator()->getp_1() + Pair->x);
                //desplaza la función de movimiento del rotor 1
                double psta = RP->CMF.getMF1()->getpsta() + Pair->x;
                double pfin = RP->CMF.getMF1()->getpfin() + Pair->x;
                if(psta < 0)
                        psta = 0;
                if(pfin < 0)
                        pfin = 0;
                RP->CMF.getMF1()->SetInterval(psta, pfin);

                //Como la función de movimiento de rot 1 es en sentido negativo,
                //un desplazamiento en sentido negativo, puede hacer que
                //la función de movuimiento rebase el origen de rot 1.

                //ACTUALIZA LA LISTA DE LOS QUE REQUIEREN DESPLAZAMIENTO:

                //borra el posicionador de la lista de
                //los que requieren desplazamiento
                delete Pair;
                Pairs.DelLast();

                //por cada posicionador adyacente operativo de la lista Invaders
                for(int i=0; i<RP->getActuator()->Adjacents.getCount(); i++) {
                        //apunta el posicionador indicado para facilitar su acceso
                        RPA = RP->getActuator()->Adjacents[i];

                        //ADVERTENCIA: esposible que el posicionador desplazado
                        //esté ocupando el único espacio al que podía desplazarse
                        //el brazo de un posicionador adyacente, de modo que
                        //el método DisplacementToAvoidInvasion(Tf, RPA)
                        //ya no sea capaz de encontrar una solución para
                        //dicho adyacente.

                        //si el posicionador está operativo y se encuentra en la lista Invaders
                        if(RPA->getOperative() && Invaders.Search(RPA)<Invaders.getCount()) {
                                //intenta...
                                try {
                                        //calcula su nuevo desplazamiento requerido en pasos
                                        dp_ = DisplacementToAvoidInvasion_JumpingMethod(Tf, RPA);

                                        //busca el posicionador adyacente en la lista de
                                        //los que requieren desplazamiento
                                        int j=0;
                                        while(j<Pairs.getCount() && Pairs[j].P!=RPA)
                                                j++;

                                        //si el posicionador está en la lista
                                        if(j < Pairs.getCount()) {
                                                //si requiere desplazamiento
                                                if(dp_ != 0) {
                                                        //actualiza el desplazamiento requerido
                                                        Pairs[j].x = dp_;
                                                        //reordena la lista
                                                        Pairs.SortDec(0, Pairs.getCount()-1);
                                                        //esta situación se produce cuando
                                                        //el posicionador desplazado no es
                                                        //el único que invadía el adyacente
                                                }
                                                //si no requiere desplazamiento
                                                else {
                                                        //borra el par de la lista
                                                        Pairs.Delete(j);
                                                        //esta situación se produce cuando
                                                        //el posicionador desplazado era
                                                        //el único que invadía el adyacente
                                                }
                                        }
                                        //si el posicionador no está en la lista
                                        else {
                                                //si requiere desplzamiento
                                                if(dp_ != 0)
                                                        //inserta el par en orden decreciente
                                                        Pairs.InsertDec(new TPairTD<TRoboticPositioner>(RPA, dp_));

                                                //esta situación no puede darse ya que
                                                //el RP desplazado
                                                //no puede invadir ninguno de sus adyacentes
                                        }
                                }
                                //si DisplacementToAvoidInvasion no ha podido encontrar una solución
                                //este caso se da cuando el desplzado ocupa la única posición
                                //a la que podía ir el adyacente
                                catch(ECantFindSolution &E) {
                                        //busca el posicionador adyacente en la lista de
                                        //los que requieren desplazamiento
                                        int j=0;
                                        while(j<Pairs.getCount() && Pairs[j].P!=RPA)
                                                j++;

                                        //si el posicionador está en la lista
                                        if(j < Pairs.getCount()) {
                                                //borra el par de la lista
                                                Pairs.Delete(j);
                                                //esta situación se produce cuando
                                                //el posicionador desplazado tapona
                                                //la única posición a la que pódía ir
                                                //el adyacente
                                        }
                                }
                                //atrapa y relanza las demás excepciones
                                catch (...) {
                                        throw;
                                }

                        } //if(RPA->getOperative()) {
                } //for(int i=0; i<RP->getActuator()->Adjacents.getCount(); i++) {
        } //while(Pairs.getCount() > 0) {

        //DETERMINA EL TIEMPO LIBRE MÍNIMO PARA LA CONFIGURACIÓN FINAL:

        //inicializa el tiempo libre mínimo a su valor máximo posible
        Tfmin = MAXDOUBLE;
        //por cada posicionador invasor
        for(int i=0; i<Invaders.getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Invaders.Get(i);

                //por cada posicionador adyacente
                for(int j=0; j<RP->getActuator()->Adjacents.getCount(); j++) {
                        //apunta el posicionador indicado para facilitar su acceso
                        RPA = RP->getActuator()->Adjacents[j];
                        //calcula el tiempo libre
                        Tf = CalculateTf(RP, RPA);
                        //actualiza el tiempo libre mínimo
                        if(Tf < Tfmin)
                                Tfmin = Tf;
                }
        }

        //si no se han resuelto todas las colisiones
        if(Tfmin <= 0) //Invaders->ThereIsCollision())
                //indica que no puede encontrar una solución
                throw ECantFindSolution("cant find a theta_1 displacement without collision to some invaders fiber posicioners");

        //indica que si ha tenido que mover algún RP
        return true;
}

//Para cada posicionador de de la lista Outsiders:
// - Programa un gesto de retracción.
// - Si tiene algún cuantificador deshabilitado:
//	lanza una excepción EImproperArgument.
// - Si está en posición de seguridad:
//	lanza una excepción EImproperArgument.
void TMotionProgramGenerator::ProgramRetraction(TRoboticPositionerList &Outsiders)
{
        //todos los posicionadores de la lista Outsiders...
        for(int i=0; i<Outsiders.getCount(); i++) {
                //apunta el posicionador para facilitar su acceso
                TRoboticPositioner *RP = Outsiders.Get(i);

/*		//...deben tener los cuantificadores habilitados
        if(RP->getActuator()->Quantify_!=true || RP->getActuator()->getArm()->Quantify___!=true)
            throw EImproperArgument("all positioners in the list Outsiders should have enabled quantificators");
  */
                //...deben estar en psición de inseguridad
                if(RP->getActuator()->ArmIsInSafeArea())
                        throw EImproperArgument("all positioners in the list Outsiders should be in unsafe positions");
        }

        //si la lista de posicionadores Outsiders, está vacía
        if(Outsiders.getCount() < 1)
                return; //no hace nada
        //a partir de aquí la lista de posicioandores Outsiders
        //contiene al menos un posicionador en posición de inseguridad
    //con el cuantificador de rot 2 habilitado

        TRoboticPositioner *RP;

        //por cada posicionador de la lista de posicionadores externos:
        for(int i=0; i<Outsiders.getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Outsiders[i];

        //programa la retracción radial
                RP->RetractArmToSafeArea();
        }

    //DEFECTOS:
    //	En las zonas cerca del límite, rot 1 deberá detenerse en seco, y el sincronismo deberá ser adecuado.
    //	En este caso la detención se produce en el cero lógico.
    //	Dado que debe detenerse en seco, la velocidad y el par empleados deberá encontrarse dentro del
    //	subespacio donde se garantice que no se pierden pasos. Este problema no afecta a la simulación.
}

//-------------------------------------------------------------------
//MÉTODOS PARA RETRAER LOS BRAZOS:

//Retrae los posicionadores retráctiles y segrega los invasores
//y los retráctiles.
//Todos los posicionadores de la lista Outsiders:
// - deben estar en posición de inseguridad;
// - deben estar operativos;
// - no deben estar en estado de colisión dinámica;
// - deben tener los cuantificadores de los rotores habilitados;
//Para todos los posicionadores de la lista Outsiders:
// - la posición inicial de los rotores quedará apilada;
// - quedará programado un gesto de retracción.
// - los cuantificadores de los rotores quedarán deshabilitados.
//Los posicionadores de la lista Retractiles quedarán retraidos.
//Los posicionadores de la lista Invaders quedarán en sus
//posiciones iniciales.
void TMotionProgramGenerator::RetractRetractiles(TRoboticPositionerList &Invaders,
    TRoboticPositionerList &Retractiles,
    TRoboticPositionerList &Outsiders)
{
        //COMPRUEBA LAS PRECONDICIONES:

        //todos los posicionadores de la lista Outsiders...
    for(int i=0; i<Outsiders.getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        TRoboticPositioner *RP = Outsiders[i];

        //...deben estar en posición de inseguridad
        if(RP->getActuator()->ArmIsInSafeArea())
            throw EImproperArgument("all positioners in list Outsiders should be in unsafe position");

                //Si un posicionador no estuviese en posición de inseguridad,
                //la programación del gesto de retracción lanzaría una excepción.

        //...deben estar operativos
        if(RP->Disabled || RP->getFaultProbability()>0)
            throw EImproperArgument("all positioners in list Outsiders should be operative");

                //El requerimineto de que todos los posicionadores estén operativos,
                //es una cuestión de economía, ya que así el método no tiene que
                //discernir si el posicionador está operativo para programarlo.

        //...no deben estar en estado de colisión dinámica
        if(RP->getActuator()->ThereIsCollisionWithAdjacent())
            throw EImproperArgument("all positioners in list Outsiders should not be in dynamic collision status");

                //El requerimiento de que los posicionadores no estén en estado
                //de colisión dinámica, es una cuestión de economía, ya que
                //los posicionadores en estado de colisión dinámica no pueden
                //ser movidos.

/*		//...deben tener los cuantificadores de los rotores habilitados
        if(RP->getActuator()->Quantify_!=true || RP->getActuator()->getArm()->Quantify___!=true)
            throw EImproperArgument("all positioners in the list Outsiders should be enabled the quantificator of the rotors");
  */
        //Los cuantificadores de los posicionadores que van a ser retraidos,
        //deben estar habilitados, porque la posición inicial debe ser estable.
    }

        //COMPRUEBA EL CASO EN QUE NO HAY QUE HACER NADA:

        //si no hay posicionadores para retraer
        if(Outsiders.getCount() < 1) {
        //no habrá posicionadores retráctiles ni invasores
        Retractiles.Clear();
        Invaders.Clear();
        return;
    }

        //En caso de que haya un solo posicionador,
        //es posible que colisione con algún obstáculo.

        //INICIALIZA LAS PROPIEDADES:

        //guarda las posiciones iniciales de los rotores
        Outsiders.PushPositions();
        //programa la retracción de los posicionadores externos
        ProgramRetraction(Outsiders);
        //deshabilita los cuantificadores de los rotores
        Outsiders.SetQuantifys(false, false);

        //Las posiciones inciiales deben quedar apiladas, para su restauración
        //cada vez que se detecte una colisión.
        //Debe programarse la retracción de los posiciondores
        //cuya retracción va a simularse.
        //Los cuantificadores de los rotores de los posicionadores que van a
        //ser retraidos debe estar deshabilitados, porque se va a realizar
        //una simulación con saltos temporales en los que las posiciones de
        //los ejes pueden ser inestables.

        //construye dos listas tampón para permitir que los parámetros
        //de salida sean los mismos que los de entrada
        TRoboticPositionerList _Retractiles;
        TRoboticPositionerList _Invaders;

    //inicializa la lista de retraibles
    _Retractiles.Add(Outsiders);

        double t; //instante de simulación
        int i, j; //índices a cada par de posicionadores adyacentes
        TRoboticPositioner *RPA; //posicionador adyacente
        double Tf; //tiempo libre entre RP y RPA
        double Tfmin; //tiempo libre mínimo
        bool notcollision; //indicador de colisión

        //inicializa el instante de simulación
        t = 0;

    //REALIZA LA BUSQUEDA POR SIMULACIÓN:

        //mientras quede tiempo de simulación
        while(t < _Retractiles.getTdis()) {

                //simula la posición en el instante t
                _Retractiles.Move(t);

                //imprime la lista de posicionadores en
        //su nuevo estado en caso necesario
                if(PaintAll != NULL) {
                        PaintAll();
                        sleep(getSleepingTime());
                }

                //DETERMINA EL TIEMPO LIBRE MÍNIMO Y SI ENCUENTRA UN TIEMPO LIBRE
                //MENOR O IGUAL QUE CERO DETERMINA QUE HAY COLISIÓN:

                Tfmin = MAXDOUBLE; //inicializa el tiempo libre mínimo a su valor máximo posible
                notcollision = true; //inicializa el indicador de no colisión a verdadero

                //busca el posicionador RP y su adyacente RPA con el tiempo libre mínimo
                //y si encuentra colisión traslada el posicionador a la lista de invasores
                //(con su posición original restaurada) y reinicia la simulación
                i = 0;
                while(i<_Retractiles.getCount() && notcollision) {
                        //apunta el posicionador indicado para facilitar su acceso
                        TRoboticPositioner *RP = _Retractiles.Get(i);

                        //por cada posicionador adyacente
                        j = 0;
                        while(j<RP->getActuator()->Adjacents.getCount() && notcollision) {
                                //apunta el posicionador adyacente indicado para facilitar su acceso
                                TRoboticPositioner *RPA = (TRoboticPositioner*)RP->getActuator()->Adjacents[j];

                                //calcula el tiempo libre
                                Tf = CalculateTf(RP, RPA);

                                //si los brazos colisionan
                                if(Tf <= 0) {
                                        //restaura la posición inicial
                                        RP->getActuator()->Restorethetas();

                                        //imprime la lista de RPs en su nuevo estado en caso necesario
                                        if(PaintAll != NULL)
                                                PaintAll();

                                        //traslada el posicionador a la lista de invasores
                                        _Retractiles.Delete(i);
                                        _Invaders.Add(RP);

                                        //ADVERTENCIA: aquí no se debe desactivar
                                        //la bandera de determinación de colisión
                                        //del posicionador pues aunque esté en su
                                        //posición inicial puede interferir con
                                        //los demás posicionadores.

                                        //reinicia la simulación
                                        t = 0;

                                        //indica que hay colisión
                                        notcollision = false;

                                } else { //si los brazos no colisionan
                                        //REGISTRA EL TIEMPO LIBRE MENOR Y SUS POSICIONADORES ADSCRITOS:

                                        //si el tiempo libre es la menor hasta ahora
                                        if(Tf < Tfmin)
                                                //actualiza el tiempo libre mínimo
                                                Tfmin = Tf;

                                        j++;
                                }
                        }
                        //inica al próximo posicionador
                        i++;
                }

                //si no ha encontrado colisión
                if(notcollision)
                        //avanza la simulación
                        t += Tfmin;

                //NOTA: el error numérico acumulado en t no tiene
                //improtancia ya que su efecto es despreciable
                //comparado con el margen de seguridad
        };

        //completa el desplazamiento hasta la posición final
        _Retractiles.MoveFin();

        //RECUERDA: en la posición final no puede haber colisiones
        //ya que estarán todos retraidos.

        //imprime la lista de posicionadores
    //en su nuevo estado en caso necesario
        if(PaintAll != NULL) {
                PaintAll();
                sleep(getSleepingTime());
        }

        //asigna las variables tampón
        Retractiles.Clear();
        Invaders.Clear();
        Retractiles.Add(_Retractiles);
        Invaders.Add(_Invaders);

    //DEFECTOS:
    //	Excluye loS RPs en orden de Id, lo cual puede dar lugar al descarte de mayor número de RPs.
}

//Simula la retracción de los posicionadores de la lista Invaders.
//Cuando detecta una colisión, la resuelve mediante giros de rot 1 y
//reinicia el proceso de simulación con las nuevas posiciones de rot 1.
//Todos los posicionadores de la lista Invaders:
//      deben tener apilada la posición inicial de los rotores;
//      deben tener deshabilitados los cuantificadores de los rotores;
//      deben tener un gesto de retracción programado;
//	quedarán retraidos en posición de seguridad;
//	pueden acabar con rot 1 desplazado;
//      quedarán con la CMF.MF1 desplazada la misma distancia que rot 1.
void TMotionProgramGenerator::RetractInvaders(TRoboticPositionerList &Invaders)
{
        //si no hay posicionadores invasores
        if(Invaders.getCount() < 1)
                return; //no hace nada

        //todos los posicionadores de la lista Invaders...
        for(int i=0; i<Invaders.getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                TRoboticPositioner *RP = Invaders.Get(i);

                //...debe tener apilada la posición inicial de los rotores
                if(RP->getActuator()->theta_1s.getCount()<1 || RP->getActuator()->getArm()->theta___3s.getCount()<1)
                        throw EImproperArgument("all positioner in the list Invades, should has stacked the initial positions of the rotors");

                //...deben tener los cuantificadores de los rotores deshabilitados
                if(RP->getActuator()->getQuantify_() || RP->getActuator()->getArm()->getQuantify___())
                        throw EImproperArgument("all positioner in the list Invaders, should has disabled the quantificators of the rotors");

                //...deben tener un gesto de retracción programado
                if(RP->CMF.getMF1()==NULL || RP->CMF.getMF2()==NULL)
                        throw EImproperArgument("all positioner in the list Invaders, should has programmed a retraction gesture");
        }

        //Si hay un solo posicionador invasor es porque
        //colisiona con algún obstáculo.

        bool invasion; //bandera de invasión
        double Tfmin; //tiempo libre mínimo

        do { //repite

                //inicializa la bandera de invasión
                invasion = false;

                //abate paso a paso los brazos de los RPs
                //y en cada paso resuelve las colisiones
                double t = 0; //inicializa la simulación
                while(t < Invaders.getTdis()) {

                        //mueve los brazos a sus posiciones correspondientes al instante t
                        Invaders.Move(t);

                        //el desplazamiento de los brazos de los posicionadores
                        //puede haber provocado colisiones entre ellos

                        //resuelve las colisiones en la pose actual
                        //determinando si se ha girado algo
                        //asi como el tiempo libre mínimo
                        invasion = TurnToSolveCollisions(Tfmin, Invaders);

                        //como el desplazamiento es un número entero de pasos
                        //el cilindro no se desviará de la posición de salto

                        //imprime la lista de RPs en su nuevo estado
                        if(PaintAll != NULL) {
                                PaintAll();
                                sleep(getSleepingTime());
                        }

/*
                        //limita el salto longitudinal respetando el spm
                        dmin -= TArmAbstract::Getspm();
                        //limita inferiormente el salto longitudinal
                        dmin = Max(SPM_MIN, dmin);
                        //determina el salto longitudinal para que no haya colisión (en mm)
                        double jl = dmin/2;
                        //limita superiormente el salto longiudinal
                        //jl = Min(, jl);
                        //determina el salto angular en radianes
                        double jtheta = jl/RP->Rc;
                        //determina el salto angular en pasos
                        double jp = RP->p_(jtheta);
                        //determina el salto temporal correspondiente al salto
                        qt = jp/VMAXABSDEF;

                        //ERROR: para determinar el salto angular en radianes
                        //debería dividirse por el radio máximo es decir
                        //Max{Rc}, aunque aquí se divide por el Rc de un
                        //posicionador cualquiera para salir del paso.
                        */

                        //salta a la próxima iteración
                        t += Tfmin;
                }

                //si ha encontrado una invasión
                if(invasion)
                        //restaura la posición inicial de los brazos
                        Invaders.RestoreArmtheta___3s();

        } while(invasion); //mientras haya una invasión

        //completa el desplazamiento hasta la posición final
        Invaders.MoveFin();

        //habilita el cuantificador del rot 1 de cada posicionador
        Invaders.SetQuantify_s(true);

        //--------------------------------------------------------------------
 /*        //para el eje 1 de cada posicicionador invasor apila:
        //      la posición inicial
        //      el estado del cuantificador
        //y desactiva la cuantificación
        for(int i=0; i<Invaders->Count; i++) {
                //apunta el posicionador indicado para facilitar su acceso
                TRoboticPositioner *RP = Invaders->Get(i);
                //apila la posición inicial del eje 1
                RP->getActuator()->Pushtheta_1();
                RP->getActuator()->PushQuantify_();
                RP->getActuator()->setQuantify_(false);
        }
  */ /**
        //Si hay un soloposicionador invasor es porque
        //colisiona con algún obstáculo.

        //guarda las posiciones iniciales de los brazos de
        //todos los posicionadores de la lista Outsiders
///        Outsiders.PushArmPositions();

        bool invasion; //bandera de invasión
        double Tfmin; //tiempo libre mínimo

        do { //repite

                //inicializa la bandera de invasión a falso
                invasion = false;

                //abate paso a paso los brazos de los RPs
                //y en cada paso resuelve las colisiones
                double t = 0; //inicializa la simulación
                while(t < Invaders->Tdis) {

                        //mueve los brazos a sus posiciones correspondientes al instante t
                        Invaders->Move(t);

                        //el desplazamiento de los brazos de los posicionadores
                        //puede haber provocado colisiones entre ellos

                        //resuelve las colisiones en la pose actual
                        //determinando si se ha girado algo
                        //asi como el tiempo libre mínimo
                        if(TurnToSolveCollisions(Tfmin, Invaders)) //si ha girado algo
                                invasion = true; //levanta la bandera de invasión

                        //como el desplazamiento es un número entero de pasos
                        //el cilindro no se desviará de la posición de salto

                        //imprime la lista de RPs en su nuevo estado
                        if(PaintAll != NULL) {
                                PaintAll();
**/ /*                                Outsiders.Paint(PloterShapes);
                                PaintTargetPoints(PloterShapes);
                                PloterShapes->Draw();*/ /**
                                Sleep(getSleepingTime());
                        }

**/ /*
                        //limita el salto longitudinal respetando el spm
                        dmin -= TArmAbstract::Getspm();
                        //limita inferiormente el salto longitudinal
                        dmin = Max(SPM_MIN, dmin);
                        //determina el salto longitudinal para que no haya colisión (en mm)
                        double jl = dmin/2;
                        //limita superiormente el salto longiudinal
                        //jl = Min(, jl);
                        //determina el salto angular en radianes
                        double jtheta = jl/RP->Rc;
                        //determina el salto angular en pasos
                        double jp = RP->p_(jtheta);
                        //determina el salto temporal correspondiente al salto
                        qt = jp/VMAXABSDEF;

                        //ERROR: para determinar el salto angular en radianes
                        //debería dividirse por el radio máximo es decir
                        //Max{Rc}, aunque aquí se divide por el Rc de un
                        //posicionador cualquiera para salir del paso.
                        */
/**
                        //salta a la próxima iteración
                        t += Tfmin;
                }

                //si ha encontrado una invasión
                if(invasion)
                        //restaura laposición inicial de los brazos
                        Invaders->RestoreArmPositions();

        } while(invasion); //mientras se encuentren invasiones

        //descarta las posiciones iniciales apiladas de los brazos
        Invaders->PopArmPositions();

        //completa el desplazamiento hasta la posición final
        Invaders->MoveFin();
**/
}

//-------------------------------------------------------------------

//<----24/04/2014
/*//Para cada RP de la lista Abatibles:
//- restaura el estado del cuantificador de su brazo;
//- desempila el estado de cuantificación de su brazo;
//- restaura la posición inicial de su brazo;
//- desempila la posición inicial de su brazo.
void TMotionProgramGenerator::PopAbatibles(TRoboticPositionerList *Abatibles)
{
        //si no hay lista de abatibles
        if(Abatibles == NULL)
                return; //no hace nada

        TRoboticPositioner *RP;

        //los cuantificadores del eje 2 de los posicionadores de la lista Abatibles deberían estar desactivados
        for(int i=0; i<Abatibles->Count; i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Abatibles->Get(i);
                //si el cuantificador del eje 2 del posicionador está activado
                if(RP->getActuator()->getArm()->getQuantify___())
                        throw EImproperArgument("the axes 2 quantificator of the positioners in the list Abatibles should be disabled");
        }

        //por cada posicionador de la lista de abatibles
        for(int i=0; i<Abatibles->Count; i++) {
                //apunta el posicionador para facilitar su acceso
                RP = Abatibles->Get(i);
                //restaura la posición del brazo
//                RP->getActuator()->getArm()->Restoretheta___3();
                //desempila la posición del brazo
                RP->getActuator()->getArm()->Poptheta___3();
                //restaura y desempila laconfiguración del cuantificador del brazo
                RP->getActuator()->getArm()->RestoreQuantify___();
                RP->getActuator()->getArm()->PopQuantify___();

                //NOTA: como el brazo está abatido en la posición estable de seguridad
                //la restauración de la cuantificación no cambiará su posición.
        }
}
  */
//Segrega los posicionadores invasores cuyo rot 1 está desplazado,
//y programa un gesto de giro desde su posición inicial a la actual.
//Todos los posicionadores de la lista Invaders:
//      deben tener apilada una posición de rot 1.
void TMotionProgramGenerator::ProgramTurns(TRoboticPositionerList& TurnsInvaders,
        TRoboticPositionerList& Invaders)
{
        //Todos los posicionadores de la lista Invaders...
        for(int i=0; i<Invaders.getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                TRoboticPositioner *RP = Invaders.Get(i);

                //...deben tener apilada una posición de rot 1
                if(RP->getActuator()->theta_1s.getCount()<1 || RP->getActuator()->IsntInDomaintheta_1(RP->getActuator()->theta_1s.getLast()))
                        throw EImproperArgument("all positioner in the list Invaders should has stacked a rot 1 position");
        }

        //inicializa la lista de los invasores que giran
        TurnsInvaders.Clear();

        //si la lista de invasores está vacía
        if(Invaders.getCount() < 1)
                return; //indica que ha completado la programación con éxito

        TRoboticPositioner *RP; //RP indicado
        double p_1ini; //posición inicial de rot 1
        double p_1fin; //posición final de rot 1

        //por cada posicionador de la lista de invasores
        for(int i=0; i<Invaders.getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Invaders.Get(i);

                //asigna la posición final del cilindro
                p_1ini = RP->getActuator()->getF().Image(RP->getActuator()->theta_1s.getLast());
                //asigna la posición final del cilindro
                p_1fin = RP->getActuator()->getp_1();

                //Otra manera de hacer esto sería:
                //      //restaura y desempila la posición inicial del cilindro
                //      RP->getActuator()->Restoretheta_1();
                //      //programa el gesto de giro hasta la posición final del cilindro
                //      RP->M1(RP->getActuator()->p_1);
                //      //vuelve a la posición final
                //      RP->getActuator()->p_1 = p_1fin;

                //si hay que girar
                if(p_1fin != p_1ini) {
                        //programa el giro de rot 1 desde la posición inicial hasta la posición final
                        RP->M1(p_1ini, p_1fin);
                        //añade el posicionador a la lista de invasores que han girado
                        TurnsInvaders.Add(RP);
                }

                //Hay que comprobar si hay o no que girar, ya que
                //las invasiones se resulven sin que algunos tengan que girar.
        }
}

//construye una lista de mensajes de instrucción a partir de
//una lista de posicionadores
TMessageList *TMotionProgramGenerator::GetMessages(TRoboticPositionerList &RPL)
{
        TRoboticPositioner *RP;
        TMessageInstruction *M;

        //contruye una lista de mensajes de instrucción
        TMessageList *LM = new TMessageList();

        //por cada posicionador de la lista
        for(int i=0; i<RPL.getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = RPL.Get(i);

                //contruye un mensaje
                M = new TMessageInstruction();
                //configura el mensaje
                M->setId(RP->getActuator()->getId());
                RP->GetInstruction(M->Instruction);

                //apunta el mensaje en la lista de mensajes
                LM->Add(M);
        }

        return LM;
}
/**
//------------------------------------------------------------------
//MÉTODOS PARA GENERAR PROGRAMAS DE MOVIMIENTO:

//Genera un programa de despliegue de hasta tres pasos:
//      1. despliegue de los invasores;
//      2. giro de los invasores;
//      3. despliegue de los desplegables.
//parámetros:
//      DeployProgram: programa de despliegue;
//      Outsiders: lista de RPs a desplegar;
//Los posicionadores quedarán abatidos en las posiciones de partida
//para ir a las posiciones finales.
void TMotionProgramGenerator::GenerateDeployProgram(
        TMotionProgram &DeployProgram,
        TRoboticPositionerList &Outsiders)
{
        //COMPRUEBA EL CASO EN EL QUE NO HAY QUE HACER NADA:

        //si no hay posicionadores desplegados
        if(Outsiders.getCount() < 1)
                //no hace nada
                return;

        //COMPRUEBA QUE SE CUMPLEN LAS PRECONDICIONES:

        //todos los posicionadores de la lista Outsiders deben estar desplegados
        for(int i=0; i<Outsiders.getCount(); i++)
                if(Outsiders[i]->getActuator()->ArmIsInSafeArea())
                        throw EImproperArgument("all fiber positioner in the list Outsiders should be deployed");

        //ninguno de los posicionadores de la lista Outsiders debe estar en estado de colisión
//        for(int i=0; i<Outsiders.getCount(); i++)
  //              if(Outsiders[i]->getActuator()->InvadeAdjacent())
    //                    throw EImproperArgument("any fiber positioner in the list Outsiders should be in collision status");

        //a partir de aquí hay almenos un posicionador no abatido

        //-------------------------------------------------------------------
        //INTENTA GENERAR EL PROGRAMA:

        //##################################################################
        //PROGRAMA EL ABATIMIENTO Y SEGREGA LOS INVASORES:
        //##################################################################

        //programa el abatimineto de
        //todos los posicionadores de la lista Outsiders
        TRoboticPositionerList *Abatibles = ProgramAbatement(Outsiders);

        //sincroniza las llegadas de los brazos de
        //todos los posicionadores de la lista Outsiders
        if(Abatibles != NULL)
                Abatibles->SinchronizeArrivals();

        //simula el abatimineto y segrega los posicionadores invasores
        TRoboticPositionerList *Invaders = SegregateInvaders(Abatibles**/ /*, Outsiders*/ /**);

        //Los posicionadores abatibles han quedado abatidos
        //de modo que no interferirán con los invasores.

        //Sino hubiese posicionadores invasores Invaders será nulo.

        //##################################################################
        //ABATE LOS INVASORES:
        //##################################################################

        //PASO 1: SIMULA EL ABATIMIENTO DE LOS POSICIONADORES INVASORES
        //RESOLVIENDO LAS INVASIONES EN CADA PASO

        //simula elabatimiento de los posicionadores invasores
        //resolviendo las colisiones en cada paso
        //mediante giros del eje 1
        AbateInvaders(Invaders);///, Outsiders);

        //Ahora los brazos de los posicionadores invasores han quedado abatidos
        //y sus cilindros en la posición angular a la que deben girar para poder
        //abatir sus brazos sin que colisionen.

        //PASO 3: DESEMPILA LAS POSICIONES INICIALES DE LOS BRAZOS
        //DE LOS POSICIONADORES ABATIBLES:

//        PopAbatibles(Abatibles);

        //PASO 4:
        //      INVIERTE EL TIEMPO DE LAS LITAS DE CONTROLADORES
        //      AÑADE LAS LISTAS DE GESTOS NO VACÍAS A LA COREOGRAFÍA
        //      Y DESTRUYE LAS LISTAS DE GESTOS VACÍAS:

        TMessageList *LM; //puntero a listas de mensajes aañadir al programa de despliegue

        //añade al programa la lista de gestos de despliegue de los invasores
        //y destruye la lista Invaders (la cual es de punteros)
        if(Invaders != NULL) {
                if(Invaders->Count > 0) {
                        Invaders->InvertTime();
                        LM = GetMessages(Invaders);
                        DeployProgram.Add(LM);
                }
                delete Invaders;
        }

        //

        //añade al programa la lista de gestos de despliegue de los abatibles
        //y destruye la lista Abatibles (la cual es de punteros)
        if(Abatibles != NULL) {
                if(Abatibles->Count > 0) {
                        Abatibles->InvertTime();
                        LM = GetMessages(Abatibles);
                        DeployProgram.Add(LM);
                }
                delete Abatibles;
        }

        //PASO 2: PROGRAMA LOS GIROS DE LOS INVASORES:

        //Programa el giro de los invasores
        TRoboticPositionerList *TurnsInvaders = ProgramTurns(Invaders);

        //sincroniza las llegadas de los gestos de las listas
        if(TurnsInvaders != NULL)
                TurnsInvaders->SinchronizeArrivals();

        //

        //añade o destruye la lista de gestos de giro de los invasores
        if(TurnsInvaders != NULL) {
                if(TurnsInvaders->Count > 0) {
                        TurnsInvaders->InvertTime();
                        LM = GetMessages(TurnsInvaders);
                        DeployProgram.Insert(1, LM);
                }
                delete TurnsInvaders;
        }

        //destruye la lista de invasores
        if(Invaders != NULL)
                delete Invaders;

        //-------------------------------------------------------------------
        //DESEMPILA LAS POSICIONES INICIALES DE TODOS LOS POSICIONADORES:

        Outsiders.PopArmPositions();
}
**/
//Genera un programa de deposicionamiento de hasta tres pasos:
//      1. retracción de los retráctiles;
//      2. giro de los invasores;
//      3. retracción de los invasores.
//parámetros:
//      DP: programa de deposicionamiento;
//      Outsiders: lista de posicionadores a retraer;
//Los posicionadores quedarán retraidos en posiciones de seguridad.
void TMotionProgramGenerator::GenerateDepositioningProgram(TMotionProgram &DP,
        TRoboticPositionerList &Outsiders)
{
        //-------------------------------------------------------------------
        //1. COMPRUEBA LAS PRECONDICIONES:

/**        //Todos los posicionadores desplegados deben estar en la lista Outsiders:

        //busca posicionadores que no cumplan la precondición
        TVector<int> indices;
        RoboticPositionerList->SearchInSafeAreaAbsentInTheList(indices, Outsiders);
        //si ha encontrado algún posicionador que incumple la precondición
        if(indices.getCount() > 0)
                return 1; //indica que los posicionadores indicados no cumplen la condición

        //Todos los posicioandores de de la lista Outsiders deben estar en posiciones de inseguridad:

        //busca posicionadores que no cumplan la precondición
        Outsiders.SearchInSafeArea(indices);
        //si ha encontrado algún posicionador que incumple la precondición
        if(indices.getCount() > 0)
                return 2; //indica que los posicionadores indicados no cumplen la condición

        //Todos los posicionadores de la lista Outsiders deben tener la posición
        //del eje 1 activada y la del eje 2 desactivada:

        //busca posicionadores que no cumplan la precondición
        Outsiders.SearchWithQuantification1offOrQuantification2on(indices);
        //si ha encontrado algún posicionador que incumple la precondición
        if(indices.getCount() > 0)
                return 3; //indica que los posicionadores indicados no cumplen la condición

        //Ninguno de los posicionadores de la lista Outsiders
        //debe estar en estado de colisión dinámica:

        //busca posicionadores que no cumplan la precondición
        RoboticPositionerList->SearchCollinding(indices);
        //si ha encontrado algún posicionador que incumple la precondición
        if(indices.getCount() > 0)
                return 4; //indica que los posicionadores indicados no cumplen la condición
**/


        //-------------------------------------------------------------------
        //2. INICIALIZA EL PROGRAMA DE DEPOSICIONAMIENTO Y SI NO HAY
        //POSICIONADORES EN POSICIONES DE INSEGURIDAD, DE  INDICA QUE
        //LA PROGRAMACIÓN DEL DEPOSICIONAMIENTO HA CONCLUIDO CON ÉXITO:

        //inicializa el programa de DEPOSICIONAMIENTO
        DP.Clear();

        //si no hay posicionadores desplegados
        if(Outsiders.getCount() < 1)
                //indica que la programación ha concluido con éxito
                return;// 0;

        //A partir de aquí hay al menos un posicionador externo.

        //-------------------------------------------------------------------
        //2. GUARDA LAS POSICIONES INICIALES DE TODOS LOS POSICIONADORES:

///        Outsiders.PushPositions();

        //-------------------------------------------------------------------
        //PROGRAMA EL ABATIMIENTO DE LOS ABATIBLES:

        //PASO 1: PROGRAMA LOS ABATIMIENTOS DE TODOS LOS POSICIONADORES:

        //Para cada RP de la lista Outsiders:
        //- apila la posición inicial de su brazo;
        //- apila el estado del cuantificador del brazo;
        //- desactiva la cuantificación del brazo;
        //- programa un gesto de abatimiento.
        //Si hay algún posicionador previamente abatido
        //lanza una excepción EImproperArgument.
///        TRoboticPositionerList *Abatibles = ProgramAbatement(Outsiders);

        //Para todos los posicionadores externos (de Outsiders):
        // - La posición del eje 2 ha quedado apilada.
        // - El cuantificador del eje 2 ha quedado desactivado y su estado apilado.

        //PASO 3: SIMULA EL ABATIMIENTO Y SEGREGA LOS POSICIONADORES INVASORES:

        //Simula el abatimiento de los brazos de los posicionadores de la lista
        //Abatibles y segrega los posicionadores invasores.
        //El escalón de cuantificación del tiempo (qt) debe ser mayor que cero.
        //El puntero Abatibles puede ser nulo.
        //La bandera de cuantificación del eje 2 de los posicionadores de
        //la lista Abatibles deberían estar desactivadas.
        //Los posicionadores de la lista Abatibles quedarán abatidos.
        //Los posicionadores de la lista Invaders quedarán sin abatir.
///        TRoboticPositionerList *Invaders = SegregateInvaders(Abatibles/*, Outsiders*/ /**);**/  ////****####
        TRoboticPositionerList Invaders, Retractiles;
        RetractRetractiles(Invaders, Retractiles, Outsiders);

        //Los posicionadores retractiles han quedado retraidos de modo que
        //no interferirán con los invasores.
        //Los posicionadores invasores han quedado en sus posiciones iniciales.

        //En este momento los posicionadores de Outsiders:
        //      tienen apiladas las posiciones iniciales de sus rotores
        //      tienen deshabilitados los cuantificadores de los rotores

        //Construye el gesto a partir de los gestos programados
        //en la lista Retráctiles:

        TMessageList *LM;

        //añade la lista de gestos de retracción de los retráctiles
        if(Retractiles.getCount() > 0) {
                LM = GetMessages(Retractiles);
                DP.Add(LM);
        }

        //retrae los posicionadores de la lista Invaders
        //y segrega los invasores de los retráctiles
        RetractRetractiles(Invaders, Retractiles, Invaders);
        //añade la lista de gestos de retracción de los retráctiles
        if(Retractiles.getCount() > 0) {
                LM = GetMessages(Retractiles);
                DP.Add(LM);
        }

        //-------------------------------------------------------------------
        //PROGRAMA EL GIRO Y ABATIMIENTO DE LOS INVASORES

        //PASO 1: SIMULA EL ABATIMIENTO DE LOS POSICIONADORES INVASORES
        //RESOLVIENDO LAS INVASIONES EN CADA PASO

        //Simula el abatimiento de los brazos de los posicionadores de la lista
        //Invaders resolviendo las invasiones en cada paso
        //mediante desplazamientos del eje 1 en sentido negativo.
        //El escalón de cuantificación del tiempo (qt) debe ser mayor que cero.
        //Las banderas de cuantificación de ambos ejes de los posicionadores de
        //la lista Invaders deberían estar desactivadas.
        //Los brazos de los posicionadores quedarán abatidos.
        //Los cilindros de los posicionadores quedarán desplazados.
///        AbateInvaders(Invaders);///, Outsiders);
        RetractInvaders(Invaders);

        //PASO 3: DESEMPILA LAS POSICIONES INICIALES DE LOS BRAZOS
        //DE LOS POSICIONADORES ABATIBLES:

//        Pop(Abatibles, Invaders);

        //NOTA: las posiciones iniciales del eje 2 de los posicioandores
        //quedó apilada en el momento de programar el abatimiento
        //y por eso ahora debe ser descartada.

        //PASO 5: AÑADE LAS LISTAS DE GESTOS NO VACÍAS A LA COREOGRAFÍA
        //Y DESTRUYE LAS LISTAS DE GESTOS VACÍAS:

        //añade o destruye la lista de gestos de abatimiento de los invasores
        if(Invaders.getCount() > 0) {
                LM = GetMessages(Invaders);
                DP.Add(LM);
        }

          //PASO 2: PROGRAMA LOS GIROS DE LOS INVASORES:

        //Programa el giro de los invasores
        TRoboticPositionerList TurnsInvaders;
        ProgramTurns(TurnsInvaders, Invaders);

        //Las posiciones de los ejes de los posicionadores no han cambiado
        //de modo que permanecen en sus posiciones finales.

        //

        //añade o destruye la lista de gestos de giro de los invasores
        if(TurnsInvaders.getCount() > 0) {
                LM = GetMessages(TurnsInvaders);
                DP.Insert(DP.getCount()-1, LM);
        }

        //-------------------------------------------------------------------
        //6. DESEMPILA LAS POSICIONES INICIALES DE TODOS LOS POSICIONADORES:

///        Outsiders.PopPositions();
}
/**
//genera un programa de ida directa desde las posiciones actuales hasta:
//      los puntos internos
//      los puntos de partida para ir a los puntos externos
void TMotionProgramGenerator::GenerateGoDirectlyProgram(
        TMotionProgram &PositioningProgram,
        TVector<double> &p_1s, TVector<double> &p___3s,
        TRoboticPositionerList &FinalOutsiders,
        TRoboticPositionerList &FinalInners)
{
        TRoboticPositioner *RP;

        //el vector p_1s debe contener una coordenada por
        //cada posicionador de la lista FinalOutsiders
        if(p_1s.getCount() != FinalOutsiders.getCount())
                throw EImproperArgument("vector p_1s should contain an coordinate by each positioner in the list FinalOutsiders");

        //el vector p___3s debe contener una coordenada por
        //cada posicionador de la lista FinalOutsiders
        if(p___3s.getCount() != FinalOutsiders.getCount())
                throw EImproperArgument("vector p___3s should contain an coordinate by each positioner in the list FinalOutsiders");

        //Cada posicionador de FinalOutsiders:
        // - los ejes deben estar en posiciones estables
        // - debe estar adscrito a un punto objetivo;
        // - el punto objetivo debe estar fuera del área de seguridad;
        // - el punto de partida para ir al punto objetivo debe tener coordenadas posicionales estables;
        // - el punto de partida para ir al punto objetivo debe estar en el dominio del posicionador.
        // - el punto de partida para ir al punto objetivo debe estar dentro del área de seguridad.
        for(int i=0; i<FinalOutsiders.getCount(); i++) {
                //apunta el RP para facilitar su acceso
                RP = FinalOutsiders[i];
                //los ejesdeben estar en posiciones estables
                if(fmod(RP->getActuator()->p_1, RP->getActuator()->p_1)>0 || fmod(RP->getActuator()->getArm()->p___3, RP->getActuator()->getArm()->p___3)>0)
                        throw EImproperArgument("axes should be in stable positions");
                //busca el punto objetivo adscrito al posicionador
                int j = SearchTargetPoint(RP);
                //el posicionador FinalOutsiders[i] debe estar adscrito a un punto objetivo
                if(j >= Count)
                        throw EImproperArgument("fiber positioner FinnalOutsiders["+IntToStr(i)+"] should be attached to a target points");
                //el punto objetivo del posicionador FinnalOutsiders[i] debe estar fuera de su área de seguridad
                if(RP->getActuator()->P3IsInSafeArea(Items[j]->TargetP3))
                        throw EImproperArgument("target points of FinnalOutsiders["+IntToStr(i)+"] should be out its safe area");
                //el punto de partida para ir al punto objetivo debe tener coordenadas posicionales estables;
                if(fmod(p_1s[i], p_1s[i])>0 || fmod(p___3s[i], p___3s[i])>0)
                        throw EImproperArgument("the starting point (p_1["+IntToStr(i)+"], p___3s["+IntToStr(i)+"]) should be stable");
                //el punto de partida para ir al punto objetivo debe estar en el dominio del posicionador.
                if(RP->getActuator()->IsntInDomainp_1(p_1s[i]) || RP->getActuator()->getArm()->IsntInDomainp___3(p___3s[i]))
                        throw EImproperArgument("the starting point (p_1["+IntToStr(i)+"], p___3s["+IntToStr(i)+"]) should be in domine FinalOutsiders["+IntToStr(i)+"]");
                //el punto de partida para ir al punto objetivo debe estar dentro del área de seguridad.
                if(RP->getActuator()->p___3IsOutSafeArea(p___3s[i]))
                        throw EImproperArgument("the coordinate p___3s["+IntToStr(i)+"] should be in safe area FinalOutsiders["+IntToStr(i)+"]");
        }
**/ /*
        //Cada posicionador de FinnalInners:
        // - los ejes deben estar en posiciones estables
        // - eben estar adscrito a un punto objetivo;
        // - el punto objetivo debe estar dentro del área de seguridad.
        for(int i=0; i<FinalInners.getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = FinalInners[i];
                //los ejesdeben estar en posiciones estables
                if(fmod(RP->getActuator()->p_1, RP->getActuator()->p_1)>0 || fmod(RP->getActuator()->getArm()->p___3, RP->getActuator()->getArm()->p___3)>0)
                        throw EImproperArgument("axes should be in stable positions");
                //busca el punto objetivo adscrito al posicionador
                int j = SearchTargetPoint(RP);
                //el posicionador FinalInners[i] debe estar adscrito a un punto objetivo
                if(j >= TargetPoints.getCount())
                        throw EImproperArgument("fiber positioner FinnalInners["+IntToStr(i)+"] should be attached to a target points");
                //el punto objetivo del posicionador FinnalInners[i] debe estar dentro de su área de seguridad
                if(RP->getActuator()->P3IsOutSafeArea(TargetPoints[j]->TargetP3))
                        throw EImproperArgument("target points of FinnalInners["+IntToStr(i)+"] should be in its safe area");
        }
  */ /**
        //PROGRAMA LAS IDAS A LOS PUNTOS DE PARTIDA
        //CORRESPONDIENTES A LOS PUNTOS EXTERNOS:

        TMessageInstruction *M;
        TMessageList *LM = new TMessageList();
        double p_1, p___3;

        //por cada posicionador con punto final externo
        for(int i=0; i<FinalOutsiders.getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = FinalOutsiders[i];
                //asigna las posiciones de destino para facilitar su acceso
                p_1 = p_1s[i];
                p___3 = p___3s[i];

                //si ambas posiciones de destino difieren de la actual
                if(p_1!=RP->getActuator()->p_1 && p___3!=RP->getActuator()->getArm()->p___3) {
                        //contruye un mensaje adscrito al posicionador
                        M = new TMessageInstruction();
                        M->Id = RP->getActuator()->Id;

                        //programa el mensajes para que vaya directamente hasta
                        //el punto de partida para ir al punto objetivo
                        M->Instruction.Name = "MM";
                        M->Instruction.Args[0] = p_1;
                        M->Instruction.Args[1] = p___3;

                        //añade el mensaje a la lista de mensajes
                        LM->Add(M);
                }
                //si no, si solo difiere la posición del eje 1
                else if(p_1 != RP->getActuator()->p_1) {
                        //contruye un mensaje adscrito al posicionador
                        M = new TMessageInstruction();
                        M->Id = RP->getActuator()->Id;

                        //programa el mnsaje para que vaya directamente hasta
                        //el punto de partida para ir al punto objtivo
                        M->Instruction.Name = "M1";
                        M->Instruction.Args[0] = p_1;

                        //añade el mensaje a la lista de mensajes
                        LM->Add(M);
                }
                //si no, si solo difiere la posición del eje 2
                else if(p___3 != RP->getActuator()->getArm()->p___3) {
                        //contruye un mensaje adscrito al posicionador
                        M = new TMessageInstruction();
                        M->Id = RP->getActuator()->Id;

                        //programa el mensaje para que vaya directamente hasta
                        //el punto de partida para ir al punto objetivo
                        M->Instruction.Name = "M2";
                        M->Instruction.Args[0] = p___3;

                        //añade el mensaje a la lista de mensajes
                        LM->Add(M);
                }
        }

        //PROGRAMA LAS IDAS A LOS PUNTOS INTERNOS:

        TDoublePoint TP;

        //por cada posicionador con punto final interno
        for(int i=0; i<FinalInners.getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = FinalInners[i];

                //busca el punto objetivo adscrito al posicionador
                int aux = SearchTargetPoint(RP);

                //di ha encontrado un punto objetivo
                if(aux < Count) {
                //---------------------------------
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[aux]->TargetP3;

                //traduce el punto objetivo a coordenadas posicionales angulares
                double theta_1, theta___3;
                RP->getActuator()->AnglesToGoP3(theta_1, theta___3, TP.x, TP.y);

                //traduce las coordenadas posicionales angulares a pasos
                p_1 = RP->getActuator()->getF().Image(theta_1);
                p___3 = RP->getActuator()->getArm()->getF().Image(theta___3);

                //si ambas posiciones de destino difieren de la actual
                if(p_1!=RP->getActuator()->p_1 && p___3!=RP->getActuator()->getArm()->p___3) {
                        //contruye un mensaje adscrito al posicionador
                        M = new TMessageInstruction();
                        M->Id = RP->getActuator()->Id;

                        //programa el mensaje para que vaya directamente hasta
                        //el punto de partida para ir al punto objetivo
                        M->Instruction.Name = "MM";
                        M->Instruction.Args[0] = p_1;
                        M->Instruction.Args[1] = p___3;

                        //añade el mensaje a la lista de mensajes
                        LM->Add(M);
                }
                //si no, si solo la posición del eje 1 difiere de la actual
                else if(p_1 != RP->getActuator()->p_1) {
                        //contruye un mensaje adscrito al posicionador
                        M = new TMessageInstruction();
                        M->Id = RP->getActuator()->Id;

                        //programa el mensaje para que vaya directamente hasta
                        //el punto de partida para ir al punto objetivo
                        M->Instruction.Name = "M1";
                        M->Instruction.Args[0] = p_1;

                        //añade el mensaje a la lista de mensajes
                        LM->Add(M);
                }
                //si solo la posición del eje 2 difiere de la actual
                else if(p___3 != RP->getActuator()->getArm()->p___3) {
                        //contruye un mensaje adscrito al posicionador
                        M = new TMessageInstruction();
                        M->Id = RP->getActuator()->Id;

                        //programa el mensaje para que vaya directamente hasta
                        //el punto de partida para ir al punto objetivo
                        M->Instruction.Name = "M2";
                        M->Instruction.Args[0] = p___3;

                        //añade el mensaje a la lista de mensajes
                        LM->Add(M);
                }
                //---------------------------------
                }
        }

        //AÑADE LA LISTA DE MENSAJES AL PROGRAMA DE POSICIONAMIENTO O LA DESTRUYE:

        //si la lista de mnsajes no stá vacía
        if(LM->Count > 0)
                PositioningProgram.Add(LM); //añade la lista
        else //si no
                delete LM; //destruye la lista
}

//Genera un programa de cambio de posiciones.
//Parámetros:
//      PCP: programa de cambio de posiciones.
//      indices: índices a los pares defectuosos.
//Valores de retorno:
//      0: programación finalizada con éxito;
//      1: los posicionadores indicados tienen
//         algúncuantificador desactivado;
//      2: los posicionadores indicados están en
//         posiciones iniciales incompatibles;
//      3: los puntos objetivo indicados están adscritos a
//         posicionadores repetidos;
//      4: los puntos objetivo indicados están adscritos a
//         posicionadores ausentes;
//      5: los puntos objetivo indicados están fuera del dominio
//         de sus posicionadores adscritos;
//      6: los puntos objetivo indicados están en
//         posiciones incompatibles.
char TMotionProgramGenerator::GeneratePositioningChangeProgram(TMotionProgram &PCP,
        TVector<int> &indices)
{
        //-------------------------------------------------------------------
        //1. COMPRUEBA LAS PRECONDICIONES:

        //COMPRUEBA QUE TODOS LOS CUANTIFICADORES ESTÁN ACTIVADOS:

        //busca los posicionadores cuyos cuantificadores no están todos activados
        RoboticPositionerList->SearchDisabledQuantificators(indices);
        //si ha encontrado algún posicionador
        if(indices.getCount() > 0)
                //indica la causa del fallo
                return 1;

        //COMRPUEBA QUE TODOS LOS POSICIONADORES ESTÁN EN
        //POSICIONES INICIALES COMPATIBLES:

        //busca los posicionadores cuyos actuadores están en colisión
        RoboticPositionerList->SearchCollinding(indices);
        //si ha encontrado algún posicionador
        if(indices.getCount() > 0)
                //indica la causa del fallo
                return 2;

        //La programación del deposicionamiento, podría generalizarse
        //al caso en que los actuadores de los posicionadores están
        //inicialmente en estado de colisión. Sin embargo esto no se
        //considera que sea una buena idea, ya que en caso de solapamiento,
        //la dirección de giro de los ejes 1 puede cambiar, ya que lo harán
        //en el sentido en que sea necesario un desplazamiento angular menor
        //para resolver las colisiones, y si eso llegase a pasar, el programa
        //generado no estaría libre de colisiones. Por eso esta condición
        //debe comprobarse aquí.

        //COMRPUEBA QUE CADA PUNTO OBJETIVO ESTÁ ADSCRITO A
        //UN SOLO POSICIONADOR:

    //busca los puntos objetivo adscritos a posicionadores repetidos
        SearchRepeatedRoboticPositioners(indices);
    //Si ha encontrado algún punto objetivo
        if(indices.getCount() > 0)
                //indica la causa del fallo
                return 3;

        //Se supone que la condición de no repetición se logra controlando
        //la adición de puntos objetivos. Esto es conveniente para detectar
        //asignaciones duplicadas a medida que se van haciendo.

        //COMPRUEBA QUE TODOS LOS POSICIONADORES ADSCRITOS A LOS PUNTOS OBJETIVO
        //ESTÁN EN LA LISTA DE POSICIONADORES:

    //busca los puntos objetivo adscritos a posicionadores ausentes
        //en la lista de posicionadores
        SearchMissingRoboticPositioners(indices);
    //Si ha encontrado algún punto objetivo
        if(indices.getCount() > 0) //si ha encontrado algún posicionador ausente
                //indica la causa del fallo
                return 4;

        //COMPRUEBA QUE TODOS LOS PUNTOS OBJETIVO ESTÁN EN
        //EL DOMINIO DE SUS POSICIONADORES ADSCRITOS:

    //busca los puntos objetivo que están fuera del dominio
        //de sus posicionadores adscritos
        SearchOutDomineTargetPoints(indices);
    //Si ha encontrado algún punto objetivo
        if(indices.getCount() > 0) //si ha encontrado algún punto fuera del dominio
                //indica la causa del fallo
                return 5;

        //-------------------------------------------------------------------
        //2. SEGREGA LOS POSICIONADORES INTERNOS Y EXTERNOS:

        TRoboticPositionerList InitialInners, InitialOutsiders;
        RoboticPositionerList->SegregateInOut(InitialInners, InitialOutsiders);

        //Nótese que la lista de posicionadores InitialInners no se usa
        //en este método, habiéndose declarado con el único fin de pasarla
        //como argumento al método SegregateInOut de RoboticPositionerList.
**/
/*        //-------------------------------------------------------------------
        //3. SEGREGA LOS POSICIONADORES CON PUNTOS OBJETIVO
        //INTERNOS Y EXTERNOS:

        TRoboticPositionerList FinalInners, FinalOutsiders;
        SegregateInOut(FinalInners, FinalOutsiders);

        //Nótese que la lista de posicionadores FinalInners se usará para
        //programar las idas directas a los puntos objetivo en las zonas
        //de seguridad.
  */ /**
        //-------------------------------------------------------------------
        //3. APILA LAS POSICIONES INICIALES Y MUEVE LOS POSICIONADORES
        //A SUS POSICIONES FINALES:

        //apila las posiciones iniciales
        RoboticPositionerList->PushPositions();
        //abate todos los actuadores
        InitialOutsiders.MoveOutsideArmsToSafePositions();
        //mueve los actuadores a sus puntos objetivo
        MoveToTargetP3();

        //ADVERTENCIA: algunos actuadores con posiciones finales externas
        //pueden no tener punto objetivo asignado.

        //-------------------------------------------------------------------
        //4. COMPRUEBA QUE TODOS LOS PUNTOS OBJETIVO ESTÁN EN
        //POSICIONES COMPATIBLES:

        //busca los puntos objetivo adscritos a posicionadores en colisión
        SearchCollindingTargetPoints(indices);
        //si ha encontrado alguna colisión
        if(indices.getCount() > 0) {
                //restaura las posiciones iniciales de todos los posicioandores
                RoboticPositionerList->RestoreAndPopPositions();
                //indica la causa del fallo
                return 6;
        }

        //Es preferible determinar los puntos objetivo adscritos a
        //los posicionadores en colisión mediante:
        //      SearchCollindingTargetPoints(indices);
        //en vez de los posicionadores en colisión mediante:
        //      FinalOutsiders.SearchCollinding(indices);
        //por dos motivos:
        // 1. El brazo de un posicionador no abatido puede colisionar con
        //    el brazo de otro posicionador adyacente si abatido, con tal
        //    de que el brazo abatido sea lo bastante ancho y el brazo no
        //    abatido esté lo bastante extendido.
        // 2. Por que el defecto está en los puntos objetivo y no en
        //    los posicionadores.

        //A partir de aquí se puede encontrar siempre un programa de
        //posicionamiento para ir de las posiciones iniciales a las finales,
        //por que se cumplen todas las condiciones.

        //-------------------------------------------------------------------
        //5. SEGREGA LOS POSICIONADORES CON POSICIONES FINALES
        //INTERNAS Y EXTERNAS:

        TRoboticPositionerList FinalInners, FinalOutsiders;
        RoboticPositionerList->SegregateInOut(FinalInners, FinalOutsiders);

        //-------------------------------------------------------------------
        //6. APILA LA CONFIGURACIÓN DE LOS CUANTIFICADORES Y LOS CONFIGURA:

        //apila la configuración de los cuantificadores
        RoboticPositionerList->PushQuantifys();
        //configura los cuantificadores para que
        //      los ejes 1 se mueva a posiciones estables
        //      los ejes 2 se muevan en función del tiempo
        RoboticPositionerList->SetQuantifys(true, false);

        //---------------------------------
        //7. PROGRAMA EL DESPLIEGUE Y GUARDA LAS POSICIONES DE PARTIDA:

        //programa el abatimiento de los posicionadores con
        //posiciones finales fuera del área de seguridad
        TMotionProgram aux;
        GenerateDeployProgram(aux, FinalOutsiders);

        //guarda las posiciones finales a las que deben ir
        //los posicionadores con posiciones finales externas
        TRoboticPositioner *RP;
        TVector<double> p_1fin;
        TVector<double> p___3fin;
        for(int i=0; i<FinalOutsiders.getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = FinalOutsiders.Get(i);
                //guarda las posiciones
                p_1fin.Add(RP->getActuator()->p_1);
                p___3fin.Add(Round(RP->getActuator()->getArm()->p___3));
        }

        //Conviene que las posiciones vengan dadas enpasos, porque van
        //a ser usadas para generar las instrucciones de movimiento
        //para las idas directas.
        //Las posiciones en pasos deben ser redondeadas para eliminar
        //los errores numéricos introducidos en la corversión
        //(puesto que las posiciones son almacenadas en radianes).
        //Como el cuantificador del eje 1 está activo, su posición en pasos
        //ya viene cuantificada.

        //-------------------------------------------------------------------
        //8. RESTAURA LAS POSICIONES INICIALES DE TODOS LOS POSICIONADORES:

        RoboticPositionerList->RestorePositions();

        //-------------------------------------------------------------------
        //9. PROGRAMA LOS ABATIMIENTOS DESDE LAS POSICIONES INICIALES:

        //borra el programa previo
//        PositioningProgram.Clear();

        //programa el abatimiento de los posicionadores con
        //posiciones iniciales fuera del área de seguridad
        GenerateRetractionProgram(PCP, InitialOutsiders);

        //-------------------------------------------------------------------
        //10. PROGRAMA LAS IDAS DIRECTAS A LAS POSICIONES FINALES INTERNAS
        //Y LOS PUNTOS DE PARTIDA PARA IR A LAS POSICIONES FINALES EXTERNAS:

        GenerateGoDirectlyProgram(PCP, p_1fin, p___3fin,
                FinalOutsiders, FinalInners);

        //-------------------------------------------------------------------
        //11. CONCATENA EL PROGRAMA DE DESPLIEGUE
        //AL PROGRAMA DE CAMBIO DE POSICIONAMIENTO:

        //por cada lista de gestos del programa de despliegue
        for(int l=0; l<aux.getCount(); l++)
                //añade la lista de gestos al programa de posicionamiento
                PCP.Add(aux.GetPointer(l));

        //vacia el programa de despliegue evitando
        //la destrucción de sus elementos
        aux.ClearWithoutDestroy();

        //-------------------------------------------------------------------
        //12. RESTAURA Y DESEMPILA LAS POSICIONES INICIALES Y EL ESTADO DE
        //LOS CUANTIFICADORES DE TODOS LOS POSICIONADORES DE LA LISTA:

        //restaura y desempila ekl estado de los cuantificadores
        //de todos los posicionadores
        RoboticPositionerList->RestoreAndPopQuantifys();
        //restaura y desempila las posiciones iniciales
        //de todos los posicionadores
        RoboticPositionerList->RestoreAndPopPositions();

        //-------------------------------------------------------------------
        //13. indica que se ha programado con éxito el posicionamiento
        return 0;
}
               **/
//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------

