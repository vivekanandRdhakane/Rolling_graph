//Author: Vivekanand Dhakane
//updated on: 10 June 2020

#include <Wire.h>
#include <avr/pgmspace.h>
#include <MsTimer2.h>
#define OLED_I2c_ADDRESS          0x3C
// registers Addresses
#define COMMAND_REG         0x80  // B1000 0000
#define DATA_REG       0x40  // B0100 0000
// commands
#define ON_CMD              0xAF
#define NORMAL_DISPLAY_CMD  0xA6
#define PAGE_ADDRESSING_MODE  0x02

#define Char_Verticle_Pages_Required 1
#define Char_Horizontal_Columns_Required 8

#define No_of_bytes_Char 9
#define First_char_ascii_code 48


// commands
#define RIGHT_SCROLL_CMD       0x26
#define LEFT_SCROLL_CMD       0x27
#define DEACTIVATE_SCROLL_CMD  0x2E
#define SET_BRIGHTNESS_CMD   0x81
#define SET_VERTICAL_SCROLL_AREA_CMD 0xA3
#define VERTICAL_RIGHT_SCROLL_CMD  0x29
#define VERTICAL_LEFT_SCROLL_CMD   0x2A

#define ACTIVATE_SCROLL_CMD    0x2F
#define Max_Y 64
#define ROLL_SPEED   5


byte count;
byte count2;
byte count3;

bool get_sample = false;
long previous_millis = 0;
byte select_speed[] = {3, 2, 1, 0, 6, 5, 4, 7};
int sampling_period[] = {1150, 580, 280, 50, 40, 30, 24, 14};
float TimeScale[] = {148, 74, 37, 7, 5.75, 4.6, 3.5, 2.3};
//const byte data[] PROGMEM{};
const byte data[] PROGMEM{
  0x04, 0x1C, 0x63, 0x41, 0x7F, 0x00, 0x00, 0x00, 0x00,  // Code for char 0
  0x03, 0x00, 0x02, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char 1
  0x04, 0x61, 0x51, 0x49, 0x4F, 0x00, 0x00, 0x00, 0x00,  // Code for char 2
  0x04, 0x00, 0x49, 0x49, 0x77, 0x00, 0x00, 0x00, 0x00,  // Code for char 3
  0x05, 0x30, 0x28, 0x26, 0x7F, 0x20, 0x00, 0x00, 0x00,  // Code for char 4
  0x04, 0x00, 0x4F, 0x49, 0x79, 0x00, 0x00, 0x00, 0x00,  // Code for char 5
  0x04, 0x3C, 0x4A, 0x49, 0x79, 0x00, 0x00, 0x00, 0x00,  // Code for char 6
  0x04, 0x01, 0x41, 0x39, 0x07, 0x00, 0x00, 0x00, 0x00,  // Code for char 7
  0x04, 0x36, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00, 0x00,  // Code for char 8
  0x04, 0x06, 0x49, 0x49, 0x3F, 0x00, 0x00, 0x00, 0x00,  // Code for char 9
  0x01, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char :
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ;
  0x01, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char <
  0x05, 0x00, 0x28, 0x28, 0x28, 0x28, 0x00, 0x00, 0x00,  // Code for char =
  0x04, 0x00, 0x24, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00,  // Code for char >
  0x04, 0x00, 0x51, 0x0D, 0x02, 0x00, 0x00, 0x00, 0x00,  // Code for char ?
  0x08, 0x00, 0x7C, 0xDA, 0xA5, 0xA5, 0xBD, 0x22, 0x1C,  // Code for char @
  0x06, 0x60, 0x18, 0x17, 0x16, 0x38, 0x40, 0x00, 0x00,  // Code for char A
  0x05, 0x00, 0x7F, 0x49, 0x4F, 0x30, 0x00, 0x00, 0x00,  // Code for char B
  0x05, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x00, 0x00, 0x00,  // Code for char C
  0x06, 0x00, 0x7F, 0x41, 0x41, 0x23, 0x1E, 0x00, 0x00,  // Code for char D
  0x04, 0x00, 0x7F, 0x49, 0x49, 0x00, 0x00, 0x00, 0x00,  // Code for char E
  0x04, 0x00, 0x7F, 0x09, 0x09, 0x00, 0x00, 0x00, 0x00,  // Code for char F
  0x06, 0x1C, 0x22, 0x41, 0x49, 0x49, 0x38, 0x00, 0x00,  // Code for char G
  0x06, 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, 0x00,  // Code for char H
  0x02, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char I
  0x02, 0x40, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char J
  0x05, 0x00, 0x7F, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00,  // Code for char K
  0x04, 0x00, 0x7F, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00,  // Code for char L
  0x07, 0x00, 0x7F, 0x0C, 0x30, 0x30, 0x0C, 0x7F, 0x00,  // Code for char M
  0x06, 0x00, 0x7F, 0x06, 0x18, 0x20, 0x7F, 0x00, 0x00,  // Code for char N
  0x06, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x3E, 0x00, 0x00,  // Code for char O
  0x05, 0x00, 0x7F, 0x11, 0x11, 0x0E, 0x00, 0x00, 0x00,  // Code for char P
  0x06, 0x1C, 0x22, 0x41, 0x41, 0x41, 0xBE, 0x00, 0x00,  // Code for char Q
  0x05, 0x00, 0x7F, 0x09, 0x19, 0x66, 0x00, 0x00, 0x00,  // Code for char R
  0x04, 0x00, 0x47, 0x49, 0x71, 0x00, 0x00, 0x00, 0x00,  // Code for char S
  0x05, 0x01, 0x01, 0x7F, 0x01, 0x01, 0x00, 0x00, 0x00,  // Code for char T
  0x05, 0x00, 0x3E, 0x40, 0x40, 0x7E, 0x00, 0x00, 0x00,  // Code for char U
  0x05, 0x03, 0x1C, 0x60, 0x38, 0x07, 0x00, 0x00, 0x00,  // Code for char V
  0x08, 0x03, 0x3C, 0x60, 0x1C, 0x07, 0x38, 0x70, 0x0F,  // Code for char W
  0x05, 0x40, 0x23, 0x1C, 0x36, 0x41, 0x00, 0x00, 0x00,  // Code for char X
  0x05, 0x01, 0x06, 0x78, 0x06, 0x01, 0x00, 0x00, 0x00,  // Code for char Y
  0x05, 0x41, 0x71, 0x49, 0x47, 0x41, 0x00, 0x00, 0x00   // Code for char Z
};


void timerIsr() // Timer interupt function
{
  get_sample = true;
}

void setup() {
  oled_begin();
  Serial.begin(9600);
  MsTimer2::set(sampling_period[ROLL_SPEED], timerIsr); // 500ms period

  print_string("TIME=" , 30, 0);
  print_string(String(TimeScale[ROLL_SPEED], 1) + " SEC" , 65, 0);

  scrollLeft(0x01, 0x07, select_speed[ROLL_SPEED]);
  MsTimer2::start();
  Serial.println("TIME Attached");
}


void loop() {

  float seconds = millis() / 1000.0;

  //Sine wave generated in count variable
  //Formula : x = A sin(2πt/T)+ offset
  count = 18 * sin(2 * PI * (seconds / 4.6)) + 28;
  count2 =18 * sin(PI+2*PI*(seconds/4.6))+ 28;

  previous_millis = millis();

  if (get_sample) //get_sample is set TRUE by timer interrupt
  {
    //First clear the column
    for (byte i = 1; i < 8; i++)
    {
      setCursor(127, i);
      writeData(0x00); // clear the column of 8 pixels
    }
    //Now plot the point
    set_pixal(127, count);
    set_pixal(127, count2);

    get_sample = false;
  }
}

void set_pixal(byte x_cord, byte y_cord)
{
  byte x_mod, y_mod, pixel_location, data, y_mod_page;




  y_cord = 63 - y_cord;
  x_mod = x_cord;
  y_mod_page = (y_cord) / 8;
  pixel_location = y_cord % 8;
  data =  0B00000001 << pixel_location;
  setCursor(x_mod, y_mod_page);
  writeData(data);




}


void scrollLeft(byte start, byte end, byte speed) {

  writeCommand(LEFT_SCROLL_CMD);  //Horizontal Scroll Setup
  writeCommand(0x00);  // dummy byte
  writeCommand(start); // start page address
  writeCommand(speed); // set time interval between each scroll
  writeCommand(end); // end page address

  writeCommand(0x01);
  writeCommand(0xFF);

  writeCommand(0x2f);  //active scrolling

}







byte print_char(char ch, byte x_cord, byte y_cord)
{
  //if ch is . OR SPACE
  if (ch == '.')
  {
    ch = '<';
  }
  else if (ch == ' ')
  {
    ch = ';';
  }



  byte ascii_to_array_index = ch - First_char_ascii_code;
  int char_start_in_array  = ascii_to_array_index * No_of_bytes_Char + 1;
  byte spacing;
  for (byte j = x_cord; j < x_cord + Char_Horizontal_Columns_Required; j++)
  {
    for (byte i = y_cord; i < y_cord + Char_Verticle_Pages_Required; i++)
    {
      setCursor(j, i);
      int array_index = char_start_in_array + (j - x_cord) * Char_Verticle_Pages_Required + (i - y_cord);
      byte char_byte = pgm_read_byte(&data[array_index]);
      writeData(char_byte);
    }
  }

  spacing = pgm_read_byte(&data[char_start_in_array - 1]);
  return spacing;
}

void print_string(String string, byte x, byte y)
{
  byte spacing = 0;
  byte x_cord = x;
  for (int i = 0; i < string.length(); i++)
  {
    char ch  = string[i];

    x_cord = x_cord + spacing + 1; //i* Char_Horizontal_Columns_Required;

    spacing = print_char(ch, x_cord, y);
  }
}
void oled_begin()
{
  Wire.begin(); // join i2c bus (address optional for master)
  turnON();
  NormalDisplayMode();
  setPageMode();
  writeCommand(0x8d); //Charge Pump
  writeCommand(0x14);
  clearFullDisplay();
}


void writeCommand(byte command) {
  Wire.beginTransmission(OLED_I2c_ADDRESS); // begin transmitting  (OLED_I2c_ADDRESS=0x3C)
  Wire.write(COMMAND_REG);                  // Command Reg Address (COMMAND_REG     =0x80 )
  Wire.write(command);                      // Command to be excuted
  Wire.endTransmission();                   // stop transmitting
}

void writeData(byte data) {
  Wire.beginTransmission(OLED_I2c_ADDRESS); // begin transmitting   (OLED_I2c_ADDRESS=0x3C)
  Wire.write(DATA_REG);                     //Data Reg Address      (DATA_REG        =0x40)
  Wire.write(data);                         //Write data
  Wire.endTransmission();                   // stop transmitting
}

void turnON() {
  writeCommand(ON_CMD);
}
void NormalDisplayMode() {

  writeCommand(NORMAL_DISPLAY_CMD);

}

void setPageMode() {
  byte addressingMode = PAGE_ADDRESSING_MODE;
  writeCommand(0x20);        //set addressing mode
  writeCommand(PAGE_ADDRESSING_MODE);   //set page addressing mode
}



//clearFullDisplay()
// In this function we are setting X,Y=0
// then write data on 128 column and
// then change page and do same till 7th page
void clearFullDisplay() {
  for (byte page = 0; page < 8; page++) {

    setCursor(0, page);
    for (byte column = 0; column < 128; column++) { //clear all columns
      writeData(0x00);
    }

  }
  setCursor(0, 0);
}

void setCursor(byte X, byte Y) {
  writeCommand(0x00 + (X & 0x0F));        //set column lower address
  writeCommand(0x10 + ((X >> 4) & 0x0F)); //set column higher address
  writeCommand(0xB0 + Y);                 //set page address

}
