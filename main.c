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


int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;


    P1DIR |= BIT0;              // habilita saida no P1.0 (LED VERMELHO)
    P1REN &= ~(BIT0);           // habilita resistor de pull up
    P1OUT &= ~(BIT0);           // zera saida

    P6DIR |= BIT6;              // habilita saida no P1.0 (LED VERDE)
    P6REN &= ~(BIT6);           // habilita resistor de pull up
    P6OUT &= ~(BIT6);           // zera saida


    P1DIR &= ~BIT2;             // P1.2 é entrada que recebe o out do sensor
    //P1SEL |= BIT2;

    //S3
    P6DIR |= BIT3;              // P6.0 até P6.3 são entradas no sensor
    P6OUT &= ~BIT3;             // as configuraremos

    //S2
    P6DIR |= BIT2;
    P6OUT &= ~BIT2;             // S2 e S3 começam zerados

    //S1
    P6DIR |= BIT1;              // S0 = 0 e S1 = 1, para freq de saída a 20% (observação do professor)
    P6OUT &= ~BIT1;

    //S0
    P6DIR |= BIT0;
    P6OUT |= BIT0;


    /*  lógica de detectar as entradas?

    while(!(P1IN & BIT2));      //esperar receber uma leitura para seguir a diante

    // seta o Timer B aqui, começa a contar, escolher o jeito certo lá de contar

    while(P1IN & BIT2);      // trava programa por estar recebendo um sinal

    // salva este tempo em uma variável

    // o tempo sai em TB0CTL de acordo com o diagrama de instruções do timer B?

    // muda a flag que escolhe a cor

    // repete o processo, taca isso tudo num while(1) pra ficar mudando a cor

     */




    /*  lógica de setar os LEDs

    if(tempo_verde > tempo_azul && tempo verde > tempo_vermelho){                       // verde dominante
        P6OUT |= BIT6;          // setamos a luz verde
        P1OUT &= ~(BIT0);       // apagamos a vermelha
    }
    if(tempo_vermelho > tempo_azul && tempo_vermelho > tempo_verde){      // vermelho dominante
        P1OUT |= BIT0;          // ligamos a luz vermelha
        P6OUT &= ~(BIT6);       // apagamos a luz verde
    }
    else{                       // azul dominante
        P1OUT |= BIT0;          // ligamos a luz vermelha
        P6OUT |= BIT6;          // ligamos a luz verde
    }

    */

	return 0;
}
