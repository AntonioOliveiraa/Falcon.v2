#include "l298n.h"
#include "Wire.h"
#include "Adafruit_TCS34725.h"

// DIAMETRO RODA: 68mm

//Declaracao dos pinos conectados aos canais de saida do encoder
const int PIN_A_ENCODER = 15;
const int PIN_B_ENCODER = 4;

//Declaracao das variaveis auxiliares para a verificacao do do motor A
int rpm_A = 0;
int rpm_B = 0;
unsigned long last_Calc_A = 0;
unsigned long last_Calc_B = 0;

//Declaracao das variaveis auxiliares para o calculo da velocidade
unsigned long counterA = 0;
unsigned long counterB = 0;

//Variavel de numero de dentes do disco de leitura
const int TEETH = 10;

//Declaracao das variaveis auxiliares para a temporizacao de um minuto
const long MINUTO = 60000;

Motor * motor1 = new Motor(14, 25, 33, 0, 1, 100, LEFT);
Motor * motor2 = new Motor(12, 27, 26, 2, 3, 100, RIGHT);

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

// Initialise with specific int time and gain values 
// Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);

//

void setup() {
  // Init Serial
  Serial.begin(115200);
  last_Calc_A = millis();

  // if (tcs.begin()) {
  //   Serial.println("Found TCS34725 sensor");
  // } else {
  //   Serial.println("No TCS34725 found... check your connections");
  //   while (1);
  // }

  // Inicializa as interrupcoes com os pinos configurados para chamar as funcoes  
  attachInterrupt(digitalPinToInterrupt(PIN_A_ENCODER), countA, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_B_ENCODER), countB, RISING);

}

void loop() {
  /*motor1->enable();
  motor2->enable();
  delay(5000);
  moveForward(motor1, motor2, 140);
  delay(5000);
  moveBackward(motor1, motor2, 140);
  delay(5000);
  stopMove(motor1, motor2);
  delay(1000);
  Serial.println("Moving Motor 1 forward!");
  motorRamp(motor1, 130, 255, FORWARDMODE, 30);
  delay(5000);
  motorRamp(motor1, 255, 130, FORWARDMODE, 30);
  delay(5000);
  Serial.println("Moving Motor 1 backward!");
  motorRamp(motor1, 130, 255, BACKWARDMODE, 30);
  delay(5000);
  motorRamp(motor1, 255, 130, BACKWARDMODE, 30);
  delay(15000);*/

  /*Serial.println("Moving Motor 2 forward!");
  motorRamp(motor2, 130, 255, FORWARDMODE, 30);
  delay(3000);
  motorRamp(motor2, 255, 130, FORWARDMODE, 30);
  delay(3000);
  Serial.println("Moving Motor 2 backward!");
  motorRamp(motor2, 130, 255, BACKWARDMODE, 30);
  delay(3000);
  motorRamp(motor2, 255, 130, BACKWARDMODE, 30);*/
  // uint16_t r, g, b, c, colorTemp, lux;

  // tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  // lux = tcs.calculateLux(r, g, b);

  // Serial.print("Color Temp: "); Serial.print(colorTemp, DEC);
  // Serial.print(" K - ");
  // Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  // Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  // Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  // Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  // Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  // Serial.println(" ");
  delay(3000);

}

// Interrupt Function
void countA() {
  //Incrementa o contador
  counterA++;
  if((counterA % TEETH) == 0){
    rpm_A = (MINUTO / (millis() - last_Calc_A));
    last_Calc_A = millis();
    counterA = 0;
    Serial.print("Duty Cycle A: ");
    Serial.println(motor1->getDutyCycle());
    Serial.print("RPM A: ");
    Serial.println(rpm_A);
  }
}

// Interrupt Function
void countB() {
  //Incrementa o contador
  counterB++;
  if((counterB % TEETH) == 0){
    rpm_B = (MINUTO / (millis() - last_Calc_B));
    last_Calc_B = millis();
    counterB = 0;
    Serial.print("Duty Cycle B: ");
    Serial.println(motor2->getDutyCycle());
    Serial.print("RPM B: ");
    Serial.println(rpm_B);
  }
}
