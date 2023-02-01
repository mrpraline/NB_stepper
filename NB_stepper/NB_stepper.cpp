//Copyright 2012 Igor Campos
//   2015 Gerrit jan Baarda
//
//Non-Blocking (NB) stepper motor driver.
// This is a fork of CustomStepper by Igor Campos.
//Simplified and optimized for less resource use.
//This version uses only integer math.
//
//This file is part of NB_stepper.
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

#include "NB_stepper.h"

NB_stepper::NB_stepper(byte pin1, byte pin2, byte pin3, byte pin4, byte stepmode, unsigned int spr, 
                       unsigned int rpm, long intialpos, bool releaseOnDone, unsigned long totals)
{
  this->setMode(stepmode);
  this->setSPR(spr);
  this->setRPM(rpm);
  this->setDirection(FORWARD);
  this->setReleaseOnDone(releaseOnDone);
  this->pin1 = pin1;
  this->pin2 = pin2;
  this->pin3 = pin3;
  this->pin4 = pin4;
  pinMode(this->pin1, OUTPUT);
  pinMode(this->pin2, OUTPUT);
  pinMode(this->pin3, OUTPUT);
  pinMode(this->pin4, OUTPUT);
  this->nextStep = 0;
  this->position = intialpos;
  this->totalSteps = totals;
  this->done = true;
}

void NB_stepper::step()
// Step until job finished
{
  if (this->stepsToGo - this->stepsGone > 0)
  {
    if (this->direction != STOP)
    {

      if (this->direction == FORWARD)
      {
        this->position++;
        this->nextStep == this->noSteps - 1 ? this->nextStep = 0 : this->nextStep++;
      }
      else if (this->direction == BACKWARD)
      {
        this->position--;
        this->nextStep == 0 ? this->nextStep = this->noSteps - 1 : this->nextStep--;
      }

      digitalWrite(this->pin1, (this->modesteps[this->nextStep] & B1000));
      digitalWrite(this->pin2, (this->modesteps[this->nextStep] & B0100));
      digitalWrite(this->pin3, (this->modesteps[this->nextStep] & B0010));
      digitalWrite(this->pin4, (this->modesteps[this->nextStep] & B0001));

      this->lastStepTime=micros();
      this->stepsGone++;
      this->totalSteps++;
    }
    else
      this->disable();
  }
  /*   else if (this->mode == CONTINUOUS)
    {
      this->stepsGone = 0;
    } */
  else if (this->mode == ROTATIONS)
  {
    this->stepsGone = 0;
    this->stepsToGo = this->spr;

    if (++this->rotationsGone > this->rotationsToGo)
      this->disable();
  }
  else if (this->mode == STEPS)
    this->disable();
}

void NB_stepper::setReleaseOnDone(boolean release)
{
  this->releaseOnDone = release;
}

void NB_stepper::disable()
// de-energize all coils when releaseOnDone is set
{
  if (releaseOnDone)
  {
    digitalWrite(this->pin1, LOW);
    digitalWrite(this->pin2, LOW);
    digitalWrite(this->pin3, LOW);
    digitalWrite(this->pin4, LOW);
  }

  this->done = true;
}

void NB_stepper::setRPM(unsigned int rpm)
{
  if (rpm > 0)
    this->rpm = rpm;

  this->calcTime();
}

void NB_stepper::setPattern(byte step)
// pick the next activation pattern for the motor
{
  if (step >= 0 && step < this->noSteps)
    this->nextStep = step;
}

void NB_stepper::setSPR(unsigned int spr)
// Set motor physical (full) steps per revolution
{
  if (spr > 0)
    this->spr = spr;

  this->calcTime();
}

void NB_stepper::calcTime() 
// calculate microseconds per motor step
{
  if (this->spr > 0 && this->rpm > 0)
    this->stepPeriod = 60000000 / (this->spr * this->rpm);
}

void NB_stepper::setDirection(byte direction)
{
  if (direction == FORWARD || direction == BACKWARD || direction == STOP)
    this->direction = direction;
}

void NB_stepper::rotateSteps(long steps)
// rotate the motor given number of steps
{
  if (steps > 0)
  {
    this->mode = STEPS;
    this->stepsToGo = steps;
    this->stepsGone = 0;
    this->done = false;
    this->step();
  }
}

/* void NB_stepper::setTimer()
{
  this->lastStepTime = micros() ;
} */

void NB_stepper::run()
// call this often, to keep processing tasks
{
  if (micros()-this->lastStepTime > this->stepPeriod)
    this->step();
}

boolean NB_stepper::isDone()
{
  return this->done;
}

long NB_stepper::getPos()
{
  return this->position;
}

void NB_stepper::setRefPos(long ref)
// Set the refrence (zero) position
{
  this->position = ref;
}

long NB_stepper::gotoPos(long toPos)
// Move to position relative to zero.
{
  long movement = toPos - this->position;
  long steps = abs(movement);

  if (movement > 0)
    this->direction = FORWARD;
  else
    this->direction = BACKWARD;
  this->rotateSteps(steps);
  return steps;
}

unsigned long NB_stepper::getTotalSteps()
// return total movement in any direction since the program started
{
  return this->totalSteps;
}
void NB_stepper::setTotalSteps(unsigned long totals)
// set totalSteps (typically obtained from storing values at previous runs
{
  this->totalSteps = totals;
}
unsigned int NB_stepper::getSPR()
{
  return this->spr;
}
byte NB_stepper::getDirection()
{
  return this->direction;
}

void NB_stepper::setMode(byte stepmode)
// set full or halfstep operation
{
  this->noSteps = (stepmode == FULLSTEP ? 4 : 8);
  this->modesteps = (stepmode == FULLSTEP ? steps.full : steps.half);
}
byte NB_stepper::getMode()
{
  return this->noSteps;
}

byte NB_stepper::getPattern()
{
  return this->initStep;
}

bool NB_stepper::getReleaseOnDone()
{
  return this->releaseOnDone;
}
unsigned long NB_stepper::getStepTime()
{
  return this->stepPeriod;
}
