#include <StandardCplusplus.h>
#include <YCommunicator.h>


YCommunicator comm;

boolean YLED_Status=false;
boolean RLED_Status=false;
int YLED_Intensity = 0;
int YLED = 9;
int RLED = 4;



void setup ()
{
  Serial.begin(115200);
  
  comm = YCommunicator();
  comm.registerCallback(0x1,toggleYellowLED);
  comm.registerCallback(0x2,ledGame);
  comm.registerCallback(0x3,toggleRedLED);
  pinMode(YLED,OUTPUT);
  pinMode(RLED,OUTPUT);
  
}  // end of setup

void loop () {
  while (Serial.available()){
    comm.write(Serial.read());
  }
  
  
  while (comm.available()){
    Serial.print((byte)comm.read());
  }
  delay(30);
  
}

void ledGame(uint8_t type, uint8_t command, uint8_t * data, uint16_t data_size){
  
  short rawIntensity = 0;
  rawIntensity = (short)data[1] ;
  YLED_Intensity = map( (int)rawIntensity,0,100,0,255);
  updateLED();
 
}


void toggleYellowLED(uint8_t type, uint8_t command, uint8_t * data, uint16_t data_size){
 
  YLED_Status = !YLED_Status;
  updateLED();
 
}

void updateLED(){
  if (YLED_Status){
    analogWrite(YLED,YLED_Intensity);
  }else{
    analogWrite(YLED,0);
  }
}

void toggleRedLED(uint8_t type, uint8_t command, uint8_t * data, uint16_t data_size){
  RLED_Status = !RLED_Status;
  if (RLED_Status){
    digitalWrite(RLED,HIGH);
  }else{
    digitalWrite(RLED,LOW);
  }
 
}




