#include <msp430.h> 
#include <stdint.h>

// Alunos: Guilherme Braga (17/0162290) e Gabriel Matheus (17/0103498)

/*
 * Ligações com o sensor TCS3200
 *
 * Placa da Texas -> Sensor
 *
 * P1.3 -> S3
 * P6.2 -> S2
 * P6.1 -> S1
 * P6.0 -> S0
 *
 * P1.2 <- OUT
 *
 * GND  -> GND
 * 3.3V -> VLC ou VCC?
 * GND  -> (OE)' output enable
 *
 */

/**
 * S2 S3
 * 0  0  -> vermelho
 * 0  1  -> azul
 * 1  0  -> sem filtro
 * 1  1  -> verde
 */


void main(){

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= BIT0;              // habilita saida no P1.0 (LED VERMELHO)
    P1REN &= ~(BIT0);           // habilita resistor de pull up
    P1OUT &= ~(BIT0);           // zera saida

    P6DIR |= BIT6;              // habilita saida no P1.0 (LED VERDE)
    P6REN &= ~(BIT6);           // habilita resistor de pull up
    P6OUT &= ~(BIT6);           // zera saida


    P1DIR &= ~(BIT2);           // P1.2 é entrada que recebe o out do sensor
    P1REN &= ~(BIT2);           // resistor de pull down para ser uma entrada
    P5OUT |= BIT2;              // https://siddharthnandhanp.wordpress.com/2015/06/07/pxren-register-pull-updown-resistors/

    //S3
    P6DIR |= BIT3;              // P6.0 até P6.3 são entradas no sensor
    //P6OUT &= ~BIT3;             // as configuraremos

    //S2
    P6DIR |= BIT2;
    //P6OUT &= ~BIT2;             // S2 e S3 serão setados no while(1)

    //S1
    P6DIR |= BIT1;              // S0 = 0 e S1 = 1, para freq de saída a 20% (observação do professor)
    P6OUT &= ~BIT1;

    //S0
    P6DIR |= BIT0;
    P6OUT |= BIT0;

    int tempo_vermelho = 0;
    int tempo_verde = 0;
    int tempo_azul = 0;

    while(1){

        /*  lógica de detectar as entradas?

        TBSSEL__SMCLK -> Seleciona o SMCLK como fonte do timer b
        MC__UP
        TBCLR -> Limpa o clock
        T = 1ms
        */

        // vermelho
        // faz S2 e S3 virarem 0 e 0 para capturarmos o vermelho
        P6OUT &= ~(BIT3);
        P6OUT &= ~(BIT2);

        while(!(P1IN & BIT2));     //esperar receber uma leitura para seguir a diante

        // começa a contar

        TB0R = 0x0000; // clear do output do timer
        TB0CCR0 = 0x8000 - 1;
        TB0CTL = (TBSSEL__SMCLK | MC__UP | TBCLR); // começa a contar

        // trava programa por estar recebendo um sinal
        while(P1IN & BIT2);

        tempo_vermelho = TB0R;

        // verde
        // faz S2 e S3 virarem 1 e 1 para capturarmos o verde
        P6OUT |= BIT3;
        P6OUT |= BIT2;

        while(!(P1IN & BIT2));   //esperar receber uma leitura para seguir a diante

        TB0R = 0x0000;  // clear do output do timer
        TB0CCR0 = 0x8000 - 1;
        TB0CTL = (TBSSEL__SMCLK | MC__UP | TBCLR); // começa a contar

        // trava programa por estar recebendo um sinal
        while(P1IN & BIT2);
        tempo_verde = TB0R;


        // azul
        // faz S2 e S3 virarem 0 e 1 para capturarmos o verde
        P6OUT |= BIT3;
        P6OUT &= ~(BIT2);

        while(!(P1IN & BIT2));   //esperar receber uma leitura para seguir a diante

        TB0R = 0x0000;   // clear do output do timer
        TB0CCR0 = 0x8000 - 1;
        TB0CTL = (TBSSEL__SMCLK | MC__UP | TBCLR); // começa a contar

        // trava programa por estar recebendo um sinal
        while(P1IN & BIT2);
        tempo_azul = TB0R;


        //  lógica de setar os LEDs
        if((tempo_verde > tempo_azul) && (tempo_verde > tempo_vermelho)){                       // verde dominante
            P6OUT |= BIT6;          // setamos a luz verde
            P1OUT &= ~(BIT0);       // apagamos a vermelha
        }
        if((tempo_vermelho > tempo_azul) && (tempo_vermelho > tempo_verde)){      // vermelho dominante
            P1OUT |= BIT0;          // ligamos a luz vermelha
            P6OUT &= ~(BIT6);       // apagamos a luz verde
        }
        else{                       // azul dominante
            P1OUT |= BIT0;          // ligamos a luz vermelha
            P6OUT |= BIT6;          // ligamos a luz verde
        }

    }

}
