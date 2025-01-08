#ifndef L298N
#define L298N

#include <Arduino.h>

#define FORWARDMODE 1
#define BACKWARDMODE 2
#define STOPMODE 0
#define LEFT 0
#define RIGHT 1

class Motor {
public:

    void classSetup(){
    //
    }
    void setEnablePin(int pin){
      this->enablePin = pin;
    }

    int getEnablePin(){
      return this->enablePin;
    }

    void setPins(int pin1, int pin2){
      pinMode(pin1, OUTPUT);
      pinMode(pin2, OUTPUT);
      if(this->mode == LEFT){
        this->pin2 = pin1;
        this->pin1 = pin2;
      } else {
        this->pin1 = pin1;
        this->pin2 = pin2;
      }
    }

    int getPin1(){
      return this->pin1;
    }

    int getPin2(){
      return this->pin2;
    }

    void setDutyCycle(int dutyCycle){
      this->dutyCycle = dutyCycle;
    }

    int getDutyCycle(){
      return this->dutyCycle;
    }

    void setMode(int mode){
      this->mode = mode;
    }

    int getMode(){
      return this->mode;
    }

    void setSide(int side){
      this->side = side;
    }

    int getSide(){
      return this->side;
    }

    void setPwmChannels(int channel1, int channel2){
      this->pwmChannel1 = channel1;
      this->pwmChannel2 = channel2;
      ledcAttachChannel(this->pin1, this->freq, this->resolution, this->pwmChannel1);
      ledcAttachChannel(this->pin2, this->freq, this->resolution, this->pwmChannel2);
    }

    int getPwmChannel1(){
      return this->pwmChannel1;
    }

    int getPwmChannel2(){
      return this->pwmChannel2;
    }

    void setFreq(int freq){
      this->freq = freq;
    }

    int getFreq(){
      return this->freq;
    }

    void setResolution(int resolution){
      this->resolution = resolution;
    }

    int getResolution(){
      return this->resolution;
    }

    void enable(){
      pinMode(this->enablePin, OUTPUT);
      digitalWrite(this->enablePin, HIGH);
    }

    void stopMotor(){
      this->setMode(STOPMODE);
      this->setDutyCycle(0);
      Serial.println("Stopping motor");
      ledcWrite(this->pin1, this->dutyCycle);
      ledcWrite(this->pin2, this->dutyCycle);
    }

    void setForward(){
      this->setMode(FORWARDMODE);
      ledcWrite(this->pin2, 0);
    }

    void setBackward(){
      this->setMode(BACKWARDMODE);
      ledcWrite(this->pin1, 0);
    }

    void run(){
      if(this->mode == FORWARDMODE){
        //Serial.println("Moving forward!");
        ledcWrite(this->pin1, this->dutyCycle);
      } else if(this->mode == BACKWARDMODE){
        //Serial.println("Moving backward!");
        ledcWrite(this->pin2, this->dutyCycle);
      } else {
        Serial.println("Please set motor mode!");
      }
    }

    Motor(int enablePin, int pin1, int pin2, int pwmChannel1,int pwmChannel2){
      this->setEnablePin(enablePin);
      this->setPins(pin1, pin2);
      pinMode(this->enablePin, OUTPUT);
      pinMode(this->pin1, OUTPUT);
      pinMode(this->pin2, OUTPUT);
      this->setPwmChannels(pwmChannel1, pwmChannel2);
    }

    Motor(int enablePin, int pin1, int pin2, int pwmChannel1, int pwmChannel2, int dutyCycle){
      this->setEnablePin(enablePin);
      this->setPins(pin1, pin2);
      pinMode(this->enablePin, OUTPUT);
      pinMode(this->pin1, OUTPUT);
      pinMode(this->pin2, OUTPUT);
      this->setPwmChannels(pwmChannel1, pwmChannel2);
      this->setDutyCycle(dutyCycle);
    }

    Motor(int enablePin, int pin1, int pin2, int pwmChannel1,int pwmChannel2, int dutyCycle, char side){
      this->setEnablePin(enablePin);
      this->setDutyCycle(dutyCycle);
      this->setSide(side);
      if(side == LEFT){
        this->setPins(pin2, pin1);
      } else {
        this->setPins(pin1, pin2);
      }
      pinMode(this->enablePin, OUTPUT);
      pinMode(this->pin1, OUTPUT);
      pinMode(this->pin2, OUTPUT);
      ledcAttachChannel(this->pin1, 30000, 8, pwmChannel1);
      ledcAttachChannel(this->pin2, 30000, 8, pwmChannel2);
    }

private:
    int enablePin = 14;
    int pin1 = 27;
    int pin2 = 26;
    int dutyCycle = 170;
    int mode = 0;
    int side = LEFT;
    int pwmChannel1 = 0;
    int pwmChannel2 = 1;
    int freq = 30000;
    int resolution = 8;
};

void motorRamp(Motor * motor, int a, int b, int mode, int interval);

void moveForward(Motor * motor1, Motor * motor2, int dutyCycle);

void moveBackward(Motor * motor1, Motor * motor2, int dutyCycle);

void stopMove(Motor * motor1, Motor * motor2);

#endif