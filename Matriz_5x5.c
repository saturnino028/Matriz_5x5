#include "Matriz_5x5.h"
#include "figuras.h"
#include "pico/bootrom.h"

#define brilho_padrao 100 //Percentual de luminosidade dos LEDs

int main()
{
    // Iniciando variaveis e sistema
    PIO pio = pio0; //Cria uma variável que armazena o bloco PIO usado
    bool clock_setado; //Armazena estado do clock
    clock_setado = set_sys_clock_khz(133000, false); //Seta o clock do sitema
    stdio_init_all(); //Inicia a library stdio (UART)
    ini_pins_teclado(); //Inicia os pinos do teclado
    iniciar_buzzer();  // Inicializa o pino do buzzer

    if(clock_setado) 
        printf("Clock  setado %ld\n", clock_get_hz(clk_sys)); //Printa a velocidade do CLOCK

    int offset = pio_add_program(pio, &Matriz_5x5_program);
    int sm = pio_claim_unused_sm(pio, true);
    Matriz_5x5_program_init(pio, sm, offset, matriz_pin);

    desenha_fig(open, brilho_padrao, pio, sm);

    while(true)
    {
        char tecla = ler_teclado(); //Fazer leitura do teclado Matricial

        //Printar a tecla pressionada
        if(tecla != '\0')
        {
            printf("Tecla: %c\n", tecla);
            sleep_ms(100);
        }
        else
        {
            int c = getchar_timeout_us(1000); //Fazer leitura da serial
            if((c != PICO_ERROR_TIMEOUT) && (c != 10))
            {
                tecla = (char)c;
                printf("Tecla: %c\n", tecla); // Converte para char e imprime no terminal
                sleep_ms(100);
            }
        }
        
        //Verificar a ação a ser adotada
        switch (tecla)
        {
            case '1':
                figura_1(pio, sm);

                sleep_ms(1000);
                desenha_fig(open, brilho_padrao, pio, sm);
                break;
            case '2':
                figura_2(pio, sm);

                sleep_ms(1000);
                desenha_fig(open, brilho_padrao, pio, sm);
                break;
            case '3':
                figura_3(pio, sm);

                sleep_ms(1000);
                desenha_fig(open, brilho_padrao, pio, sm);
                break;
            case '4':
                figura_4(pio, sm);

                sleep_ms(1000);
                desenha_fig(open, brilho_padrao, pio, sm);
                break;
            case '5':
                figura_5(pio, sm);

                sleep_ms(1000);                  
                desenha_fig(open, brilho_padrao, pio, sm);
                break;
            case '6':
                break;
            case '7':
                break;
            case '8':
                break;
            case '9':
                break;
            case '0':
                break;
            case 'A':
                desenha_fig(matriz_apagada, 0, pio, sm);
                break;
            case 'B':
                desenha_fig(matriz_azul, 100, pio, sm);
                break;
            case 'C':
                desenha_fig(matriz_vermelha, 80, pio, sm);
                break;
            case 'D':
                desenha_fig(matriz_verde, 50, pio, sm);
                break;
            case '*':
                entrar_modo_gravacao();
                break;
            case '#':
                desenha_fig(matriz_branca, 20, pio, sm);
                break;
            default:
                break;
        }
        sleep_ms(100);
    }
}

void iniciar_buzzer()
{
    gpio_init(BUZZER_PIN);          // Inicializa o pino
    gpio_set_dir(BUZZER_PIN, GPIO_OUT); // Configura o pino como saída
}


/**
 * @brief Iniciar pinos do teclado
 */
void ini_pins_teclado()
{
    for(int i = 0; i<cols; i++)
    {
        gpio_init(col_pins[i]);
        gpio_set_dir(col_pins[i], GPIO_OUT);
        gpio_put(col_pins[i], 0);
    }

    for(int i = 0; i<lins; i++)
    {
        gpio_init(lin_pins[i]);
        gpio_set_dir(lin_pins[i], GPIO_IN);
        gpio_pull_down(lin_pins[i]);
    }
}

/**
 * @brief Ler Teclado
 * @return retorna o caracter lido
 */
char ler_teclado(){
    //mapa de teclas do teclado
    const char key_map[lins][cols] = 
    {
        {'*','0','#','D'},
        {'7','8','9','C'},
        {'4','5','6','B'},
        {'1','2','3','A'}
    };

    for(int col = 0; col<cols; col++)
    {
        gpio_put(col_pins[col], 1);
        for(int row= 0; row<lins; row++)
        {
        if(gpio_get(lin_pins[row]))
        {
            gpio_put(col_pins[col], 0);
            return key_map[row][col];
        }
        }
        gpio_put(col_pins[col], 0);
    }
    return '\0';
}

/**
 * @brief Desenha figura (aceita um padrão hexa, vindo direto do https://www.piskelapp.com/)
 * @param _matriz aceita uma matriz 1x25 com o padrão dos leds
 * @param _intensidade aceita um inteiro de 0 a 100 refente a intensidade do led
 * @param pio é o periferico pio que vai ser utilizado
 * @param sm é a máquina de estados dentro do pio escolhido que será usada (4 possibilidades)
 * 
    //Mapa leds

    25, 24, 23, 22, 21,

    16, 17, 18, 19, 20,  

    15, 14, 13, 12, 11,

    06, 07, 08, 09, 10,    

    05, 04, 03, 02, 01  
 */
void desenha_fig(uint32_t *_matriz, uint8_t _intensidade, PIO pio, uint sm){
    uint32_t pixel = 0; uint8_t r, g, b;

    for(int i = 24; i>19; i--) //Linha 1
    {
        pixel = _matriz[i];
        b = ((pixel >> 16) & 0xFF)*(_intensidade/100.00); // Isola os 8 bits mais significativos (azul)
        g = ((pixel >> 8) & 0xFF)*(_intensidade/100.00);  // Isola os 8 bits intermediários (verde)
        r = (pixel & 0xFF)*(_intensidade/100.00);         // Isola os 8 bits menos significativos (vermelho)
        pixel = 0;
        pixel = (g << 16) | (r << 8) | b;
        pio_sm_put_blocking(pio, sm, pixel<<8u);
    }

    for(int i = 15; i<20; i++) //Linha 2
    {
        pixel = _matriz[i];
        b = ((pixel >> 16) & 0xFF)*(_intensidade/100.00); // Isola os 8 bits mais significativos (azul)
        g = ((pixel >> 8) & 0xFF)*(_intensidade/100.00);  // Isola os 8 bits intermediários (verde)
        r = (pixel & 0xFF)*(_intensidade/100.00);         // Isola os 8 bits menos significativos (vermelho)
        pixel = 0;
        pixel = (b << 16) | (r << 8) | g;
        pixel = (g << 16) | (r << 8) | b;
        pio_sm_put_blocking(pio, sm, pixel<<8u);

    }

    for(int i = 14; i>9; i--) //Linha 3
    {
        pixel = _matriz[i];
        b = ((pixel >> 16) & 0xFF)*(_intensidade/100.00); // Isola os 8 bits mais significativos (azul)
        g = ((pixel >> 8) & 0xFF)*(_intensidade/100.00);  // Isola os 8 bits intermediários (verde)
        r = (pixel & 0xFF)*(_intensidade/100.00);         // Isola os 8 bits menos significativos (vermelho)
        pixel = 0;
        pixel = (g << 16) | (r << 8) | b;
        pio_sm_put_blocking(pio, sm, pixel<<8u);
    }

    for(int i = 5; i<10; i++) //Linha 4
    {
        pixel = _matriz[i];
        b = ((pixel >> 16) & 0xFF)*(_intensidade/100.00); // Isola os 8 bits mais significativos (azul)
        g = ((pixel >> 8) & 0xFF)*(_intensidade/100.00);  // Isola os 8 bits intermediários (verde)
        r = (pixel & 0xFF)*(_intensidade/100.00);         // Isola os 8 bits menos significativos (vermelho)
        pixel = 0;
        pixel = (g << 16) | (r << 8) | b;
        pio_sm_put_blocking(pio, sm, pixel<<8u);
    }

    for(int i = 4; i > -1 ; i--) //Linha 5
    {
        pixel = _matriz[i];
        b = ((pixel >> 16) & 0xFF)*(_intensidade/100.00); // Isola os 8 bits mais significativos (azul)
        g = ((pixel >> 8) & 0xFF)*(_intensidade/100.00);  // Isola os 8 bits intermediários (verde)
        r = (pixel & 0xFF)*(_intensidade/100.00);         // Isola os 8 bits menos significativos (vermelho)
        pixel = 0;
        pixel = (g << 16) | (r << 8) | b;
        pio_sm_put_blocking(pio, sm, pixel<<8u);
    }
}

// Função para tocar o buzzer
void buzzer_tocar(int frequencia, int duracao_ms)
{
    int periodo_us = 1000000 / frequencia; // Período em microsegundos
    int ciclos = (duracao_ms * 1000) / periodo_us; // Número de ciclos

    for (int i = 0; i < ciclos; i++) {
        gpio_put(BUZZER_PIN, 1); // Liga o buzzer
        sleep_us(periodo_us / 2); // Metade do período
        gpio_put(BUZZER_PIN, 0); // Desliga o buzzer
        sleep_us(periodo_us / 2); // Metade do período
    }
}

void entrar_modo_gravacao() {
    printf("Entrando no modo de gravação...\n");
    // Se quiser reiniciar no modo de gravação USB:
    reset_usb_boot(0, 0); 
}

void figura_1(PIO pio, int sm)
{ 
    desenha_fig(tecla1_f1, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f2, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f3, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f4, brilho_padrao, pio, sm);
    buzzer_tocar(800, 50); // Toca sincronizado ao frame 4
    sleep_ms(500);
    desenha_fig(tecla1_f5, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f6, brilho_padrao, pio, sm);
    buzzer_tocar(800, 50); // Toca sincronizado ao frame 6
    sleep_ms(500);
    desenha_fig(tecla1_f7, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f8, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f9, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f10, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f11, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f12, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f13, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f14, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f15, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f16, brilho_padrao, pio, sm);
    buzzer_tocar(800, 50); // Toca sincronizado ao frame 16
    sleep_ms(500);
    desenha_fig(tecla1_f17, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f18, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f19, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f20, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(tecla1_f21, brilho_padrao, pio, sm);
    buzzer_tocar(800, 50); // Toca sincronizado ao frame 21
    sleep_ms(500);
    desenha_fig(tecla1_f22, brilho_padrao, pio, sm);
    return;
}

void figura_2(PIO pio, int sm)
{
    desenha_fig(x_f1, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(x_f2, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(x_f3, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(x_f4, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(x_f5, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(x_f6, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(x_f7, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(x_f8, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(x_f9, brilho_padrao, pio, sm);
    sleep_ms(500);
    desenha_fig(x_f10, brilho_padrao, pio, sm);
    sleep_ms(500);
    return;
}

void figura_3(PIO pio, int sm)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        desenha_fig(colisao1, brilho_padrao, pio, sm);
        sleep_ms(250);
        desenha_fig(colisao2, brilho_padrao, pio, sm);
        sleep_ms(250);
        desenha_fig(colisao3, brilho_padrao, pio, sm);
        sleep_ms(500);
        desenha_fig(colisao4, brilho_padrao, pio, sm);
        sleep_ms(1000);
        desenha_fig(colisao5, brilho_padrao, pio, sm);
        sleep_ms(1000);
        desenha_fig(colisao6, brilho_padrao, pio, sm);
    }
    return;
}

void figura_4(PIO pio, int sm)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        desenha_fig(pong1, brilho_padrao, pio, sm);
        sleep_ms(500);
        desenha_fig(pong2, brilho_padrao, pio, sm);
        sleep_ms(500);
        desenha_fig(pong3, brilho_padrao, pio, sm);
        sleep_ms(500);
        desenha_fig(pong4, brilho_padrao, pio, sm);
        sleep_ms(500);
        desenha_fig(pong5, brilho_padrao, pio, sm);
    }
    return;
}

void figura_5(PIO pio, int sm)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        // animação de ondas
        desenha_fig(onda1, brilho_padrao, pio, sm);
        sleep_ms(500);
        desenha_fig(onda2, brilho_padrao, pio, sm);
        sleep_ms(500);
        desenha_fig(onda3, brilho_padrao, pio, sm);
        sleep_ms(500);
        desenha_fig(onda4, brilho_padrao, pio, sm);
        sleep_ms(500);
        desenha_fig(onda5, brilho_padrao, pio, sm);
        sleep_ms(500);
    }
    return;
}