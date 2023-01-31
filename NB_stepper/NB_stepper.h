//Copyright 2012 Igor Campos
//          2015 Gerrit jan Baarda
//
//Non-Blocking (NB) stepper motor driver.
//Simplified and optimized fork of CustomStepper by Igor Campos.
//This version has no floating point math.
//
//NB_stepper is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//NB_stepper is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with NB_stepper.  If not, see <http://www.gnu.org/licenses/>.

#ifndef NB_STEPPER_H
#define NB_STEPPER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define CONTINUOUS 0
#define ROTATIONS 1
#define STEPS 2
#define STOP 0
#define FORWARD 1  // FORWARD means: absolute postion increases with positive step values
#define BACKWARD 2
#define FULLSTEP 4
#define HALFSTEP 8

class NB_stepper
{
private:
  struct
  {
    byte half[8] = {B1100, B0100, B0110, B0010, B0011, B0001, B1001, B1000};
    byte full[4] = {B1100, B0110, B0011, B1001};
  } steps;
  boolean done;
  byte *modesteps;
  byte mode;
  byte pin1;
  byte pin2;
  byte pin3;
  byte pin4;
  byte noSteps;
  byte nextStep;
  byte direction;
  byte initStep;
  boolean releaseOnDone;
  long stepsToGo;
  long stepsGone;
  long rotationsToGo;
  long rotationsGone;
  unsigned long totalSteps;
  unsigned int rpm;
  unsigned int spr;
  unsigned long stepPeriod;
  unsigned long lastStepTime;
  long position;
 // void setTimer();
  void calcTime();
  void disable();
  void step();
  void setDirection(byte direction = FORWARD);
  byte getDirection();
  void rotateSteps(long steps);

public:
  NB_stepper(byte pin1, byte pin2, byte pin3, byte pin4, byte mode = HALFSTEP, unsigned int spr = 96, unsigned int rpm = 12, long intialpos=0, bool releaseOnDone = true, unsigned long totals = 0);
  void setRPM(unsigned int RPM); // set revolutions per minute
  void setSPR(unsigned int SPR); // set motor Steps per revolution
  unsigned int getSPR();         // get Steps per revolution
  void setMode(byte mode);
  byte getMode();
  void run();
  void setRefPos(long ref = 0);
  long getPos();
  long gotoPos(long pos);
  void setReleaseOnDone(boolean release = true); // release coils when nothing to do ( reduces power consumption )
  bool getReleaseOnDone();
  unsigned long getTotalSteps();
  void setTotalSteps(unsigned long totals = 0);
  boolean isDone();
  void setPattern(byte step); // Set initial pattern
  byte getPattern();
  unsigned long getStepTime();
};

#endif
