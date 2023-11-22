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
  do {
    caliMenu(string_table, pos_current, upper_limit, color, true);
    //pos_current = getLeftRight(upper_limit, pos_current, lower_limit, 0);
    // Serial.print("Pos current: ");
    // Serial.println(pos_current);
    //set motor speed 0 as calibration use default speed not motor speed
    //moveMotor(type, pos_current,0);
  } while(digitalRead(A_BUTTON) != LOW);
  
  return pos_current;
}