#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <math.h>
#include <EEPROM.h>
#include "SoftwareSerial.h"


// Motor Parameters
#define MOTOR_STEPS 200 //
#define RPM 1000
#define FOCUS 0
#define ZOOM 1
#define MS_STEP 8 // default 1/8 microstep, 200*8 = 1600 microstepping
#define CALI_ACCEL 400

//Display Pins
#define TFT_CS   9
#define TFT_RST  12  // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC   8
#define TFT_MOSI 11  // Data out
#define TFT_SCLK 10 // Clock out

/* Motor Pins */
#define rear_DIR    6
#define rear_STEP   7
#define front_DIR   0
#define front_STEP  1

//Display Joystick 
#define UP_BUTTON 2
#define DOWN_BUTTON 18
#define LEFT_BUTTON 16
#define RIGHT_BUTTON 20
#define CENTER_BUTTON 3
#define A_BUTTON 15
#define B_BUTTON 17
#define X_BUTTON 19
#define Y_BUTTON 21

/*RX and TX for JQ8900 communication*/
//It's important to note that those RX and TX labels are with respect to the device itself. 
//So the RX from one device should go to the TX of the other, and vice-versa
#define HBYTE(X) ((X & 0xFF00) >> 8) 
#define LBYTE(X) (X & 0xFF) 
#define RX 5
#define TX 4

//control camera shutter  - may need change FOCUS_CAMERA GPIO PIN
#define FOCUS_CAMERA 21
#define SHUTTER_CAMERA 22

/* Colour Strings */
#define WHITE     0xFFFF
#define BLACK     0x0000
#define AQUA      0xFEC0
#define DEEPPINK  0xF8B2  
#define DARKGREEN 0x0320
#define RED       0xF800 
#define LIGHTSKYBLUE 0x867F 
#define SNOW 0xFFDF
#define VIOLET 0xEC1D 
#define YELLOWGREEN 0x9E66
#define GOLDENROD 0xF81F
#define AZURE 0xF7FF
#define CORAL 0xFBEA 
#define CADETBLUE 0x5CF4
#define LIME 0x07E0

/* Symbols */
#define UP_ARROW    0x18
#define DOWN_ARROW  0x19
#define RIGHT_ARROW 0x1A
#define LEFT_ARROW  0x1B
#define SELECT      0x2A


/* Things that are saved
 *  - focus_range     (Max range of focus)
 *  - zoom_range      (Max range of zoom)
 *  - focus_current   (Current focus value)
 *  - zoom_current    (Current zoom value)
 *  - orientation     (Orientation of motors)
 *  - shutter_speed   (Shutter speed)
 *  - motor_time
 */
int zoom_range = 0; 
int focus_range = 0;    
int zoom_current = 0;     
int focus_current = 0;   
int orientation = 0;    
int shutter_time = 0;
int motor_time = 0;   
int exposure_option_set = 0; //default
int camera_shutter_open = 0; //default 0 is not open


// Global Variables
int max_option = 0;
int option_selected = 0;
int updateMenu = true;
int home_screen = -1;
int configuration_screen = -1;
int camera_setting_screen = -1;
int camera_positioning_screen = -1;
int motor_calibration_screen1 = -1;
//int motor_calibration_screen2 = -1;
int exposure_option_screen = -1;
int action_screen_1 = -1;
int zoom_movements_menu1 = -1;
int zoom_movements_menu2 = -1;
int focus_movements_menu1 = -1;
int focus_movements_menu2 = -1;
int zoom_focus_movements_menu1 = -1;
int zoom_focus_movements_menu2 = -1;
int zoom_focus_movements_menu3 = -1;
int fixed_paterns_menu1 = -1;
int fixed_paterns_menu2 = -1;

/* Motor Objects*/
AccelStepper front_motor(AccelStepper::DRIVER, front_STEP, front_DIR);
AccelStepper rear_motor(AccelStepper::DRIVER, rear_STEP, rear_DIR);

/*Control multiple steppers motor*/
MultiStepper steppers;

/* Display Object, set up the displat init */
Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

//Comminication to JQ8900
SoftwareSerial SoftwareSerial(RX,TX); // RX, TX

/***** Defining Strings on Display *****/

//home_screen options focus on configuration,
const char home_header[] PROGMEM = "|- Home Menu -|";
const char home_0[] PROGMEM = "Camera Callibration";
const char home_1[] PROGMEM = "Actions - Z/F";
const char home_2[] PROGMEM = "Actions - Custom";

//main_menu1 options focus on configuration,
const char mm_configuration_header[] PROGMEM = "|- Configuration Menu -|";
const char mm_configuration_0[] PROGMEM = "Camera Settings";
const char mm_configuration_1[] PROGMEM = "Motor Calibration";
const char mm_configuration_2[] PROGMEM = "RESET all values";

////main_menu2 options focus on actions,
const char mm_action1_header[] PROGMEM = "|- Action Menu-1 -|";
const char mm_action1_0[] PROGMEM = "Zoom Movements";
const char mm_action1_1[] PROGMEM = "Focus Movements";
const char mm_action1_2[] PROGMEM = "ZoomFocus Movements";

/////Calibration/////
const char cs_name[] PROGMEM = "|- Camera Settings -|";
const char cs_0[] PROGMEM = "Zoom/Focus Position";
const char cs_1[] PROGMEM = "Shutter Time"; //Camera Shutter Time, Shutter Speed
const char cs_2[] PROGMEM = "Motor Movement Time";

//position orientation 
const char pm_name[] PROGMEM = "|- Positioning Setting -|";
const char pm_0[] PROGMEM = "Zoom at the Back";
const char pm_1[] PROGMEM = "Zoom at the Front";

//motor callibration
const char mc1_name[] PROGMEM = "|- Calibration -|";
const char mc1_0[] PROGMEM = "Z_Cali";
const char mc1_1[] PROGMEM = "F_Cali";
const char mc1_2[] PROGMEM = "POV_Cali";
const char mc1_3[] PROGMEM = "Exposure";

//exposure option
const char exposure_option_name[] PROGMEM = "|- Exposure -|";
const char exposure_option_0[] PROGMEM = "Pre";
const char exposure_option_1[] PROGMEM = "Split";
const char exposure_option_2[] PROGMEM = "After";

//Zoom page1
const char zm1_name[] PROGMEM = "|- Zoom Movements -|";
const char zm1_0[] PROGMEM = "Zoom Max";
const char zm1_1[] PROGMEM = "Zoom Min";
const char zm1_2[] PROGMEM = "Zoom Max&B";
const char zm1_3[] PROGMEM = "Zoom Min&B";
//Zoom page2
const char zm2_name[] PROGMEM = "|- Zoom Movements -|";
const char zm2_0[] PROGMEM = "Zoom to Value";
const char zm2_1[] PROGMEM = "Zoom Value&B";

//Focus page1
const char fm1_name[] PROGMEM = "|- Focus Movements -|";
const char fm1_0[] PROGMEM = "Focus Max";
const char fm1_1[] PROGMEM = "Focus Min";
const char fm1_2[] PROGMEM = "Focus Max&B";
const char fm1_3[] PROGMEM = "Focus Min&B";
//Focus page2
const char fm2_name[] PROGMEM = "|- Focus Movements -|";
const char fm2_0[] PROGMEM = "Focus Value";
const char fm2_1[] PROGMEM = "Focus Value&B";

//Zoom_Focus page1
const char zf1_name[] PROGMEM = "|- Zoom&Focous Movements -|";
const char zf1_0[] PROGMEM = "Z[MAX]F[MAX]";
const char zf1_1[] PROGMEM = "Z[MIN]F[MIN]";
const char zf1_2[] PROGMEM = "Z[MAX]F[MIN]";
const char zf1_3[] PROGMEM = "Z[Min]F[MAX]";
//Zoom_Focus page2
const char zf2_name[] PROGMEM = "|- Zoom&Focous Movements -|";
const char zf2_0[] PROGMEM = "ZF[MAX]&B";
const char zf2_1[] PROGMEM = "ZF[MIN]&B";
const char zf2_2[] PROGMEM = "Zmax_Fmin&B";
const char zf2_3[] PROGMEM = "Zmin_Fmax&B";
//Zoom_Focus page3
const char zf3_name[] PROGMEM = "|- Zoom&Focous Movements -|";
const char zf3_0[] PROGMEM = "ZF Value";
const char zf3_1[] PROGMEM = "ZF Value&B";

//presets page1
const char preset1_name[] PROGMEM = "|----- Presets -----|";
const char preset1_0[] PROGMEM = "Bokeh";
const char preset1_1[] PROGMEM = "Firework";
const char preset1_2[] PROGMEM = "ZoomBlur";
const char preset1_3[] PROGMEM = "SineWave ";

//presets page2
const char preset2_name[] PROGMEM = "|----- Presets -----|";
const char preset2_0[] PROGMEM = "ZigZag-pend";


/* String Table */
const char *const home_menu[] PROGMEM = {home_0, home_1,home_2}; //Home_menu table
const char *const main_menu_1[] PROGMEM = {mm_configuration_0, mm_configuration_1, mm_configuration_2}; //main_menu1 table
const char *const main_menu_2[] PROGMEM = {mm_action1_0, mm_action1_1, mm_action1_2}; //main_menu2 table

const char *const camera_settings_menu[] PROGMEM = {cs_0, cs_1, cs_2};
const char *const positioning_menu[] PROGMEM = {pm_0,pm_1};
const char *const motor_calibration_menu1[] PROGMEM {mc1_0, mc1_1, mc1_2, mc1_3};
const char *const exposure_option_menu[] PROGMEM {exposure_option_0, exposure_option_1,exposure_option_2};


const char *const zoom_menu1[] PROGMEM = {zm1_0,zm1_1,zm1_2,zm1_3};
const char *const zoom_menu2[] PROGMEM = {zm2_0,zm2_1};

const char *const focus_menu1[] PROGMEM = {fm1_0,fm1_1,fm1_2,fm1_3};
const char *const focus_menu2[] PROGMEM = {fm2_0,fm2_1};

const char *const zoomfocus_menu1[] PROGMEM = {zf1_0,zf1_1,zf1_2,zf1_3};
const char *const zoomfocus_menu2[] PROGMEM = {zf2_0,zf2_1,zf2_2,zf2_3};
const char *const zoomfocus_menu3[] PROGMEM = {zf3_0,zf3_1};

const char *const preset1_menu[] PROGMEM = {preset1_0, preset1_1, preset1_2, preset1_3};
const char *const preset2_menu[] PROGMEM = {preset2_0};

// Function Declaration
void initializing_Page();
void updateScreen(float delay_ms=0);
int getUpDown(int max_option, int current_option, int delay_ms);
void resetToHomeScreen();
int menu(int array_size,const char *menu_header,const char *const string_table[], int option_selected, int footer=2, uint16_t color=DEEPPINK);

int home_menu_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected, uint16_t color=DEEPPINK);
int get_HomeMenu_Update(int s);

int configuration_menu_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color=DEEPPINK);
int get_Configuration_Menu_Update(int s);

int cameraSetting_menu_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color=DEEPPINK);
int get_CameraSetting_Menu_update(int s);

int positioning_menu_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color=DEEPPINK);
int get_positioning_Menu_update(int s);

int motor_calibration_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color=DEEPPINK);
int get_motor_calibration_menu1_update(int s);

int exposure_menu_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color=DEEPPINK);
int get_exposure_menu_update(int s);

int action_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color=DEEPPINK);
int get_Action_screen_1_Menu_update(int s);

int zoom_movements_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color=DEEPPINK);
int zoom_movements_menu1_update(int s);

int zoom_movements_menu2_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color=DEEPPINK);
int zoom_movements_menu1_update(int s);

int focus_movements_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color=DEEPPINK);
int focus_movements_menu1_update(int s);

int focus_movements_menu2_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color=DEEPPINK);
int focus_movements_menu2_update(int s);

int zoomfocus_movements_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color=DEEPPINK);
int zoomfocus_movements_menu1_update(int s);

int zoomfocus_movements_menu2_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color=DEEPPINK);
int zoomfocus_movements_menu2_update(int s);

int zoomfocus_movements_menu3_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color=DEEPPINK);
int zoomfocus_movements_menu3_update(int s);

int custome_movements_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color=DEEPPINK);
int custom_movements_menu1_update(int s);

int custome_movements_menu2_screen(int array_size,const char *menu_name ,const char *const string_table[], int option_selected,uint16_t color=DEEPPINK);
int custom_movements_menu2_update(int s);

void setup() {
  // put your setup code here, to run once:
  
  //for print statement
  Serial.begin(9600);
  EEPROM.begin(512);

  //for jp8900-16pin
  SoftwareSerial.begin(9600);

  //camera shutter and focus for capturing pictures 
  // pinMode(FOCUS_CAMERA, OUTPUT);
  // pinMode(SHUTTER_CAMERA, OUTPUT);
  // digitalWrite(FOCUS_CAMERA, HIGH);  
  // digitalWrite(SHUTTER_CAMERA, HIGH);
  // delay(150);

  // ***** Joystick and Buttons *****
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(CENTER_BUTTON, INPUT_PULLUP);
  pinMode(A_BUTTON, INPUT_PULLUP);
  pinMode(B_BUTTON, INPUT_PULLUP);
  pinMode(X_BUTTON, INPUT_PULLUP);
  pinMode(Y_BUTTON, INPUT_PULLUP);

  // ***** Motor *****
  rear_motor.setMaxSpeed(RPM);
  front_motor.setMaxSpeed(RPM);
  steppers.addStepper(rear_motor);
  steppers.addStepper(front_motor);

  // ***** Display *****
  tft.init(240, 240);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);//clear screen
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);

  // ***** EEPROM Read *****
  // reads the stored memory
  // focus_range = EEPROM.read(0);
  // zoom_range = EEPROM.read(1);
  // focus_current = EEPROM.read(2);
  // zoom_current = EEPROM.read(3);
  orientation = EEPROM.read(4);
  // shutter_time = EEPROM.read(5);
  // motor_time = EEPROM.read(6);
  exposure_option_set = EEPROM.read(7);

  // ***** Default Values *****
  // if empty (==255), setting default values to 0
  // for current positions -> Move the motor to stored current 
  // to be implemented when finalise...

  //Show ICM APP 'loading page'
  //initializing_Page();


}

void loop() {
  switch (home_screen) {

    //callibration
    case 0: {
      //configuration menu
      switch (configuration_screen) {
        //camera_setting
        case 0: {
          switch (camera_setting_screen) {
            // Zoom-Focus position screen
            case 0: {
              switch (camera_positioning_screen) {
                // zoom at the back
                case 0:{
                  //Serial.println("zoom at the back");
                  orientation = 0;
                  EEPROM.write(4,orientation);
                  EEPROM.commit();
                  camera_positioning_screen = -1;
                  //go back to prev screen after selection
                  camera_setting_screen = -1;
                  break;
                }
                // zoom at the front
                case 1:{
                  //Serial.println("zoom at the front");
                  orientation = 1;
                  EEPROM.write(4,orientation);
                  EEPROM.commit();
                  camera_positioning_screen = -1;
                  //go back to prev screen after selection
                  camera_setting_screen = -1;
                  break;                
                }
                // show [positioning settings menu]
                default: 
                  // max_option = positioning_menu_screen(2,pm_name ,positioning_menu, option_selected);
                  positioning_menu_screen(2,pm_name ,positioning_menu, option_selected,DEEPPINK);
                  camera_positioning_screen = get_positioning_Menu_update(camera_positioning_screen);
                  break;
              }      
              break;
            }
            // Set Shutter Time screen - shutter time of the DSLR camera
            case 1:{
                break;
            } 
            // set motor movement time -  time needed to execute a sequence
            case 2: {
              break;
            }
            //show [camera settings menu]
            default:
              // max_option = cameraSetting_menu_screen(3,cs_name ,camera_settings_menu, option_selected,DEEPPINK);
              cameraSetting_menu_screen(3,cs_name ,camera_settings_menu, option_selected,DEEPPINK);
              camera_setting_screen = get_CameraSetting_Menu_update(camera_setting_screen);
              break;
          }
          break;
        }
        //motor callibration
        case 1: {
          switch (motor_calibration_screen1){

            //Zoom Calibration
            case 0: {
              break;
            }
            //Focus Calibration
            case 1: {
              break;
            }
            //POV Calibration
            case 2: {
              break;
            }
            //Exposure Setting
            case 3: {
                  switch(exposure_option_screen){
                    //pre
                    case 0:{
                      exposure_option_set = 0;
                      exposure_option_screen = -1;
                      //go back to prev screen after selection
                      motor_calibration_screen1 = -1;
                      EEPROM.write(7, exposure_option_set);
                      EEPROM.commit();
                      break;
                    }
                    //split
                    case 1:{
                      exposure_option_set = 1;
                      exposure_option_screen = -1;
                      //go back to prev screen after selection
                      motor_calibration_screen1 = -1;
                      EEPROM.write(7, exposure_option_set);
                      EEPROM.commit();
                      break;
                    }
                    //after
                    case 2:{
                      exposure_option_set = 2;
                      exposure_option_screen = -1;
                      //go back to prev screen after selection
                      motor_calibration_screen1 = -1;
                      EEPROM.write(7, exposure_option_set);
                      EEPROM.commit();
                      break;
                    }
                    default:
                    //max_option = (3,exposure_option_name,exposure_option_menu, option_selected,DEEPPINK);  
                    exposure_menu_screen(3,exposure_option_name,exposure_option_menu, option_selected,DEEPPINK);
                    exposure_option_screen = get_exposure_menu_update(exposure_option_screen);
                    break;
                  }
              break;
            }

            // motor_calibration_menu2 if needed
            // case 4:{
            //   break;
            // }

            //Show motor_calibration_menu1
            default:
              //max_option = motor_calibration_menu1_screen(4,mc1_name,motor_calibration_menu1, option_selected,DEEPPINK);
              motor_calibration_menu1_screen(4,mc1_name,motor_calibration_menu1, option_selected,DEEPPINK);
              motor_calibration_screen1 = get_motor_calibration_menu1_update(motor_calibration_screen1);
              break;
          }
          break;
        }
        //reset all setting
        case 2: {
          break;
        }
        
        //Show configuation_menu
        default:
          //max_option = configuration_menu_screen(3,mm_configuration_header,main_menu_1, option_selected,DEEPPINK);  
          configuration_menu_screen(3,mm_configuration_header,main_menu_1, option_selected,DEEPPINK);  
          configuration_screen = get_Configuration_Menu_Update(configuration_screen);
          break;
      }
      break;
    }

    //Actions ZF
    case 1:{
      switch(action_screen_1){      
        //zoom movement menu
        case 0: {
          switch(zoom_movements_menu1){
            //zoom to max 
            case 0:{
              break;
            }
            //zoom to min 
            case 1:{
              break;
            }
            //zoom to max and back 
            case 2:{
              break;
            }
            //zoom to min and back
            case 3:{
              break;
            }
            //zoom_movements_menu2
            case 4: {
              switch (zoom_movements_menu1) {
                
                //zoom to value 
                case 0:{
                  break;
                }

                //zoom to value and back
                case 1:{
                  break;
                }

                default:
                   //max_option = zoom_movements_menu2_screen(2,zm2_name ,zoom_menu2, option_selected,DEEPPINK);
                   zoom_movements_menu2_screen(2,zm2_name ,zoom_menu2, option_selected,DEEPPINK);
                   zoom_movements_menu2 = zoom_movements_menu2_update(zoom_movements_menu2);
                   if(zoom_movements_menu2 == -2){
                    zoom_movements_menu1 = -1;
                   }

              }
              break;
            }

            default:
              //max_option = zoom_movements_menu1_screen(4,zm1_name ,zoom_menu1, option_selected,DEEPPINK)
              zoom_movements_menu1_screen(4,zm1_name ,zoom_menu1, option_selected,DEEPPINK);
              zoom_movements_menu1 = zoom_movements_menu1_update(zoom_movements_menu1);
          }
          break;
        }
        //focus movement menu
        case 1: {
          switch(focus_movements_menu1){
            //focus to max 
            case 0:{
              break;
            }
            //focus to min 
            case 1:{
              break;
            }
            //focus to max and back 
            case 2:{
              break;
            }
            //focus to min and back
            case 3:{
              break;
            }
            //focus_movements_menu2
            case 4: {
              switch (focus_movements_menu2) {              
                //focus to value 
                case 0:{
                  break;
                }
                //focus to value and back
                case 1:{
                  break;
                }
                //show focus_movements_menu2
                default:
                   //max_option = focus_movements_menu2_screen(2,zm2_name ,zoom_menu2, option_selected,DEEPPINK);
                   focus_movements_menu2_screen(2,fm2_name ,focus_menu2, option_selected,DEEPPINK);
                   focus_movements_menu2 = zoom_movements_menu2_update(focus_movements_menu2);
                   if(focus_movements_menu2 == -2){
                    focus_movements_menu1 = -1;
                   }

              }
              break;
            }
            //show focus_movements_menu1
            default:
              //max_option = focus_movements_menu1_screen(4,fm1_name ,focus_menu1, option_selected,DEEPPINK)
              focus_movements_menu1_screen(4,fm1_name ,focus_menu1, option_selected,DEEPPINK);
              focus_movements_menu1 = zoom_movements_menu1_update(focus_movements_menu1);
          }
          break;
        }
        
        //zoom-focus movement menu
        case 2: {

          switch (zoom_focus_movements_menu1) {
            //Z[MAX] F[MAX]
            case 0: {
              break;
            }
            //Z[MIN] F[MIN]
            case 1: {
              break;
            }
            //Z[MAX] F[MIN]
            case 2: {
              break;
            }
            //Z[Min] F[MAX]
            case 3: {
              break;
            }
            //zoom_focus_movements_menu2
            case 4: {
              switch (zoom_focus_movements_menu2) {
                //Z[MAX] F[MAX] back
                case 0: {
                  break;
                }
                //Z[MIN] F[MIN] back
                case 1: {
                  break;
                }
                //Z[MAX] F[MIN] back
                case 2: {
                  break;
                }
                //Z[Min] F[MAX] back
                case 3: {
                  break;
                }   
                //zoom_focus_movements_menu3
                case 4: {

                  switch (zoom_focus_movements_menu3) {
                    //ZF Value
                    case 0: {
                      break;
                    }
                    //ZF Value back
                    case 1: {
                      break;
                    }
                    //show zoom_focus_page3
                    default:
                      //  max_option = zoomfocus_movements_menu3_screen(2, zf3_name,zoomfocus_menu3,option_selected,DEEPPINK);
                      zoomfocus_movements_menu3_screen(2, zf3_name,zoomfocus_menu3,option_selected,DEEPPINK);
                      zoom_focus_movements_menu3 = zoomfocus_movements_menu3_update(zoom_focus_movements_menu3);
                      if(zoom_focus_movements_menu3 == -2){
                          zoom_focus_movements_menu2 = -1;
                        }
                      break;
                  }
                  break;
                }   

              //show zoom_focus_page2
              default:
                // max_option = zoomfocus_movements_menu2_screen(4, zf2_name ,zoomfocus_menu2, option_selected,DEEPPINK);
                zoomfocus_movements_menu2_screen(4, zf2_name ,zoomfocus_menu2, option_selected,DEEPPINK);
                zoom_focus_movements_menu2 = zoomfocus_movements_menu2_update(zoom_focus_movements_menu2);
                if(zoom_focus_movements_menu2 == -2){
                    zoom_focus_movements_menu1 = -1;
                  }
                break;
              }
              break;
            }
            //show zoom_focus_page1
            default:
              // max_option = zoomfocus_movements_menu1_screen(4, zf1_name ,zoomfocus_menu1, option_selected,DEEPPINK);
              zoomfocus_movements_menu1_screen(4, zf1_name ,zoomfocus_menu1, option_selected,DEEPPINK);
              zoom_focus_movements_menu1 = zoomfocus_movements_menu1_update(zoom_focus_movements_menu1);
              break;
          }
          break;
        }
        default:
          // max_option = action_menu1_screen(3,mm_action1_header,main_menu_2,option_selected,DEEPPINK);
          action_menu1_screen(3,mm_action1_header,main_menu_2,option_selected,DEEPPINK);
          action_screen_1 = get_Action_screen_1_Menu_update(action_screen_1);
          break;
      }
      break;
    }

    //Action Custom
    case 2:{

      switch (fixed_paterns_menu1) {
        //Bokeh Effect
        case 0: {
          break;
        }
        //Firework Effect
        case 1: {
          break;
        }
        //Zoom Blur Effect
        case 2: {
          break;
        }
        //Sine Wave Effect
        case 3: {
          break;
        }

        //fixed_paterns_menu2
        case 4: {
          switch (fixed_paterns_menu2) {
            
            //ZigZag
            case 0: {
              break;
            }
            
            //show fixed_paterns_menu2
            default:
              //max_option = custome_movements_menu1_screen(1, preset2_name, preset2_menu, option_selected,DEEPPINK);
              custome_movements_menu2_screen(1, preset2_name, preset2_menu, option_selected,DEEPPINK);
              fixed_paterns_menu2 = custom_movements_menu2_update(fixed_paterns_menu2);
              if(fixed_paterns_menu2 == -2){
                    fixed_paterns_menu1 = -1;
              }
              break;
            
          }
          break;
        }

        //show fixed_paterns_menu1
        default:
          // max_option = custome_movements_menu1_screen(4, preset1_name, preset1_menu, option_selected,DEEPPINK);
          custome_movements_menu1_screen(4, preset1_name, preset1_menu, option_selected,DEEPPINK);
          fixed_paterns_menu1 = custom_movements_menu1_update(fixed_paterns_menu1);
          break;
      }
      break;
    }

    //Home page
    default:
        //max_option = home_menu_screen(3,home_header,home_menu, option_selected,DEEPPINK);  
        home_menu_screen(3,home_header,home_menu, option_selected,DEEPPINK);  
        home_screen = get_HomeMenu_Update(home_screen);
        break;
  }
  //option_selected = getUpDown(max_option, option_selected, 0);

}
