#ifndef MATRIZ_5X5_H
#define MATRIZ_5X5_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pio_wave.pio.h"


//número de LEDs
#define NUM_PIXELS 25

//Pino de Dados para a Matriz
#define matriz_pin 7

//Pino Buzzer
#define BUZZER_PIN 21

//definição dos pinos do teclado matricial
#define lins 4
#define cols 4
//vetores pinos teclado
const uint8_t lin_pins[lins] = {20, 9, 8, 4}; //L4,L3,L2,L1
const uint8_t col_pins[cols] = {19, 18, 17, 16}; //C1,C2,C3,C4

//Criação de Pio
PIO pio = pio0;
extern int sm;

//prototipo de funções do teclado
void ini_pins_teclado();
char ler_teclado();

void iniciar_buzzer();
void buzzer_tocar(int frequencia, int duracao_ms);

void entrar_modo_gravacao();

void figura_1(PIO pio, int sm);
void figura_2(PIO pio, int sm);
void figura_3(PIO pio, int sm);
void figura_4(PIO pio, int sm);
void figura_5(PIO pio, int sm);


//Funções Matriz de Led
/**
 * 
    //Mapa leds

    25, 24, 23, 22, 21,

    16, 17, 18, 19, 20,  

    15, 14, 13, 12, 11,

    06, 07, 08, 09, 10,    

    05, 04, 03, 02, 01  
 */
void imprimir_binario(int num);
uint32_t matrix_rgb(double b, double r, double g);
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);
void desenha_fig(uint32_t *_matriz, uint8_t _intensidade, PIO pio, uint sm);

#endif // MATRIZ_5X5_H