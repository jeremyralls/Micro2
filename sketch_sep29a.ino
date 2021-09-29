boolean toggle1 = 0;
boolean flag1 = false;
boolean flag2 = false;
int count = 0;

void setup(){

  //set pins as outputs
  pinMode(2, OUTPUT);
  pinMode(11, INPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(6, OUTPUT);

  Serial.begin(9600);

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
  
  if(flag1 == false) {
    if (toggle1){
      digitalWrite(2,HIGH);
      toggle1 = 0;
    }
    else{
      digitalWrite(2,LOW);
      toggle1 = 1;
    }
  }
  else {
    if(toggle1 == 0) {
      count++;
      toggle1 = 1;
      Serial.println(count+1);
    }
    else{
      toggle1 = 0;
      count++;
    }
  }
}

void loop(){
  //do other things here
  if(digitalRead(11) == HIGH) {
    flag1 = true;
  }

  if(count <= 20 && flag1 == true) {
    digitalWrite(2, HIGH);
    if(count == 18) {
      digitalWrite(6, HIGH);
    }
  }
  else if (count <= 40 && flag1 == true){
    if(count == 21) {
      digitalWrite(6, LOW);
    }
    digitalWrite(2, LOW);
    digitalWrite(4, HIGH);
    if(count == 38) {
      digitalWrite(6, HIGH);
    }
  }
  else if(count <= 43 && flag1 == true) {
    digitalWrite(4, LOW);
    digitalWrite(3, HIGH);
  }
  else {
    digitalWrite(6, LOW);
    digitalWrite(3, LOW);
    count = 0;
  }
  
  if (toggle1) {
    
  }
  else {
  }
}
