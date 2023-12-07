// ******** INTERFACE Functions **********
/*
 * Functions here integrates all the 3 modules before this
 * This allows creation of certain patterns or outcomes
 * much easier without a messy code in the main file
 */

// calibrating len(s) motor
int calibrate(int type, const char *const string_table[], int upper_limit, int lower_limit, uint16_t color) {
  int pos_current;
  pos_current = type ? zoom_current : focus_current;
  caliMenu(string_table, pos_current, upper_limit, color);
  while(true) {
    caliMenu(string_table, pos_current, upper_limit, color, true);
    pos_current = getLeftRight_value(upper_limit, pos_current, lower_limit, 0);
    // Serial.print("Pos current: ");
    // Serial.println(pos_current);
    //set motor speed 0 as calibration use default speed not motor speed
    moveMotor(type, pos_current,0);
    int option = get_motor_calibration_update();

    //no home option for motor callibration
    if(option == 0){
      //placeholder
    }

    //set the value and exist
    else if(option == 1){
      break;
    }

  }
  return pos_current;
}

// callibrate pov
int chooseDist(int type, int count, const char *const string_table[], bool goBack, uint16_t color) {
  int pos_current, upper_limit;
  pos_current = type ? zoom_current : focus_current;
  upper_limit = type ? zoom_range : focus_range;
  moveMotorMenu(count, string_table, pos_current, upper_limit, color);
  while(true) {
    moveMotorMenu(count, string_table, pos_current, upper_limit, color);
    pos_current = getLeftRight_value(upper_limit, pos_current, 0, 0);
    // Serial.print("Pos current: ");
    // Serial.println(pos_current);
    //set motor speed 0 as calibration use default speed not motor speed
    moveMotor(type, pos_current,0);
    int option = get_motor_calibration_update();

    //no home option for motor callibration
    if(option == 0){
      //placeholder
    }

    //set the value and exist
    else if(option == 1){
      break;
    }

  }


  if (goBack) {
    // needs to go back to original spot
    Serial.println("needs to go back to original spot");
    delay(500);
    tft.setCursor(0, 59);
    tft.setTextColor(WHITE,BLACK);
    tft.println(F("Returning to         "));
    tft.setTextColor(RED,BLACK);
    tft.print(F("PREVIOUS "));
    tft.setTextColor(WHITE,BLACK);
    tft.println(F("location          "));
    for (int i=2; i<count; i++) {
      tft.println("                  ");
    }
    // returns to original spot
    //go back acceleration and speed could be faster
    moveMotor(type, type ? zoom_current : focus_current, 0);
  }
  
  return pos_current;
}

void goDist(int type, const char title[], int pos_desired, uint16_t color, float motor_time, float motor_div,bool goBack, bool lastSequence,bool showScreen) {
  // Serial.print("@goDist motor_time = ");
  // Serial.println(motor_time);
  
  int pos_current, upper_limit;
  pos_current = type ? zoom_current : focus_current;
  upper_limit = type ? zoom_range : focus_range;

  if(showScreen){
      // start sound
      play_sound_1();
      printMoveSteps(type, title, color, 0); 
  }

  //move motor depending on excess_option_set
  switch (excess_option_set){
    //pre, delay then move
    case 0:{
      Serial.println("Pre");
      Serial.print("(shutter_time - motor_time) / motor_div = ");
      Serial.print("remainder_time = ");
      Serial.println((shutter_time - motor_time) / motor_div);
      float remainder_time = (shutter_time - motor_time) / motor_div;
      if(camera_shutter_open == 0){
        open_Shutter();
        camera_shutter_open = 1;
      }
      //delay in ms
      delay(remainder_time * 1000);
      moveMotor(type, pos_desired, motor_time / motor_div);
      break;
    }
    //split
    case 1:{
      Serial.println("Split");
      Serial.print("shutter_time - motor_time = ");
      Serial.println((shutter_time - motor_time) / motor_div);
      float remainder_time = (shutter_time - motor_time) / motor_div;
      float front_remainder_time = remainder_time / 2;
      float back_remainder_time = remainder_time / 2;
      if(camera_shutter_open == 0){
        open_Shutter();
        camera_shutter_open = 1;
      }
      //delay in ms
      delay(front_remainder_time * 1000);
      moveMotor(type, pos_desired, motor_time / motor_div);
      //delay in ms
      delay(back_remainder_time * 1000);
      break;
    }
    //after, move then delay
    case 2 :{
      Serial.println("After");
      Serial.print("shutter_time - motor_time = ");
      Serial.println((shutter_time - motor_time) / motor_div);
      float remainder_time = (shutter_time - motor_time) / motor_div;
      if(camera_shutter_open == 0){
        open_Shutter();
        camera_shutter_open = 1;
      }
      moveMotor(type, pos_desired, motor_time / motor_div);
      //delay in ms
      delay(remainder_time * 1000);
      break;
    }
    default:{
      break;
    }
    
  }

  //zoom / focus current postion after moving to desired position
  if (type) { // ZOOM
    zoom_current = pos_desired;
  } 
  else { // FOCUS
    focus_current = pos_desired;
  }

  //end sound 
  if(lastSequence){    
    close_Shutter();
    if(camera_shutter_open == 1){
        play_sound_2();
        camera_shutter_open = 0;
      }
    updateScreen(100);
  }

  // returns to original spot, go back acceleration and speed could be faster
  if (goBack) {
    if(showScreen){
      printMoveSteps(type, title, color,1);
    }
    moveMotor(type, pos_current, 0);
    //zoom / focus current postion after moving to desired position
    if (type) { // ZOOM
      zoom_current = pos_current;
    } else { // FOCUS
      focus_current = pos_current;
    }
   updateScreen(0);
  }
}

void goMultiDist(const char title[], int zoom_desired, int focus_desired, uint16_t color, float motor_time, float motor_div ,bool goBack,bool lastSequence,bool showScreen) {
  // Serial.print("@goMultiDist motor_time = ");
  // Serial.println(motor_time);

  int prev_zoom, prev_focus;
  prev_zoom = zoom_current;
  prev_focus = focus_current;

  if(showScreen){
      //start sound
      play_sound_1();
      printMoveSteps(3, title, color, 0);
  }
  //Serial.println("Both moving to position");

  //move motor depending on excess_option_set
  switch (excess_option_set){
    //pre, delay then move
    case 0:{
      float remainder_time = (shutter_time - motor_time) / motor_div;
      if(camera_shutter_open == 0){
        open_Shutter();
        camera_shutter_open = 1;
      }
      //delay in ms
      delay(remainder_time * 1000);
      moveMultiMotor(zoom_desired, focus_desired, motor_time / motor_div);
      break;
    }
    //split
    case 1:{
      float remainder_time = (shutter_time - motor_time) / motor_div;
      float front_remainder_time = remainder_time / 2;
      float back_remainder_time = remainder_time / 2;
      if(camera_shutter_open == 0){
        open_Shutter();
        camera_shutter_open = 1;
      }
      //delay in ms
      delay(front_remainder_time * 1000);
      moveMultiMotor(zoom_desired, focus_desired, motor_time / motor_div);
      //delay in ms
      delay(back_remainder_time * 1000);
      break;
    }
    //after, move then delay
    case 2 :{
      float remainder_time = (shutter_time - motor_time) / motor_div;
      if(camera_shutter_open == 0){
        open_Shutter();
        camera_shutter_open = 1;
      }
      moveMultiMotor(zoom_desired, focus_desired, motor_time / motor_div);
      //delay in ms
      delay(remainder_time * 1000);
      break;
    }
    default:{
      break;
    }
    
  }

  //zoom / focus current postion after moving to desired position
  zoom_current = zoom_desired;
  focus_current = focus_desired;

  if(lastSequence){
    close_Shutter();
    if(camera_shutter_open == 1){
        play_sound_2();
        camera_shutter_open = 0;
      }
    //close shutter(); //nikonTime(1000);
    updateScreen(100);
  }

  // returns to original spot
  if (goBack) {
    if(showScreen){
      Serial.println("Both going back to position");
      printMoveSteps(3, title, color, 1);
    }
    moveMultiMotor(prev_zoom, prev_focus,0);
    //see this got any differentce .....
    zoom_current = prev_zoom;
    focus_current = prev_focus;  
    updateScreen();
  }

}