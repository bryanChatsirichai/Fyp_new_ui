// ******** JOYSTICK Functions **********
/*
 * checks for up and down joystick movement
 */
int getUpDown(int max_option, int current_option, int delay_ms) {
  if (digitalRead(DOWN_BUTTON) == LOW) {
    delay(delay_ms);
    updateMenu = true;
    return (current_option == max_option-1) ? 0 : ++current_option;
  }

  if (digitalRead(UP_BUTTON) == LOW) {
    delay(delay_ms);
    updateMenu = true;
    return (current_option == 0) ? max_option-1 : --current_option;
  }
  return current_option;

}

int get_HomeMenu_Update(int s) {
  
  //Home menu but since this already Home do nthing
  if (digitalRead(A_BUTTON) == LOW) {

  }
  if (digitalRead(B_BUTTON) == LOW) {
    //update 
    s = 0;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(X_BUTTON) == LOW) {
    //update 
    s = 1;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
    if (digitalRead(Y_BUTTON) == LOW) {
    //update 
    s = 2;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  return s;
}

int get_Configuration_Menu_Update(int s) {
  
  //Go Home menu
  if (digitalRead(A_BUTTON) == LOW) {
    resetToHomeScreen();
    s = -1;
    updateMenu = true;
  }
  if (digitalRead(B_BUTTON) == LOW) {
    //update 
    s = 0;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(X_BUTTON) == LOW) {
    //update 
    s = 1;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
    if (digitalRead(Y_BUTTON) == LOW) {
    //update 
    s = 2;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  return s;
}

int get_CameraSetting_Menu_update(int s){
  
  //Go Home menu
  if (digitalRead(A_BUTTON) == LOW) {
    resetToHomeScreen();
    s = -1;
    updateMenu = true;
  }
  if (digitalRead(B_BUTTON) == LOW) {
    //update 
    s = 0;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(X_BUTTON) == LOW) {
    //update 
    s = 1;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
    if (digitalRead(Y_BUTTON) == LOW) {
    //update 
    s = 2;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  return s;
}

int get_positioning_Menu_update(int s){
  
  //Go Home menu
  if (digitalRead(A_BUTTON) == LOW) {
    resetToHomeScreen();
    s = -1;
    updateMenu = true;
  }
  if (digitalRead(B_BUTTON) == LOW) {
    //update 
    s = 0;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(X_BUTTON) == LOW) {
    //update 
    s = 1;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  return s;
}

int get_motor_calibration_menu1_update(int s){
  
    //Go Home menu
    if (digitalRead(A_BUTTON) == LOW) {
      resetToHomeScreen();
      s = -1;
      updateMenu = true;
    }
    if (digitalRead(B_BUTTON) == LOW) {
      //update 
      s = 0;
      tft.fillScreen(ST77XX_BLACK);//clear screen
      updateMenu = true;
    }
    if (digitalRead(X_BUTTON) == LOW) {
      //update 
      s = 1;
      tft.fillScreen(ST77XX_BLACK);//clear screen
      updateMenu = true;
    }
      if (digitalRead(Y_BUTTON) == LOW) {
      //update 
      s = 2;
      tft.fillScreen(ST77XX_BLACK);//clear screen
      updateMenu = true;
    }
    return s;

    //Go Home menu (prev version with scroll)
    // if (digitalRead(CENTER_BUTTON) == LOW) {
    //     resetToHomeScreen();
    //     s = -1;
    //     updateMenu = true;
    //   }

    // if (digitalRead(A_BUTTON) == LOW) {
    //   s = 0;
    //   tft.fillScreen(ST77XX_BLACK);//clear screen
    //   updateMenu = true;
    // }
    // if (digitalRead(B_BUTTON) == LOW) {
    //   //update 
    //   s = 1;
    //   tft.fillScreen(ST77XX_BLACK);//clear screen
    //   updateMenu = true;
    // }
    // if (digitalRead(X_BUTTON) == LOW) {
    //   //update 
    //   s = 2;
    //   tft.fillScreen(ST77XX_BLACK);//clear screen
    //   updateMenu = true;
    // }
    // if (digitalRead(Y_BUTTON) == LOW) {
    //   //update 
    //   s = 3;
    //   tft.fillScreen(ST77XX_BLACK);//clear screen
    //   updateMenu = true;
    // } 
    // return s;
}

int get_options_menu1_update(int s){
  //Go Home menu
  if (digitalRead(CENTER_BUTTON) == LOW) {
      resetToHomeScreen();
      s = -1;
      updateMenu = true;
    }

  if (digitalRead(A_BUTTON) == LOW) {
    s = 0;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(B_BUTTON) == LOW) {
    //update 
    s = 1;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(X_BUTTON) == LOW) {
    //update 
    s = 2;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  // if (digitalRead(Y_BUTTON) == LOW) {
  //   //update 
  //   s = 3;
  //   tft.fillScreen(ST77XX_BLACK);//clear screen
  //   updateMenu = true;
  // } 
  return s;
}


int get_excess_menu_update(int s){
  //Go Home menu
  if (digitalRead(A_BUTTON) == LOW) {
    resetToHomeScreen();
    s = -1;
    updateMenu = true;
  }
  if (digitalRead(B_BUTTON) == LOW) {
    //update 
    s = 0;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(X_BUTTON) == LOW) {
    //update 
    s = 1;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
    if (digitalRead(Y_BUTTON) == LOW) {
    //update 
    s = 2;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  return s;
}

int get_Action_screen_1_Menu_update(int s){
  //Go Home menu
  if (digitalRead(A_BUTTON) == LOW) {
    resetToHomeScreen();
    s = -1;
    updateMenu = true;
  }
  if (digitalRead(B_BUTTON) == LOW) {
    //update 
    s = 0;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(X_BUTTON) == LOW) {
    //update 
    s = 1;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
    if (digitalRead(Y_BUTTON) == LOW) {
    //update 
    s = 2;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  return s;
}

int get_zoom_movements_menu1_update(int s){
  //Go Home menu
  if (digitalRead(CENTER_BUTTON) == LOW) {
      resetToHomeScreen();
      s = -1;
      updateMenu = true;
    }

  if (digitalRead(A_BUTTON) == LOW) {
    s = 0;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(B_BUTTON) == LOW) {
    //update 
    s = 1;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(X_BUTTON) == LOW) {
    //update 
    s = 2;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(Y_BUTTON) == LOW) {
    //update 
    s = 3;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  } 

    //scroll down to zoom_movements_menu2
    if (digitalRead(DOWN_BUTTON) == LOW) {
    //update 
    s = 4;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }

  return s;
}

int get_zoom_movements_menu2_update(int s){
  //Go Home menu
  if (digitalRead(CENTER_BUTTON) == LOW) {
      resetToHomeScreen();
      s = -1;
      updateMenu = true;
  }
  if (digitalRead(A_BUTTON) == LOW) {
    s = 0;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(B_BUTTON) == LOW) {
    //update 
    s = 1;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }


  //scroll up to zoom_movements_menu1
  // -2 just a number to differentiate between -1s
  if (digitalRead(UP_BUTTON) == LOW) {
  //update 
  s = -2;
  tft.fillScreen(ST77XX_BLACK);//clear screen
  updateMenu = true;
  }

  return s;
}

int get_focus_movements_menu1_update(int s){
  //Go Home menu
  if (digitalRead(CENTER_BUTTON) == LOW) {
      resetToHomeScreen();
      s = -1;
      updateMenu = true;
    }

  if (digitalRead(A_BUTTON) == LOW) {
    s = 0;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(B_BUTTON) == LOW) {
    //update 
    s = 1;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(X_BUTTON) == LOW) {
    //update 
    s = 2;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(Y_BUTTON) == LOW) {
    //update 
    s = 3;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  } 

    //scroll down to focus_movements_menu2
    if (digitalRead(DOWN_BUTTON) == LOW) {
    //update 
    s = 4;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }

  return s;
}

int get_focus_movements_menu2_update(int s){
  //Go Home menu
  if (digitalRead(CENTER_BUTTON) == LOW) {
      resetToHomeScreen();
      s = -1;
      updateMenu = true;
  }
  if (digitalRead(A_BUTTON) == LOW) {
    s = 0;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(B_BUTTON) == LOW) {
    //update 
    s = 1;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }


  //scroll up to focus_movements_menu1
  // -2 just a number to differentiate between -1s
  if (digitalRead(UP_BUTTON) == LOW) {
  //update 
  s = -2;
  tft.fillScreen(ST77XX_BLACK);//clear screen
  updateMenu = true;
  }

  return s;
}

int get_zoomfocus_movements_menu1_update(int s){
  //Go Home menu
  if (digitalRead(CENTER_BUTTON) == LOW) {
      resetToHomeScreen();
      s = -1;
      updateMenu = true;
    }

  if (digitalRead(A_BUTTON) == LOW) {
    s = 0;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(B_BUTTON) == LOW) {
    //update 
    s = 1;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(X_BUTTON) == LOW) {
    //update 
    s = 2;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(Y_BUTTON) == LOW) {
    //update 
    s = 3;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  } 

    //scroll down 
    if (digitalRead(DOWN_BUTTON) == LOW) {
    //update 
    s = 4;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }

  return s;
}

int get_zoomfocus_movements_menu2_update(int s){
  //Go Home menu
  if (digitalRead(CENTER_BUTTON) == LOW) {
      resetToHomeScreen();
      s = -1;
      updateMenu = true;
    }

  if (digitalRead(A_BUTTON) == LOW) {
    s = 0;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(B_BUTTON) == LOW) {
    //update 
    s = 1;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(X_BUTTON) == LOW) {
    //update 
    s = 2;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(Y_BUTTON) == LOW) {
    //update 
    s = 3;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  } 
    //scroll up 
    // -2 just a number to differentiate between -1
    if (digitalRead(UP_BUTTON) == LOW) {
    //update 
    s = -2;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
    }

    //scroll down 
    if (digitalRead(DOWN_BUTTON) == LOW) {
    //update 
    s = 4;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }

  return s;
}

int get_zoomfocus_movements_menu3_update(int s){
  //Go Home menu
  if (digitalRead(CENTER_BUTTON) == LOW) {
      resetToHomeScreen();
      s = -1;
      updateMenu = true;
  }
  if (digitalRead(A_BUTTON) == LOW) {
    s = 0;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(B_BUTTON) == LOW) {
    //update 
    s = 1;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  //scroll up 
  // -2 just a number to differentiate between -1s
  if (digitalRead(UP_BUTTON) == LOW) {
  //update 
  s = -2;
  tft.fillScreen(ST77XX_BLACK);//clear screen
  updateMenu = true;
  }

  return s;
}

int get_custom_movements_menu1_update(int s){
  //Go Home menu
  if (digitalRead(CENTER_BUTTON) == LOW) {
      resetToHomeScreen();
      s = -1;
      updateMenu = true;
    }

  if (digitalRead(A_BUTTON) == LOW) {
    s = 0;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(B_BUTTON) == LOW) {
    //update 
    s = 1;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(X_BUTTON) == LOW) {
    //update 
    s = 2;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(Y_BUTTON) == LOW) {
    //update 
    s = 3;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  } 

    //scroll down 
    if (digitalRead(DOWN_BUTTON) == LOW) {
    //update 
    s = 4;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }

  return s;
}

int get_custom_movements_menu2_update(int s){
  //Go Home menu
  if (digitalRead(CENTER_BUTTON) == LOW) {
      resetToHomeScreen();
      s = -1;
      updateMenu = true;
  }
  if (digitalRead(A_BUTTON) == LOW) {
    s = 0;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  if (digitalRead(B_BUTTON) == LOW) {
    //update 
    s = 1;
    tft.fillScreen(ST77XX_BLACK);//clear screen
    updateMenu = true;
  }
  //scroll up 
  // -2 just a number to differentiate between -1s
  if (digitalRead(UP_BUTTON) == LOW) {
  //update 
  s = -2;
  tft.fillScreen(ST77XX_BLACK);//clear screen
  updateMenu = true;
  }
  return s;
}