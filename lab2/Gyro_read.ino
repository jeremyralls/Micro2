#include<Wire.h>
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;


void setup() {
  // put your setup code here, to run once:
  pinMode(8, INPUT);
  pinMode(9, OUTPUT);
  pinMode(A4, INPUT); //up and down
  pinMode(A5, INPUT); //left adn right
  Serial.begin(9600);
  digitalWrite(9,LOW);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
}

void loop() {
  // put your main code here, to run repeatedly:

    Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  
 if (Serial.read()== 'b'){
  digitalWrite(9,HIGH);
  delay(200);
  digitalWrite(9,LOW);
 
 }
 if(AcX > 7000){
    Serial.write('s');
 }
 else if(AcX < -7000){
    Serial.write('w');
 }
 else if(AcY < -5000){
    Serial.write('a');
 }
 else if(AcY > 5000){
    Serial.write('d');
 }


}
