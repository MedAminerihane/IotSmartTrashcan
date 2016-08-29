//http://www.seeedstudio.com/wiki/LinkIt_ONE_Tutorial_-_Mini_Servo

int trig = 3; 
int echo = 5; 
long lecture_echo; 
long cm; 

int proxiSensorVal = 0; 
bool machineFull=false;
int acceptedValue = 10;//Distance en CM

int nbrDeScan;                 // variable to read the value from the analog pin
   
   void setup()
   {
     Serial.begin(9600);  
     nbrDeScan=0;
   }
   
   void loop() {
       proxiSensorVal =distanceCan();
       //Serial.println(proxiSensorVal);
       if(proxiSensorVal<acceptedValue){
       nbrDeScan=nbrDeScan+1;
       Serial.println("nbrDeScan= "+nbrDeScan);
       delay(1500);
       }
       if(nbrDeScan==4){;
       Serial.println("poubelle pleine !!! ");
       nbrDeScan=nbrDeScan+1;
       }
       
   }
   
   int distanceCan(){
    digitalWrite(trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trig, LOW); 
  lecture_echo = pulseIn(echo, HIGH); 
  cm = lecture_echo / 58; 
  //Serial.println(cm);
  return cm;
  }
