/******************************************
* Pràctiques de C
* UB
* Joc de la vida
* C. Serre, setembre 2020
*******************************************/

#include <msp430.h> 
#include <stdlib.h>
#include <stdint.h>

#define MAX_TURNOS 15
#define MAX_X 8
#define MAX_Y 8

//Mascaras: el bit 0 representa el estado actual, el bit 4 el proximo estado.
#define MUNDO_VIVA 0x01
#define COPIA_VIVA 0x10
#define MUNDO_MUERTA 0xFE
#define COPIA_MUERTA 0xEF
//Funciones proporcionadas por el profesor:
//-----------------------------------------
void init_relojes();
uint8_t vecinos(uint8_t *el_mundo, uint8_t dim_x, uint8_t dim_y, uint8_t x, uint8_t y);
//-------------------------


//Funciones a programar por el alumno:
//------------------------------------------------------------------------------------------------

uint8_t *crear_mundo(uint8_t dim_x, uint8_t dim_y){
    //Reservar un bloque de memoria para almacenar el mundo, mediante la función malloc(),
    uint8_t *taulell; // declarem el punter anomenat taulell, i les variables d'aquest punter són de 8 bits s/ signe
    taulell = malloc(dim_x*dim_y); //amb la funció malloc, reservem tot l'espai que necessitem del taulell, i ens retorna el punter de la primera adreça del taulell que s'ha creat
    return taulell; //la funció ens retornarà un punter apuntant a la primera adreça del malloc
}

//Funcion para poblar el  mundo con celulas aleatoriamente vivas o muertas:
void llenar_mundo(uint8_t *taulell, uint8_t dim_x, uint8_t dim_y){
    //rellenar todas las celdas del tablero (mundo) con 0 o 1 aleatoriamente.
    //Puede resultar util el uso de
    //un_valor = rand()%n; //genera aleatoriamente numeros enteros con valor entre 0 y n-1 .
    char dim_taulell;
    unsigned int i;
    dim_taulell=dim_x*dim_y;//posem en una variable la dimensió del taulell, per a poder fer el bucle for
    for(i=0; i<dim_taulell; i++){ //creem el bucle for que s'executi 64 vegades
        taulell[i]=rand()%2; //a cada valor de taulell o se li posa un 1 o un 0
        taulell[i] &= MUNDO_VIVA; // en assegurem de que tots els altres valors siguin 0 i només sobrevisqui l'últim bit (tant si es 0 com 1)
        // i afegim el resultat a taulell [i]
    }
}

//------------------------------------------------------------------------------------------------

void avanzar_turno(uint8_t *un_mundo, uint8_t dim_x, uint8_t dim_y){
    //Recorrer el mundo con 2 bucles
    //Para cada celda, usar la funcion vecinos() proporcionada, para determinar cuantos vecinos tiene.

    //------ DECLARO VARIABLES ---------

    uint8_t i, j, fila_y, columna_x, recompte_veins; // declarem variables per bucles for i per determinar en la casella que ens trobem
    unsigned int casella; // declaro aquesta variable per saber exactament en quina casella ens trobem dins del bucle
    for(i=0;i<dim_y;i++){
        fila_y=i;//recorrem totes les files, de la 0 a la 7
        for(j=0;j<dim_x;j++){
            columna_x=j;//recorrem totes les columnes, de la 0 a la 7, un cop recorregudes, sortirem del bucle i baixarem de fila (per aixo dos bucles)
            casella=(fila_y*dim_x)+columna_x; //calculem en quina casella ens trobem
            recompte_veins=vecinos(un_mundo, dim_x, dim_y, columna_x, fila_y); // funcio que ens compta els veins que tenim

            // --------- CONDICIONALS------------

            //caso 2 vecinos: //sin cambio
            //Copiamos el bit 0 al bit 4, sin cambiar el bit 0
            if (recompte_veins == 2){ // cas de que tenim justament 2 veins
                if(un_mundo[casella] & 0x01){ // cas de que la casella que ens trobem estigui viva
                    un_mundo[casella] = un_mundo[casella]<<4; //posem un 1 al bit 4
                    un_mundo[casella] = un_mundo[casella] | 0x01; //i com no ha de variar la posicio 0, amb una OR li posem aquest 1 al final
                }
                else{
                    un_mundo[casella]=un_mundo[casella]<<4; // en el cas de que hi hagi un 0 al final, simplement movem aquest
                    // 0 a la posició 4 i no toquem res més perque tots els numeros que s'afegeixen a la dreta ja son 0
                }
            }
            //caso 3 vecinos: Nace o se mantiene viva
            //hay que poner a 1 el bit 4, sin cambiar el bit 0;
            else if (recompte_veins==3){
                if (un_mundo[casella] & 0x01){
                  un_mundo[casella] <<= 4; // ho fem de manera curta, desplacem el bit 0, que es un 1 a la posicio 4
                  un_mundo[casella] |= 0x01; // i afegim l'1 que hi havia al bit 0
                }
                else{//és a dir, no tenim un 1 a l'ultima posicio
                    un_mundo[casella] |= 0x10; // afegeixo un 1 a la posicio 4
                            //(independentment del valor que hi hagi, al bit 4 hi apareixerà un 1 gracies a l'operador | )
                            // i el bit 0 NO el toco
                }
            }
            //Cualquier otro caso: //Muere por soledad o por sobrepoblacion
                            //hay que poner a 0 el bit 4, sin cambiar el bit 0;
            else{// ni 2 ni 3 veins, es a dir, o sobrepoblació o soledat
                if(un_mundo[casella] & 0x10){ // mirem si el bit 4 és un 1
                    un_mundo[casella] &= ~0x10; // bàsicament fem el 0x10 negat, d'aquesta manera fem que el bit 4 sigui 0
                    //i tots els bits altres no variin de valor
                }
                else{//si el bit 4 no és un 1 ha de ser un 0, per tant, no fem res
                    continue;
                }
            }
        }
    }
}
//------------------------------------------------------------------------------------------------
//Funcion para comprobar la estabilidad/inestabilidad del tablero entre cada turno

uint8_t estabilidad(uint8_t *un_mundo, uint8_t dim_x, uint8_t dim_y){
    //Devolver 0 si es inestable.
    //Devolver 1 si es estable.
    //Hay que recorrer todas las celdas del tablero, comparando para cada una su bit 0 con su bit 4.
    uint8_t pos_x, pos_y, casella, bit_0, bit_4; //declaro variables
    for(pos_y=0; pos_y<dim_y; pos_y++){//valor de la posicio y, no fico la i perque aixi m'estalvio variables
        for(pos_x=0;pos_x<dim_x;pos_x++){
            casella=pos_y*dim_x+pos_x; //calculo la casella exacta per moure'ns pel punter facilment
            bit_0=un_mundo[casella] & 0x01; //copio NOMES l'últim bit, tots els altres son un 0
            bit_4=(un_mundo[casella] &0x10) >>4; //copio NOMES el quart bit, i el poso a l'últim bit, ja que si comparo
            // 0x01 i 0x10 sempre donarà que NO es el mateix ja que comparo un 1 amb un 16 degut al bit posicio en el que es troba
            if(bit_0==bit_4)
                continue;
            else{
                return 0; // si son diferents retornem un 0
            }
        }
    }
    return 1;//si no hi ha hagut cap valor diferent, vol dir que estem en un estat estable i per tant, retornem un 1
}

//------------------------------------------------------------------------------------------------
//funcion para actualizar el estado de poblacion de las celdas entre cada turno

void actualizar_mundo(uint8_t *un_mundo, uint8_t dim_x, uint8_t dim_y){
    //Mover el bit 4 (nuevo estado) al bit 0 (estado actual).
    //El bit 4 (proximo estado)  debe quedar reinicializado a 0
    uint8_t pos_x, pos_y, casella; //declaro variables
    for(pos_y=0; pos_y<dim_y; pos_y++){//valor de la posicio y, no fico la i perque aixi m'estalvio variables
        for(pos_x=0;pos_x<dim_x;pos_x++){
            casella=pos_y*dim_x+pos_x; //calculo la casella exacta per moure'ns pel punter facilment
            un_mundo[casella]>>= 4; // desplaçem el bit 4 al bit 0, i el bit 4 queda a 0
        }
    }
}

//------------------------------------------------------------------------------------------------

void main(void)
{

    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    //Inicializaciones
    volatile unsigned char torn_actual = 0; //declaro el torn actual com a volàtil, perquè variarà durant el programa
    unsigned char dim_x=8;
    unsigned char dim_y=8;
    // tant la dimensió x com la y les poso a 8, ja que el taulell és de 64 caselles
    //una millora podria ser que la dimensió del taulell, la escrigui l'usuari, cosa que no seria gaire mal de cap tenint en compte que hem fet el programa a base de funcions
    uint8_t *taulell_creat; // creem el punter del taulell, que aquest és creat per la funcio crear_mundo, que serà la primera que cridarem
    uint8_t estable; // per quan cridem la funció estabilitat
//--------------------------------------------------------------------------------------------------
    /*
    Desarrollo del juego: A programar por el alumno...
    se ha de ir incrementando el turno, comprobando la estabilidad/inestabilidad entre cada turno,
    y si sigue inestable, actualizando el mundo para el siguiente turno.
    El juego termina al alcanzar el numero maximo de turnos o si se alcanza un estado estable
    (un estado en el que todas las celdas se quedan en un estado igual al estado del turno anterior)
    */
    taulell_creat = crear_mundo(dim_x, dim_y); // cridem la funció crear_mundo i li passem les mides del taulell
    //aquesta funció s'encarrega, amb l'ajuda de malloc, de reservar l'espai de memòria que utilitzarem pel joc de la vida

    //un cop creat el taulell, hem d'omplir-lo, per tant, utilitzaré la funcio llenar_mundo
    //emplenar el món bàsicament és omplir les adreçes de memòria que ens ha creat el malloc
    llenar_mundo(taulell_creat, dim_x, dim_y);
    /*
     * Com és una funció que no retorna res, no és necessari assignar-li cap variable.
     * Simplement haurem afegit un 1 o un 0 al bit 0 de cada casella
     */

    //Un cop ja tenim el taulell omplert, ara és l'hora de començar a jugar
    /*
     * Per fer-ho faré un bucle while que executi les funcions següents sempre que torn_actual<15:
     * ESTABILITAT
     * Si no és estable, AVANÇAR TORN | Si és estable, sortir del bucle
     * ACTUALITZAR TORN
     *torn_actual++
     */
    while (torn_actual < 15){
        estable = estabilidad(taulell_creat, dim_x, dim_y);
        if (estable == 1){
            break;
        }
        else{
            avanzar_turno(taulell_creat, dim_x, dim_y); // avançem el torn, és a dir comptem els veïns i ho posem en el bit 4
            // depenent de quants veïns tingui la casella, morirà (un 0) es quedarà igual (o 0 o 1), o naixerà (un 1)
            actualizar_mundo(taulell_creat, dim_x, dim_y); // agafem el bit 4 i el posem al bit 0
            torn_actual++;
        }
    }


    // Una vez finalizado el juego:
        _NOP();//solo para poder poner un breakpoint y ver el resultado final en el debugger

    //Liberar la memoria ocupada por el tablero, con la función free();
        free(taulell_creat);

}//fin del main()


//-------------------------------------------
//Definicion de las funciones proporcionadas:
//-------------------------------------------

void init_relojes(){
    //Relojes:  ACLK = VLO = 11kHz, MCLK = SMCLK = 1MHz
      BCSCTL1 = CALBC1_1MHZ;    // DCO a 1MHz
      DCOCTL =  CALDCO_1MHZ;    // MCLK y SMCLK a 1MHz
      BCSCTL3 |= LFXT1S_2;      // ACLK = VLO ~ 11kHz (no tenemos cuarzo)
}

//Funcion para determinar cuantos vecinos tiene una celda en las coordenadas (x,y):
//Parametros: un puntero al mundo, las dimensiones del mundo, y las coordenadas de la celda a evaluar.
//Salida: el numero de vecinos vivos encontrados.
uint8_t vecinos(uint8_t *el_mundo, uint8_t dim_x, uint8_t dim_y, uint8_t x, uint8_t y){
    uint8_t i,j; //para el control de 2 bucles (filas x columnas).
    volatile int8_t xv, yv;  //offset de las coordenadas de los vecinos. Pueden resultar negativas en los bordes
                    //en cual caso se considerara que no hay vecino.
    uint8_t num_vecinos = 0; //Esta variable es para ir sumando los vecinos encontrados. Por eso es importante inicializarla a 0

//Cada celda se encuentra en el centro de un bloque de 3x3.
//Tendrá vecinos repartidos en 3 filas x 3 columnas.
//Ahora, vamos a recorrer todas las celdas vecinas en 2 bucles:
    for(i = 0; i < 3; i++){ //Recorriendo filas
        yv = y -1 + i; //Coordenadas "y" de las celdas vecinas
        if ((yv < 0) || (yv >= dim_y)) //Si la celda vecina se encuentra fuera del tablero,
            continue; //excluyendo bordes, pasamos directamente al siguiente valor de "i" (siguiente fila).
        for(j = 0; j < 3; j++){ //Recorriendo columnas dentro de cada fila
            xv = x - 1 + j; //Coordenadas "x" de las celdas vecinas
            if ((xv < 0) || (xv >= dim_x) || ((xv == x) && (yv == y)))
                continue;//no cuentan como vecinos los bordes, ni la misma celda que se esta comprobando. Pasamos directamente al siguiente valor de "j" (siguiente columna).
            else {
                if(el_mundo[yv*dim_x + xv] & MUNDO_VIVA) //Si hay una celula viva en esta celda,
                    num_vecinos++; //la sumamos al numero de vecinos.
            }
        }
    }
    return num_vecinos;
}
