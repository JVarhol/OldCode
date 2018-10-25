
/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>

/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 16 chars 2 line display
// A FEW use address 0x3F
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
#define DS1307_ADDRESS 0x68
byte zero = 0x00; //workaround for issue #527
/*-----( Declare Variables )-----*/
int blState = 1;
int bsel = 6;
int bup = 5;
int bdown = 7;
int stateBup = 0;
int stateBdown = 0;
int stateBsel = 0;
int a1Min = 0; //settable 1
int a2Min = 0; //settable 2
int a1Hour = 0;//settable 3
int a2Hour = 0;//settable 4
int a1Period = 1;
int a2Period = 1;
int aSetDisp;
int tPeriod;
int dayCheck;
int second;
int minute;
int hour;
int weekDay;
int monthDay;
int month;
int year;
int alarm1State = 0;
int alarm2State = 0;
int alarmPin = 4;
int alarm1LED = 11; //LED to indicate if alarm is on or off
int alarm2LED = 12; //LED to indicate if alarm is on or off
int alarm1SetLED = 8; //LED to indicate if alarm is set
int alarm2SetLED = 9; //LED to indicate if alarm is set
int alarmOffPin = 10; //Pin STATE GOES TO 1 then 0 to disable alarm




void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  //First you need to setting the current time! then comment it like this afterward !
  //setDateTime();  //SETtime
  pinMode(bsel, INPUT);
  pinMode(bup, INPUT);
  pinMode(bdown, INPUT);
  pinMode(alarmPin, OUTPUT);
  pinMode(alarm1LED, OUTPUT);
  pinMode(alarm2LED, OUTPUT);
  pinMode(alarm1SetLED, OUTPUT);
  pinMode(alarm2SetLED, OUTPUT);
  pinMode(alarmOffPin, INPUT);

  Serial.begin(9600);  // Used to type in characters

  lcd.begin(16, 2);  // initialize the lcd for 16 chars 2 lines, turn on backlight
  Wire.begin();

  blToggle();  //turn off backlight
  blToggle();  //turn on backlight

}/*--(end setup )---*/

byte bcdToDec(byte val)  {
  // Convert binary coded decimal to normal decimal numbers
  return ( (val / 16 * 10) + (val % 16) );
}

byte decToBcd(byte val) {
  // Convert normal decimal numbers to binary coded decimal
  return ( (val / 10 * 16) + (val % 10) );
}

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  clockfunc();
}

void clockfunc()
{
  {
    delay(150);
    stateBsel = digitalRead(bsel);

    if (stateBsel == 1) {
      delay(250);
      stateBsel = digitalRead(bsel);
      if (stateBsel == 1) {
        Menus();
      }
      blToggle();
      delay(25);
      Serial.println("bSEL");
    }
    delay(100);
    printDate();
    indicateLEDs();
  }

  //alarmTrigger();
  //alarmDeactivate();
  clockfunc();
}

void printDate() {
  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);

  byte zero = 0x00;
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  second = bcdToDec(Wire.read());
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  monthDay = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());
  int sp ;
  if (hour > 12) {
    hour = hour - 12;
    tPeriod = 2;
  } else {
    tPeriod = 1;
  }

  if (hour == 12) {
    hour = 12;
    tPeriod = 2;
  }
  if ((hour == 0)
     ) {
    hour = 12;
    tPeriod = 1;
  }

  if (weekDay != dayCheck) lcd.clear();
  dayCheck = weekDay;
  sp = hour * 100 + minute;
  lcd.setCursor(0, 0);
  if (weekDay == 1) lcd.print("Monday");
  if (weekDay == 2) lcd.print("Tuesday");
  if (weekDay == 3) lcd.print("Wednesday");
  if (weekDay == 4) lcd.print("Thursday");
  if (weekDay == 5) lcd.print("Friday");
  if (weekDay == 6) lcd.print("Saturday");
  if (weekDay == 7) lcd.print("Sunday");
  lcd.print(" ");
  if (month == 1) lcd.print("Jan");
  if (month == 2) lcd.print("Feb");
  if (month == 3) lcd.print("Mar");
  if (month == 4) lcd.print("Apr");
  if (month == 5) lcd.print("May");
  if (month == 6) lcd.print("Jun");
  if (month == 7) lcd.print("Jul");
  if (month == 8) lcd.print("Aug");
  if (month == 9) lcd.print("Sep");
  if (month == 10) lcd.print("Oct");
  if (month == 11) lcd.print("Nov");
  if (month == 12) lcd.print("Dec");
  lcd.print(" ");
  lcd.print(monthDay);
  lcd.setCursor(0, 1); //set Starting position

  lcd.print("  ");
  if (hour < 10) {
    lcd.print("0");
    lcd.print(hour);
  } else {
    lcd.print(hour);
  }
  lcd.print(":");
  if (minute < 10) {
    lcd.print("0");
    lcd.print(minute);
  } else {
    lcd.print(minute);
  }
  lcd.print(":");
  if (second < 10) {
    lcd.print("0");
    lcd.print(second);
  } else {
    lcd.print(second);
  }
  lcd.print("  ");
  if (tPeriod == 1) {
    lcd.print("AM");
  }
  if (tPeriod == 2) {
    lcd.print("PM");
  }
  //lcd.print("");
}


void setDateTime() {

  byte second =      00; //0-59
  byte minute =      8; //0-59
  byte hour =        20; //0-23
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




void blToggle() {  //toggles state of backlight based on value of blState - 1/0
  if (blState == 1) {
    blState = 0;
    lcd.noBacklight();
    Serial.println("bOff");
  } else {
    if (blState == 0) {
      blState = 1;
      lcd.backlight();
      Serial.println("bOn");
    }
  }
}

void Menus() {
  int MenuID = 0;
  lcd.clear();
  lcd.write("Main Menu");
  while (true) {


    stateBup = digitalRead(bup);
    stateBdown = digitalRead(bdown);
    stateBsel = digitalRead(bsel);

    if (stateBup == 1) {
      lcd.clear();
      (MenuID += 1);
      delay(200);
    }
    if (stateBdown == 1) {
      lcd.clear();
      (MenuID -= 1);
      delay(200);
    }
    if (stateBsel == 1) {
      if (MenuID == 1) setAlarm1();
      if (MenuID == 2) checkAlarms();
      if (MenuID == 3) toggleAlarms();
      if (MenuID == 4) {
        delay(100);
        lcd.clear();
        clockfunc();
      }
    }
    if (MenuID <= 0) MenuID = (0);
    if (MenuID >= 5) MenuID = (0);
    if (MenuID <= 0) {
      lcd.setCursor(0, 0);
      lcd.write("Main Menu");
    }
    if (MenuID == 1) {
      lcd.setCursor(0, 0);
      lcd.write("Main Menu");
      lcd.setCursor(0, 1);
      lcd.print("Set Alarms");
    }

    if (MenuID == 2) {
      lcd.setCursor(0, 0);
      lcd.write("Main Menu");
      lcd.setCursor(0, 1);
      lcd.print("Check Alarms");
    }

    if (MenuID == 3) {
      lcd.setCursor(0, 0);
      lcd.write("Main Menu");
      lcd.setCursor(0, 1);
      lcd.print("Alarms On/Off");
    }

    if (MenuID == 4) {
      lcd.setCursor(0, 0);
      lcd.write("Main Menu");
      lcd.setCursor(0, 1);
      lcd.print("Exit");
    }

  }
}
void setAlarm1() {
  int a1HourTemp;
  int a2HourTemp;

  if (a1Period == 1) {
    a2HourTemp = a2Hour;
  }
  if (a1Period == 2) {
    a1HourTemp = a1Hour + 12;
  }

  if (a2Period == 1) {
    a1HourTemp = a1Hour;
  }
  if (a2Period == 2) {
    a2HourTemp = a2Hour + 12;
  }
  lcd.clear();
  delay(100);
  int b;
  int a;

  stateBup = digitalRead(bup);
  stateBdown = digitalRead(bdown);
  stateBsel = digitalRead(bsel);

  if (stateBup == 1) {
    lcd.clear();
    (b += 1);
    delay(200);
  }
  if (stateBdown == 1) {
    lcd.clear();
    (b -= 1);
    delay(200);
  }
  a = 1;
  lcd.setCursor(0, 0);
  lcd.print("Alarm1 ");
  lcd.print(a1Hour);
  lcd.print(":");
  lcd.print(a1Min);
  lcd.print(" ");
  if (a1Period == 1) {
    lcd.print("AM");
  }
  if (a1Period == 2) {
    lcd.print("PM");
  }

  lcd.setCursor(0, 1);
  lcd.print("Alarm2 ");
  lcd.print(a2Hour);
  lcd.print(":");
  lcd.print(a2Min);
  lcd.print(" ");
  if (a2Period == 1) {
    lcd.print("AM ");
  }
  if (a2Period == 2) {
    lcd.print("PM ");
  }


  while (a == 1) {
    delay(100);
    if (a1HourTemp > 12) {
      a1Hour = a1HourTemp - 12;
      a1Period = 2;
    } else {
      a1Hour = a1HourTemp;
      a1Period = 1;
    }
    if (a1HourTemp == 12) {
      a1Hour = 12;
      a1Period = 2;
    }
    if (a1HourTemp == 0) {
      a1Hour = 12;
      a1Period = 1;
    }
    stateBup = digitalRead(bup);
    stateBdown = digitalRead(bdown);
    stateBsel = digitalRead(bsel);
    lcd.setCursor(0, 0);
    lcd.print("Alarm1 ");
    lcd.print(a1Hour);
    lcd.print(":");
    lcd.print(a1Min);
    lcd.print(" ");
    if (a1Period == 1) {
      lcd.print("AM  ");
    }
    if (a1Period == 2) {
      lcd.print("PM  ");
    }
    if (stateBsel == 1) {
      (a += 1);
      delay(200);
    }

    if (stateBup == 1) {
      (a1HourTemp += 1);
      delay(200);
      if (a1HourTemp == 24) a1HourTemp = 0;
    }
    if (stateBdown == 1) {
      (a1HourTemp -= 1);
      delay(200);
      if (a1HourTemp < 0) a1HourTemp = 23;
    }
  }
  while (a == 2) {
    delay(100);
    stateBup = digitalRead(bup);
    stateBdown = digitalRead(bdown);
    stateBsel = digitalRead(bsel);
    lcd.setCursor(0, 0);
    lcd.print("Alarm1 ");
    lcd.print(a1Hour);
    lcd.print(":");
    lcd.print(a1Min);
    lcd.print(" ");
    if (a1Period == 1) {
      lcd.print("AM  ");
    }
    if (a1Period == 2) {
      lcd.print("PM  ");
    }
    if (stateBsel == 1) {
      (a += 1);
      delay(200);
    }

    if (stateBup == 1) {
      (a1Min += 1);
      delay(200);
      if (a1Min == 60) a1Min = 0;
    }
    if (stateBdown == 1) {
      (a1Min -= 1);
      delay(200);
      if (a1Min < 0) a1Min = 59;
    }
  }

  while (a == 3) {
    delay(100);
    if (a2HourTemp > 12) {
      a2Hour = a2HourTemp - 12;
      a2Period = 2;
    } else {
      a2Hour = a2HourTemp;
      a2Period = 1;
    }
    if (a2HourTemp == 12) {
      a2Hour = 12;
      a2Period = 2;
    }
    if (a2HourTemp == 0) {
      a2Hour = 12;
      a2Period = 1;
    }
    stateBup = digitalRead(bup);
    stateBdown = digitalRead(bdown);
    stateBsel = digitalRead(bsel);
    lcd.setCursor(0, 1);
    lcd.print("Alarm2 ");
    lcd.print(a2Hour);
    lcd.print(":");
    lcd.print(a2Min);
    lcd.print(" ");
    if (a2Period == 1) {
      lcd.print("AM  ");
    }
    if (a2Period == 2) {
      lcd.print("PM  ");
    }
    if (stateBsel == 1) {
      (a += 1);
      delay(200);
    }

    if (stateBup == 1) {
      (a2HourTemp += 1);
      delay(200);
      if (a2HourTemp == 24) a2HourTemp = 0;
    }
    if (stateBdown == 1) {
      (a2HourTemp -= 1);
      delay(200);
      if (a2HourTemp < 0) a2HourTemp = 23;
    }
  }
  while (a == 4) {
    delay(100);
    stateBup = digitalRead(bup);
    stateBdown = digitalRead(bdown);
    stateBsel = digitalRead(bsel);
    lcd.setCursor(0, 1);
    lcd.print("Alarm2 ");
    lcd.print(a2Hour);
    lcd.print(":");
    lcd.print(a2Min);
    lcd.print(" ");
    if (a2Period == 1) {
      lcd.print("AM  ");
    }
    if (a2Period == 2) {
      lcd.print("PM  ");
    }
    if (stateBsel == 1) {
      (a += 1);
      delay(200);
      lcd.clear();
    }

    if (stateBup == 1) {
      (a2Min += 1);
      delay(200);
      if (a2Min == 60) a2Min = 0;
    }
    if (stateBdown == 1) {
      (a2Min -= 1);
      delay(200);
      if (a2Min < 0) a2Min = 59;
    }
  }

}


void checkAlarms() {
  stateBsel = 0;
  delay(100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("Alarm1: ");
  lcd.print(a1Hour);
  lcd.write(":");
  if (a1Min < 10) {
    lcd.print("0");
    lcd.print(a1Min);
  } else {
    lcd.print(a1Min);
  }
  lcd.print(" ");
  if (a1Period == 1) {
    lcd.print("AM  ");
  }
  if (a1Period == 2) {
    lcd.print("PM  ");
  }
  lcd.setCursor(0, 1);
  lcd.write("Alarm1: ");
  lcd.print(a2Hour);
  lcd.write(":");
  if (a2Min < 10) {
    lcd.print("0");
    lcd.print(a2Min);
  } else {
    lcd.print(a2Min);
  }
  lcd.print(" ");
  if (a2Period == 1) {
    lcd.print("AM  ");
  }
  if (a2Period == 2) {
    lcd.print("PM  ");
  }
  while (stateBsel == 0) {
    delay(200);
    stateBsel = digitalRead(bsel);
  }
  delay(200);
  lcd.clear();
}
void toggleAlarms() {
  lcd.clear();
  delay(100);
  int b;
  int a;

  stateBup = digitalRead(bup);
  stateBdown = digitalRead(bdown);
  stateBsel = digitalRead(bsel);

  if (stateBup == 1) {
    lcd.clear();
    (b += 1);
    delay(200);
  }
  if (stateBdown == 1) {
    lcd.clear();
    (b -= 1);
    delay(200);
  }
  a = 1;
  lcd.setCursor(0, 0);
  if (alarm1State == 0) lcd.print("Alarm 1 - Off  ");
  if (alarm1State == 1) lcd.print("Alarm 1 - On   ");
  lcd.setCursor(0, 1);
  if (alarm2State == 0) lcd.print("Alarm 2 - Off  ");
  if (alarm2State == 1) lcd.print("Alarm 2 - On   ");

  while (a == 1) {
    stateBup = digitalRead(bup);
    stateBdown = digitalRead(bdown);
    stateBsel = digitalRead(bsel);
    if (stateBsel == 1) {
      lcd.clear();
      (a += 1);
      delay(200);
    }
    lcd.setCursor(0, 0);
    if (alarm1State == 0) lcd.print("Alarm 1 - Off  ");
    if (alarm1State == 1) lcd.print("Alarm 1 - On   ");
    lcd.setCursor(0, 1);
    if (alarm2State == 0) lcd.print("Alarm 2 - Off  ");
    if (alarm2State == 1) lcd.print("Alarm 2 - On   ");

    if (stateBup == 1) {
      lcd.clear();
      (alarm1State += 1);
      delay(200);
    }
    if (stateBdown == 1) {
      lcd.clear();
      (alarm1State -= 1);
      delay(200);
    }
    if (alarm1State > 1) alarm1State = 0;
    if (alarm1State < 0) alarm1State = 1;

  }
  while (a == 2) {
    stateBup = digitalRead(bup);
    stateBdown = digitalRead(bdown);
    stateBsel = digitalRead(bsel);
    if (stateBsel == 1) {
      lcd.clear();
      (a += 1);
      delay(200);
    }
    lcd.setCursor(0, 0);
    lcd.setCursor(0, 0);
    if (alarm1State == 0) lcd.print("Alarm 1 - Off  ");
    if (alarm1State == 1) lcd.print("Alarm 1 - On   ");
    lcd.setCursor(0, 1);
    if (alarm2State == 0) lcd.print("Alarm 2 - Off  ");
    if (alarm2State == 1) lcd.print("Alarm 2 - On   ");


    if (stateBup == 1) {
      lcd.clear();
      (alarm2State += 1);
      delay(200);
    }
    if (stateBdown == 1) {
      lcd.clear();
      (alarm2State -= 1);
      delay(200);
    }
    if (alarm2State > 1) alarm2State = 0;
    if (alarm2State < 0) alarm2State = 1;
  }
  lcd.clear();

}
void alarmTrigger() {
  if (alarm1State == 1) {
    if (a1Hour == hour) {
      if (a1Period == tPeriod) {
        if (a1Min == minute) {
          digitalWrite(alarmPin, HIGH);
        }
      }
    }
  }
  if (alarm2State == 1) {
    if (a2Hour == hour) {
      if (a2Period == tPeriod) {
        if (a2Min == minute) {
          digitalWrite(alarmPin, HIGH);
        }
      }
    }
  }
}
void alarmDeactivate() {
  int offPinState;
  offPinState = digitalRead(alarmOffPin);
  if (offPinState == 1) delay(5000);
  offPinState = digitalRead(alarmOffPin);
  if (offPinState == 0) digitalWrite(alarmPin, LOW);
  delay(3000);
}

void indicateLEDs() {
  if (alarm1State == 1) digitalWrite(alarm1LED, HIGH); else digitalWrite(alarm1LED, LOW);
  if (alarm2State == 1) digitalWrite(alarm2LED, HIGH); else digitalWrite(alarm2LED, LOW);
  if (a1Hour == 0) digitalWrite(alarm1SetLED, HIGH); else digitalWrite(alarm1SetLED, LOW);
  if (a2Hour == 0) digitalWrite(alarm2SetLED, HIGH); else digitalWrite(alarm2SetLED, LOW);
}

