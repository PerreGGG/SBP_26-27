/* acelero_alumnos_main.c
 * C. Serre, 2018-2020
 */

#include <msp430g2553.h>
#include <stdlib.h>
#include <stdint.h>
#include "acelero.h"

//Funciones:
void init_acelero(uint8_t id,str_acel *acel)
{/* El parametro de la funcion es un puntero a una struc,
    porque quiero modificar los valores de sus elementos.
*/
    acel->tiempo=0;
    acel->aceleracion=0;
    acel->id=id;
    //acel->cuenta=0;
}

float girar_acel(uint8_t *ptr_acel){
    /*
     * Rebem el punter, el qual apuntarà al valor de la llista i+2. Ja que a i+2
     * comencen els 4 bytes del float en big endian.
     *Recorrem aquests 4 bits del punter i els posem al array de manera inversa.
     * I un cop ja tenim l'array amb els 4 bytes ordenats els guardem a la variable float
     * acel_actual.
     */
    float acel_actual;
    uint8_t traduccio[4]; // declarem les dues variables necessàries
    traduccio[3]=ptr_acel[0];
    traduccio[2]=ptr_acel[1];
    traduccio[1]=ptr_acel[2];
    traduccio[0]=ptr_acel[3];
    acel_actual=*(float*)traduccio; // guradem l'array de 4 posicions a un float
    return acel_actual; // ens retorna l'acclereació la qual haurem d'usar per comparar
}

//Funcion principal del programa:
void main(void)
{//Variables para manejar los datos:
    str_acel un_acelerometro, //una struc para los datos de algun acelerometro cualquiera
            estadisticas_acelerometros[elementos]; /*Array d'estructures.
    *Es a dir, cada valor de l'array té la estructura dins.
    *En aquest cas, array de 4 valors. El valor 0 és una estructura (la qual també té 4 valors) amb temps,
    *aceleracio, etc.
    *Això ens serveix per no haver de fer 4 variables d'acceleracio,
    *una per cada id (com havia pensat jo en un principi).
     */
    uint8_t id_acelero=0;

    WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
    BCSCTL1 = CALBC1_8MHZ;                // Set DCO to 8MHz

    //Inicializamos todas nuestras estructuras:
    for (id_acelero=0;id_acelero<elementos;id_acelero++) {
        init_acelero(id_acelero,&estadisticas_acelerometros[id_acelero]);
    }

    // -------------A partir d'aquí escric el meu codi-------------------------------------
    uint8_t *ptr_num_bigendian; //fem un punter que apunti a valors d'1 byte
    // I com s'ha especificat en un document, la funcio que he creat "girar_acel" ja s'encarrega de recòrrer els 4 bytes del float
    float acel_max_actual, acel_actual_llista;
    int i=0;
    for(i;i<sizeof(datos_acelero);i=i+6){
        // ptr_num_bigendian=&(i+2); //punter que apunti a la posicio de i+2 (on comença el float en bigendian)
        ptr_num_bigendian=&datos_acelero[i+2]; //manera correcta d'apuntar al primer byte de l'acceleracio
                /*
                 * La seguent línia està bé però primer hem de comparar el número, per afegir el número de la llista si es
                 * més gran, és a dir aquesta línia estarà dins del condicional quan dada_llista>dada_guardada_anteriorment
        estadisticas_acelerometro[datos_acelero[i]-1].aceleracion=//punter que apunti a i+2 (i que agafi 4 bytes);
        //la funcio que gira el numero
         *
         */
        acel_max_actual=estadisticas_acelerometros[datos_acelero[i]-1].aceleracion;
        //Aquesta línia ens guarda l'últim valor d'acceleració que s'ha guardat a la llista, a una variable
        //la qual ens servirà per comparar amb el valor de la llista de valors
        acel_actual_llista=girar_acel(ptr_num_bigendian);
        /*
         * A la funció li passem el punter el qual apunta al començament del nostre float bigendian.
         * La funció s'encarrega de tornar el valor per a la correcta comparació.
         * Guardem aquest float en una variable que ens serveixi per a comparar
         */

        //I ara fem els condicionals
        if (acel_max_actual <= acel_actual_llista){
            estadisticas_acelerometros[datos_acelero[i]-1].aceleracion=acel_actual_llista; //canviem el valor de la llista
            estadisticas_acelerometros[datos_acelero[i]-1].tiempo=datos_acelero[i+1];// i a més posem el temps actual
            // això ens servirà a l'hora de mostrar quin és l'acceleració màxima i en quin instant de temps es produeix

        }
        else{
            continue; //no fem res, i torne'm a recòrrer el bucle per a mirar la següent dada
        }
    }

    //------- Mostrar els RESULTATS----------------------------------------
    /*
     * Acceleròmetre 1:
        t = 200
         acceleracio maxima = 9.81
        Acceleròmetre 2:
         0
         9.81
        Acceleròmetre 3:
        102
        4.905
        Acceleròmetre 4:
        163
        9.81
     */
    // Not Operation Directive
    _NOP(); //solo para poder poner un breakpoint y asi poder ver el resultado final en el debugger
}
