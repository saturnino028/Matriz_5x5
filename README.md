# Atividade - Microcontroladores – GPIO - Parte 2

Atividade realizada com auxílio do simulador de eletrônica Wokwi para controlar pinos GPIO com o emprego de um teclado matricial 4x4. Com base nas teclas pressionadas, há o acionamento de uma matriz de LEDs (LED-RGB 5x5 WS2812) e o controle sonoro de um buzzer. É possível também utilizar o teclado do computador via comunicação serial para executar as funções.

A simulação no Wokwi conta com:

1) Teclado matricial 4x4;
2) LED-RGB 5x5 WS2812 (Vermelho, Verde e Azul);
3) Buzzer;
4) Placa de Desenvolvimento Raspberry Pi Pico W.

## Instruções de Uso

Cada uma das 16 teclas do teclado matricial 4x4 possui uma funcionalidade:

- Tecla 1: Executa animação 1 (cobrinha);
- Tecla 2: Executa animação 2 (X);
- Tecla 3: Executa animação 3 (colisão);
- Tecla 4: Executa animação 4 (pong);
- Tecla 5: Executa animação 5 (ondas);
- Tecla 6: TODO;
- Tecla 7: TODO;
- Tecla 8: TODO;
- Tecla 9: TODO;
- Tecla 0: TODO;
- Tecla *: Habilita o modo de gravação via software;
- Tecla #: Liga a cor branca em todos os LEDs da matriz (20% de luminosidade);
- Tecla A: Desliga todos os LEDs da matriz;
- Tecla B: Liga a cor azul em todos os LEDs da matriz (100% de luminosidade);
- Tecla C: Liga a cor vermelha em todos os LEDs da matriz (80% de luminosidade);
- Tecla D: Liga a cor verde em todos os LEDs da matriz (50% de luminosidade).

Para utilizar o programa com o simulador Wokwi, o usuário deve:

- Clonar este repositório: git clone https://github.com/saturnino028/U4C2O1234A---Matriz-de-LEDs.git;
- Usando a extensão Raspberry Pi Pico importar o projeto;
- Compilar o código e clicar no arquivo diagram.json.

Para utilizar o programa com o kit de desenvolvimento BitDogLab, o usuário deve:

- Clonar este repositório: git clone https://github.com/saturnino028/U4C2O1234A---Matriz-de-LEDs.git;
- Usando a extensão Raspberry Pi Pico importar o projeto;
- Compilar o projeto;
- Plugar a BitDogLab usando um cabo apropriado e gravar o código.

## Atenção

O brilho dos LEDs é muito intenso, para evitar danos aos olhos recomendamos que o brilho_padrao seja ajustado para 10% quando estiver usando o kit BitDogLab.

## Demonstração

<!-- TODO: adicionar link do vídeo -->
Vídeo demonstrando as funcionalidades da solução implementada: [Vídeo Demonstrativo](#).
