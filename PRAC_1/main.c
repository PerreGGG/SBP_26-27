/*
 * File:   elements_main_alumnes.c
 *      Author: UB, 2011-2024
 */

#include <msp430g2553.h> //Utilidad de esta libreria? Vindria a ser el diccionari del nostre micro per comuninacr-nos amb ell

#include <stdio.h> //Utilidad de esta libreria? Es la llibreria basica per les funcions d'entrada i sortida del codi C

//Variables globales:
volatile  unsigned char tabla[12];
volatile unsigned char cadena[]="0123456789";
const unsigned char texto[] = "\nTamanyo de tipos de variables, en BYTES: \n";
char texto2[42];
unsigned char dummy = 0;

//Prototipo de las funciones
int sumar(int numero);
void Inicializar_tabla();

//Programa principal
void main(void) //Inicio del programa
{
    // Estas variables son globales o locales? Son locals, perque estan dins una funcio, en aquest cas, la main
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
        /*
        * Que texto2, hem dit especificament que ocupa 32 posicions de memoria, i n'ocupa mes
        * El que he fet es substituir texto2[32] per texto2[42], on hem declarat
        */

        //Analizar el tamanyo de los diferentes tipos de variables, en BYTES:
        tabla[0]=sizeof(char);// 1 byte
        tabla[1]=sizeof(short);//2 bytes
        tabla[2]=sizeof(int);//2 bytes
        tabla[3]=sizeof(long int);//4 bytes
        tabla[4]=sizeof(float);//4 bytes
        tabla[5]=sizeof(double);//8 bytes, un double es un float pero amb mes decimals per poder ser mes exactes
        tabla[6]=sizeof(long double);// 8 bytes
        tabla[7]=sizeof(p_char); //puntero,  2 bytes
        tabla[8]=sizeof(p_int); //puntero 2 bytes. En els dos casos passa el mateix,
        // estem mirant la mida del punter en si (on hi ha l'adreça), si volguessim mirar el que hi ha dins
        //del punter s'ha de fer sizeof(*p_char)
        tabla[9]=sizeof(cadena);//ocupa 11 bytes perquè l'ultim es un zero indicant que s'acaba la variable
        tabla[10]=sizeof(tabla); // tabla = c, que c en decimal son 12 bytes, és a dir, el tamany que hem declarat
        tabla[11]=sizeof(tabla[5]); //jo diria que com marquem que volem la mida del que
        //hi ha a la pos 5 de la taula, que es un double, ha de ser 8
        //Comentar los resultados, y observar como difieren estos tamanos de los tamanos estandares.

        v = cadena[0]; // crec que sera v=0
        w = cadena[9]; // crec que sera w=9
        z = texto[1]; // crec que sera z=a
        //Valor de v, w, z? Porque?
/*
 * v=48 perquè el 0 en la taula del ascii es el 48
 * w quan he compilat no ha funcionat perque la cadena va del [0] al [9], ho he canviat i ara imprimeix el 9
 * z=T, que es el segon caràcter de la cadena text, ja que el primer es \n, i T en ascii es 84
 */
        //Operadores aritmeticos:

        a = 2; b = 3; c = 0; x = 0; y = 0;
        c=++b; // c=b=3 b=b+1=4
        x=b++; //x=b+1=4
        ++y;//y=y+1=1?
        a++;//a=a+1=3
        //  Cual es el resultado de a,b,c,x,y?; explicar.
/*
 * c=4 i b=4 perque c=++b vol dir que primer b=b+1=4 i despres c=b=4
 * b=5 i x=4,perque primer x=b=4 i despres b=b+1=5
 * segons la IA, ++var es preincrement, primer s'incrementa i despres s'iguala
 * segons la IA, var++ es proincrement, primer s'iguala i despres d'incrementa
 * ++y i a++ volen dir exactament el mateix
 */
        y+=x;
        //  Cual es el resultado de a,b,c,x,y?; explicar.
// y=y+x, x=4 i y=1 per tant, la y passa a valer 5
        y-=x;
        //  Cual es el resultado de a,b,c,x,y?; explicar.
//y=y-x 5-4=1
        y*=x;
        //  Cual es el resultado de a,b,c,x,y?; explicar.
//y=y*x=1*4=4
        y=+x; //y=(+x) per tant y=4
        //  Cual es el resultado de a,b,c,x,y?; explicar.

        y=-x;// y=(-x), per tant y=-4
        //  Cual es el resultado de a,b,c,x,y?; explicar.
// com s'ha declarat que no te signe, el -4 es 4 en binari (pero te zeros al davant) els
//zeros passen a 1 i l'1 a 0 i es fa el ca2, però a l'hora de llegir, com no té signe,
//el primer 1 no es de signe i per aixo llegiex aquest numero tant gran
        y+=--x; //x=x-1=3 i despres y=y+x
        //  Cual es el resultado de a,b,c,x,y?; explicar.
//y=65535 i no 65536, ja que 65535 es 2^16-1, que son exactament 2bytes,
//el que ocupa un int, per tant aquest numero no pot augmentar mes
// degut a que ha arribat al límit de capacitat
        y+=x--; //y=y+x=65535+3, 65535+1=0+1=1+1=2 i despres x=x-1=2
        //  Cual es el resultado de a,b,c,x,y?; explicar.
/*
*si s'arriba al numero de 2^16-1 que es el numero maxim que pot ocupar un numero de 16 bits i es supera
*els 2 bytes pateixen overflow i tornen a 0 i a partir d'aqui segueixen amb la suma
*/
        y*=x;
        //  Observar y explicar porque el resultado de y=*x; es:
        /*
         * Teoricament el que estas assignan aquí es que la variable y tingui el valor del punter x (degut al símbol *).
         * Però x NO es un punter, llavors dona error
         */
        // Si volem y*x, hem de posar-lo abans

        y/=x;
        //  Observar y explicar porque el resultado de y=/x; es:
        /*
         * L'error ve donat com abans, perque / ha d'anar abans de l'igual si el que volem es dividir
         */
        //  El resultat de y/x= y=y/x=4/2=2
        x=2;
        y=2; //poso les variables a 0 per fer les operacions en notacio normal
        y = y + x; // =4
        y = y - x; // =2
        y =y* x; // =4
        y = x; // =2
        y = -x; // -2 (pero s'ha de tenir en compte que y no te signe, no sera correcte el resultat)
        //y += --x;
        y=1;
        y = y + (x-1); //x-1=1, y=y+1=2
        //y += x--;
        y = y + x; //=4
        x = x+1; // en aquest cas, com es un post increment de x, ho he separat en dues línies
        y = y*x;
        y=16;
        x=2;
        y =y/x;

        y=65535;
        w=y; // abans de fer el step over, w es un char, i un unsigned char va del 0 al 256, per tant probablement
        //el resultat serà una cosa rara
        z=w;
        //Observar y explicar los valores de z y de w
/*
 * y dona el mateix valor, es un int i donem el valor maxim que entra en 2 bytes
 * w es un char que nomes ocupa 1byte i un int ocupa 2bytes, per tant es trunca al 255 (2^8=1byte)
 * z es un signed char per tant llegira el primer 1 com un signe negatiu, i entenc que equival al -1
 */
        for(i=0,suma=0;i<10;suma+=i++);

        i--; //Cual es valor de i? 9 Porque? Perque aquesta funcio el fa decrementar 1
        suma1=sumar(i);
        //Cual es valor de i? 9 Porque? perque la funcio retorna el numero a la variable suma1
        //mirar la funcio
        suma2=67; //faig aixo perque si el numero no canvia, a la pestanya de variables no em surt en groc
        //aixi puc detectar si el problema es de l'operacio o problema de com esta escrit
        suma2 =(i*i++)>>1; //Cual es valor de i? 10 Porque? Perque multiplica i despres incrementa i=1
        i=9;
        suma3=67;
        suma3 =(i*++i)>>1; //Cual es valor de i? 10 Porque? Perque primer s'incrementa i es guarda el valor a i,
        //despres es multiplica per si mateix i es divideix entre 2, pero aixo ja entra a la variable de suma3
        //Comentar las operaciones que hacemos sobre las variables suma1, suma2, y suma3? Estan comentades
        //respectivament en la seva linia
        //Deberian dar el mismo resultado? No Porque? Perque el lexic del llenguatge C juga un paper important.
        //Depenent de què i com s'escrigui a la linia, el llenguatge fara unes funcions en un ordre diferent

        for (i = 0, x = 1, z = 2, tope = 10; i <= tope; x *= z, i++ );
        //Cuales son los valores finales de i, x, z, tope? Explicar.
/*
 * i=11 perque el bucle s'executa fins que i=10 i s'executa llavors per tant, i=11
 * z=2
 * x=2048 que es basicament 2^11. Ja que l'operacio es x=z*x,i com z=2 doncs x es va elevant al quadrat
 */
        //Operadores de bits:
        z=32; //32 equival a l'espai buit a la taula ASCII
        z>>1; //Cual es el valor de z?32 Porque? Perque no hi ha =, per tant no es guarda a memoria
        z>>=3;//Y ahora? Porque? 4 A que otra operacion es equivalente? A dividir entre 2 tres cops
        z<<=4;//Y ahora? Porque? 64 A que otra operacion es equivalente? A multiplicar per 2 quatre cops
        z|= 0x07; //Resultado? 47 (en hexadecimal) Explicar Perque basicament es una porta OR bit a bit.
        //Si hi ha un 1 en qualsevol posicio de qualsevol de z o 07 a la sortida tindrem un 1
        z&= 0x43; //Que operador es? 43 Resultado? Explicar. Porta AND bit a bit, si les dues var tenen un 1,
        //la sortida es 1, sino no
        z^= 0x01; //Que operador es? XOR Resultado? Explicar. 42 en hexadecimal. Si las variables tenen numeros diferents la sortida es 1,
        //si son iguals la sortida es 0
        z^= 0x01; //Que operador es?XOR Resultado? Explicar. 43. L'explicacio es la mateixa d'abans

        // Not Operation Directive
        _NOP();
    } //Fin del bucle infinito
} //Fin del programa


// Declaracion del cuerpo de las funciones:

int sumar(int numero) {//Implementation de la funcion sumar()
    numero*=numero;
    numero++;
    return numero>>1;
    //Que pretendemos hacer con esta funcion? Fer aquesta operacio num=num*num i despres incrementar la variable num
    // S'ha de dir que en aquest cas el nostre numero es la i
    //Que variables vemos en la ventana Variables? veiem suma1 i i pero no varien cap de les dues
    //Porque? Veiem numero i no i perque
    // com jo veig les funcions, quan les definim, la variable que posem d'entrada, esta generalitzada
    // i a partir d'aqui implementem la funcio i nomes quan la cridem, es quan li posem la variable real amb la
    //qual volem que la funcio actui amb ella
}

void Inicializar_tabla() {//Implementation de la funcion Inicializar_tabla()
    unsigned char i; // declarem variables local i
    for(i=0;i<sizeof(tabla);i++){
        tabla[i]=0;
    }
    //Explicar lo que hace esta funcion.
    /*
     * Es un bucle for que es repeteix tantes vegades com el que ocupa la taula.
     * Simplement el que fa es fer que tots els valors de la taula siguin 0.
     */
}

//Preguntas adicionales al alumno:
//Desarrollar cada operacion del programa de notacion "compacta" a notacion "normal", y comentar. FET A DALT
//Como seria la operacion de division con notacion compacta?
// var1 /= var2
//A que otra operacion es equivalente el desplazamiento de bits a la derecha a = a>>1?
// A multiplicar per 2, a=a*2
//Y la del desplazamiento al otro lado a = a<<n?
// A dividir per 2^n
