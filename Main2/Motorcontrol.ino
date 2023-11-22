// ******** MOTOR CONTROL Functions **********
// MOTOR_STEPS 200 //
// RPM 1000
// #define FOCUS 0
// #define ZOOM 1
// MS_STEP 8 // default 1/8 microstep, 200*8 = 1600 microstepping
// CALI_ACCEL 400
// 1 step 1.8 degrees
/* 
 *  -- Calc RPM --
 *   
 * Calculate RPM by ratio
 * May not be needed since the speed
 * in AccelMotor is in steps/s
 */
float calcRPM(int ms_steps, float seconds) {
  // return (float)(steps * MS_STEP)/seconds;
  Serial.print("ms_steps");
  Serial.println(ms_steps);
  delay(500);
  return (float)(ms_steps)/seconds;
}

/*
 * Calculating the acceleration
 * - Requires some finetuning
 */
float calcAccel(int steps, float seconds) {
  Serial.print("steps");
  Serial.println(steps);
  delay(500);
  float max_speed = calcRPM(steps, seconds);
  return (float)max_speed / seconds;
}

/*
 * to MilliSeconds converter
 */
long toMS(float seconds) {
  return seconds * 1000;
}

/*
 * Move motor to desired location
 * - Given the position
 * - Move the motor to pos
 * - Return motor to pos
 * type = 0 [FOCUS], 1 [ZOOM]
 *  orientation 0 [Focus Front, Zoom Rear]
 *  orientation 1 [Zoom Front, Focus Rear]
 */

void moveMotor(int type, int pos_desired, float motor_time) {
  Serial.print("@moveMotor motor_time = ");
  Serial.println(motor_time);
  // zoom_range, focus_range, zoom_current, focus_current, 
  // zoom_min, focus_min, motor_time
  AccelStepper *stepper;
  int pos_current;

// reverse 1 motor movement to mirror 
  bool reverse = false;
  
  if (type) { // ZOOM
    // 0 = false  ,1 = true
    stepper = orientation ? &front_motor : &rear_motor;
    pos_current = zoom_current;
    // testing
    //reverse = orientation ? true : false;
    //
  } else { // FOCUS
    stepper = orientation ? &rear_motor : &front_motor;
    pos_current = focus_current;
  }
  
  int steps_to_move = (pos_desired - pos_current) * MS_STEP;

  Serial.print("Current Position: ");
  Serial.println(pos_current);
  Serial.print("pos_desired: ");
  Serial.println(pos_desired);
  Serial.print("Steps to move: ");
  Serial.println(steps_to_move);

  //camera movement base on motor time
  if (motor_time != 0) {
    Serial.print("steps_to_move");
    Serial.println(steps_to_move);
    delay(100);
    //stepper->setAcceleration(calcAccel(abs(steps_to_move), (float)motor_time));

    //try
    stepper->setMaxSpeed(steps_to_move / motor_time);
    stepper->setAcceleration(steps_to_move / motor_time);
  }
  else{
    //motor time pass in as 0 use default config
    //eg) for callibration, go back when done...
    //stepper->setMaxSpeed(RPM);
    stepper->setAcceleration(CALI_ACCEL);
    stepper->setMaxSpeed(RPM);
  }
  // if +ve, move clockwise
  // else -ve, move anti-clockwise
  //if (steps_to_move == 0) return;
  //stepper->moveTo((steps_to_move > 0) ? pos_desired * MS_STEP : -pos_desired * MS_STEP);

  if(stepper == &rear_motor){
    stepper->moveTo(pos_desired * MS_STEP);
  }
  else{
      stepper->moveTo(-pos_desired * MS_STEP);
  }


  
  //blocking statement
  //delay(shutter_spd);
  while (stepper->distanceToGo() != 0) {
    stepper->run();
    // if(shutter_spd != 0) {
    //   delay(toMS((float)shutter_spd/abs(steps_to_move)));
    // }
  }

  //zoom / focus current postion after moving to desired position
  // if (type) { // ZOOM
  //   zoom_current = pos_desired;
  // } else { // FOCUS
  //   focus_current = pos_desired;
  // }
}

void setAccel(int type, float accel) {
  AccelStepper *stepper;
  if (type) {
    stepper = orientation ? &front_motor : &rear_motor;
  } else {
    stepper = orientation ? &rear_motor : &front_motor;
  }

  stepper->setAcceleration(accel);
}

void setCurrentPos(int type, float value) {
  AccelStepper *stepper;
  if (type) {
    stepper = orientation ? &front_motor : &rear_motor;
  } else {
    stepper = orientation ? &rear_motor : &front_motor;
  }

  stepper->setCurrentPosition(value);
}

/*
 * MultiStepper methods
 * Functions available: moveTo & runSpeedToPosition
 * To not move a motor, put '-1' in the value
 */
void moveMultiMotor(int zoom_value, int focus_value, float motor_time) {
  Serial.print("@moveMultiMotor motor_time = ");
  Serial.println(motor_time);
  long rear_position;
  long front_position;
  long positions[2]; 
  
  // reverse 1 motor move ment to mirror suppose to be front but test on rear_motor till new pico arrive
  bool reverse = false;
  
  if (zoom_value == -1) {
    rear_position = orientation ? focus_value : zoom_current;
    front_position = orientation ? zoom_current : focus_value;
    // positions[0] = orientation ? focus_value : zoom_current;
    // positions[1] = orientation ? zoom_current : focus_value;
  } else if (focus_value == -1) {
    rear_position = orientation ? focus_current : zoom_value;
    front_position = orientation ? zoom_value : focus_current;
    // positions[0] = orientation ? focus_current : zoom_value;
    // positions[1] = orientation ? zoom_value : focus_current;
  } else if (focus_value == -1 && zoom_value == -1) {
    rear_position = orientation ? focus_current : zoom_current;
    front_position = orientation ? zoom_current : focus_current;
    // positions[0] = orientation ? focus_current : zoom_current;
    // positions[1] = orientation ? zoom_current : focus_current;
  } else {
    rear_position = orientation ? focus_value : zoom_value;
    front_position = orientation ? zoom_value : focus_value;
    // positions[0] = orientation ? focus_value : zoom_value;
    // positions[1] = orientation ? zoom_value : focus_value;
  }

  // Add the microstep
  // long focus_steps = abs(focus_value-focus_current) * MS_STEP;
  // long zoom_steps = abs(zoom_value-zoom_current) * MS_STEP;
  // long average_steps = (focus_steps+zoom_steps)/2;
  // positions[0] = orientation ? focus_steps  : zoom_steps;  
  // positions[1] = orientation ? zoom_steps  : focus_steps;  

  long focus_steps_to_move = (focus_value-focus_current) * MS_STEP;
  long zoom_steps_to_move = (zoom_value-zoom_current) * MS_STEP;


  //adjust speed accordingly
  //camera movement base on motor time
  if (motor_time != 0) {
    //try
    rear_motor.setSpeed(orientation ? (focus_steps_to_move / motor_time) : (zoom_steps_to_move / motor_time));
    rear_motor.setAcceleration(orientation ? (focus_steps_to_move / motor_time) : (zoom_steps_to_move / motor_time));
    front_motor.setSpeed(orientation ?  (zoom_steps_to_move / motor_time) :  (focus_steps_to_move / motor_time));
    front_motor.setAcceleration(orientation ?  (zoom_steps_to_move / motor_time) :  (focus_steps_to_move / motor_time));

  } else if (motor_time == 0) {
    //motor time pass in as 0 use default config
    //eg) for callibration, go back when done...
    rear_motor.setMaxSpeed(RPM);
    rear_motor.setAcceleration(CALI_ACCEL);
    front_motor.setMaxSpeed(RPM);
    front_motor.setAcceleration(CALI_ACCEL);
  }

  // rear_motor.moveTo(rear_position * MS_STEP);
  // front_motor.moveTo(front_position * MS_STEP);
  positions[0] = rear_position * MS_STEP;
  positions[1] = front_position * MS_STEP;
  // rear_motor.moveTo(positions[0]);
  // front_motor.moveTo(positions[1]);
  //steppers.moveTo(positions);

// reverse 1 motor move ment to mirror suppose to be front but test on rear_motor till new pico arrive
  rear_motor.moveTo(positions[0]);
  front_motor.moveTo(-positions[1]);

  while (rear_motor.distanceToGo() != 0 || front_motor.distanceToGo() != 0) {
    rear_motor.run();
    front_motor.run();
    // if (shutter_time != 0) {
    //   delay(toMS((float)shutter_time/average_steps));
    // }
  } 
  
  //steppers.runSpeedToPosition(); // Blocks until all are in position

  // if (zoom_value == -1) {
  //   focus_current = focus_value;
  // } else if (focus_value == -1) {
  //   zoom_current = zoom_value;
  // } else {
  //   focus_current = focus_value;
  //   zoom_current = zoom_value;
  // }
  //steppers.runSpeedToPosition(); // Blocks until all are in position
  // Serial.print("front_motor.currentPosition ");
  // Serial.println(front_motor.currentPosition());
  // Serial.print("rear_motor.currentPosition ");
  // Serial.println(rear_motor.currentPosition());
  // Serial.print("focus_current ");
  // Serial.println(focus_current);
  // Serial.print("zoom_current ");
  // Serial.println(zoom_current);
  // delay(1000);
}
