//Archivo:	Main_lab_10.s
//dispositivo:	PIC16F887
//Autor:		Dylan Ixcayau
//Compilador:	XC8, MPLABX V5.45

//Programa:	PWM
//Hardware:	LEDs y terminal

//Creado:	5 de mayo, 2021
//Ultima modificacion: 9 de mayo, 2021
// ----------------- Laboratorio No.9 ----------------------------------------

// PIC16F887 Configuration Bit Settings
// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = ON      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//-------------------------Librerias--------------------------------------------
#include <xc.h>
#include <stdint.h>

//--------------------------directivas del compilador---------------------------

#define _XTAL_FREQ 8000000 //_delay_ms(x)

//--------------------------funciones-------------------------------------------
void USART_Tx(char data);   //Llamamos a la funcion de envio de datos
char USART_Rx();            //Llamamos a la funcion de recepcion de datos
void USART_Cadena(char *str); //Llamamos a la funcion de creacion de cadenas
void setup(void);
//---------------------------variables------------------------------------------

char valor;     //declaramos al variable valor como caracter


//---------------------funcion principal----------------------------------------
void main(void){
    setup();
    while (1) {
        USART_Cadena("\r Que accion desea ejecutar? \r");   //Cadenas
        USART_Cadena(" 1) Desplegar cadena de caracteres \r");
        USART_Cadena(" 2) Cambiar PORTA \r");
        USART_Cadena(" 3) Cambiar PORTB \r \r");
        
        while(PIR1bits.RCIF == 0);  //espero a que envien un dato
        
        valor = USART_Rx();   //Paso el valor de que recibe el pic a la variable
          
        switch(valor){      //Men?? desplegado en las cadenas
            case ('1'):     //Si se ingresa el 1
                USART_Cadena(" Holiwis :D \r"); //Despliega el char
                break;      //Nos saca y repite el menu
                        
            case ('2'):     //Si se ingresa el 2
                USART_Cadena(" Ingresa un caracter para el puerto A: ");
                while(PIR1bits.RCIF == 0);//Si la bandera esta apagada
                PORTA = USART_Rx();  //lo paso al puerto A
                break;
                        
            case ('3'):     //Si se ingresa el 3
                USART_Cadena(" Ingresa un caracter para el puerto B: ");
                while(PIR1bits.RCIF == 0);
                PORTB = USART_Rx();  //lo paso al puerto A
                break;
        }
    }
    
    return;
}


    void USART_Tx(char data){   //Rutina para mandar datos en el tx
        while(TXSTAbits.TRMT == 0); //Si el TRMT esta lleno
        TXREG = data;   //se pasa de data a TXREG
    }

    char USART_Rx(){        //Rutina para recibir los datos
        return RCREG;       //Se pasa a RCREG
    }

    void USART_Cadena(char *str){       //Rutina para hacer las cadenas
        while(*str != '\0'){ //no sea igual a nulo
            USART_Tx(*str);  //Mando el dato de un lugar especifico
            str++;          
    }
}
    
void setup(void){
    ANSEL = 0X00;   //Solo pines digitales
    ANSELH = 0x00;  //Solo pines digitales

    TRISA = 0x00; // Salida todo
    TRISB = 0x00; // 
    
    PORTA = 0x00;   //Limmpiamos el puerto A
    PORTB = 0x00;
    
    OSCCONbits.IRCF = 0b111 ;  // config. de oscilador interno
    OSCCONbits.SCS = 1;         //reloj interno

                            //Confi. serial comunication
    TXSTAbits.SYNC = 0;     //asincrono
    TXSTAbits.BRGH = 1;     //high speed
    BAUDCTLbits.BRG16 = 0;  //uso los 16 bits
   
    SPBRG = 51;   //revisar tabla BAUD RATES FOR ASYNCHRONOUS MODES (CONTINUED)                      
    SPBRGH = 0;    //pagina 168 del datasheet del 2009         
    
    RCSTAbits.SPEN = 1;     //enciendo el modulo
    RCSTAbits.RX9 = 0;      //No trabajo a 9 bits
    RCSTAbits.CREN = 1;     //activo recepci??n
    TXSTAbits.TXEN = 1;     //activo transmision 
}
