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
#define FOCUS_CAMERA 21 //26
#define SHUTTER_CAMERA 22 //27

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
 // Global Variables 
int first_time = 0; //first time the pico on
int zoom_range = 0; 
int focus_range = 0;    
int zoom_current = 0;     
int focus_current = 0;   
int orientation = 0;    
int shutter_time = 0;
int motor_time = 0;   
int excess_option_set = 0; //default
int camera_shutter_open = 0; //default 0 is not open
int rear_rotation_direction = 0; //default
int front_rotation_direction = 0; //default
//shutter_time
int max_shutter_time = 60;
int max_motor_time = 60; //to be update in bar

// Global Variables, control menus
int updateMenu = true;
int max_option = 0;
int home_screen = -1;
int configuration_screen = -1;
int camera_setting_screen = -1;
int camera_positioning_screen = -1;
int motor_calibration_screen1 = -1;
int options_menu1 = -1;
int rotation_menu1 = -1;
int rear_rotation_menu1 = -1;
int front_rotation_menu1 = -1;
//int motor_calibration_screen2 = -1;
int excess_option_screen = -1;
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


//symbol setting for display,calibration,interface ui etc,...
int symbol_padding = 40;
int symbol_size = 20;


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
const char home_0[] PROGMEM = "Camera Config";
const char home_1[] PROGMEM = "Simple Actions";
const char home_2[] PROGMEM = "Preset Actions ";

//main_menu1 options focus on configuration,
const char mm_configuration_header[] PROGMEM = "|- Configuration Menu -|";
const char mm_configuration_0[] PROGMEM = "Camera Settings";
const char mm_configuration_1[] PROGMEM = "Motor Calibration";
const char mm_configuration_2[] PROGMEM = "Options";


////main_menu2 options focus on actions,
const char mm_action1_header[] PROGMEM = "|- Action Menu-1 -|";
const char mm_action1_0[] PROGMEM = "Z-Movements";
const char mm_action1_1[] PROGMEM = "F-Movements";
const char mm_action1_2[] PROGMEM = "Z/F-Movements";

/////Calibration/////
const char cs_name[] PROGMEM = "|- Camera Settings -|";
const char cs_0[] PROGMEM = "1-Shutter Time"; //Camera Shutter Time, Shutter Speed
const char cs_1[] PROGMEM = "2-Motor Time";
const char cs_2[] PROGMEM = "3-Excess";

//position orientation 
const char pm_name[] PROGMEM = "|- Positioning Setting -|";
const char pm_0[] PROGMEM = "Zoom at the Back";
const char pm_1[] PROGMEM = "Zoom at the Front";

const char shutter_menu[] PROGMEM = "|- Shutter Time(s) -|";
const char motor_time_menu[] PROGMEM = "Motor Move Time(s)";

//motor callibration
const char mc1_name[] PROGMEM = "|- Calibration -|";
const char mc1_0[] PROGMEM = "Z_Cali";
const char mc1_1[] PROGMEM = "F_Cali";
const char mc1_2[] PROGMEM = "POV_Cali";


//options page
const char option_name[] PROGMEM = "|- Options -|";
const char option_0[] PROGMEM = "Switch Z/F";
const char option_1[] PROGMEM = "Rotation";
const char option_2[] PROGMEM = "RESET Cam";
const char option_3[] PROGMEM = "RESET Cali";

//rotation page
const char rotation_name[] PROGMEM = "|- Rotations -|";
const char rotation_0[] PROGMEM = "Rear motor";
const char rotation_1[] PROGMEM = "Front motor";

//zoom rotation page
const char rear_rotation_name[] PROGMEM = "|- Rear Rotation -|";
const char rear_rotation_0[] PROGMEM = "Rotation-0";
const char rear_rotation_1[] PROGMEM = "Rotation-1";

//focus rotation page
const char front_rotation_name[] PROGMEM = "|- Front Rotation -|";
const char front_rotation_0[] PROGMEM = "Rotation-0";
const char front_rotation_1[] PROGMEM = "Rotation-1";

//clibrating ZF len(s)
const char cali_zoom[] PROGMEM = "|--Calibrate Zoom --|";
const char cali_focus[] PROGMEM = "|--Calibrate Focus--|";
const char string_cali_initial_position[] PROGMEM = "Initial Position (zero)";
const char string_cali_final_position[] PROGMEM = "Final Position (max)";
const char string_cali[] PROGMEM = "Move joystick to set";
const char string_left[] PROGMEM = "  Min";
const char string_right[] PROGMEM = "  Max";
// const char string_cali[] PROGMEM = "Move joystick to extreme";
// const char string_left[] PROGMEM = "  left";
// const char string_right[] PROGMEM = "  right";

const char adjust_zoom[] PROGMEM = "|--- Adjust Zoom ---|";
const char adjust_focus[] PROGMEM = "|---Adjust Focus ---|";
const char string_36[] PROGMEM = "Adjust [ZOOM] lens";
const char string_36_1[] PROGMEM = "Adjust [FOCUS] lens";
const char string_37[] PROGMEM = "to the desired Image";
const char string_38[] PROGMEM = "to desired Outcome";

//excess option
const char excess_option_name[] PROGMEM = "|- Excess -|";
const char excess_option_0[] PROGMEM = "Pre";
const char excess_option_1[] PROGMEM = "Split";
const char excess_option_2[] PROGMEM = "After";

//Zoom page1
const char zm1_name[] PROGMEM = "|- Zoom Movements -|";
const char zm1_0[] PROGMEM = "Max";
const char zm1_1[] PROGMEM = "Min";
const char zm1_2[] PROGMEM = "Max&Back";
const char zm1_3[] PROGMEM = "Min&Back";
//Zoom page2
const char zm2_name[] PROGMEM = "|- Zoom Movements -|";
const char zm2_0[] PROGMEM = "Value";
const char zm2_1[] PROGMEM = "Value&Back";

//Focus page1
const char fm1_name[] PROGMEM = "|- Focus Movements -|";
const char fm1_0[] PROGMEM = "Max";
const char fm1_1[] PROGMEM = "Min";
const char fm1_2[] PROGMEM = "Max&Back";
const char fm1_3[] PROGMEM = "Min&Back";
//Focus page2
const char fm2_name[] PROGMEM = "|- Focus Movements -|";
const char fm2_0[] PROGMEM = "Value";
const char fm2_1[] PROGMEM = "Value&Back";

//Zoom_Focus page1
const char zf1_name[] PROGMEM = "|- Zoom&Focous Movements -|";
const char zf1_0[] PROGMEM = "ZF_Max";
const char zf1_1[] PROGMEM = "ZF_Min";
const char zf1_2[] PROGMEM = "Zmax_Fmin";
const char zf1_3[] PROGMEM = "Zmin_Fmax";
//Zoom_Focus page2
const char zf2_name[] PROGMEM = "|- Zoom&Focous Movements -|";
const char zf2_0[] PROGMEM = "ZF_Max&B";
const char zf2_1[] PROGMEM = "ZF_Min&B";
const char zf2_2[] PROGMEM = "Zmax_Fmin&B";
const char zf2_3[] PROGMEM = "Zmin_Fmax&B";
//Zoom_Focus page3
const char zf3_name[] PROGMEM = "|- Zoom&Focous Movements -|";
const char zf3_0[] PROGMEM = "ZF Value";
const char zf3_1[] PROGMEM = "ZF Value&B";

//presets page1
const char preset1_name[] PROGMEM = "|----- Presets -----|";
const char preset1_0[] PROGMEM = "Bokeh";
const char preset1_1[] PROGMEM = "Firework-F";
const char preset1_2[] PROGMEM = "Firework-ZF";
const char preset1_3[] PROGMEM = "Z-Blur-min";

//presets page2
const char preset2_name[] PROGMEM = "|----- Presets -----|";
const char preset2_0[] PROGMEM = "Z-Blur-max";
const char preset2_1[] PROGMEM = "SinWave-1 inout";
const char preset2_2[] PROGMEM = "SinWave-2 outin";

const char counttext_1[] PROGMEM = "Get Ready!";
const char counttext_2[] PROGMEM = "3";
const char counttext_3[] PROGMEM = "2";
const char counttext_4[] PROGMEM = "1";
const char counttext_5[] PROGMEM = "SNAP!";

/* String Table */
const char *const home_menu[] PROGMEM = {home_0, home_1,home_2}; //Home_menu table
const char *const main_menu_1[] PROGMEM = {mm_configuration_0, mm_configuration_1, mm_configuration_2}; //main_menu1 table
const char *const main_menu_2[] PROGMEM = {mm_action1_0, mm_action1_1, mm_action1_2}; //main_menu2 table

const char *const camera_settings_menu[] PROGMEM = {cs_0, cs_1, cs_2};
const char *const positioning_menu[] PROGMEM = {pm_0,pm_1};
const char *const motor_calibration_menu1[] PROGMEM {mc1_0, mc1_1, mc1_2};
const char *const options_menu[] PROGMEM {option_0, option_1,option_2,option_3};

const char *const rotation_menu[] PROGMEM {rotation_0, rotation_1};
const char *const rear_rotation_menu[] PROGMEM {rear_rotation_0, rear_rotation_1};
const char *const front_rotation_menu[] PROGMEM {front_rotation_0, front_rotation_1};

const char *const excess_option_menu[] PROGMEM {excess_option_0, excess_option_1,excess_option_2};

const char *const zoom_menu1[] PROGMEM = {zm1_0,zm1_1,zm1_2,zm1_3};
const char *const zoom_menu2[] PROGMEM = {zm2_0,zm2_1};

const char *const focus_menu1[] PROGMEM = {fm1_0,fm1_1,fm1_2,fm1_3};
const char *const focus_menu2[] PROGMEM = {fm2_0,fm2_1};

const char *const zoomfocus_menu1[] PROGMEM = {zf1_0,zf1_1,zf1_2,zf1_3};
const char *const zoomfocus_menu2[] PROGMEM = {zf2_0,zf2_1,zf2_2,zf2_3};
const char *const zoomfocus_menu3[] PROGMEM = {zf3_0,zf3_1};

const char *const calizoom_left[] PROGMEM = {cali_zoom, string_cali_initial_position, string_cali, string_left};
const char *const calizoom_right[] PROGMEM = {cali_zoom, string_cali_final_position, string_cali, string_right};

const char *const califocus_left[] PROGMEM = {cali_focus, string_cali_initial_position, string_cali, string_left};
const char *const califocus_right[] PROGMEM = {cali_focus, string_cali_final_position, string_cali, string_right};

const char *const focus_adjust[] PROGMEM = {adjust_focus, string_36_1, string_37};
const char *const focus_dist[] PROGMEM = {fm1_2, string_36, string_38};

const char *const zoom_adjust[] PROGMEM = {adjust_zoom, string_36, string_37};
const char *const zoom_dist[] PROGMEM = {zm1_2, string_36, string_38};

const char *const preset1_menu[] PROGMEM = {preset1_0, preset1_1, preset1_2, preset1_3};
const char *const preset2_menu[] PROGMEM = {preset2_0,preset2_1,preset2_2};

const char *const countdown[] PROGMEM = {counttext_1, counttext_2, counttext_3, counttext_4, counttext_5};


// Function Declaration
void initializing_Page();
void updateScreen(float delay_ms=0);
int getUpDown(int max_option, int current_option, int delay_ms);
void resetToHomeScreen();
void countdownMenu();

void hotbar(const char title[], int current, int max_range, bool haveBack=false, int header=-1, int footer=-1, uint16_t color=WHITE, bool updateBar=false);
int getLeftRight_value(int range, int current, int low_limit, int delay_ms);
int get_camera_calibration_update();
int get_motor_calibration_update();

//void caliMenu(const char *const string_table[], int current_step, int max_steps, uint16_t color, bool updateBar);
void moveMotorMenu(int count, const char *const string_table[], int current_step, int max_steps, uint16_t color=WHITE, bool updateBar=false);

void printMoveSteps(int type, const char title[], uint16_t color, int movement_display_option);
void setAccel(int type, float accel);
void setCurrentPos(int type, float value);

void moveMotor(int type, int pos_desired, float motor_time = motor_time);
int chooseDist(int type, int count, const char *const string_table[], bool goBack=false, uint16_t color=WHITE);
void goDist(int type, const char title[], int pos_desired, uint16_t color=WHITE, float motor_time = motor_time,float motor_div = 1,bool goBack=true,bool lastSequence=true,bool showScreen=true);

void moveMultiMotor(int zoom_value, int focus_value, float motor_time = motor_time);
void goMultiDist(const char title[], int zoom_desired, int focus_desired, uint16_t color=WHITE, float motor_time = motor_time, float motor_div = 1,bool goBack=true,bool lastSequence=true,bool showScreen=true);


void home_menu_screen(int array_size,const char *menu_name ,const char *const string_table[], uint16_t color=DEEPPINK);
int get_HomeMenu_Update(int s);

void configuration_menu_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color=DEEPPINK);
int get_Configuration_Menu_Update(int s);

void cameraSetting_menu_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color=DEEPPINK);
int get_CameraSetting_Menu_update(int s);

void positioning_menu_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color=DEEPPINK);
int get_positioning_Menu_update(int s);

void motor_calibration_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color=DEEPPINK);
int get_motor_calibration_menu1_update(int s);

void options_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color=DEEPPINK);
int get_options_menu1_update(int s);

void rotation_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color);
int get_rotation_menu1_update(int s);

void rear_rotation_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color);
int get_rear_rotation_menu1_update(int s);

void front_rotation_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color);
int get_front_rotation_menu1_update(int s);

void caliMenu(const char *const string_table[], int current_step, int max_steps=200, uint16_t color=WHITE, bool updateBar=false);
int calibrate(int type, const char *const string_table[], int upper_limit, int lower_limit, uint16_t color=DEEPPINK);

void excess_menu_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color=DEEPPINK);
int get_excess_menu_update(int s);

void action_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color=DEEPPINK);
int get_Action_screen_1_Menu_update(int s);

void zoom_movements_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color=DEEPPINK);
int get_zoom_movements_menu1_update(int s);

void zoom_movements_menu2_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color=DEEPPINK);
int get_zoom_movements_menu2_update(int s);

void focus_movements_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color=DEEPPINK);
int get_focus_movements_menu1_update(int s);

void focus_movements_menu2_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color=DEEPPINK);
int get_focus_movements_menu2_update(int s);

void zoomfocus_movements_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color=DEEPPINK);
int get_zoomfocus_movements_menu1_update(int s);

void zoomfocus_movements_menu2_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color=DEEPPINK);
int get_zoomfocus_movements_menu2_update(int s);

void zoomfocus_movements_menu3_screen(int array_size,const char *menu_name ,const char *const string_table[],uint16_t color=DEEPPINK);
int get_zoomfocus_movements_menu3_update(int s);

void custome_movements_menu1_screen(int array_size,const char *menu_name ,const char *const string_table[], uint16_t color=DEEPPINK);
int get_custom_movements_menu1_update(int s);

void custome_movements_menu2_screen(int array_size,const char *menu_name ,const char *const string_table[], uint16_t color=DEEPPINK);
int get_custom_movements_menu2_update(int s);

void open_Shutter();
void close_Shutter();


void open_Shutter() { // Controls the shutter of a Nikon camera
  //Serial.println("Open");
  digitalWrite(FOCUS_CAMERA, HIGH);  
  digitalWrite(SHUTTER_CAMERA, HIGH);
  delay(150);
  digitalWrite(FOCUS_CAMERA, LOW);
  digitalWrite(SHUTTER_CAMERA, LOW);
  delay(150);
  digitalWrite(FOCUS_CAMERA, HIGH);  
  digitalWrite(SHUTTER_CAMERA, HIGH);
  delay(1000);
}
void close_Shutter() { // Controls the shutter of a Nikon camera
  //Serial.println("Close");
  digitalWrite(FOCUS_CAMERA, HIGH);  
  digitalWrite(SHUTTER_CAMERA, HIGH);
  delay(150);
  digitalWrite(FOCUS_CAMERA, LOW);
  digitalWrite(SHUTTER_CAMERA, LOW);
  delay(150);
  digitalWrite(FOCUS_CAMERA, HIGH);  
  digitalWrite(SHUTTER_CAMERA, HIGH);
  delay(1000);
}


void setup() {
  // put your setup code here, to run once:
  
  //for print statement
  Serial.begin(9600);
  EEPROM.begin(512);

  //for jp8900-16pin
  SoftwareSerial.begin(9600);

  //camera shutter and focus for capturing pictures 
  pinMode(FOCUS_CAMERA, OUTPUT);
  pinMode(SHUTTER_CAMERA, OUTPUT);
  digitalWrite(FOCUS_CAMERA, HIGH);  
  digitalWrite(SHUTTER_CAMERA, HIGH);
  delay(150);

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
  focus_range = EEPROM.read(0);
  zoom_range = EEPROM.read(1);
  focus_current = EEPROM.read(2);
  zoom_current = EEPROM.read(3);
  orientation = EEPROM.read(4);
  shutter_time = EEPROM.read(5);
  motor_time = EEPROM.read(6);
  excess_option_set = EEPROM.read(7);
  rear_rotation_direction = EEPROM.read(8);
  front_rotation_direction = EEPROM.read(9);
  first_time = EEPROM.read(10);


  //set back last know position after on/off
  setAccel(ZOOM, CALI_ACCEL);
  setAccel(FOCUS, CALI_ACCEL);
  setCurrentPos(ZOOM, zoom_current * MS_STEP);
  setCurrentPos(FOCUS, focus_current * MS_STEP);

  // ***** Default Values *****
  // if empty (==255), setting default values to 0
  // for current positions -> Move the motor to stored current 
  // to be implemented when finalise...
  if(first_time == 255){
    focus_range = 0;
    zoom_range = 0;
    focus_current = 0;
    zoom_current = 0;
    orientation = 0;
    shutter_time = 0;
    motor_time = 0;
    excess_option_set = 0; 
    rear_rotation_direction = 0;
    front_rotation_direction = 0;
    first_time = 0;
                        
    EEPROM.write(0, focus_range);
    EEPROM.write(1, zoom_range);
    EEPROM.write(2, focus_current);
    EEPROM.write(3, zoom_current);
    EEPROM.write(4, orientation);
    EEPROM.write(5, shutter_time);
    EEPROM.write(6, motor_time);
    EEPROM.write(7, excess_option_set);
    EEPROM.write(8, rear_rotation_direction);
    EEPROM.write(9, front_rotation_direction);
    EEPROM.write(10, first_time);
    EEPROM.commit();
  }

  //Show ICM APP 'loading page'
  initializing_Page();


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
            // Set Shutter Time screen - shutter time of the DSLR camera
            case 0:{
                //display shutter speed bar (motor calibration).
                // int max_shutter_time = 40;
                int old_shutter_time = shutter_time; //if go_home before setting
                hotbar(shutter_menu, shutter_time, max_shutter_time, true,0,1);
                while(true){
                  hotbar(shutter_menu, shutter_time, max_shutter_time, true, 0, 1, GOLDENROD, true);
                  int option = get_camera_calibration_update();
                  shutter_time = getLeftRight_value(max_shutter_time, shutter_time,0, 0);

                  //go home not set value
                  if(option == 0){
                    //reset to home inside get_camera_calibration_update();
                    shutter_time = old_shutter_time;
                    camera_setting_screen = -1;
                    break;
                  }
                  else if (option == 1){
                    EEPROM.write(5, shutter_time);
                    EEPROM.commit();
                    camera_setting_screen = -1;
                    break;
                  }
                }
                break;
            } 
            // set motor movement time -  time needed to execute a sequence
            case 1: {
              int motor_time_max = shutter_time;
              int old_motor_time = motor_time;
               hotbar(motor_time_menu, motor_time, motor_time_max, true,0,1);
              while(true){
                hotbar(motor_time_menu, motor_time, motor_time_max, true,0,1, GOLDENROD, true);
                int option = get_camera_calibration_update();
                motor_time = getLeftRight_value(motor_time_max, motor_time,0, 0);

                //go home not set value
                if(option == 0){
                  //reset to home inside get_camera_calibration_update();
                  motor_time = old_motor_time;
                  camera_setting_screen = -1;
                  break;
                }

                //Set value
                else if(option == 1){
                  EEPROM.write(6, motor_time);
                  EEPROM.commit();
                  camera_setting_screen = -1;
                  break;
                }
              }
              break;
            }
            //Excess Setting
            case 2: {
                  switch(excess_option_screen){
                    //pre
                    case 0:{
                      excess_option_set = 0;
                      excess_option_screen = -1;
                      //go back to prev screen after selection
                      camera_setting_screen = -1;
                      EEPROM.write(7, excess_option_set);
                      EEPROM.commit();
                      break;
                    }
                    //split
                    case 1:{
                      excess_option_set = 1;
                      excess_option_screen = -1;
                      //go back to prev screen after selection
                      camera_setting_screen = -1;
                      EEPROM.write(7, excess_option_set);
                      EEPROM.commit();
                      break;
                    }
                    //after
                    case 2:{
                      excess_option_set = 2;
                      excess_option_screen = -1;
                      //go back to prev screen after selection
                      camera_setting_screen = -1;
                      EEPROM.write(7, excess_option_set);
                      EEPROM.commit();
                      break;
                    }
                    default:
                      excess_menu_screen(3,excess_option_name,excess_option_menu,DEEPPINK);
                      excess_option_screen = get_excess_menu_update(excess_option_screen);
                      break;
                  }
              break;
            }
            //show [camera settings menu]
            default:
              cameraSetting_menu_screen(3,cs_name ,camera_settings_menu,DEEPPINK);
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
                zoom_current = 0;
                //moveMotor(ZOOM, zoom_current * MS_STEP);
                //setCurrentPos(ZOOM, 0);
                setAccel(ZOOM, CALI_ACCEL);
                setCurrentPos(ZOOM, zoom_current * MS_STEP);
            
                // set to minimum left
                //int minZoom = calibrate(ZOOM, calizoom_left, 0, -MOTOR_STEPS, DEEPPINK);
                int minZoom = calibrate(ZOOM, calizoom_left, MOTOR_STEPS, -MOTOR_STEPS, DEEPPINK);
                setCurrentPos(ZOOM, 0); // set to 0
                zoom_current = 0;
                //updateScreen(100);
                
                // set to maximum right
                int maxZoom = calibrate(ZOOM, calizoom_right, MOTOR_STEPS, 0, DEEPPINK);
                //moveMotor(ZOOM, 0,0); // returns back to 0
                zoom_current = 0;
                //zoom_range = maxZoom - minZoom;
                zoom_range = maxZoom - zoom_current;
                //updateScreen(100);
                EEPROM.write(1, zoom_range);  

                //move and set zoom current to be the midlle of the minMax(range)
                int zoom_middle = zoom_range / 2;
                zoom_current = zoom_middle;
                moveMotor(ZOOM, zoom_middle,0);

                //minimum becomes absolute min pos
                EEPROM.write(3, zoom_current);
                EEPROM.commit();
                motor_calibration_screen1 = -1;
                break;
            }
            //Focus Calibration
            case 1: {
                focus_current = 0;
                //moveMotor(FOCUS, focus_current * MS_STEP);
                //setCurrentPos(FOCUS, 0);
                setAccel(FOCUS, CALI_ACCEL);
                setCurrentPos(FOCUS, focus_current * MS_STEP);
            
                // set to minimum left
                //int minFocus = calibrate(FOCUS, califocus_left, 0, -MOTOR_STEPS, DEEPPINK);
                int minFocus = calibrate(FOCUS, califocus_left, MOTOR_STEPS, -MOTOR_STEPS, DEEPPINK);
                setCurrentPos(FOCUS, 0); // set to 0
                focus_current = 0;
                //updateScreen(100);
                
                // set to maximum right
                int maxFocus = calibrate(FOCUS, califocus_right, MOTOR_STEPS, 0, DEEPPINK);
                //moveMotor(FOCUS, 0,0); // returns back to 0
                focus_current = 0;
                //focus_range = maxFocus - minFocus;
                focus_range = maxFocus - focus_current;
                //updateScreen(100);
                EEPROM.write(0, focus_range);  

                //move and set focyus current to be the midlle of the minMax(range)
                int focus_middle = focus_range / 2;
                focus_current = focus_middle;
                moveMotor(FOCUS, focus_middle,0);

                //minimum becomes absolute min pos
                EEPROM.write(2, focus_current);
                
                
                motor_calibration_screen1 = -1;
                break;
            }
            //POV Calibration
            case 2: {
                setAccel(ZOOM, CALI_ACCEL);
                setCurrentPos(ZOOM, zoom_current * MS_STEP);
                zoom_current = chooseDist(ZOOM, 3, zoom_adjust, false, DEEPPINK);
                EEPROM.write(3, zoom_current);

                setAccel(FOCUS, CALI_ACCEL);
                setCurrentPos(FOCUS, focus_current * MS_STEP);
                focus_current = chooseDist(FOCUS, 3, focus_adjust, false, DEEPPINK);
                EEPROM.write(2, focus_current);
                
                motor_calibration_screen1 = resetScreen(motor_calibration_screen1);
                EEPROM.commit();
              break;
            }
            // case 3: {
            //   break;
            // }

            // motor_calibration_menu2 if needed
            // case 4:{
            //   break;
            // }

            //Show motor_calibration_menu1
            default:
              motor_calibration_menu1_screen(3,mc1_name,motor_calibration_menu1,DEEPPINK);
              motor_calibration_screen1 = get_motor_calibration_menu1_update(motor_calibration_screen1);
              break;
          }
          break;
        }
        //options screen
        case 2: {
          switch(options_menu1){
            //Switch Z/F, Zoom-Focus position screen
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
                  options_menu1 = -1;
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
                  options_menu1 = -1;
                  break;                
                }
                // show [positioning settings menu]
                default: 
                  positioning_menu_screen(2,pm_name ,positioning_menu,DEEPPINK);
                  camera_positioning_screen = get_positioning_Menu_update(camera_positioning_screen);
                  break;
              }      
              break;
            }
            //motor rotation option
            case 1:{
              switch(rotation_menu1){
                //zoom rotation option
                case 0: {
                  switch(rear_rotation_menu1){
                    //zoom rotation 0
                    case 0:{
                      rear_rotation_direction = 0;
                      EEPROM.write(8, rear_rotation_direction);
                      EEPROM.commit();
                      rear_rotation_menu1 = -1;
                      rotation_menu1 = -1;

                      break;
                    }
                    //zoom rotation 1
                    case 1:{
                      rear_rotation_direction = 1;
                      EEPROM.write(8, rear_rotation_direction);
                      EEPROM.commit();
                      rear_rotation_menu1 = -1;
                      rotation_menu1 = -1;
                      break;
                    }
                    default:
                      rear_rotation_menu1_screen(2,rear_rotation_name,rear_rotation_menu,DEEPPINK);
                      rear_rotation_menu1 = get_rear_rotation_menu1_update(rear_rotation_menu1);
                      break;
                  }
                  break;
                }
                //focus rotation option
                case 1:{
                  switch(front_rotation_menu1){
                    //focus rotation 0
                    case 0:{
                      front_rotation_direction = 0;
                      EEPROM.write(9, front_rotation_direction);
                      EEPROM.commit();
                      front_rotation_menu1 = -1;
                      rotation_menu1 = -1;

                      break;
                    }
                    //focus rotation 1
                    case 1:{
                      front_rotation_direction = 1;
                      EEPROM.write(9, front_rotation_direction);
                      EEPROM.commit();
                      front_rotation_menu1 = -1;
                      rotation_menu1 = -1;
                      break;
                    }
                    default:
                      front_rotation_menu1_screen(2,front_rotation_name,front_rotation_menu,DEEPPINK);
                      front_rotation_menu1 = get_front_rotation_menu1_update(front_rotation_menu1);
                      break;
                  }
                  break;
                }
                default:
                  rotation_menu1_screen(2,rotation_name,rotation_menu,DEEPPINK);
                  rotation_menu1 = get_rotation_menu1_update(rotation_menu1);
                  break;

              }
              break;
            }
            //reset camera setting
            case 2:{
              EEPROM.write(5,0);
              EEPROM.write(6,0);                 
              EEPROM.write(7,0);
              shutter_time = 0;
              motor_time = 0;
              excess_option_set = 0;
              EEPROM.commit();
              // options_menu1 = resetScreen(options_menu1);
              options_menu1 = -1;
              break;
            }
            //reset calibration
            case 3:{
              EEPROM.write(0,0);
              EEPROM.write(1,0);
              EEPROM.write(2,0);
              EEPROM.write(3,0);
              EEPROM.write(4,0);
              focus_range = 0;
              zoom_range = 0;
              focus_current = 0;
              zoom_current = 0;
              orientation = 0;
              EEPROM.commit();
              // options_menu1 = resetScreen(options_menu1);
              options_menu1 = -1;
              break;
            }
            default:
              options_menu1_screen(4,option_name,options_menu,DEEPPINK);
              options_menu1 = get_options_menu1_update(options_menu1);
              break; 
          }
          break;
        }
        
        //Show configuation_menu
        default:
          configuration_menu_screen(3,mm_configuration_header,main_menu_1,DEEPPINK);  
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
              Serial.println("Zoom to max");
              countdownMenu();
              //return to starting position by default
              goDist(ZOOM, zm1_0, zoom_range, SNOW, motor_time,1,true,true,true);
              zoom_movements_menu1 = resetScreen(zoom_movements_menu1);
              break;
            }
            //zoom to min 
            case 1:{
                Serial.println("Zoom to min");
                countdownMenu();
                //global motor time pass in by default
                //return to starting position by default
                goDist(ZOOM, zm1_1, 0, SNOW, motor_time,1,true,true,true);
                zoom_movements_menu1 = resetScreen(zoom_movements_menu1);
                break;
            }
            //zoom to max and back 
            case 2:{
                    Serial.println("Zoom to max and back");                    
                    // Serial.print("previous_pos");
                    // Serial.println(previous_pos);
                    int previous_pos = zoom_current;
                    countdownMenu();
                    //going back is now part of motor_time
                    goDist(ZOOM, zm1_2, zoom_range, SNOW, motor_time,2,false,false,true);
                    goDist(ZOOM, zm1_2, previous_pos, SNOW, motor_time,2,false,true,false);
                    zoom_movements_menu1 = resetScreen(zoom_movements_menu1);
                    break;
            }
            //zoom to min and back
            case 3:{
                    Serial.println("Zoom to min and back");                    
                    // Serial.print("previous_pos");
                    // Serial.println(previous_pos);
                    int previous_pos = zoom_current;
                    countdownMenu();
                    //going back is now part of motor_time
                    goDist(ZOOM, zm1_3, 0, SNOW, motor_time,2,false,false,true);
                    goDist(ZOOM, zm1_3, previous_pos, SNOW, motor_time,2,false,true,false);
                    zoom_movements_menu1 = resetScreen(zoom_movements_menu1);
                    break;
            }
            //zoom_movements_menu2
            case 4: {
              switch (zoom_movements_menu2) {
                
                //zoom to value 
                case 0:{
                Serial.println("Zoom to a value");
                int pos_desired;
                //choose dist and reset back to starting pos
                pos_desired = chooseDist(ZOOM, 3, zoom_dist, true, YELLOWGREEN);
                updateScreen();
                countdownMenu();
                //return to starting position by default
                goDist(ZOOM, zm2_0, pos_desired, YELLOWGREEN, motor_time,1,true,true,true);
                zoom_movements_menu2 = resetScreen(zoom_movements_menu2);
                break;
                }

                //zoom to value and back
                case 1:{
                    Serial.println("Zoom to a value and back");
                    // Serial.print("previous_pos");
                    // Serial.println(previous_pos);
                    int pos_desired;
                    int previous_pos = zoom_current;

                    //choose dist and reset back to starting pos
                    pos_desired = chooseDist(ZOOM, 3, zoom_dist, true, YELLOWGREEN);
                    updateScreen(100);
                    countdownMenu();

                    //going back is now part of motor_time
                    goDist(ZOOM, zm2_0, pos_desired, SNOW, motor_time,2,false,false,true);
                    goDist(ZOOM, zm2_1, previous_pos, SNOW, motor_time,2,false,true,false);
                    zoom_movements_menu2 = resetScreen(zoom_movements_menu2);
                    break;
                }

                default:
                   zoom_movements_menu2_screen(2,zm2_name ,zoom_menu2,DEEPPINK);
                   zoom_movements_menu2 = get_zoom_movements_menu2_update(zoom_movements_menu2);
                   if(zoom_movements_menu2 == -2){
                    zoom_movements_menu1 = -1;
                    zoom_movements_menu2 = -1;
                   }

              }
              break;
            }

            default:
              zoom_movements_menu1_screen(4,zm1_name ,zoom_menu1,DEEPPINK);
              zoom_movements_menu1 = get_zoom_movements_menu1_update(zoom_movements_menu1);
          }
          break;
        }
        //focus movement menu
        case 1: {
          switch(focus_movements_menu1){
            //focus to max 
            case 0:{
                Serial.println("Focus to max");
                countdownMenu();
                goDist(FOCUS, fm1_0, focus_range, SNOW,motor_time,1,true,true,true);
                focus_movements_menu1 = resetScreen(focus_movements_menu1);
                break;
            }
            //focus to min 
            case 1:{
                Serial.println("Focus to min");
                countdownMenu();
                goDist(FOCUS, fm1_1, 0, SNOW,motor_time,1,true,true,true);
                focus_movements_menu1 = resetScreen(focus_movements_menu1);
                break;
            }
            //focus to max and back 
            case 2:{
                Serial.println("Focus to max and back");                    
                // Serial.print("previous_pos");
                // Serial.println(previous_pos);
                int previous_pos = focus_current;
                countdownMenu();
                //going back is now part of motor_time
                goDist(FOCUS, fm1_2, focus_range, SNOW, motor_time,2,false,false,true);
                goDist(FOCUS, fm1_2, previous_pos, SNOW, motor_time,2,false,true,false);
                focus_movements_menu1 = resetScreen(focus_movements_menu1);
                break;
            }
            //focus to min and back
            case 3:{
                Serial.println("Focus to min and back");                    
                // Serial.print("previous_pos");
                // Serial.println(previous_pos);
                int previous_pos = focus_current;
                countdownMenu();
                //going back is now part of motor_time
                goDist(FOCUS, fm1_3, 0, SNOW, motor_time,2,false,false,true);
                goDist(FOCUS, fm1_3, previous_pos, SNOW, motor_time,2,false,true,false);
                focus_movements_menu1 = resetScreen(focus_movements_menu1);
                break;
            }
            //focus_movements_menu2
            case 4: {
              switch (focus_movements_menu2) {              
                //focus to value 
                case 0:{
                  Serial.println("Focus to a value");
                  int pos_desired;
                  pos_desired = chooseDist(FOCUS, 3, focus_dist, true, YELLOWGREEN);
                  updateScreen();
                  countdownMenu();
                  goDist(FOCUS, fm2_0, pos_desired, YELLOWGREEN,motor_time,1,true,true,true);
                  focus_movements_menu2 = resetScreen(focus_movements_menu2);
                  break;
                }
                //focus to value and back
                case 1:{
                  Serial.println("Focus to a value and back");
                  int pos_desired;
                  int previous_pos = focus_current;
                  pos_desired = chooseDist(FOCUS, 3, focus_dist, true, YELLOWGREEN);
                  updateScreen();
                  countdownMenu();
                  //going back is now part of motor_time
                  goDist(FOCUS, fm2_1, pos_desired, SNOW, motor_time,2,false,false,true);
                  goDist(FOCUS, fm2_1, previous_pos, SNOW, motor_time,2,false,true,false);
                  focus_movements_menu2 = resetScreen(focus_movements_menu2);
                  break;
                }
                //show focus_movements_menu2
                default:
                   focus_movements_menu2_screen(2,fm2_name ,focus_menu2,DEEPPINK);
                   focus_movements_menu2 = get_zoom_movements_menu2_update(focus_movements_menu2);
                   if(focus_movements_menu2 == -2){
                    focus_movements_menu1 = -1;
                    focus_movements_menu2 = -1;
                   }

              }
              break;
            }
            //show focus_movements_menu1
            default:
              focus_movements_menu1_screen(4,fm1_name ,focus_menu1,DEEPPINK);
              focus_movements_menu1 = get_zoom_movements_menu1_update(focus_movements_menu1);
          }
          break;
        }      
        //zoom-focus movement menu
        case 2: {
          switch (zoom_focus_movements_menu1) {
            //Z[MAX] F[MAX]
            case 0: {
              Serial.println("ZF both to max");
              countdownMenu();
              goMultiDist(zf1_0, zoom_range, focus_range, VIOLET,motor_time,1,true,true,true);
              zoom_focus_movements_menu1 = resetScreen(zoom_focus_movements_menu1);
              break;
            }
            //Z[MIN] F[MIN]
            case 1: {
              Serial.println("ZF both to min");
              countdownMenu();
              goMultiDist(zf1_1, 0, 0, AZURE,motor_time,1,true,true,true);
              zoom_focus_movements_menu1 = resetScreen(zoom_focus_movements_menu1);
              break;
            }
            //Z[MAX] F[MIN]
            case 2: {
              Serial.println("zoom to max, focus to min");
              countdownMenu();
              goMultiDist(zf1_2, zoom_range, 0, CORAL,motor_time,1,true,true,true);
              zoom_focus_movements_menu1 = resetScreen(zoom_focus_movements_menu1);
              break;
            }
            //Z[Min] F[MAX]
            case 3: {
              Serial.println("zoom to min, focus to max");
              countdownMenu();
              goMultiDist(zf1_3, 0, focus_range, CORAL,motor_time,1,true,true,true);
              zoom_focus_movements_menu1 = resetScreen(zoom_focus_movements_menu1);
              break;
            }
            //zoom_focus_movements_menu2
            case 4: {
              switch (zoom_focus_movements_menu2) {
                //Z[MAX] F[MAX] back
                case 0: {
                  Serial.println("ZF max and back");
                  int previous_zoom_pos = zoom_current;
                  int previous_focus_pos = focus_current;
                  // Serial.print("previous_zoom_pos");
                  // Serial.println(previous_zoom_pos);
                  // Serial.print("previous_focus_pos");
                  // Serial.println(previous_focus_pos);
                  countdownMenu();
                  //going back is now part of motor_time
                  goMultiDist(zf2_0, zoom_range, focus_range, VIOLET, motor_time,2, false, false,true);
                  goMultiDist(zf2_0, previous_zoom_pos, previous_focus_pos, VIOLET, motor_time,2, false, true,false);
                  zoom_focus_movements_menu2 = resetScreen(zoom_focus_movements_menu2);
                  break;
                }
                //Z[MIN] F[MIN] back
                case 1: {
                  Serial.println("ZF min and back");
                  int previous_zoom_pos = zoom_current;
                  int previous_focus_pos = focus_current;
                  // Serial.print("previous_zoom_pos");
                  // Serial.println(previous_zoom_pos);
                  // Serial.print("previous_focus_pos");
                  // Serial.println(previous_focus_pos);
                  countdownMenu();
                  //going back is now part of motor_time
                  goMultiDist(zf2_1, 0, 0, VIOLET, motor_time,2, false, false,true);
                  goMultiDist(zf2_1, previous_zoom_pos, previous_focus_pos, VIOLET, motor_time,2, false, true,false);
                  zoom_focus_movements_menu2 = resetScreen(zoom_focus_movements_menu2);
                  break;
                }
                //Z[MAX] F[MIN] back
                case 2: {
                  Serial.println("Z[max] F[min] and back");
                  int previous_zoom_pos = zoom_current;
                  int previous_focus_pos = focus_current;
                  // Serial.print("previous_zoom_pos");
                  // Serial.println(previous_zoom_pos);
                  // Serial.print("previous_focus_pos");
                  // Serial.println(previous_focus_pos);
                  countdownMenu();
                  //going back is now part of motor_time
                  goMultiDist(zf2_2, zoom_range, 0, VIOLET, motor_time,2, false, false,true);
                  goMultiDist(zf2_2, previous_zoom_pos, previous_focus_pos, VIOLET, motor_time,2, false, true,false);
                  zoom_focus_movements_menu2 = resetScreen(zoom_focus_movements_menu2);
                  break;
                }
                //Z[Min] F[MAX] back
                case 3: {
                  Serial.println("Z[min] F[max] and back");
                  int previous_zoom_pos = zoom_current;
                  int previous_focus_pos = focus_current;
                  // Serial.print("previous_zoom_pos");
                  // Serial.println(previous_zoom_pos);
                  // Serial.print("previous_focus_pos");
                  // Serial.println(previous_focus_pos);
                  countdownMenu();
                  //going back is now part of motor_time
                  goMultiDist(zf2_3, 0, focus_range, VIOLET, motor_time,2, false, false,true);
                  goMultiDist(zf2_3, previous_zoom_pos, previous_focus_pos, VIOLET, motor_time,2, false, true,false);
                  zoom_focus_movements_menu2 = resetScreen(zoom_focus_movements_menu2);
                  break;
                }   
                //zoom_focus_movements_menu3
                case 4: {
                  switch (zoom_focus_movements_menu3) {
                    //ZF Value
                    case 0: {
                      Serial.println("ZF to certain dist");
                      int zoom_desired, focus_desired;
                      zoom_desired = chooseDist(ZOOM, 3, zoom_dist, true, YELLOWGREEN);
                      if (zoom_desired == zoom_current) {
                        zoom_desired = -1; // failsafe
                      }
                      updateScreen(100);
                      focus_desired = chooseDist(FOCUS, 3, focus_dist, true, RED);
                      if (focus_desired == focus_current) {
                        focus_desired = -1; // failsafe
                      }
                      updateScreen(100);
                      countdownMenu();
                      goMultiDist(zf3_0, zoom_desired, focus_desired, LIME,motor_time,1,true,true,true);
                      zoom_focus_movements_menu3 = resetScreen(zoom_focus_movements_menu3);
                      break;
                    }
                    //ZF Value back
                    case 1: {
                      Serial.println("ZF value and back");
                      int previous_zoom_pos = zoom_current;
                      int previous_focus_pos = focus_current;
                      // Serial.print("previous_zoom_pos");
                      // Serial.println(previous_zoom_pos);
                      // Serial.print("previous_focus_pos");
                      // Serial.println(previous_focus_pos);
                      int zoom_desired, focus_desired;
                      zoom_desired = chooseDist(ZOOM, 3, zoom_dist, true, YELLOWGREEN);
                      if (zoom_desired == zoom_current) {
                        zoom_desired = -1; // failsafe
                      }
                      updateScreen(100);
                      focus_desired = chooseDist(FOCUS, 3, focus_dist, true, RED);
                      if (focus_desired == focus_current) {
                        focus_desired = -1; // failsafe
                      }
                      updateScreen(100);
                      countdownMenu();
                      //going back is now part of motor_time
                      goMultiDist(zf3_1, zoom_desired, focus_desired, VIOLET, motor_time,2, false, false,true);
                      goMultiDist(zf3_1, previous_zoom_pos, previous_focus_pos, VIOLET, motor_time,2, false, true,false);
                      zoom_focus_movements_menu3 = resetScreen(zoom_focus_movements_menu3);
                      break;
                    }
                    //show zoom_focus_page3
                    default:
                      zoomfocus_movements_menu3_screen(2, zf3_name,zoomfocus_menu3,DEEPPINK);
                      zoom_focus_movements_menu3 = get_zoomfocus_movements_menu3_update(zoom_focus_movements_menu3);
                      if(zoom_focus_movements_menu3 == -2){
                          zoom_focus_movements_menu2 = -1;
                          zoom_focus_movements_menu3 = -1;
                        }
                      break;
                  }
                  break;
                }   

              //show zoom_focus_page2
              default:
                zoomfocus_movements_menu2_screen(4, zf2_name ,zoomfocus_menu2,DEEPPINK);
                zoom_focus_movements_menu2 = get_zoomfocus_movements_menu2_update(zoom_focus_movements_menu2);
                if(zoom_focus_movements_menu2 == -2){
                    zoom_focus_movements_menu1 = -1;
                    zoom_focus_movements_menu2 = -1;
                  }
                break;
              }
              break;
            }
            //show zoom_focus_page1
            default:
              zoomfocus_movements_menu1_screen(4, zf1_name ,zoomfocus_menu1,DEEPPINK);
              zoom_focus_movements_menu1 = get_zoomfocus_movements_menu1_update(zoom_focus_movements_menu1);
              break;
          }
          break;
        }
        default:
          action_menu1_screen(3,mm_action1_header,main_menu_2,DEEPPINK);
          action_screen_1 = get_Action_screen_1_Menu_update(action_screen_1);
          break;
      }
      break;
    }

    //Action Custom
    case 2:{
      switch (fixed_paterns_menu1) {
        // Bokeh Effect - changing focus to minimum from pov
        //(S: Focus Max, Zoom Current. F: Focus Current, Zoom Widest)
        case 0: {
          //Serial.println("Bokeh Effect");
          countdownMenu();
          goDist(FOCUS, preset1_0, 0, SNOW,motor_time,1,true,true,true);
          fixed_paterns_menu1 = resetScreen(fixed_paterns_menu1);
          // =============================================
          // setting lens to starting position (old version)
          // int previous_pos = focus_current;
          // printMoveSteps(-1, preset1_0, CADETBLUE, 2); 
          // moving motor, haven start pattern yet so use default motor speed
          // moveMotor(FOCUS, focus_range, 0);
          // focus_current = focus_range;
          // start pattern sequence
          // updateScreen(100);
          // countdownMenu();
          // goDist(FOCUS, preset1_0, previous_pos, VIOLET, motor_time,2,false,false,true);
          // goDist(ZOOM, preset1_0, 0, VIOLET, motor_time,2,true,true,false);
          // fixed_paterns_menu1 = resetScreen(fixed_paterns_menu1);
          break;
        }
        //Firework-F same as bokeh(Focus Max, then min, then return to original)
        case 1: {
          Serial.println("Firework-F");
          countdownMenu();
          goDist(FOCUS, preset1_1, 0, SNOW,motor_time,1,true,true,true);
          fixed_paterns_menu1 = resetScreen(fixed_paterns_menu1);
          //===================================
          // int previous_pos = focus_current;
          // setting lens to starting position
          // printMoveSteps(-1, preset1_1, CADETBLUE, 2);
          // moving motor, haven start pattern yet so use default motor speed 
          // moveMotor(FOCUS, focus_range,0);
          // focus_current = focus_range;
          // updateScreen(100);
          // countdownMenu();
          // goDist(FOCUS, preset1_1, 0, AZURE, ((float)3/4)*motor_time, false,false,true);
          // goDist(FOCUS, preset1_1, previous_pos, AZURE, ((float)1/4)*motor_time,false,true,false);
          // goDist(FOCUS, preset1_1, 0, AZURE, motor_time, 1.33,false,false,true);
          // goDist(FOCUS, preset1_1, previous_pos, AZURE,motor_time,4,false,true,false);
          // fixed_paterns_menu1 = resetScreen(fixed_paterns_menu1);
          break;
        }
        // Firework-ZF Effect, zoom the wide(min), focus to min at the same time === ZF-min
        case 2: {
          Serial.println("Firework-ZF");
          countdownMenu();
          goMultiDist(preset1_2, 0, 0, AZURE,motor_time,1,true,true,true);
          fixed_paterns_menu1 = resetScreen(fixed_paterns_menu1);
          //================================
          // int previous_pos = zoom_current;
          // // setting lens to starting position
          // printMoveSteps(-1, preset1_2, CADETBLUE, 2);
          // moving motor, haven start pattern yet so use default motor speed 
          // moveMotor(ZOOM, focus_range,0);
          // zoom_current = zoom_range;
          // updateScreen(100);
          // countdownMenu();
          // goDist(ZOOM, preset1_2, 0, AZURE, ((float)3/4)*motor_time, false,false,true);
          // goDist(ZOOM, preset1_2, previous_pos, AZURE, ((float)1/4)*motor_time,false,true,false);
          // goDist(ZOOM, preset1_2, 0, AZURE, motor_time, 1.33,false,false,true);
          // goDist(ZOOM, preset1_2, previous_pos, AZURE,motor_time,4,false,true,false);
          // fixed_paterns_menu1 = resetScreen(fixed_paterns_menu1);
          break;
        }
        //ZoomBlur min === zoom to min
        case 3: {
          Serial.println("ZoomBlur min");
          countdownMenu();
          //global motor time pass in by default
          //return to starting position by default
          goDist(ZOOM, preset1_3, 0, SNOW, motor_time,1,true,true,true);
          fixed_paterns_menu1 = resetScreen(fixed_paterns_menu1);
          //========================================
          // int previous_zoom_pos = zoom_current;
          // int previous_focus_pos = focus_current;
          // setting lens to starting position
          // printMoveSteps(-1, preset1_3, CADETBLUE, 2); // setting lens to starting position
          //asume going back part of motor time
          // updateScreen(100);
          // countdownMenu();
          // goMultiDist(preset1_2, zoom_range, focus_range, VIOLET, motor_time,2, false, false,true);
          // goMultiDist(preset1_2, previous_zoom_pos, previous_focus_pos, VIOLET, motor_time,2, false, true,false);
          // fixed_paterns_menu1 = resetScreen(fixed_paterns_menu1);
          break;
        }

        //fixed_paterns_menu2
        case 4: {
          switch (fixed_paterns_menu2) {     
            //Zoomblue-max === zoom to max
            case 0: {
              //Serial.println("Zoom to max");
              countdownMenu();
              //return to starting position by default
              goDist(ZOOM, preset2_0, zoom_range, SNOW, motor_time,1,true,true,true);
              fixed_paterns_menu2 = resetScreen(fixed_paterns_menu2);
              break;
            }
              //SinWave-1 assume focus - small to wide wave
              //assume 3 wave motion
              case 1: {
              int initial_pos = focus_current;
              int min_distance_range = initial_pos - 0;
              int max_distance_range = focus_range - initial_pos;
              
              int total_waves = 3;
              int initial_wave = 1;
              int last_wave = 3;
              countdownMenu();
              for (int cur_wave = 1; cur_wave <= total_waves; cur_wave++) {
                int max_direction_to_travel = initial_pos + ((max_distance_range / total_waves) * cur_wave);
                int min_direction_to_travel = initial_pos - ((min_distance_range / total_waves) * cur_wave);
                //trial and error (unless got better idea)
                int wave_motor_div = total_waves * 12;
                //first time initiate start sound and keep screen there
                if(cur_wave == initial_wave){
                  goDist(FOCUS, preset2_1, max_direction_to_travel, SNOW, motor_time,wave_motor_div,false,false,true);
                  goDist(FOCUS, preset2_1, initial_pos, SNOW, motor_time,wave_motor_div,false,false,false);
                  goDist(FOCUS, preset2_1, min_direction_to_travel, SNOW, motor_time,wave_motor_div,false,false,false);
                  goDist(FOCUS, preset2_1, initial_pos, SNOW, motor_time,wave_motor_div,false,false,false);
                }
                //last wave
                else if(cur_wave == last_wave){
                  goDist(FOCUS, preset2_1, max_direction_to_travel, SNOW, motor_time,wave_motor_div,false,false,false);
                  goDist(FOCUS, preset2_1, initial_pos, SNOW, motor_time,wave_motor_div,false,false,false);
                  goDist(FOCUS, preset2_1, min_direction_to_travel, SNOW, motor_time,wave_motor_div,false,false,false);
                  //last movement and press close shutter
                  goDist(FOCUS, preset2_1, initial_pos, SNOW, motor_time,wave_motor_div,false,true,false);
                }
                //rest of the waves
                else{
                  goDist(FOCUS, preset2_1, max_direction_to_travel, SNOW, motor_time,wave_motor_div,false,false,false);
                  goDist(FOCUS, preset2_1, initial_pos, SNOW, motor_time,wave_motor_div,false,false,false);
                  goDist(FOCUS, preset2_1, min_direction_to_travel, SNOW, motor_time,wave_motor_div,false,false,false);
                  goDist(FOCUS, preset2_1, initial_pos, SNOW, motor_time,wave_motor_div,false,false,false);
                }
              }

              fixed_paterns_menu2 = resetScreen(fixed_paterns_menu2);
              break;
            }
            //SinWave-2 - wide to small wave
            case 2: {
              int initial_pos = focus_current;
              int min_distance_range = initial_pos - 0;
              int max_distance_range = focus_range - initial_pos;
              int total_waves = 3;
              int initial_wave = 3;
              int last_wave = 1;
              countdownMenu();
              for (int cur_wave = 3; cur_wave >= last_wave; cur_wave--) {
                int max_direction_to_travel = initial_pos + ((max_distance_range / total_waves) * cur_wave);
                int min_direction_to_travel = initial_pos - ((min_distance_range / total_waves) * cur_wave);
                //trial and error (unless got better idea)
                int wave_motor_div = total_waves * 12;

                //first time initiate start sound and keep screen there
                if(cur_wave == initial_wave){
                  goDist(FOCUS, preset2_1, max_direction_to_travel, SNOW, motor_time,wave_motor_div,false,false,true);
                  goDist(FOCUS, preset2_1, initial_pos, SNOW, motor_time,wave_motor_div,false,false,false);
                  goDist(FOCUS, preset2_1, min_direction_to_travel, SNOW, motor_time,wave_motor_div,false,false,false);
                  goDist(FOCUS, preset2_1, initial_pos, SNOW, motor_time,wave_motor_div,false,false,false);
                }
                //last wave
                else if(cur_wave == last_wave){
                  goDist(FOCUS, preset2_1, max_direction_to_travel, SNOW, motor_time,wave_motor_div,false,false,false);
                  goDist(FOCUS, preset2_1, initial_pos, SNOW, motor_time,wave_motor_div,false,false,false);
                  goDist(FOCUS, preset2_1, min_direction_to_travel, SNOW, motor_time,wave_motor_div,false,false,false);
                  //last movement and press close shutter
                  goDist(FOCUS, preset2_1, initial_pos, SNOW, motor_time,wave_motor_div,false,true,false);
                }
                //rest of the waves
                else{
                  goDist(FOCUS, preset2_1, max_direction_to_travel, SNOW, motor_time,wave_motor_div,false,false,false);
                  goDist(FOCUS, preset2_1, initial_pos, SNOW, motor_time,wave_motor_div,false,false,false);
                  goDist(FOCUS, preset2_1, min_direction_to_travel, SNOW, motor_time,wave_motor_div,false,false,false);
                  goDist(FOCUS, preset2_1, initial_pos, SNOW, motor_time,wave_motor_div,false,false,false);
                }
              }
              fixed_paterns_menu2 = resetScreen(fixed_paterns_menu2);
              break;
            }
            
            //show fixed_paterns_menu2
            default:
              custome_movements_menu2_screen(3, preset2_name, preset2_menu,DEEPPINK);
              fixed_paterns_menu2 = get_custom_movements_menu2_update(fixed_paterns_menu2);
              if(fixed_paterns_menu2 == -2){
                    fixed_paterns_menu1 = -1;
                    fixed_paterns_menu2 = -1;
              }
              break;        
          }
          break;
        }

        //show fixed_paterns_menu1
        default:
          custome_movements_menu1_screen(4, preset1_name, preset1_menu,DEEPPINK);
          fixed_paterns_menu1 = get_custom_movements_menu1_update(fixed_paterns_menu1);
          break;
      }
      break;
    }

    //Home page
    default:
        home_menu_screen(3,home_header,home_menu,DEEPPINK);  
        home_screen = get_HomeMenu_Update(home_screen);
        break;
  }

}
