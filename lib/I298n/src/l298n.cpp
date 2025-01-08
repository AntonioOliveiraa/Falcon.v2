#include "l298n.h"
#include <Arduino.h>

void moveForward(Motor * motor1, Motor * motor2, int dutyCycle){
  motor1->setForward();
  motor2->setForward();
  motor1->setDutyCycle(dutyCycle);
  motor2->setDutyCycle(dutyCycle);
  motor1->run();
  motor2->run();
}

void moveBackward(Motor * motor1, Motor * motor2, int dutyCycle){
  motor1->setBackward();
  motor2->setBackward();
  motor1->setDutyCycle(dutyCycle);
  motor2->setDutyCycle(dutyCycle);
  motor1->run();
  motor2->run();
}

void stopMove(Motor * motor1, Motor * motor2){
  motor1->stopMotor();
  motor2->stopMotor();
  motor1->setDutyCycle(0);
  motor2->setDutyCycle(0);
}

void motorRamp(Motor * motor, int a, int b, int mode, int interval){
  int dutyCycle = 0;
  if(mode == STOPMODE){
    motor->stopMotor();
  }else if(mode == FORWARDMODE){
    motor->setForward();
  }
  else if(mode == BACKWARDMODE){
    motor->setBackward();
  }
  motor->setDutyCycle(dutyCycle);
  motor->run();
  if(a > b){
    for(int i = a; i >= b; i--){
      dutyCycle = i;
      motor->setDutyCycle(dutyCycle);
      motor->run();
      delay(interval);
    }
  } else if(a <= b){
    for(int i = a; i <= b; i++){
      dutyCycle = i;
      motor->setDutyCycle(dutyCycle);
      motor->run();
      delay(interval);
    }
  }
}

