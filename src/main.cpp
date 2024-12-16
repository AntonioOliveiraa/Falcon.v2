/*******************************************************************************
* Primeiros Passos com o Modulo Encoder E2-Q2 (v1.0)
*
* Codigo que determina o sentido e a rotacao do eixo do motor DC com
* caixa de reducao e eixo duplo e exibe os dados no monitor serial. As
* informacoes obtidas do motor sao calculadas e exibidas no monitor serial
* a cada minuto. Para controlar o motor, pressione os botoes ("Btn0" e "Btn1")
* da propria Julieta.
*
* Copyright 2020 RoboCore.
* Escrito por Giovanni de Castro (29/10/2020).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version (<https://www.gnu.org/licenses/>).
*******************************************************************************/

#include <Arduino.h>

//Declaracao dos pinos de controle do motor DC da Julieta
const int PINO_INA = 7;
const int PINO_ENA = 5;

//Declaracao dos pinos conectados aos botoes da Julieta
const int PINO_BOTAO1 = A0;
const int PINO_BOTAO2 = A1;

//Declaracao dos pinos conectados aos canais de saida do encoder
const int PINO_CH2 = 2;
const int PINO_CH1 = 3;

//Declaracao das variaveis auxiliares para a verificacao do sentido
int estado;
int ultimo_estado;
boolean sentido;

//Declaracao das variaveis auxiliares para o calculo da velocidade
unsigned long contador1;
unsigned long contador2;
const int NUMERO_CONTADORES = 2;
const int NUMERO_LEITURAS = 2;
//Variavel de numero de dentes do disco de leitura
const int NUMERO_DENTES = 6; //Altere se necessario

//Declaracao das variaveis auxiliares para a temporizacao de um minuto
unsigned long tempo_antes = 0;
const long MINUTO = 60000;

//---------------------------------------------------------------------------------------
void setup() {

  //Inicializacao do monitor serial
  Serial.begin(9600);
  Serial.println("Monitoramento de Sentido e de Rotacao de Motor DC");

  //Configura os pinos de controle do motor como uma saida
  pinMode(PINO_INA, OUTPUT);
  pinMode(PINO_ENA, OUTPUT);

  //Inicia os pinos de controle do motor em nivel logico baixo
  digitalWrite(PINO_INA, LOW);
  analogWrite(PINO_ENA, 0);

  //Configura os pinos conectados aos botoes como uma entrada
  pinMode(PINO_BOTAO1, INPUT_PULLUP);
  pinMode(PINO_BOTAO2, INPUT_PULLUP);

  //Configuracao dos pinos conectados aos canais do encoder como entrada
  pinMode(PINO_CH2, INPUT);
  pinMode(PINO_CH1, INPUT);

  //Inicializa as interrupcoes com os pinos configurados para chamar as funcoes  
  //"contador_pulso2" e "contador_pulso1" respectivamente a cada mudanca de estado das portas
  attachInterrupt(digitalPinToInterrupt(PINO_CH2), contador_pulso2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PINO_CH1), contador_pulso1, CHANGE);

}

//---------------------------------------------------------------------------------------
void loop() {

  //Controla o sentido dos motores e reinicia as leituras
  if (digitalRead(PINO_BOTAO1) == LOW || digitalRead(PINO_BOTAO2) == LOW) { //Se um dos botoes for pressionado
    delay(30);
    if (digitalRead(PINO_BOTAO1) == LOW || digitalRead(PINO_BOTAO2) == LOW) {
      
      if (digitalRead(PINO_BOTAO1) == LOW) { //Se o botao "0" for pressionado
        //Aciona o motor em um dos sentidos
        digitalWrite(PINO_INA, HIGH);
      } else { //Se o botao "1" for pressionado
        //Aciona o motor no sentido inverso
        digitalWrite(PINO_INA, LOW);
      }

      //Zera os contadores e reinicia a contagem de tempo
      contador1 = 0;
      contador2 = 0;
      tempo_antes = millis();

      //Para o motor e aguarda um segundo para o eixo parar completamente
      analogWrite(PINO_ENA, 0);
      delay(1000);

      //Rampa de aceleracao do motor
      for (int i = 0; i < 255; i++) {
        analogWrite(PINO_ENA, i);
        delay(10);
      }
    }
  }

  //Verifica a contagem de tempo e exibe as informacoes coletadas do motor
  if ((millis() - tempo_antes) > MINUTO) { //A cada minuto

    //Verifica a variavel "sentido"
    if (sentido) { //Se ela for verdadeira ("true")
      Serial.print("Sentido: Horario");
      Serial.print("       |  ");
    } else { //Se ela for falsa ("false")
      Serial.print("Sentido: Anti-Horario");
      Serial.print("  |  ");
    }

    //Calcula a velocidade e exibe no monitor
    int media = (contador1 + contador2) / (NUMERO_CONTADORES); //Calcula a media dos contadores
    int velocidade = media / (NUMERO_DENTES  * NUMERO_LEITURAS); //Calcula a velocidade de acordo com o numero de dentes do disco
    Serial.print("Velocidade: ");
    Serial.print(velocidade);
    Serial.println(" RPM");

    //Zera os contadores e reinicia a contagem de tempo.
    contador1 = 0;
    contador2 = 0;
    tempo_antes = millis();

  }

}

//---------------------------------------------------------------------------------------
//Funcao de interrupcao
void contador_pulso2() {

  //Incrementa o contador
  contador2++;

  //Verifica o sentido de rotacao do motor
  estado = digitalRead(PINO_CH2);
  if (ultimo_estado == LOW && estado == HIGH) {
    if (digitalRead(PINO_CH1) == LOW) {
      sentido = true;
    } else {
      sentido = false;
    }
  }
  ultimo_estado = estado;

}

//Funcao de interrupcao
void contador_pulso1() {

  //Incrementa o contador
  contador1++;

}