void hotbar(const char title[], int current, int max_range, int current_option, bool haveBack, int header, int footer, uint16_t color, bool updateBar) {
  if (!updateMenu) {
    return;
  }
  updateMenu = false;
  
  int divs = (tft.width()-30)/(float)max_range * abs(current);
  tft.setCursor(0, 0);
  int rect_y = 80;

  /*
  // add if needed
  if (header != 0) {
    if (!updateBar) {
      tft.setTextColor(AQUA);
      tft.print(F("Shutter Speed: "));
      tft.setTextColor(WHITE);
      tft.print(shutter_speed);
      switch (header) {
        case 1: {
          tft.setTextColor(AQUA);
          tft.print(F("Front Motor: "));
          tft.setTextColor(WHITE);
          tft.println(orientation ? "Zoom" : "Focus");
          tft.setTextColor(AQUA);
          tft.print(F("Rear Motor: "));
          tft.setTextColor(WHITE);
          tft.println(orientation ? "Focus" : "Zoom");
        }
        case 2: {
          tft.setTextColor(AQUA);
          tft.print(F("Focus Range: "));
          tft.setTextColor(WHITE);
          tft.println(focus_range);
          tft.setTextColor(AQUA);
          tft.print(F("Zoom Range: "));
          tft.setTextColor(WHITE);
          tft.println(zoom_range);
        }
      }
      tft.println(); 
    }
    rect_y =75;
  }
  */

  option_selected ? tft.setTextColor(WHITE,BLACK) : tft.setTextColor(BLACK,WHITE);
  if (title != NULL) {
    tft.setTextSize(1);
    tft.print(title);
  }
  //
  
  tft.setTextSize(2);
  tft.println();
  tft.setTextColor(WHITE);
  tft.setCursor(0, 50);
  tft.print(F("Max Range: "));
  tft.println(max_range);

  tft.setCursor(10, rect_y + 30);
  option_selected ? tft.setTextColor(BLACK,WHITE) : tft.setTextColor(WHITE,BLACK);
  if (haveBack) {
    //page has back button
    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    tft.setCursor(160, 25);
    //tft.println("Home - *");
    tft.println(F("[Home]"));
  }

  //comfirm buttom
  tft.setTextColor(WHITE,BLACK);
  tft.setTextSize(2);
  tft.setCursor(170, 200);
  //tft.println("Home - *");
  tft.println(F("[Set]"));

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
        tft.drawChar(2,110,LEFT_ARROW,WHITE,BLACK,2);
        tft.drawChar(17,110,RIGHT_ARROW,WHITE,BLACK,2);
        tft.setCursor(32,120);
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


int get_calibration_update() {
  int s = -1;
  //Go Home menu
  if (digitalRead(A_BUTTON) == LOW) {
    resetToHomeScreen();
    s = 0;
    option_selected = 0; //resets options
    updateMenu = true;
  }

  //go back camera_setting_screen
  if (digitalRead(Y_BUTTON) == LOW) {
  s = 1;
  option_selected = 0; //resets options
  tft.fillScreen(ST77XX_BLACK);//clear screen
  updateMenu = true;
  }
  return s;
}

/* --- Calibrate Screen ---
 String_table will determine if zoom/focus
*/
 void caliMenu(const char *const string_table[], int current_step, int max_steps, uint16_t color, bool updateBar) {
  if (!updateMenu) {
    return;
  }
  hotbar(NULL, current_step, max_steps, 0, false, false, 1, color, updateBar);
  tft.setTextSize(1);
  int i = 0;
  tft.setCursor(0,0);
  tft.setTextColor(color);
  tft.println(string_table[i++]);
  tft.setCursor(0, 59);
  tft.setTextColor(WHITE);
  tft.print(string_table[i++]);
  tft.setTextColor(DARKGREEN);
  tft.println(string_table[i++]);
  tft.setTextColor(WHITE);
  
  return;
}