boolean toggle1 = 0;    //used to read if the buton is pressed
boolean flag1 = false;  //used to read if button is pressed ot start traffic light sequence
boolean flag2 = false;  //used when dubugging
int count = 0;          //used to count seconds using interupt to add to count

void setup(){


  pinMode(2, OUTPUT);   //sets pin 2 to output to contol RED LED
  pinMode(11, INPUT);   //sets pin 11 to input to read BUTTON PRESS
  pinMode(4, OUTPUT);   //sets pin 4 to output to control GREEN LED
  pinMode(3, OUTPUT);   //sets pin 3 to output to control YELLOW LED
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
  
  if(flag1 == false) {  //check if button flag hs not been change to true
    if (toggle1){       //check if toggle is 0 or 1 to see if led nedds to be on or off
      digitalWrite(2,HIGH); //turn on LED
      toggle1 = 0;          //change toggle to 0 to send to turning led off loop next interrupt
    }
    else{
      digitalWrite(2,LOW);  //turn LEd off
      toggle1 = 1;          //send back to turn LED on loop when interrupt triggers
    }
  }
  else {                //for when button is pressed
    if(toggle1 == 0) {  //chceck if LED is on or off from previous toggle or on a low value from the low interrupt
      count++;          //add to count for timing light chnages
      toggle1 = 1;      //change to other toggle for low reading on interrupt
      Serial.println(count+1);  //used to debug and see if count is added properly
    }
    else{               //for when toggle is set high
      toggle1 = 0;      //set toggle to low
      count++;          //add to count making count go up twice every second
    }
  }
}

void loop(){
  //main code for traffic light control
  if(digitalRead(11) == HIGH) {     //check if Botton is pressed
    flag1 = true;                   //chnge button pressed flag to true
  }
                                        //flag1 must be also be true or else code from before button 
                                        //press will ececute not this code even after button press
  if(count <= 20 && flag1 == true) {    //from button press to 10 seconds this will execute
    digitalWrite(2, HIGH);              //turn on RED LED
    if(count == 18) {                   //for last 3 seconds of red light
      digitalWrite(6, HIGH);            //activate buzzer
    }
  }
  else if (count <= 40 && flag1 == true){   //from 10 seconds to 20 seconds this will execute
    if(count == 21) {                       //during switch to green
      digitalWrite(6, LOW);                 //turn BUZZER off from red ligth warning 
    }
    digitalWrite(2, LOW);                   //turn off RED LED
    digitalWrite(4, HIGH);                  //turn on GREEN LED
    if(count == 38) {                       //for last 3 seconds of green light
      digitalWrite(6, HIGH);                //activate buzzer
    }
  }
  else if(count <= 43 && flag1 == true) {   //from 20 seconds to 23 seconds this will execute
    digitalWrite(4, LOW);                   //turn off GREEN LED
    digitalWrite(3, HIGH);                  //turn on YELLOW LED
  }
  else {                        //once 23 seconds have passed this will execute
    digitalWrite(6, LOW);       //turn off buzzer from yellow light
    digitalWrite(3, LOW);       //turn off YELLOW LED
    count = 0;                  //reset count to 0 so code loops back to 0 seconds
  }
  
  if (toggle1) {
    
  }
  else {
  }
}
