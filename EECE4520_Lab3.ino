#include <Wire.h>
#include <LiquidCrystal.h>
#include <DS3231.h>

// DC motor pins
#define ENABLE 5
#define DIRA 3
#define DIRB 4

// button pin
#define BUTTON 2

int i;
bool dFlag = false; // motor direction flag

// clock variables
DS3231 clock;
RTCDateTime dt;

// LCD interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// ISA variables
boolean toggle = 0;
int hour;
int minute;
int second;
bool direction1;
 
void setup() {
  // set DC motor directions
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);

  // set button input
  pinMode(BUTTON, INPUT);

  // initialize DS3231
  clock.begin();
  clock.setDateTime(__DATE__, __TIME__);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("Hello, World!");

  cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts
}

// Timer1's interrupt service routing (ISR)
// The code in ISR will be executed every time timer1 interrupt occurs
// That is, the code will be called once every second
// TODO
//   you need to set a flag to trigger an evaluation of the conditions
//   in your state machine, then potentially transit to next state
//
ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)

  //Startup (red LED flashing before button is pushed)
  if(toggle) {       //check if toggle is 0 or 1 to see if led needs to be on or off
    hour = dt.hour;
    minute = dt.minute;
    second = dt.second;
    direction1 = dFlag;
    toggle = 0;          //change toggle to 0 to turn off LED next interrupt
  }
  else{
    hour = dt.hour;
    minute = dt.minute;
    second = dt.second;
    direction1 = dFlag;
    toggle = 1;          //change toggle to 1 to turn on LED next interrupt
  }
}

void loop() {
    dt = clock.getDateTime();

    if(second == 0) {
      digitalWrite(ENABLE, HIGH);
    }
    if(second == 30) {
      digitalWrite(ENABLE, LOW);
    }

    if(digitalRead(BUTTON) == HIGH) {
      if(dFlag == true) {
        dFlag = false;
      }
      else {
        dFlag = true;
      }
    }

    if(dFlag == true) {
      digitalWrite(DIRB, LOW);
      digitalWrite(DIRA, HIGH);
    }
    else {
      digitalWrite(DIRA, LOW);
      digitalWrite(DIRB, HIGH);
    }

    // print time and motor state to LCD
    lcd.setCursor(0, 0);
    lcd.print(String(hour) + ":" + String(minute) + ":" + String(second) + ", Dir:");
    if(direction1 == true) {
      lcd.print("CC,");
    }
    else {
      lcd.print("C, ");
    }

    lcd.setCursor(0, 1);
    lcd.print("Spd: Full");
    
    delay(1000);
}
   
