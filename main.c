#include <msp430.h>
#include <stdint.h>

/*
 * Professor: Daniel Café
 * Turma: D
 * Alunos: Guilherme Braga (17/0162290) e Gabriel Matheus (17/0103498)
 *
 */

/*
 * Ligações com o sensor TCS3200
 *
 * Sentido: MSP430 FR2355 -> Sensor
 *
 * P6.3 -> S3
 * P6.2 -> S2
 * P6.1 -> S1
 * P6.0 -> S0
 *
 * P1.6 <- OUT
 *
 * GND  -> GND
 * 3.3V -> VCC
 * GND  -> (OE)' = output enable  barrado
 *
 */

/**
 * S2 S3
 * 0  0  -> azul
 * 0  1  -> verde
 * 1  0  -> branco
 * 1  1  -> vermelho
 */

void main(){

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       // destrava os pinos digitais

    P1DIR |= BIT0;              // habilita saida no P1.0 (LED VERMELHO)
    P1REN &= ~(BIT0);           // habilita resistor de pull up
    P1OUT &= ~(BIT0);           // zera saida

    P6DIR |= BIT6;              // habilita saida no P1.0 (LED VERDE)
    P6REN &= ~(BIT6);           // habilita resistor de pull up
    P6OUT &= ~(BIT6);           // zera saida


    P1DIR &= ~(BIT6);           // entrada relacionada ao clock
    P1REN |= BIT6;              // habilitamos sua leitura
    P1OUT |= BIT6;              // inicializamos com zero
    P1SEL1 |= (BIT6);


    //S3
    P6DIR |= BIT3;              // P6.0 até P6.3 são entradas no sensor
    //P6OUT &= ~BIT3;

    //S2
    P6DIR |= BIT2;
    //P6OUT &= ~BIT2;           // S2 e S3 serão setados no while(1)

    //S1
    P6DIR |= BIT1;              // S0 = 0 e S1 = 1, setados apenas 1 vez, para freq de saída a 20% (observação do professor)
    P6OUT &= ~BIT1;

    //S0
    P6DIR |= BIT0;
    P6OUT |= BIT0;

    //----------------------------------------------------------
    // Variáveis auxiliares

    int vermelho = 0;
    int verde = 0;
    int azul = 0;

    int x_vermelho, y_vermelho;
    int x_verde, y_verde;
    int x_azul, y_azul;

    //----------------------------------------------------------
    // Começo do Clock: Modo Contínuo, ACLK
    TB0CTL = TBSSEL__ACLK | MC__CONTINUOUS;

    // CAP: Seleciona entre captura e comparação
    // CM: Configura o modo de captura,
    // SCS: define a sincronia do clock com o sinal de captura
    TB0CCTL1 = CAP | SCS | CM_1;

    while(1){

        // Medimos o tempo sempre entre 2 flancos de subida
        // assim vemos o sinal maior e ativamos o LED correspondente

        // Vermelho
        //----------------------------------------------------------

        P6OUT |= BIT2;
        P6OUT |= BIT3;

        while(!(TB0CCTL1 & CCIFG));     // aguardar o flanco
        TB0CCTL1 &= ~CCIFG;
        x_vermelho = TB0CCR1;

        while(!(TB0CCTL1 & CCIFG));     // aguardar o flanco
        TB0CCTL1 &= ~CCIFG;
        y_vermelho = TB0CCR1;


        vermelho = (y_vermelho - x_vermelho);

        // Verde
        //----------------------------------------------------------


        P6OUT |= BIT3; // s3
        P6OUT &= ~BIT2; // s2

        while(!(TB0CCTL1 & CCIFG));     // aguardar o flanco
        TB0CCTL1 &= ~CCIFG;
        x_verde = TB0CCR1;

        while(!(TB0CCTL1 & CCIFG));     // aguardar o flanco
        TB0CCTL1 &= ~CCIFG;
        y_verde = TB0CCR1;

        verde = (y_verde - x_verde);


        // Azul
        //----------------------------------------------------------

        P6OUT &= ~(BIT3); // s3
        P6OUT &= ~(BIT2); // s2

        while(!(TB0CCTL1 & CCIFG));     // aguardar o flanco
        TB0CCTL1 &= ~CCIFG;
        x_azul = TB0CCR1;

        while(!(TB0CCTL1 & CCIFG));     // aguardar o flanco
        TB0CCTL1 &= ~CCIFG;
        y_azul = TB0CCR1;


        azul = (y_azul - x_azul);


        //Lógica de setar os LEDs
        //----------------------------------------------------------

        if((verde > azul) && (verde > vermelho)){                       // verde dominante
            P6OUT |= BIT6;          // setamos a luz verde
            P1OUT &= ~(BIT0);       // apagamos a vermelha
        }

        if((vermelho > azul) && (vermelho > verde)){                    // vermelho dominante
            P1OUT |= BIT0;          // ligamos a luz vermelha
            P6OUT &= ~(BIT6);       // apagamos a luz verde
        }

        if((azul > vermelho) && (azul > verde )){                       // azul dominante
            P1OUT |= BIT0;          // ligamos a luz vermelha
            P6OUT |= BIT6;          // ligamos a luz verde

        }

    }


}
