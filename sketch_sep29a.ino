boolean toggle1 = 0;    //used to toggle LEDs on and off and create loops using interrupts
boolean flag1 = false;  //used to read if button is pressed
boolean flag2 = false;  //used when debugging
int count = 0;          //used to count seconds after pattern is started

void setup(){


  pinMode(2, OUTPUT);   //sets pin 2 to output to contol red LED
  pinMode(11, INPUT);   //sets pin 11 to input to read the button press
  pinMode(4, OUTPUT);   //sets pin 4 to output to control green LED
  pinMode(3, OUTPUT);   //sets pin 3 to output to control yellow LED
  pinMode(6, OUTPUT);   //sets pin 6 to output to control buzzer

  Serial.begin(9600);   //allows for dubugging to console when needed in debugging process

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

}//end setup

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
  if(flag1 == false) {  //check if button flag has not been change to true
    if (toggle1){       //check if toggle is 0 or 1 to see if led needs to be on or off
      digitalWrite(2,HIGH); //turn on LED
      toggle1 = 0;          //change toggle to 0 to turn off LED next interrupt
    }
    else{
      digitalWrite(2,LOW);  //turn LED off
      toggle1 = 1;          //change toggle to 1 to turn on LED next interrupt
    }
  }
  //Pattern starts (after button is pushed)
  else {                //for when button is pressed
    if(toggle1 == 0) {  //check if LED is on or off from previous toggle or on a low value from the low interrupt
      count++;          //increment count
      toggle1 = 1;      //change to other toggle for low reading on interrupt
      Serial.println(count+1);  //used to debug and see if count is added properly
    }
    else{               //for when toggle is set high
      toggle1 = 0;      //set toggle to low
      count++;          //increment count
    }
  }
}

void loop(){
  //main code for traffic light control
  if(digitalRead(11) == HIGH) {     //check if button is pressed
    flag1 = true;                   //set button pressed flag to true
  }
  
  if(count <= 20 && flag1 == true) {    //execute for 20 seconds after button is pressed
    digitalWrite(2, HIGH);              //turn on red LED
    if(count == 18) {                   //activate buzzer for the last 3 seconds
      digitalWrite(6, HIGH);
    }
  }
  else if (count <= 40 && flag1 == true){   //execute for an additional 20 seconds after button is pressed and red state is done
    if(count == 21) {
      digitalWrite(6, LOW);                 //turn buzzer off after the 3 seconds
    }
    
    digitalWrite(2, LOW);                   //turn off red LED
    digitalWrite(4, HIGH);                  //turn on green LED
    if(count == 38) {
      digitalWrite(6, HIGH);                //activate buzzer for the last 3 seconds
    }
  }
  else if(count <= 43 && flag1 == true) {   //execute for an additional 3 seconds after button is pressed and green state is done (note that the buzzer doesn't turn off because it stays on for the entirety of the yellow state)
    digitalWrite(4, LOW);                   //turn off green LED
    digitalWrite(3, HIGH);                  //turn on yellow LED
  }
  else {                        //once 23 seconds have passed this will execute
    digitalWrite(6, LOW);       //turn off buzzer from yellow light
    digitalWrite(3, LOW);       //turn off yellow LED
    count = 0;                  //reset count to 0 so code loops back to 0 seconds
  }
}
