/****************************   Emplacement Des PINs  ********************************************/
/*
L'afficheur se place grace a sa connectique dans les ports :SCL|SDA|5V|GND

l'antenne gsm se met dans :GSM/ANT

A0 ==> bleu du capteur de metal(noir pour masse et marron pour 5v)

A1 ==> VIDE

A1 ==> VIDE

Si on met le clavier devant en face de nos yeux et que le cable gris est a droite alors :
D2 ==> D9   :::   Gris ==> marron

D10 ==> Orangé Data du servo moteur ( le marron pour la masse ,  rouge pour le 5v )

D11 ==> Trig de capteur de proximité
D12 ==> Echo de capteur de proximité

D13 ==> "VIDE"


*/
/****************************   Emplacement Des PINs  ********************************************/



#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>
#include "rgb_lcd.h"

//2 libraries for sms
#include <LTask.h>
#include <LGSM.h>

/**********************************************************/
//les etats de la machine:

bool machineFull=false;
bool adminNotified=false;

bool canAvailable=false;

bool canAccepted=false;
int nbrCansByUser=0;

bool firstValidation=false;
bool secondValidation=false;

String userNum;

String SMS;



/**********************************************************/
String inData;

// définition des broches utilisées 
int trig = 11; 
int echo = 12; 
long lecture_echo; 
long cm;

int fullValue = 10;//Distance en CM
int acceptedValue = 40;//Distance en CM

// Declaration du keypad 4X4
const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad

//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols]=
{
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};
//Code that shows the the keypad connections to the arduino terminals   2==>9  9==>2
byte rowPins[numRows] = {10,8,7,6}; //Rows 0 to 3
byte colPins[numCols]= {5,4,3,2}; //Columns 0 to 3
//initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);


rgb_lcd lcd;

Servo myservo;              // create servo object to control a servo

//const int resetButton = 11;     // pin bouton RESET
//int buttonResetState = 0;         // variable for reading the RESET status

   

int metalSensorPin = A0;    // pin metal sensor
int metalSensorVal = 0;       // valeur que retourne le sensor de metal 60 - 125
/****** Moch lezem ************/
int proxiSensorPin = A1;    // pin metal sensor
int proxiSensorVal = 0;       // valeur que va retourner le sensor de proximité quand une cannete tembe

int proxiFullSensorPin = A2;    // pin metal sensor
int proxiFullSensorVal = 0;       // valeur que va retourner le sensor de proximité quand la machine est pleine
/****** Moch lezem ************/





void setup() {
 
   Serial.begin(9600); 
   while(!LSMS.ready())
    {
        delay(1000);
    }
    
    Serial.println("GSM OK!!");

  userNum=String("");
  
  pinMode(trig, OUTPUT); 
  digitalWrite(trig, LOW); 
  pinMode(echo, INPUT); 
   
   
  myservo.attach(9); // attaches the servo on pin 9 to the servo object
  
  lcd.begin(16, 2);  // set up the LCD's number of columns and rows:
  lcd.print("Bienvenue a vous!!!");
  lcd.setRGB(000, 000, 255);

  //pinMode(resetButton, INPUT); // initialize le bouton reset comme entrée 
  

  
}

void loop() {

    char p_num[20];
            int len = 0;
            char dtaget[500];
  if(LSMS.available()) // Check if there is new SMS
    {
    
        LSMS.remoteNumber(p_num, 20); // display Number part
        Serial.println("There is new message.");
        
        Serial.print("Number:");
        Serial.println(p_num);
        Serial.print("Content:"); // display Content part     

        while(true)
        {
            int v = LSMS.read();
            if(v < 0)
            break;

            dtaget[len++] = (char)v;
            Serial.print((char)v);
        }

        Serial.println();
        LSMS.flush(); // delete message

        
        if((dtaget[0] == 'O' && dtaget[1] == 'K') || (dtaget[0] == 'o' && dtaget[1] == 'k'))
        {
            SMS="OK";
        }

        if((dtaget[0] == 'N' && dtaget[1] == 'O') || (dtaget[0] == 'n' && dtaget[1] == 'o'))
        {
            SMS="OK";
        }
    }
  
  //   ||  PARTIE  proxiFullSensorPin ||   ||  PARTIE  proxiFullSensorPin ||   ||  PARTIE  proxiFullSensorPin ||   ||  PARTIE  proxiFullSensorPin ||   ||  PARTIE  proxiFullSensorPin ||
  //si la machine n'est pas pleine ecouter le proxiFullSensorPin 
  if(!machineFull){
     proxiFullSensorVal = analogRead(proxiFullSensorPin);
     if(proxiFullSensorVal>fullValue){
     machineFull=true;  
     }
    }
   //si la machine est pleine et que l'admin n'a pas été notifié , le notifier et faire le scroll text et le colorier aussi 
  if(machineFull&&!adminNotified){
    /***************************************************************/
    //Ici notifier  l'admin
   /***************************************************************/ 
    //allumer l'ecran en rouge
    //ecrire sur l'ecran machine pleine
    lcd.setRGB(255, 000, 000);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print("Machine pleine!");
    adminNotified=true;
    scrollText();
    }
  //   ||  PARTIE  proxiFullSensorPin ||   ||  PARTIE  proxiFullSensorPin ||   ||  PARTIE  proxiFullSensorPin ||   ||  PARTIE  proxiFullSensorPin ||   ||  PARTIE  proxiFullSensorPin ||   ||  PARTIE  proxiFullSensorPin ||  
   
   
     if(!machineFull){
   
       
       //ouvrir le clapet si une canette est presente si non la fermer
       metalSensorVal = analogRead(metalSensorPin);
       if(metalSensorVal<80){
        canAvailable=true; 
       }
       else if(metalSensorVal>120){
       canAvailable=false; 
       }
       if(canAvailable){
         //Serial.print("ok");
         myservo.write(20); 
         
         
         }
       else if(!canAvailable){
         //Serial.print( "no");
         //myservo.write(10); 
         }
         
         
       //les actions quand une cannette passe devant le capteur de proximité 
       
       proxiSensorVal =distanceCan();
       Serial.println(proxiSensorVal);
       if(proxiSensorVal<acceptedValue){
       
         myservo.write(110);
       canAccepted=true;
       canAvailable=false;
       }
       if(canAccepted && !secondValidation){
         nbrCansByUser=nbrCansByUser+1;
         lcd.setCursor(0, 0);
         delay(400);
         if(nbrCansByUser==1){
           lcd.print("                ");
           lcd.setCursor(0, 0);
           lcd.print("1 canette");
           }
         else{
           lcd.print("                ");
           lcd.setCursor(0, 0);
           lcd.print(nbrCansByUser);
           lcd.print(" canettes");
           }
           lcd.setCursor(0, 1);
           lcd.print("                ");
           lcd.setCursor(0, 0);
        lcd.setRGB(000, 255, 000);
        canAccepted=false;
        firstValidation=false;
        delay(400);
       }
       
       
       //les actions quand l'utilisateur valide
       char keypressed = myKeypad.getKey();
        if (keypressed != NO_KEY &&keypressed=='A')
       {
         //si la premiere fois qu'il valide
         if(!firstValidation&&!secondValidation){
           if(nbrCansByUser==0){
             lcd.setRGB(255, 000, 000);
             lcd.print("                ");
             lcd.setCursor(0, 0);
             lcd.print("Aucune cannette !!");
             delay(1500);
             lcd.setRGB(000, 000, 255);
             lcd.print("                ");
             lcd.setCursor(0, 0);
             lcd.print("Bienvenue a vous!!!");
           }
           else{
             lcd.setRGB(000, 255, 000);
             lcd.print("                ");
             lcd.setCursor(0, 0);
             lcd.print("Votre num :     ");
             lcd.setCursor(0, 1);
             lcd.print(userNum);
             firstValidation=true;
           }
         }
         //si l'utilisateur a valider son nomble de cannette il faut entrer 8 numeros
        else if(firstValidation&&!secondValidation){
          if(userNum.length()<8){
            lcd.setRGB(255, 000, 000);
            lcd.setCursor(0, 0);
             lcd.print("                ");
             lcd.setCursor(0, 0);
             lcd.print("Movais numero !!");
             delay(1000);
            lcd.setRGB(000, 255, 000);
             lcd.print("                ");
             lcd.setCursor(0, 0);
             lcd.print("Votre num :     ");
          }
          else{
             lcd.print("                ");
             lcd.setCursor(0, 0);
            lcd.print("Veillez         ");
            lcd.setCursor(0, 1);
            lcd.print("patienter ...   ");
           secondValidation=true; 
           /***************************************************/
           //ICI envoie de la requette a l'operateur
           /***************************************************/
          }
        }
         
       }
       //si il a valider une premiere fois , et que le num n'est pas superieur a 8 et qu'il saisi seulement des numero
       else if (firstValidation && !secondValidation && userNum.length()<=7 &&keypressed != NO_KEY &&keypressed!='A'&&keypressed!='B'&&keypressed!='C'&&keypressed!='D'&&keypressed!='*'&&keypressed!='#')
       {
         lcd.setCursor(0, 0);
             lcd.print("Votre num :     ");
         userNum=userNum+keypressed;
         Serial.println(userNum);
         lcd.print("                ");
           lcd.setCursor(0, 1);
           lcd.print(userNum);
         
       }
       //si il efface un num : don il a deja valider une fois et que le num n'est pas ==0
       else if (firstValidation &&!secondValidation  && keypressed=='B')
       {
         if( userNum!=0){
         userNum=userNum.substring(0,(userNum.length()-1));
             lcd.print("                ");
           lcd.setCursor(0, 1);
           lcd.print(userNum.substring(0,(userNum.length()-1)));
           }
           else{
             lcd.print("                  ");
           lcd.setCursor(0, 1);
           lcd.print("                   ");
             }
         
       }
       //tout annuler !!!
       else if (/*firstValidation && userNum!=0 &&*/ !secondValidation && keypressed=='C')
       {
         resetAllValues();
       }
       
       
       
       //Si il y'a une une reponse de l'operateur
       /********************************************/
       //ICI traiter la reponse de l'operateur puis faire la fonction resetAllValues
       if(secondValidation){
           if (SMS.equals("OK")) {
            Serial.println("C'est bon tout va bien !!!");
               lcd.setCursor(0, 0);
               lcd.print("Merci a vous           ");
               lcd.setCursor(0, 1);
               lcd.print("a la prochaine...      ");
               delay(2000);
            resetAllValues();
            nbrCansByUser=0;
            }
            else if (SMS.equals("NO")) {
               lcd.setCursor(0, 0);
               lcd.print("Désolé connexion       ");
               lcd.setCursor(0, 1);
               lcd.print("impossible             ");
               delay(2000);
               resetAllValues();
               nbrCansByUser=0;
              }
         
        }
       /********************************************/
      
      
      }// la fin de "if(!machineFull)"
   






 // read the state of the RESETbutton value:
 // buttonResetState = digitalRead(resetButton);
 // if (buttonResetState == HIGH) {
     /***************************************************************/
    //Ici faire le reset de la poubelle
    /***************************************************************/
  //}
}





















// LES FONCTIONS UTILES || LES FONCTIONS UTILES || LES FONCTIONS UTILES || LES FONCTIONS UTILES || LES FONCTIONS UTILES || LES FONCTIONS UTILES || LES FONCTIONS UTILES ||
//FAIRE SCROLLER LE TEXT
void scrollText(){
  while(true){
  // scroll 13 positions (string length) to the left
    // to move it offscreen left:
    for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
        // scroll one position left:
        lcd.scrollDisplayLeft();
        // wait a bit:
        delay(150);
    }
    // scroll 29 positions (string length + display length) to the right
    // to move it offscreen right:
    for (int positionCounter = 0; positionCounter < 29; positionCounter++) {
        // scroll one position right:
        lcd.scrollDisplayRight();
        // wait a bit:
        delay(150);
    }
    // scroll 16 positions (display length + string length) to the left
    // to move it back to center:
    for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
        // scroll one position left:
        lcd.scrollDisplayLeft();
        // wait a bit:
        delay(150);
    }
    // delay at the end of the full loop:
    delay(1000);
   }
}


//RETOURNE LA VALEUR EN CM DE L'OBJET DEVANT
int distanceCan(){
    digitalWrite(trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trig, LOW); 
  lecture_echo = pulseIn(echo, HIGH); 
  cm = lecture_echo / 58; 
  //Serial.println(cm);
  return cm;
  }

String readSMS(){
         //des truc de read SMS
           char p_num[20];
            int len = 0;
            char dtaget[500];
  if(LSMS.available()) // Check if there is new SMS
    {
    
        LSMS.remoteNumber(p_num, 20); // display Number part
        Serial.println("There is new message.");
        
        Serial.print("Number:");
        Serial.println(p_num);
        Serial.print("Content:"); // display Content part     

        while(true)
        {
            int v = LSMS.read();
            if(v < 0)
            break;

            dtaget[len++] = (char)v;
            Serial.print((char)v);
        }

        Serial.println();
        LSMS.flush(); // delete message

        
        if((dtaget[0] == 'O' && dtaget[1] == 'K') || (dtaget[0] == 'o' && dtaget[1] == 'k'))
        {
            SMS="OK";
        }

        if((dtaget[0] == 'N' && dtaget[1] == 'O') || (dtaget[0] == 'n' && dtaget[1] == 'o'))
        {
            SMS="OK";
        }
    }
  
  
  }


void resetAllValues(){
             lcd.print("                ");
           lcd.setCursor(0, 0);
           lcd.print("Bienvenue a vous!!!");
           lcd.setCursor(0, 1);
           lcd.print("                ");
            lcd.setRGB(000, 000, 255);
            
          
           canAvailable=false;
          
           canAccepted=false;
          
           firstValidation=false;
           secondValidation=false;
          SMS="";
           userNum="";
  }
  
  
