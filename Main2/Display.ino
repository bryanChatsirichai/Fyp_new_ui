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
  action_screen_1 = -1;
  zoom_movements_menu1 = -1;
  zoom_movements_menu1 = -1;
  focus_movements_menu1 = -1;
  focus_movements_menu2 = -1;
  zoom_focus_movements_menu1 = -1;
  zoom_focus_movements_menu2 = -1;
  zoom_focus_movements_menu3 = -1;
  
}

/* Reset Screen */
int resetScreen(int s) {
  tft.fillScreen(ST77XX_BLACK);//clear screen
  s = -1;
  updateMenu = true;
  option_selected = 0;
  return s;
}

/* Update Screen */
void updateScreen(float delay_ms) {
  delay(delay_ms);
  tft.fillScreen(ST77XX_BLACK);//clear screen
  updateMenu = true;
}

// ******** DISPLAY Functions **********

int home_menu_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color) {
  int total_num = array_size;
  int max_option = array_size;
  if (!updateMenu) {
    //updateMenu false
    return total_num; 
  }
  updateMenu = false;
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.println(menu_name);
  tft.setTextColor(WHITE,BLACK);
  tft.setTextSize(2);
  tft.setCursor(130, 25);
  tft.println("Home - *");
  int rect_y = 67;

  for (int i=0; i<total_num; i++) {
    tft.drawRect(0,rect_y,tft.width(),35,WHITE);
    tft.setCursor(2,rect_y+10);
    tft.setTextColor(WHITE,BLACK);
    tft.print(string_table[i]);
    rect_y = rect_y+55;
  }
  tft.setTextColor(WHITE,BLACK);

  //maybe don't need  
  return total_num; 
}

int configuration_menu_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color){
  int total_num = array_size;
  int max_option = array_size;
  if (!updateMenu) {
    //updateMenu false
    return total_num; 
  }
  updateMenu = false;
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.println(menu_name);
  tft.setTextColor(WHITE,BLACK);
  tft.setTextSize(2);
  tft.setCursor(130, 25);
  tft.println("Home - *");
  int rect_y = 67;

  for (int i=0; i<total_num; i++) {
    tft.drawRect(0,rect_y,tft.width(),35,WHITE);
    tft.setCursor(2,rect_y+10);

    tft.setTextColor(WHITE,BLACK);
    tft.print(string_table[i]);
    rect_y = rect_y+55;
  }
  tft.setTextColor(WHITE,BLACK);
  //maybe don't need  
  return total_num; 
}

int cameraSetting_menu_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color){
  int total_num = array_size;
  int max_option = array_size;
  if (!updateMenu) {
    //updateMenu false
    return total_num; 
  }
  updateMenu = false;
  
  
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
  tft.print(F("Front Motor: "));
  tft.setTextColor(WHITE,BLACK);
  tft.print(orientation ? "Zoom " : "Focus");
  tft.setCursor(0,45);
  tft.setTextColor(AQUA);
  tft.print(F("Rear Motor: "));
  tft.setTextColor(WHITE,BLACK);
  tft.print(orientation ? "Focus" : "Zoom ");

  tft.setTextColor(WHITE,BLACK);
  tft.setTextSize(2);
  tft.setCursor(130, 25);
  tft.println("Home - *");
  int rect_y = 67;

  for (int i=0; i<total_num; i++) {
    tft.drawRect(0,rect_y,tft.width(),35,WHITE);
    tft.setCursor(2,rect_y+10);

    tft.setTextColor(WHITE,BLACK);
    tft.print(string_table[i]);
    rect_y = rect_y+55;
  }
  tft.setTextColor(WHITE,BLACK);
  //maybe don't need  
  return total_num; 
}

int action_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color){
    int total_num = array_size;
    int max_option = array_size;
    if (!updateMenu) {
      //updateMenu false
      return total_num; 
    }
    updateMenu = false;
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);
    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    tft.setCursor(130, 25);
    tft.println("Home - *");
    int rect_y = 67;

    for (int i=0; i<total_num; i++) {
      tft.drawRect(0,rect_y,tft.width(),35,WHITE);
      tft.setCursor(2,rect_y+10);

      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);
    //maybe don't need  
    return total_num; 
}

int zoom_movements_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color){
    int total_num = array_size;
    int max_option = array_size;
    if (!updateMenu) {
      //updateMenu false
      return total_num; 
    }
    updateMenu = false;
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    //joystick icon only show arrows if can press them to go next page
    // tft.drawChar(12,60,UP_ARROW,WHITE,BLACK,2);
    tft.drawChar(12,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(30, 100);
    tft.println("- Home");
    tft.drawChar(12,140,DOWN_ARROW,WHITE,BLACK,2);
    tft.setCursor(30, 140);
    tft.println("- Next");

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      tft.drawRect(75,rect_y,tft.width()-75,35,WHITE);
      tft.setCursor(82,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);
    //maybe don't need  
    return total_num; 
}

int zoom_movements_menu2_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color){
    int total_num = array_size;
    int max_option = array_size;
    if (!updateMenu) {
      //updateMenu false
      return total_num; 
    }
    updateMenu = false;
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    //joystick icon only show arrows if can press them to go next page
    tft.drawChar(12,60,UP_ARROW,WHITE,BLACK,2);
    tft.setCursor(30, 60);
    tft.println("- Back");
    tft.drawChar(12,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(30, 100);
    tft.println("- Home");
    //tft.drawChar(12,140,DOWN_ARROW,WHITE,BLACK,2);

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      tft.drawRect(75,rect_y,tft.width()-75,35,WHITE);
      tft.setCursor(82,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);
    //maybe don't need  
    return total_num; 
}

int focus_movements_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color){
    int total_num = array_size;
    int max_option = array_size;
    if (!updateMenu) {
      //updateMenu false
      return total_num; 
    }
    updateMenu = false;
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    //joystick icon only show arrows if can press them to go next page
    // tft.drawChar(12,60,UP_ARROW,WHITE,BLACK,2);
    tft.drawChar(12,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(30, 100);
    tft.println("- Home");
    tft.drawChar(12,140,DOWN_ARROW,WHITE,BLACK,2);
    tft.setCursor(30, 140);
    tft.println("- Next");

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      tft.drawRect(75,rect_y,tft.width()-75,35,WHITE);
      tft.setCursor(82,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);
    //maybe don't need  
    return total_num; 
}

int focus_movements_menu2_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color){
    int total_num = array_size;
    int max_option = array_size;
    if (!updateMenu) {
      //updateMenu false
      return total_num; 
    }
    updateMenu = false;
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    //joystick icon only show arrows if can press them to go next page
    tft.drawChar(12,60,UP_ARROW,WHITE,BLACK,2);
    tft.setCursor(30, 60);
    tft.println("- Back");
    tft.drawChar(12,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(30, 100);
    tft.println("- Home");
    //tft.drawChar(12,140,DOWN_ARROW,WHITE,BLACK,2);

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      tft.drawRect(75,rect_y,tft.width()-75,35,WHITE);
      tft.setCursor(82,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);
    //maybe don't need  
    return total_num; 
}

int zoomfocus_movements_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color){
    int total_num = array_size;
    int max_option = array_size;
    if (!updateMenu) {
      //updateMenu false
      return total_num; 
    }
    updateMenu = false;
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    //joystick icon only show arrows if can press them to go next page
    // tft.drawChar(12,60,UP_ARROW,WHITE,BLACK,2);
    tft.drawChar(12,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(30, 100);
    tft.println("- Home");
    tft.drawChar(12,140,DOWN_ARROW,WHITE,BLACK,2);
    tft.setCursor(30, 140);
    tft.println("- Next");

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      tft.drawRect(75,rect_y,tft.width()-75,35,WHITE);
      tft.setCursor(82,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);
    //maybe don't need  
    return total_num; 
}

int zoomfocus_movements_menu2_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color){
    int total_num = array_size;
    int max_option = array_size;
    if (!updateMenu) {
      //updateMenu false
      return total_num; 
    }
    updateMenu = false;
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    //joystick icon only show arrows if can press them to go next page
    tft.drawChar(12,60,UP_ARROW,WHITE,BLACK,2);
    tft.setCursor(30, 60);
    tft.println("- Back");
    tft.drawChar(12,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(30, 100);
    tft.println("- Home");
    tft.drawChar(12,140,DOWN_ARROW,WHITE,BLACK,2);
    tft.setCursor(30, 140);
    tft.println("- Next");

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      tft.drawRect(75,rect_y,tft.width()-75,35,WHITE);
      tft.setCursor(82,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);
    //maybe don't need  
    return total_num; 
}

int zoomfocus_movements_menu3_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color){
    int total_num = array_size;
    int max_option = array_size;
    if (!updateMenu) {
      //updateMenu false
      return total_num; 
    }
    updateMenu = false;
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.println(menu_name);

    //joystick icon only show arrows if can press them to go next/prev page
    tft.drawChar(12,60,UP_ARROW,WHITE,BLACK,2);
    tft.setCursor(30, 60);
    tft.println("- Back");
    tft.drawChar(12,100,SELECT,WHITE,BLACK,2);
    tft.setCursor(30, 100);
    tft.println("- Home");
    //tft.drawChar(12,140,DOWN_ARROW,WHITE,BLACK,2);

    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(2);
    int rect_y = 12;

    for (int i=0; i<total_num; i++) {
      tft.drawRect(75,rect_y,tft.width()-75,35,WHITE);
      tft.setCursor(82,rect_y+10);
      tft.setTextColor(WHITE,BLACK);
      tft.print(string_table[i]);
      rect_y = rect_y+55;
    }
    tft.setTextColor(WHITE,BLACK);
    //maybe don't need  
    return total_num; 
}

int menu(int array_size,const char *menu_name ,const char *const string_table[], int option_selected, int footer, uint16_t color) {
  int total_num = array_size;
  if (!updateMenu) {
    //updateMenu false
    return total_num; 
  }
  updateMenu = false;
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.println(menu_name);
  tft.setTextColor(WHITE,BLACK);
  tft.setTextSize(2);
  int rect_y = 12;

  for (int i=0; i<total_num; i++) {
    tft.drawRect(0,rect_y,tft.width(),23,WHITE);
    tft.setCursor(2,rect_y+4);
    if (i == option_selected) {
      tft.setTextColor(BLACK,WHITE);
    } else {
      tft.setTextColor(WHITE,BLACK);
    }
    tft.print(string_table[i]);
    rect_y = rect_y+23;
  }
  tft.setTextColor(WHITE,BLACK);

  switch(footer) {
    case 1:
      tft.drawChar(2,130,LEFT_ARROW,WHITE,BLACK,2);
      tft.drawChar(17,130,RIGHT_ARROW,WHITE,BLACK,2);
      tft.setCursor(32,134);
      tft.println(F(": Adjust values"));
      tft.drawChar(2,145,SELECT,WHITE,BLACK,2);
      tft.setCursor(17,149);
      tft.println(F(": Press to set"));
      break;
    case 2:
      tft.drawChar(2,130,UP_ARROW,WHITE,BLACK,2);
      tft.drawChar(17,130,DOWN_ARROW,WHITE,BLACK,2);
      tft.setCursor(32,134);
      tft.println(F(": Navigate"));
      tft.drawChar(2,145,SELECT,WHITE,BLACK,2);
      tft.setCursor(17,149);
      tft.println(F(": Press to set"));
      break;
    default:
    break;
  }
  return total_num; 
}

