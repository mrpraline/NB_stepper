//Copyright 2012 Igor Campos
//			2015 Gerrit jan Baarda
//
//  Simplified version of CustomStepper
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

#include <NB_stepper.h>

//Full constructor, just the first 4 parameters are necessary, they are the pins connected to the motor,
//the others are optional, and default to the following below:
//The 5th paramater is the steps sequence, it can have a maximum of 8 steps,
//the 6th parameter is the SPR (Steps Per Rotation),
//the 7th parameter is the RPM,
//the 8th parameter is the rotation orientation.
//the 9th parameter is the initial excitation pattern of the motor. 
//the 10th parameter will remove the motor power when the motor is at rest. 
//The optional parameters can be queried and set by get/set methods at a later stage.

NB_stepper stepper(6, 7, 8, 9,  HALFSTEP, 96, 12, CW, 0, true);
boolean rotate1 = false;
boolean rotatedeg = false;
boolean crotate = false;

void setup()
{
// set the speed of the motor to 300 RPMs
  Serial.begin(115200);
  while (!Serial)
    {
      ; // wait for serial port to connect. Needed for Leonardo only
    }
  //sets the RPM
  stepper.setRPM(120);
  //sets the Steps Per Rotation, in this case it is 64 * the 283712/4455 annoying ger ratio
  //for my motor (it works with float to be able to deal with these non-integer gear ratios)
  stepper.setSPR(96);
}

void loop()
{
  //when a command is finished it the isDone will return true, it is important to notice that
  //you can't give one command just after another since they don't block the execution, 
  //which allows the program to control multiple motors
  if (stepper.isDone() &&  rotate1 == false)
  {
    //this method sets the direction of rotation, has 3 allowed values (CW, CCW, and STOP) 
    //clockwise and counterclockwise for the first 2
    stepper.setDirection(CW);
    //this method sets the motor to rotate a given number of times, if you don't specify it, 
    //the motor will rotate untilyou send another command or set the direction to STOP
    stepper.rotate(1);
    rotate1 = true;
  }
  if (stepper.isDone() && rotate1 == true && rotatedeg == false)
  {
    stepper.setDirection(CW);
    //this method makes the motor rotate a given number of steps. 
	//it rotates to the direction currently set
    stepper.rotateSteps(24);
    rotatedeg = true;
  }
  if (stepper.isDone() && rotatedeg == true && crotate == false)
  {
    stepper.setDirection(CCW);
    //this will rotate until you stop it with another comand or set the direction to STOP
    stepper.rotate();
    crotate = true;
  }
  //this is very important and must be placed in your loop, it is this that makes the motor steps
  //when necessary
  stepper.run();
}