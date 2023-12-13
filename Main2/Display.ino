 //Show ICM APP 'initilizing page'
void initializing_Page(){   
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    tft.setCursor(60,100);
    tft.print(F("ICM APP"));
    updateScreen();
}

//Go Home mainmenu
void resetToHomeScreen(){
  tft.fillScreen(ST77XX_BLACK);//clear screen
  home_screen = -1;
  configuration_screen = -1;
  camera_setting_screen = -1;
  camera_positioning_screen = -1;
  motor_calibration_screen1 = -1;
  //motor_calibration_screen2 = -1;
  excess_option_screen = -1;
  action_screen_1 = -1;
  zoom_movements_menu1 = -1;
  zoom_movements_menu2 = -1;
  focus_movements_menu1 = -1;
  focus_movements_menu2 = -1;
  zoom_focus_movements_menu1 = -1;
  zoom_focus_movements_menu2 = -1;
  zoom_focus_movements_menu3 = -1;
  fixed_paterns_menu1 = -1;
  fixed_paterns_menu2 = -1;
}

/* Reset Screen */
int resetScreen(int s) {
  tft.fillScreen(ST77XX_BLACK);//clear screen
  s = -1;
  return s;
}

/* Update Screen */
void updateScreen(float delay_ms) {
  delay(delay_ms);
  tft.fillScreen(ST77XX_BLACK);//clear screen
}

/* A simple way to print a countdown menu */
void countdownMenu() {
  int i=0;
  tft.setTextSize(3);
  tft.setCursor(0,0);
  tft.println(countdown[i]);
  delay(500);
  for (i=1; i<4; i++) {
    tft.setTextSize(4);
    tft.setCursor(0,30);
    tft.setTextColor(RED,BLACK);
    tft.println(countdown[i]);
    delay(1000);
  }
  tft.setTextSize(3);
  tft.println(countdown[i]);
  
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.print("Exposing camera \nfor: ");
  tft.setTextSize(2);
  tft.setTextColor(AQUA);
  
  tft.setTextColor(AQUA,BLACK);
  tft.setCursor(50,105);
  tft.print(shutter_time);
  tft.print("s  ");
  delay(1000);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  updateScreen();
}

void printMoveSteps(int type, const char title[], uint16_t color, int movement_display_option) {
  tft.setTextSize(1);
  tft.setCursor(0,0);
  tft.setTextColor(AQUA);
  tft.print(F("Shutter Time: "));
  tft.setTextColor(WHITE);
  tft.println(shutter_time);
  tft.setTextColor(AQUA);
  tft.print(F("Motor Time: "));
  tft.setTextColor(WHITE);
  tft.println(motor_time);
  if(type == -1){
      //nothing
  }
  else {
    //type
    tft.setTextColor(AQUA, BLACK);
    tft.print(type ? "Zoom" : "Focus");
    tft.print(F(" Range: "));
    tft.setTextColor(WHITE, BLACK);
    tft.println(type ? zoom_range : focus_range);
  }
  tft.println();

  tft.print(title);
  tft.drawLine(0, 75, tft.width(), 75,WHITE);
  tft.setCursor(0, 85);
  tft.setTextColor(WHITE);
  
  switch(movement_display_option) {
    case 0: {
      tft.println(F("EXECUTING "));
      tft.setTextColor(LIGHTSKYBLUE, BLACK);
      tft.print(F("DESIRED "));
      tft.setTextColor(WHITE, BLACK);
      tft.println(F("PATTERN"));
      break;
    }
    case 1: {
      tft.println(F("Returning to "));
      tft.setTextColor(RED, BLACK);
      tft.print(F("PREVIOUS "));
      tft.setTextColor(WHITE, BLACK);
      tft.println(F("Location"));
      break;
    }
    case 2: {
      tft.println(F("Moving to "));
      tft.setTextColor(LIME, BLACK);
      tft.print(F("STARTING "));
      tft.setTextColor(WHITE, BLACK);
      tft.println(F("position"));
      break;
    }
    default: 
      break;
  }
}


// ******** DISPLAY pages Functions **********
void home_menu_screen(int array_size,const char *menu_name ,const char *const string_table[], uint16_t color) {
  int total_num = array_size;

  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.println(menu_name);
  //home-fullbar
  tft.setTextColor(WHITE,BLACK);
  tft.setTextSize(2);
  tft.setCursor(2, 23);
  tft.print("Home");
  tft.drawRect(0,12,tft.width(),35,WHITE);
  tft.drawChar(tft.width()- symbol_size,25,RIGHT_ARROW,LIME,BLACK,2);
  
  //other options
  int rect_y = 67;
  for (int i=0; i<total_num; i++) {
    //tft.drawRect(0,rect_y,tft.width()-symbol_padding,35,WHITE);
    tft.drawRect(0,rect_y,tft.width(),35,WHITE);
    tft.setCursor(2,rect_y+10);
    tft.setTextColor(WHITE,BLACK);
    tft.print(string_table[i]);

    //symbol
    tft.drawChar(tft.width() - symbol_size,rect_y+10,RIGHT_ARROW,LIME,BLACK,2);
    rect_y = rect_y+55;
  }
  tft.setTextColor(WHITE,BLACK); 
}

void configuration_menu_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color){
  int total_num = array_size;

  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.println(menu_name);
  //home-fullbar
  tft.setTextColor(WHITE,BLACK);
  tft.setTextSize(2);
  tft.setCursor(2, 23);
  tft.print("Home");
  tft.drawRect(0,12,tft.width(),35,WHITE);
  tft.drawChar(tft.width()- symbol_size,25,RIGHT_ARROW,LIME,BLACK,2);
  
  //other options
  int rect_y = 67;
  for (int i=0; i<total_num; i++) {
    //tft.drawRect(0,rect_y,tft.width()-symbol_padding,35,WHITE);
    tft.drawRect(0,rect_y,tft.width(),35,WHITE);
    tft.setCursor(2,rect_y+10);

    tft.setTextColor(WHITE,BLACK);
    tft.print(string_table[i]);

    //symbol
    tft.drawChar(tft.width() - symbol_size,rect_y+10,RIGHT_ARROW,LIME,BLACK,2);

    rect_y = rect_y+55;
  }
  tft.setTextColor(WHITE,BLACK);
}

void cameraSetting_menu_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color){
  int total_num = array_size;

  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.println(menu_name);

  tft.setCursor(0, 15);
  tft.setTextColor(AQUA);
  tft.print(F("Shutter Time: "));
  tft.setTextColor(WHITE);
  tft.print(shutter_time);
  tft.setCursor(0,25);
  tft.setTextColor(AQUA);
  tft.print(F("Motor Time: "));
  tft.setTextColor(WHITE);
  tft.println(motor_time);

  tft.setCursor(0,35);
  tft.setTextColor(AQUA);
  tft.print(F("Excess: "));
  tft.setTextColor(WHITE);
  switch (excess_option_set) {
    //pre
    case 0:{
      tft.print("Pre");
      break;
    }
    //split
    case 1:{
      tft.print("Split");
      break;
    }
    //after
    case 2:{
      tft.print("After");
      break;
    }
  }
  //home-halfbar
  tft.setTextColor(WHITE,BLACK);
  tft.setTextSize(2);
  tft.setCursor(132, 25);
  tft.print("Home");
  tft.drawRect(127,18,tft.width()-127,30,WHITE);
  tft.drawChar(tft.width()- symbol_size,25,RIGHT_ARROW,LIME,BLACK,2);

  //other options
  int rect_y = 67;
  for (int i=0; i<total_num; i++) {
    tft.drawRect(0,rect_y,tft.width(),35,WHITE);
    tft.setCursor(2,rect_y+10);

    tft.setTextColor(WHITE,BLACK);
    tft.print(string_table[i]);

    //symbol
    tft.drawChar(tft.width() - symbol_size,rect_y+10,RIGHT_ARROW,LIME,BLACK,2);
    rect_y = rect_y+55;
  }
  tft.setTextColor(WHITE,BLACK);
}

void positioning_menu_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color){
    int total_num = array_size;

    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);
    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    tft.setCursor(130, 25);
    //tft.println("Home - *");
    //symbol
    tft.print("Home");
    tft.drawChar(tft.width()- symbol_size,25,RIGHT_ARROW,LIME,BLACK,2);
    int rect_y = 67;

    for (int i=0; i<total_num; i++) {
      //tft.drawRect(0,rect_y,tft.width()-symbol_padding,35,WHITE);
      tft.drawRect(0,rect_y,tft.width(),35,WHITE);
      tft.setCursor(2,rect_y+10);

      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      
      //symbol
      tft.drawChar(tft.width() - symbol_size,rect_y+10,RIGHT_ARROW,LIME,BLACK,2);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);
}

void motor_calibration_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color){
    int total_num = array_size;

    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    tft.setCursor(0,15);
    tft.setTextColor(AQUA);
    tft.print(F("Front: "));
    tft.setTextColor(WHITE,BLACK);
    tft.print(orientation ? "Zoom " : "Focus");
    tft.setCursor(0,25);
    tft.setTextColor(AQUA);
    tft.print(F("Rear: "));
    tft.setTextColor(WHITE,BLACK);
    tft.print(orientation ? "Focus" : "Zoom ");

    tft.setCursor(0, 35);
    tft.setTextColor(AQUA);
    tft.print(F("Z_Range: "));
    tft.setTextColor(WHITE);
    tft.print(zoom_range);  
    tft.setCursor(0,45);
    tft.setTextColor(AQUA);
    tft.print(F("F_Range: "));
    tft.setTextColor(WHITE);
    tft.println(focus_range);


    tft.setCursor(0, 55);
    tft.setTextColor(AQUA);
    tft.print(F("Z_Current: "));
    tft.setTextColor(WHITE);
    tft.print(zoom_current);
    tft.setCursor(0,65);
    tft.setTextColor(AQUA);
    tft.print(F("F_Current: "));
    tft.setTextColor(WHITE);
    tft.println(focus_current);  

    //old version 
    //joystick icon only show arrows if can press them to go next page
    //tft.drawChar(12,60,UP_ARROW,WHITE,BLACK,2);
    // tft.drawChar(12,100,SELECT,WHITE,BLACK,2);
    // tft.setCursor(30, 100);
    // tft.println("- Home");
    //tft.drawChar(12,140,DOWN_ARROW,WHITE,BLACK,2);
    // tft.setCursor(30, 140);
    // tft.println("- Next");
    // tft.setTextColor(WHITE,BLACK);
    // tft.setTextSize(2);
    // int rect_y = 12;
    // for (int i=0; i<total_num; i++) {
    //   tft.drawRect(90,rect_y,tft.width()-90,35,WHITE);
    //   tft.setCursor(97,rect_y+10);
    //   tft.setTextColor(WHITE,BLACK);
    //   tft.print(string_table[i]);
    //   rect_y = rect_y+55;
    // }

    //home-halfbar
    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    tft.setCursor(132, 25);
    tft.print("Home");
    tft.drawRect(127,18,tft.width()-127,30,WHITE);
    tft.drawChar(tft.width()- symbol_size,25,RIGHT_ARROW,LIME,BLACK,2);
    

    //other options
    int rect_y = 67;
    for (int i=0; i<total_num; i++) {
      tft.drawRect(100,rect_y,tft.width()-100,35,WHITE);

      tft.setCursor(105,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);

      //symbol
      tft.drawChar(tft.width() - symbol_size,rect_y+10,RIGHT_ARROW,LIME,BLACK,2);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);

    //instruction(s)
    tft.setTextColor(AQUA);
    tft.setTextSize(1);
    tft.setCursor(0, 180);
    //tft.setTextColor(WHITE,BLACK);
    tft.println("Calibrate");
    tft.println("Motor(s)");
    tft.println("before");
    tft.print("setting pov.");

}

void options_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color){
    int total_num = array_size;

    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    tft.setCursor(0,15);
    tft.setTextColor(AQUA);
    tft.print(F("Excess:"));
    tft.setTextColor(WHITE);
    switch (excess_option_set) {
      //pre
      case 0:{
        tft.print("Pre");
        break;
      }
      //split
      case 1:{
        tft.print("Split");
        break;
      }
      //after
      case 2:{
        tft.print("After");
        break;
      }
    }

    tft.setCursor(0,25);
    tft.setTextColor(AQUA);
    tft.print(F("Front: "));
    tft.setTextColor(WHITE,BLACK);
    tft.print(orientation ? "Zoom " : "Focus");
    tft.setCursor(0,35);
    tft.setTextColor(AQUA);
    tft.print(F("Rear: "));
    tft.setTextColor(WHITE,BLACK);
    tft.print(orientation ? "Focus" : "Zoom ");

    tft.setCursor(0,45);
    tft.setTextColor(AQUA);
    tft.print(F("Shutter(s): "));
    tft.setTextColor(WHITE);
    tft.println(shutter_time);
    tft.setCursor(0,55);
    tft.setTextColor(AQUA);
    tft.print(F("Motor(s): "));
    tft.setTextColor(WHITE);
    tft.println(motor_time);

    tft.setCursor(0, 65);
    tft.setTextColor(AQUA);
    tft.print(F("Z_Range: "));
    tft.setTextColor(WHITE);
    tft.print(zoom_range);
    tft.setCursor(0,75);
    tft.setTextColor(AQUA);
    tft.print(F("F_Range: "));
    tft.setTextColor(WHITE);
    tft.println(focus_range);


    //joystick icon only show arrows if can press them to go next/prev page
    //box to seaperate controls (may need edit dimensionsin future depending on headers if needed)
    //tft.drawRect(2,50,60,122,WHITE); //(rest of the boxes use this)
    tft.drawRect(2,90,60,82,WHITE);
    //tft.drawChar(8,60,UP_ARROW,WHITE,BLACK,2);
    //tft.setCursor(26, 60);
    //tft.println("-Back");
    tft.drawChar(8,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(26, 100);
    tft.println("-Home");
    //tft.drawChar(8,140,DOWN_ARROW,WHITE,BLACK,2);
    //tft.setCursor(26, 140);
    //tft.println("-Next");

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      tft.drawRect(90,rect_y,tft.width()-90,35,WHITE);
      tft.setCursor(97,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);

  }






void excess_menu_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color){
    int total_num = array_size;

    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);
    tft.setTextColor(WHITE,BLACK);
    //home-fullbar
    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    tft.setCursor(2, 23);
    tft.print("Home");
    tft.drawRect(0,12,tft.width(),35,WHITE);
    tft.drawChar(tft.width()- symbol_size,25,RIGHT_ARROW,LIME,BLACK,2);

    //other options
    tft.drawChar(tft.width()- symbol_size,25,RIGHT_ARROW,LIME,BLACK,2);
    int rect_y = 67;
    for (int i=0; i<total_num; i++) {
      //tft.drawRect(0,rect_y,tft.width()-symbol_padding,35,WHITE);
      tft.drawRect(0,rect_y,tft.width(),35,WHITE);
      tft.setCursor(2,rect_y+10);

      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      
      //symbol
      tft.drawChar(tft.width() - symbol_size,rect_y+10,RIGHT_ARROW,LIME,BLACK,2);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);

}

void action_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color){
    int total_num = array_size;

    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);
    //home-fullbar
    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    tft.setCursor(2, 23);
    tft.print("Home");
    tft.drawRect(0,12,tft.width(),35,WHITE);
    tft.drawChar(tft.width()- symbol_size,25,RIGHT_ARROW,LIME,BLACK,2);
    
    //other options
    int rect_y = 67;
    for (int i=0; i<total_num; i++) {
      //tft.drawRect(0,rect_y,tft.width()-symbol_padding,35,WHITE);
      tft.drawRect(0,rect_y,tft.width(),35,WHITE);
      tft.setCursor(2,rect_y+10);

      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      
      //symbol
      tft.drawChar(tft.width() - symbol_size,rect_y+10,RIGHT_ARROW,LIME,BLACK,2);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);

}

void zoom_movements_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color){
    int total_num = array_size;

    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    //joystick icon only show arrows if can press them to go next/prev page
    //box to seaperate controls
    tft.drawRect(2,50,60,122,WHITE);
    //tft.drawChar(8,60,UP_ARROW,WHITE,BLACK,2);
    //tft.setCursor(26, 60);
    //tft.println("-Back");
    tft.drawChar(8,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(26, 100);
    tft.println("-Home");
    tft.drawChar(8,140,DOWN_ARROW,WHITE,BLACK,2);
    tft.setCursor(26, 140);
    tft.println("-Next");

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      tft.drawRect(75,rect_y,tft.width()-75,35,WHITE);      
      tft.setCursor(82,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);

      //symbol
      tft.drawChar(tft.width() - symbol_size,rect_y+10,RIGHT_ARROW,LIME,BLACK,2);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);

}

void zoom_movements_menu2_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color){
    int total_num = array_size;

    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    //joystick icon only show arrows if can press them to go next/prev page
    //box to seaperate controls
    tft.drawRect(2,50,60,122,WHITE);
    tft.drawChar(8,60,UP_ARROW,WHITE,BLACK,2);
    tft.setCursor(26, 60);
    tft.println("-Back");
    tft.drawChar(8,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(26, 100);
    tft.println("-Home");
    //tft.drawChar(8,140,DOWN_ARROW,WHITE,BLACK,2);
    //tft.setCursor(26, 140);
    //tft.println("-Next");

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      tft.drawRect(75,rect_y,tft.width()-75,35,WHITE);
      tft.setCursor(82,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);

      //symbol
      tft.drawChar(tft.width() - symbol_size,rect_y+10,RIGHT_ARROW,LIME,BLACK,2);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);

}

void focus_movements_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color){
    int total_num = array_size;

    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    //joystick icon only show arrows if can press them to go next/prev page
    //box to seaperate controls
    tft.drawRect(2,50,60,122,WHITE);
    //tft.drawChar(8,60,UP_ARROW,WHITE,BLACK,2);
    //tft.setCursor(26, 60);
    //tft.println("-Back");
    tft.drawChar(8,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(26, 100);
    tft.println("-Home");
    tft.drawChar(8,140,DOWN_ARROW,WHITE,BLACK,2);
    tft.setCursor(26, 140);
    tft.println("-Next");

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      tft.drawRect(75,rect_y,tft.width()-75,35,WHITE);
      tft.setCursor(82,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      
      //symbol
      tft.drawChar(tft.width() - symbol_size,rect_y+10,RIGHT_ARROW,LIME,BLACK,2);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);

}

void focus_movements_menu2_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color){
    int total_num = array_size;

    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    //joystick icon only show arrows if can press them to go next/prev page
    //box to seaperate controls
    tft.drawRect(2,50,60,122,WHITE);
    tft.drawChar(8,60,UP_ARROW,WHITE,BLACK,2);
    tft.setCursor(26, 60);
    tft.println("-Back");
    tft.drawChar(8,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(26, 100);
    tft.println("-Home");
    //tft.drawChar(8,140,DOWN_ARROW,WHITE,BLACK,2);
    //tft.setCursor(26, 140);
    //tft.println("-Next");

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      tft.drawRect(75,rect_y,tft.width()-75,35,WHITE);
      tft.setCursor(82,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      //symbol
      tft.drawChar(tft.width() - symbol_size,rect_y+10,RIGHT_ARROW,LIME,BLACK,2);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);

}

void zoomfocus_movements_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color){
    int total_num = array_size;

    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    //joystick icon only show arrows if can press them to go next/prev page
    //box to seaperate controls
    tft.drawRect(2,50,60,122,WHITE);
    //tft.drawChar(8,60,UP_ARROW,WHITE,BLACK,2);
    //tft.setCursor(26, 60);
    //tft.println("-Back");
    tft.drawChar(8,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(26, 100);
    tft.println("-Home");
    tft.drawChar(8,140,DOWN_ARROW,WHITE,BLACK,2);
    tft.setCursor(26, 140);
    tft.println("-Next");

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      tft.drawRect(75,rect_y,tft.width()-75,35,WHITE);
      tft.setCursor(82,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      
      //symbol
      tft.drawChar(tft.width() - symbol_size,rect_y+10,RIGHT_ARROW,LIME,BLACK,2);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);

}

void zoomfocus_movements_menu2_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color){
    int total_num = array_size;

    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    //joystick icon only show arrows if can press them to go next/prev page
    //box to seaperate controls
    tft.drawRect(2,50,60,122,WHITE);
    tft.drawChar(8,60,UP_ARROW,WHITE,BLACK,2);
    tft.setCursor(26, 60);
    tft.println("-Back");
    tft.drawChar(8,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(26, 100);
    tft.println("-Home");
    tft.drawChar(8,140,DOWN_ARROW,WHITE,BLACK,2);
    tft.setCursor(26, 140);
    tft.println("-Next");

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      tft.drawRect(75,rect_y,tft.width()-75,35,WHITE);
      tft.setCursor(82,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      //symbol
      tft.drawChar(tft.width() - symbol_size,rect_y+10,RIGHT_ARROW,LIME,BLACK,2);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);

}

void zoomfocus_movements_menu3_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color){
    int total_num = array_size;

    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    //joystick icon only show arrows if can press them to go next/prev page
    //box to seaperate controls
    tft.drawRect(2,50,60,122,WHITE);
    tft.drawChar(8,60,UP_ARROW,WHITE,BLACK,2);
    tft.setCursor(26, 60);
    tft.println("-Back");
    tft.drawChar(8,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(26, 100);
    tft.println("-Home");
    //tft.drawChar(8,140,DOWN_ARROW,WHITE,BLACK,2);
    //tft.setCursor(26, 140);
    //tft.println("-Next");

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      tft.drawRect(75,rect_y,tft.width()-75,35,WHITE);
      tft.setCursor(82,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      //symbol
      tft.drawChar(tft.width() - symbol_size,rect_y+10,RIGHT_ARROW,LIME,BLACK,2);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);

}

void custome_movements_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color){
    int total_num = array_size;
    int max_option = array_size;

    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    //joystick icon only show arrows if can press them to go next page
    //box to seaperate controls
    tft.drawRect(2,50,60,122,WHITE);
    //tft.drawChar(8,60,UP_ARROW,WHITE,BLACK,2);
    //tft.setCursor(26, 60);
    //tft.println("-Back");
    tft.drawChar(8,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(26, 100);
    tft.println("-Home");
    tft.drawChar(8,140,DOWN_ARROW,WHITE,BLACK,2);
    tft.setCursor(26, 140);
    tft.println("-Next");

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      //tft.drawRect(75,rect_y,tft.width()-symbol_padding,35,WHITE);
      tft.drawRect(75,rect_y,tft.width()-75,35,WHITE);
      tft.setCursor(82,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      //symbol
      tft.drawChar(tft.width() - symbol_size,rect_y+10,RIGHT_ARROW,LIME,BLACK,2);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);

}

void custome_movements_menu2_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color){
    int total_num = array_size;
    int max_option = array_size;

    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    //joystick icon only show arrows if can press them to go next/prev page
    //box to seaperate controls
    tft.drawRect(2,50,60,122,WHITE);
    tft.drawChar(8,60,UP_ARROW,WHITE,BLACK,2);
    tft.setCursor(26, 60);
    tft.println("-Back");
    tft.drawChar(8,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(26, 100);
    tft.println("-Home");
    //tft.drawChar(8,140,DOWN_ARROW,WHITE,BLACK,2);
    //tft.setCursor(26, 140);
    //tft.println("-Next");

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      //tft.drawRect(75,rect_y,tft.width()-symbol_padding,35,WHITE);
      tft.drawRect(75,rect_y,tft.width()-75,35,WHITE);
      tft.setCursor(82,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      //symbol
      tft.drawChar(tft.width() - symbol_size,rect_y+10,RIGHT_ARROW,LIME,BLACK,2);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);

}



