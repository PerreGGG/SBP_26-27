/*
 * File:   elements_main_alumnes.c
 *      Author: UB, 2011-2024
 */

#include <msp430g2553.h> //Utilidad de esta libreria?
#include <stdio.h> //Utilidad de esta libreria?

//Variables globales:
volatile  unsigned char tabla[12];
volatile unsigned char cadena[]="0123456789";
const unsigned char texto[] = "\nTamanyo de tipos de variables, en BYTES: \n";
char texto2[32];
unsigned char dummy = 0;

//Prototipo de las funciones
int sumar(int numero);
void Inicializar_tabla();

//Programa principal
void main(void) //Inicio del programa
{
    // Estas variables son globales o locales?
    volatile unsigned char *p_char;
    volatile unsigned int *p_int;
    volatile unsigned int i, suma, suma1, suma2, suma3, tope;
    volatile unsigned int a, b, c, x, y,v;
    volatile unsigned char w;
    volatile signed char z = 127;

    // Stop Watch Dog Timer:
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT

    while(1){ //Entramos a un bucle infinito
        Inicializar_tabla();

        dummy = sprintf(texto2, "Tamanyo de tipos de variables, en BYTES");//Que problema ocurre con esta linea?

        //Analizar el tamanyo de los diferentes tipos de variables, en BYTES:
        tabla[0]=sizeof(char);
        tabla[1]=sizeof(short);
        tabla[2]=sizeof(int);
        tabla[3]=sizeof(long int);
        tabla[4]=sizeof(float);
        tabla[5]=sizeof(double);
        tabla[6]=sizeof(long double);
        tabla[7]=sizeof(p_char); //puntero
        tabla[8]=sizeof(p_int); //puntero
        tabla[9]=sizeof(cadena);
        tabla[10]=sizeof(tabla);
        tabla[11]=sizeof(tabla[5]);
        //Comentar los resultados, y observar como difieren estos tamanos de los tamanos estandares.

        v = cadena[0];
        w = cadena[10];
        z = texto[1];
        //Valor de v, w, z? Porque?

        //Operadores aritmeticos:

        a = 2; b = 3; c = 0; x = 0; y = 0;
        c=++b;
        x=b++;
        ++y;
        a++;
        //  Cual es el resultado de a,b,c,x,y?; explicar.

        y+=x;
        //  Cual es el resultado de a,b,c,x,y?; explicar.

        y-=x;
        //  Cual es el resultado de a,b,c,x,y?; explicar.

        y*=x;
        //  Cual es el resultado de a,b,c,x,y?; explicar.

        y=+x;
        //  Cual es el resultado de a,b,c,x,y?; explicar.

        y=-x;
        //  Cual es el resultado de a,b,c,x,y?; explicar.

        y+=--x;
        //  Cual es el resultado de a,b,c,x,y?; explicar.

        y+=x--;
        //  Cual es el resultado de a,b,c,x,y?; explicar.

        y=*x;
        //  Observar y explicar porque el resultado de y=*x; es:...
        //   ..."error..."

        y=/x;
        //  Observar y explicar porque el resultado de y=/x; es:...
        //  ... "error..."

        y=65535;
        w=y;
        z=w;
        //Observar y explicar los valores de z y de w

        for(i=0,suma=0;i<10;suma+=i++);

        i--; //Cual es valor de i? Porque?
        suma1=sumar(i);
        //Cual es valor de i? Porque?
        suma2 =(i*i++)>>1; //Cual es valor de i? Porque?
        i=9;
        suma3 =(i*++i)>>1; //Cual es valor de i? Porque?
        //Comentar las operaciones que hacemos sobre las variables suma1, suma2, y suma3?
        //Deberian dar el mismo resultado? Porque

        for (i = 0, x = 1, z = 2, tope = 10; i <= tope; x *= z, i++ );
        //Cuales son los valores finales de i, x, z, tope? Explicar.

        //Operadores de bits:
        z=32;
        z>>1; //Cual es el valor de z? Porque?
        z>>=3;//Y ahora? Porque? A que otra operacion es equivalente?
        z<<=4;//Y ahora? Porque? A que otra operacion es equivalente?
        z|= 0x07; //Que operador es? Resultado? Explicar.
        z|= 0x07; //Resultado? Explicar.
        z&= 0x43; //Que operador es? Resultado? Explicar.
        z^= 0x01; //Que operador es? Resultado? Explicar.
        z^= 0x01; //Que operador es? Resultado? Explicar.

        // Not Operation Directive
        _NOP();
    } //Fin del bucle infinito
} //Fin del programa


// Declaracion del cuerpo de las funciones:

int sumar(int numero) {//Implementation de la funcion sumar()
    return (numero*numero++)>>1;
    //Que pretendemos hacer con esta funcion?
    //Que variables vemos en la ventana Variables? Porque?
}

void Inicializar_tabla() {//Implementation de la funcion Inicializar_tabla()
    unsigned char i;
    for(i=0;i<sizeof(tabla);i++){
        tabla[i]=0;
    }
    //Explicar lo que hace esta funcion.
}

//Preguntas adicionales al alumno:
//Desarrollar cada operacion del programa de notacion "compacta" a notacion "normal", y comentar.
//Como seria la operacion de division con notacion compacta?
//A que otra operacion es equivalente el desplazamiento de bits a la derecha a = a>>1?
//Y la del desplazamiento al otro lado a = a<<n?
