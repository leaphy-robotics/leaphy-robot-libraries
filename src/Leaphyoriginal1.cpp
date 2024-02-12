// Leahyoriginal1.cpp
// suitable for Fundumoto Shield
// Copyright Science4Kids& Stichting Leaphy 2018
// version 1.0.0    25 JULI 2018

#include "Leaphyoriginal1.h"


/* Allow motor pins to be overriden for different robot types */
void setMotorPins(int m1_pwm, int m1_dir, int m2_pwm, int m2_dir)
{
    motor_pins.m1_pwm = m1_pwm;
    motor_pins.m1_dir = m1_dir;
    motor_pins.m2_pwm = m2_pwm;
    motor_pins.m2_dir = m2_dir;
}


float getDistance()
{
    float duration, distance;
    int tries = 0;

    pinMode(US_TRIG, OUTPUT);
    digitalWrite(US_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(US_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(US_TRIG, LOW);
        duration = pulseIn(US_ECHO, HIGH, 30000);
        distance = 0.034 * duration / 2;
    if(distance == 0 ){
      distance = distance + 1313;
    }

    return distance;
}

int getLineFollower(int fpSide)
{
    uint8_t pinLINE = fpSide == LEFT  ? LINE_LEFT :
                      fpSide == RIGHT ? LINE_RIGHT : -1;
    return digitalRead(pinLINE);
}

void setTone(int fpTone, int fpBeat)
{
    tone(TONE_PIN, fpTone, fpBeat);
}

void setMotor(int fpMotor, int fpSpeed)
{
    /* Select right pins for the given motor */
    uint8_t pinDIR = fpMotor == MOTOR_LEFT  ? motor_pins.m1_dir :
                     fpMotor == MOTOR_RIGHT ? motor_pins.m2_dir : -1;
    uint8_t pinPWM = fpMotor == MOTOR_LEFT  ? motor_pins.m1_pwm :
                     fpMotor == MOTOR_RIGHT ? motor_pins.m2_pwm : -1;
    /* Set DIR and PWM pins */
    pinMode(pinDIR, OUTPUT);
    pinMode(pinPWM, OUTPUT);
    uint8_t direction = fpSpeed > 0 ? 1 : 0;
    int speed = abs(fpSpeed);
    digitalWrite(pinDIR, direction);
    analogWrite(pinPWM, speed);
}

void moveMotors(int fpDirection, int fpSpeed)
{
    switch (fpDirection)
    {
        case FORWARD:
            setMotor(MOTOR_LEFT,   fpSpeed);
            setMotor(MOTOR_RIGHT,  fpSpeed);
            break;
        case BACKWARD:
            setMotor(MOTOR_LEFT,  -fpSpeed);
            setMotor(MOTOR_RIGHT, -fpSpeed);
            break;
        case LEFT:
            setMotor(MOTOR_LEFT,  -fpSpeed);
            setMotor(MOTOR_RIGHT,  fpSpeed);
            break;
        case RIGHT:
            setMotor(MOTOR_LEFT,   fpSpeed);
            setMotor(MOTOR_RIGHT, -fpSpeed);
            break;
    }
}

void setLed(int fpRed, int fpGreen, int fpBlue)
{
    analogWrite(LED1_RED, fpRed);
    analogWrite(LED1_GREEN, fpGreen);
    analogWrite(LED1_BLUE, fpBlue);
}

void i2cSelectChannel(uint8_t channel, bool push=TRUE) {
    if (push) i2cChannelStack.addLast(channel);
    if(channel <= 7) channel = (1 << channel);
    Wire.beginTransmission(0x70);
    Wire.write(channel);
    Wire.endTransmission();
}

void i2cRestoreChannel() {
    i2cChannelStack.removeLast();
    uint8_t channel = i2cChannelStack.get(i2cChannelStack.size() - 1);
    i2cSelectChannel(channel, FALSE);
}

uint8_t i2cGetChannel() {
    uint8_t channel = i2cChannelStack.get(i2cChannelStack.size() - 1);
    if (channel > 7) channel = 0;
    return channel;
}