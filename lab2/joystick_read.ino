void setup() {
  // put your setup code here, to run once:
  pinMode(8, INPUT);
  pinMode(9, OUTPUT);
  pinMode(A0, INPUT); //up and down
  pinMode(A1, INPUT); //left adn right
  Serial.begin(9600);
  digitalWrite(9,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
 if (Serial.read()== 'b'){
  digitalWrite(9,HIGH);
  delay(200);
  digitalWrite(9,LOW);
 
 }
 if(analogRead(A0) < 400){
    Serial.write('s');
 }
 else if(analogRead(A0) > 600){
    Serial.write('w');
 }
 else if(analogRead(A1) < 400){
    Serial.write('a');
 }
 else if(analogRead(A1) > 600){
    Serial.write('d');
 }


}
