void hotbar(const char title[], int current, int max_range, bool haveBack, int header, int footer, uint16_t color, bool updateBar) {
  if (!updateMenu) {
    return;
  }
  updateMenu = false;
  
  int divs = (tft.width()-30)/(float)max_range * abs(current);
  tft.setCursor(0, 0);
  int rect_y = 95;

  // title but for shutter-time and motor-time,  cali title settle in caliMenu
  if (title != NULL) {
    tft.setTextColor(DEEPPINK);
    tft.setTextSize(1);
    tft.print(title);
  }
  //
  // tft.setTextSize(2);
  // tft.println();
  // tft.setTextColor(WHITE);
  // tft.setCursor(0, 65);
  // tft.print(F("Max Range: "));
  // tft.println(max_range);

  if (haveBack) {
    //page has back button
    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    // tft.setCursor(140, 25);
    // tft.println(F("[Home]"));
    // tft.drawChar(tft.width()-160,25,RIGHT_ARROW,LIME,BLACK,2);
    tft.setCursor(132, 26);
    tft.print("Home");
    tft.drawRect(127,18,tft.width()-127,30,WHITE);
    tft.drawChar(tft.width()- symbol_size,26,RIGHT_ARROW,LIME,BLACK,2);
  }

  //comfirm buttom
  tft.setTextColor(WHITE,BLACK);
  tft.setTextSize(2);
  // tft.setCursor(170, 200);
  // tft.println(F("[Set]"));
  tft.setCursor(162, 201);
  tft.print("Set");
  tft.drawRect(157,193,tft.width()-157,30,WHITE);
  tft.drawChar(tft.width()- symbol_size,201,RIGHT_ARROW,LIME,BLACK,2);

  /* Draw Hotbar */
  tft.drawRect(0,rect_y,tft.width()-26,14,WHITE);
  tft.fillRect(2,rect_y+2,tft.width()-30,10,BLACK); // reset inner rectangle
  tft.fillRect(2,rect_y+2,divs,10,color);
  tft.setCursor(105,rect_y+3);
  tft.setTextColor(WHITE,BLACK);
  tft.print(current);
  tft.print(" ");

  if (!updateBar) {
    tft.setTextSize(1);
    switch(footer) {
      case 1:
        tft.drawChar(2,120,LEFT_ARROW,WHITE,BLACK,2);
        tft.drawChar(17,120,RIGHT_ARROW,WHITE,BLACK,2);
        tft.setCursor(32,130);
        tft.println(F(": Adjust values"));
        // tft.drawChar(2,105,SELECT,WHITE,BLACK,2);
        // tft.setCursor(17,109);
        // tft.println(F(": Press to set"));
        break;
      case 2:
        // tft.drawChar(2,80,UP_ARROW,WHITE,BLACK,2);
        // tft.drawChar(17,80,DOWN_ARROW,WHITE,BLACK,2);
        // tft.setCursor(32,84);
        // tft.println(F(": Navigate"));
        // tft.drawChar(2,95,SELECT,WHITE,BLACK,2);
        // tft.setCursor(17,99);
        // tft.println(F(": Press to set"));
        break;
      case 3:
        //shutter speed footer, bottom of screen
        // tft.drawChar(2,85,LEFT_ARROW,WHITE,BLACK,2);
        // tft.drawChar(17,85,RIGHT_ARROW,WHITE,BLACK,2);
        // tft.setCursor(32,90);
        // tft.println(F(": Adjust values-"));
        // tft.drawChar(2,100,UP_ARROW,WHITE,BLACK,2);
        // tft.drawChar(17,100,DOWN_ARROW,WHITE,BLACK,2);
        // tft.setCursor(32,104);
        // tft.println(F(": Navigate"));
        // tft.drawChar(2,115,SELECT,WHITE,BLACK,2);
        // tft.setCursor(17,119);
        // tft.println(F(": Press to set"));
        break;
      default:
      break;
    }
  }
  return;
}

/*
 * checks for left and right joystick
 * able to set to lower limit if required
 */
int getLeftRight_value(int range, int current, int low_limit, int delay_ms) {
  if (digitalRead(RIGHT_BUTTON) == LOW) {
    delay(delay_ms);
    updateMenu = true;
    return (current == range) ? current : ++current; 
  }
  if (digitalRead(LEFT_BUTTON) == LOW) {
    delay(delay_ms);
    updateMenu = true;
    return (current == low_limit) ? low_limit : --current;
  }
  return current;
}

int get_camera_calibration_update() {
  int s = -1;
  //Go Home menu
  if (digitalRead(A_BUTTON) == LOW) {
    resetToHomeScreen();
    s = 0;
    updateMenu = true;
  }

  //go back camera_setting_screen
  if (digitalRead(Y_BUTTON) == LOW) {
  s = 1;
  tft.fillScreen(ST77XX_BLACK);//clear screen
  updateMenu = true;
  }
  return s;
}

int get_motor_calibration_update() {
  int s = -1;
  //go back camera_setting_screen
  if (digitalRead(Y_BUTTON) == LOW) {
  s = 1;
  tft.fillScreen(ST77XX_BLACK);//clear screen
  updateMenu = true;
  }
  return s;
}

/* --- Motor Calibrate Screen ---
*/
 void caliMenu(const char *const string_table[], int current_step, int max_steps, uint16_t color, bool updateBar) {
  if (!updateMenu) {
    return;
  }
  hotbar(NULL, current_step, max_steps, false, false, 1, color, updateBar);

  
  tft.setTextSize(1);
  int i = 0;

  //title
  tft.setCursor(0,0);
  tft.setTextColor(color);
  tft.println(string_table[i++]);

  //just above the hot bar to show setting initial or maximum
  tft.setCursor(2,60);
  tft.setTextColor(AQUA);
  tft.println(string_table[i++]);

  //can add more text in needed... (show the text below the hotbar)              
  tft.setCursor(32,140);
  //tft.setCursor(0, 59);
  tft.setTextColor(WHITE);
  tft.print(string_table[i++]);
  tft.setTextColor(DARKGREEN);
  tft.println(string_table[i++]);
  tft.setTextColor(WHITE);
  return;
}

/* --- Move Motor Screen ---
 * Similar to cali Menu 
 */
void moveMotorMenu(int count, const char *const string_table[], int current_step, int max_steps, uint16_t color, bool updateBar) {
  if (!updateMenu) return;
  int i=0;
  hotbar(NULL, current_step, max_steps, false, 0, 1, color, updateBar);
  tft.setTextSize(1);
  tft.setCursor(0,0);
  tft.setTextColor(color);
  tft.println(string_table[i]);
  tft.setTextColor(WHITE);
  //tft.setCursor(32,140);
  for (i=1; i<count; i++) {
    tft.setCursor(32,130 + i*10);
    tft.println(string_table[i]);
  }
  tft.setTextColor(WHITE);
}