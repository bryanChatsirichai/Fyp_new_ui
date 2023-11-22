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