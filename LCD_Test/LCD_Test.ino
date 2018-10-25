/* YourDuino.com Example Software Sketch
  16 character 2 line I2C Display
  Backpack Interface labelled "A0 A1 A2" at lower right.
  ..and
  Backpack Interface labelled "YwRobot Arduino LCM1602 IIC V1"
  MOST use address 0x27, a FEW use 0x3F
  terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here:
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>

/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 16 chars 2 line display
// A FEW use address 0x3F
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

/*-----( Declare Variables )-----*/
//NONE
int bsel = 6;
int bup = 5;
int bdown = 7;
int x = 0;
int y = 0;
int stateBup = 0;
int stateBdown = 0;
int stateBsel = 0;
int a = 0;
int b = 0;

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  pinMode(bsel, INPUT);
  pinMode(bup, INPUT);
  pinMode(bdown, INPUT);

  Serial.begin(9600);  // Used to type in characters

  lcd.begin(16, 2);  // initialize the lcd for 16 chars 2 lines, turn on backlight

  // ------- Quick 3 blinks of backlight  -------------
  for (int i = 0; i < 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // finish with backlight on

  //-------- Write characters on the display ------------------
  // NOTE: Cursor Position: (CHAR, LINE) start at 0
  lcd.setCursor(0, 0); //Start at character 4 on line 0
  lcd.print("    < Menu >    ");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("HI!YourDuino.com");
  delay(500);

  // Wait and then tell user they can start the Serial Monitor and type in characters to
  // Display. (Set Serial Monitor option to "No Line Ending")
  lcd.clear();
  lcd.setCursor(0, 0); //Start at character 0 on line 0
  lcd.print("Use Serial Mon");
  lcd.setCursor(0, 1);
  lcd.print("Type to display");


}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  {

    stateBup = digitalRead(bup);
    stateBdown = digitalRead(bdown);
    stateBsel = digitalRead(bsel);
    if (stateBup == 1) {
      (x += 1);
      delay(100);
      Serial.println("bUP");
    }
    if (stateBdown == 1) {
      (x -= 1);
      delay(100);
      Serial.println("bDOWN");
    }
    if (stateBsel == 1) {
      (x = 0);
      delay(100);
      Serial.println("bSEL");
    }
    delay(100);
    // clear the screen
    lcd.clear();

    a = (x+48);
    lcd.write(a);
    Serial.println(a);
    delay(10);

  }
}


/* --(end main loop )-- */


/* ( THE END ) */

