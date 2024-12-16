#include <Arduino.h>

// Declaracao dos pinos de controle do motor DC da Julieta
const int PINO_INA = 23;
const int PINO_ENA = 22;

// Declaracao dos pinos conectados aos canais de saida do encoder
const int PINO_CH2 = 19;
const int PINO_CH1 = 18;

// Declaracao das variaveis auxiliares para a verificacao do sentido
int estado;
int ultimo_estado;
boolean sentido;

// Declaracao das variaveis auxiliares para o calculo da velocidade
unsigned long contador1;
unsigned long contador2;
const int NUMERO_CONTADORES = 2;
const int NUMERO_LEITURAS = 2;
// Variavel de numero de dentes do disco de leitura
const int NUMERO_DENTES = 10; // Altere se necessario

// Declaracao das variaveis auxiliares para a temporizacao de um minuto
unsigned long tempo_antes = 0;
const long MINUTO = 60000;

// Variaveis para configuracao do sentido e velocidade do motor
const boolean SENTIDO_HORARIO = true;
const int VELOCIDADE_MAXIMA = 255; // Velocidade maxima do motor

//---------------------------------------------------------------------------------------
void setup() {

  // Inicializacao do monitor serial
  Serial.begin(115200);
  Serial.println("Monitoramento de Sentido e de Rotacao de Motor DC");

  // Configura os pinos de controle do motor como uma saida
  pinMode(PINO_INA, OUTPUT);
  pinMode(PINO_ENA, OUTPUT);

  // Inicia os pinos de controle do motor em nivel logico baixo
  digitalWrite(PINO_INA, LOW);
  ledcAttachPin(PINO_ENA, 0);  // Configurando canal PWM para ESP32
  ledcSetup(0, 5000, 8);       // Frequencia de 5 kHz, resolucao de 8 bits
  ledcWrite(0, 0);

  // Configuracao dos pinos conectados aos canais do encoder como entrada
  pinMode(PINO_CH2, INPUT);
  pinMode(PINO_CH1, INPUT);

  // Inicializa as interrupcoes com os pinos configurados para chamar as funcoes  
  // "contador_pulso2" e "contador_pulso1" respectivamente a cada mudanca de estado das portas
  attachInterrupt(digitalPinToInterrupt(PINO_CH2), contador_pulso2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PINO_CH1), contador_pulso1, CHANGE);

  // Inicia o motor em sentido horario e acelera gradualmente
  iniciar_motor(SENTIDO_HORARIO);
}

//---------------------------------------------------------------------------------------
void loop() {

  // Verifica a contagem de tempo e exibe as informacoes coletadas do motor
  if ((millis() - tempo_antes) > MINUTO) { // A cada minuto

    // Verifica a variavel "sentido"
    if (sentido) { // Se ela for verdadeira ("true")
      Serial.print("Sentido: Horario");
      Serial.print("       |  ");
    } else { // Se ela for falsa ("false")
      Serial.print("Sentido: Anti-Horario");
      Serial.print("  |  ");
    }

    // Calcula a velocidade e exibe no monitor
    int media = (contador1 + contador2) / (NUMERO_CONTADORES); // Calcula a media dos contadores
    int velocidade = media / (NUMERO_DENTES  * NUMERO_LEITURAS); // Calcula a velocidade de acordo com o numero de dentes do disco
    Serial.print("Velocidade: ");
    Serial.print(velocidade);
    Serial.println(" RPM");

    // Zera os contadores e reinicia a contagem de tempo.
    contador1 = 0;
    contador2 = 0;
    tempo_antes = millis();
  }
}

//---------------------------------------------------------------------------------------
// Funcao de interrupcao
void contador_pulso2() {

  // Incrementa o contador
  contador2++;

  // Verifica o sentido de rotacao do motor
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

// Funcao de interrupcao
void contador_pulso1() {

  // Incrementa o contador
  contador1++;
}

// Funcao para iniciar o motor
void iniciar_motor(boolean sentido_horario) {

  // Define o sentido do motor
  if (sentido_horario) {
    digitalWrite(PINO_INA, HIGH);
  } else {
    digitalWrite(PINO_INA, LOW);
  }

  // Zera os contadores e reinicia a contagem de tempo
  contador1 = 0;
  contador2 = 0;
  tempo_antes = millis();

  // Rampa de aceleracao do motor
  for (int i = 0; i <= VELOCIDADE_MAXIMA; i++) {
    ledcWrite(0, i);
    delay(10);
  }
}
