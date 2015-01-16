//---------------------------------------------------------------------------
//Archivo: FiberMOSModelConstants.h
//Contenido: formulario acerca de, de MEGARA
//Última actualización: 26/07/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FIBERMOSMODELCONSTANTS_H
#define FIBERMOSMODELCONSTANTS_H

/*#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
*/
//ADVERTENCIA: si se nombrara una constante como L a secas, habría conflicto
//con el parámetro L definido en los métodos de las clases de listas.
//Dicho conflicto podría resolverse incluyendo la definición de la constante L
//despues de la inclusión de los módulos que contienen las clases de listas
//que se vayan a utilizar. Pero para evitar este inconveniente se prefiere
//utilizar nombres de contantes más específicos y evitar los nombres genéricos
//como L.

//--------------------------------------------------------------------------
//Constantes del Fiber MOS Model:
//--------------------------------------------------------------------------

//VALORES POR DEFECTO DE LAS TOLERANCIAS DE FABRICACIÓN:

//valor por defecto de
//el margen de error de orientación en S1 por mecanizado (en rad)
#define MEGARA_Eo 0.001
//valor por defecto de
//el margen de error de posición en S0 por mecanizado (en mm)
#define MEGARA_Ep 0.01

//valor por defecto de
//el tiempo máximo entre el último almacenamiento de la posición
//y la detención de ambos ejes (en ms)
#define MEGARA_Tstop 1
//valor por defecto de
//desfase temporal máximo entre el posicionadores en movimiento (en ms)
#define MEGARA_Tshif 1

//valor por defecto de
//el margen de error de orientación de S0 (en rad)
#define MEGARA_PAem 0.001
//valor por defecto de
//el margen de error de apuntado de S0 (en mm)
#define MEGARA_Pem 0.1

//VALORES POR DEFECTO DE LAS PROPIEDADES FÍSICAS:

//longitud del brazo
//debe ser mayor que cero
#define MEGARA_L 5.80237021 //mm
//contorno de la plantilla de un brazo
#define MEGARA_Contour____Text "{({2.77263413, 2.25}, {2.64034656, 2.3}, {2.77263413, 2.45}, 0.2), ({2.77263413, 2.25}, {5.80237021, 2.25}), ({5.80237021, 2.25}, {5.80237021, -2.25}, {5.80237021, 0}, 2.250), ({5.80237021, -2.25}, {2.77263413, -2.25}), ({2.64034656, -2.3}, {2.77263413, -2.25}, {2.77263413, -2.45}, 0.2), ({2.64034656, -2.3}, {1.80711062, -2.3}), ({1.80711062, -2.3}, {1.80711062, 2.3}), ({1.80711062, 2.3}, {2.64034656, 2.3})}"

//número de pasos de una vuelta completa del eje 1 (SB: Steps Back)
#define MEGARA_SB1 8123.07692 //steps

//orientación de S1 respecto S0
#define MEGARA_thetaO1 0 //rad
//límite inferior del inervalo de giro de P1 en S1
#define MEGARA_theta_1min -0.043633231299858194 //rad
//límite superior del inervalo de giro de P1 en S1
#define MEGARA_theta_1max 6.326818538479438194 //rad

//número de pasos de una vuelta completa del eje 2 (SB: Steps Back)
#define MEGARA_SB2 10000 //steps

//límite inferior del inervalo de giro de P3 en S3
#define MEGARA_theta___3min -0.043633231299858194 //rad
//límite superior del inervalo de giro de P3 en S3
#define MEGARA_theta___3max 3.185225884889648194 //rad

//SPM de asignación de precisión
#define MEGARA_SPMall_p 0.100007634797586 //mm

//radio apical del posicionador
#define MEGARA_r_max 13.940281559018

//contorno de la barrera del área de exclusión de un posicionador
#define MEGARA_BarrierContour_ "{({13.940281559018, 0}, {-13.940281559018, 0}, {0, 0}, 13.940281559018), ({-13.940281559018, 0}, {13.940281559018, 0}, {0, 0}, 13.940281559018)}"

//SPM estático para absorber los errores de mecanizado y numérico
#define MEGARA_SPMsta MEGARA_Eo*MEGARA_r_max + MEGARA_Ep

//No se debe olvidar que aunque las pruebas permitan acotar el margen de error
//de posicionamiento de la fibra dentro de determindos límites, el margen de
//error que deberá tenerse en cuenta para evitar colisiones, es el que afecta
//a la posición final del brazo del posicionador, el cual será mayor cuando
//la calibración fina se efectúe observando la fibra.

//Para reducir SPMmec así como el error de posicionamiento
//del punto P3, es posible realizar un procedimiento de calibración
//para medir las siguientes propiedades:
//      P0: posición del eje1 en S0;
//      thetaO1: orientación de S1 en S0;
//      Arm->theta__O3: orientación de S3 en S2;
//      L01: longitud del antebrazo o radio del cilindro;
//      Arm->L13: distancia entre el eje 2 y el centro de la fibra;
//      F(theta_1): f. de compresión del cuant. del eje 1;
//      Arm->F(theta___3): f. de compresión del cuant. del eje 2.
//Conocer las siguientes propiedades también puede reducir
//el margen de error, pero puede resultar más difícil medirlas,
//porque no es posible hacerlo retroiluminando las fibras:
//      Arm->L12: longitud del brazo;
//      Arm->theta____3: offset angular entre Arm->P2 y Arm->P3.
//
//Se advierte que interesa que SPMsta sea pequeño, por que es
//una medida del margen de error en el posicionamiento de la fibra.
//Además un SPMsta relativamente elevado tiene poco impacto en
//la eficiencia de los algoritmos de asignación y posicionamiento.

//SPMsta debe ser lo bastante grande para comprender:
//- el error mecánico debido al mecanizado del posicionador;
//- el error de orientación de los sistemas de referencia  de los ejes;
//- el error de ubicación de la fibra en el brazo;
//- el error mecánico de orientación en el anclaje del posicionador;
//- el error mecánico de posicionamiento en el anclaje del posicionador;
//- el error de posición debido a la falta de correspondencia de
//  F(theta_1) y Arm->F(theta___3) con al realidad.

//distancia entre posicionadores en mm
#define MEGARA_D 20.1

//En MEGARA:
//      Con L=5.80237021: 4*L*cos(M_PI/6) = 20.1

//CONSTANTES DE LAS ÁREAS DE EXCLUSIÓN:

//contorno de la barrera del EA correspondiente al LIFU
#define MEGARA_LIFU_Conatour_Text "{({-15, -15}, {-15, 15}), ({-15, 15}, {15, 15}), ({15, 15}, {15, -15}), ({15, -15}, {-15, -15})}"
//contorno de la barrera del EA correspondiente al SIFU
#define MEGARA_SIFU_Conatour_Text "{({-10, -10}, {-10, 10}), ({-10, 10}, {10, 10}), ({10, 10}, {10, -10}), ({10, -10}, {-10, -10})}"

//CONSTANTS OF THE MOTION FUNCTIONS:

#define MEGARA_VMAXABSDEF 0.6 //(steps/ms)
#define MEGARA_AMAXABSDEF 0.0007 //(steps/ms^2)

//---------------------------------------------------------------------------
//Constantes del Telescope Projection Model
//---------------------------------------------------------------------------

//radio del plano focal de GTC en mm
#define GTC_R 130.091752531644

//---------------------------------------------------------------------------
#endif // FIBERMOSMODELCONSTANTS_H
