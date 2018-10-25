#include <Wire.h>
#include <LiquidCrystal_I2C.h>   //library
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
#define DS1307_ADDRESS 0x68

byte zero = 0x00; //workaround for issue #527

void setup()  {
  Wire.begin();
  Serial.begin(9600);

  lcd.begin(16, 2);  // initialize the lcd for 16 chars 2 lines, turn on backlight                    
  lcd.backlight();
  // Print a message to the LCD.
  lcd.print("fayalab clock!");
  //lcd.setCursor(8, 1); //set Starting position
  //lcd.print("times"); //print counter
}

void loop()
{
  printDate();
  //First you need to setting the current time! then comment it like this afterward !
  setDateTime();  //SETtime
}

byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}

byte decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
  return ( (val/10*16) + (val%10) );
}


void printDate(){
  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);

  byte zero = 0x00;
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());
  int sp ;
  
  sp = hour*100 + minute;
  lcd.setCursor(0, 1); //set Starting position
  //lcd.print(year);
  //lcd.print("/");
  lcd.print(month);
  lcd.print("/");
  lcd.print(monthDay);
  lcd.print(" ");
  lcd.print(hour);
  lcd.print(":");
  lcd.print(minute);
  lcd.print(":");
  lcd.print(second);
  //lcd.print("");
}


void setDateTime(){

  byte second =      00; //0-59
  byte minute =      10; //0-59
  byte hour =        21; //0-23
  byte weekDay =     3; //1-7
  byte monthDay =    13; //1-31
  byte month =       7; //1-12
  byte year  =       16; //0-99

  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero); //stop Oscillator

  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(weekDay));
  Wire.write(decToBcd(monthDay));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));

  Wire.write(zero); //start 
  Wire.endTransmission();

}
