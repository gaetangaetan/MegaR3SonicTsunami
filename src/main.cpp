#include <Arduino.h>
#include <Ultrasonic.h>

#include <SoftwareSerial.h>

#include <SparkFun_Tsunami_Qwiic.h> //http://librarymanager/All#SparkFun_Tsunami_Super_WAV_Trigger

TsunamiQwiic tsunami;


// Initialise on software serial port.
// SoftwareSerial SoftSerial(6, 7);
// DY::Player player(&SoftSerial);

bool exclusiveplaying = true;
bool playing = false;
int trackplaying = 1;

#define MINDIST 2
#define MAXDIST 50
#define DELTAMAX 3
#define DELTAINC 3
#define NUMBEROFSONGS 8
#define NUMBERSAMPLES 0 //2 // nombre de mesures en dessous du threshold nécessaires (-1) (pour éviter les fausses alertes dues à des valeurs fantômes)
//#define THRESHOLD 30
#define ACTIVATIONTHRESHOLD 10



int samples =  0;

int THRESHOLD = 30;

/* les pins 30,32,...,48 sont assignées au pins TRIG (et ECHO) des capteurs*/

Ultrasonic ultrasonic1(30, 30);
Ultrasonic ultrasonic2(32, 32);
Ultrasonic ultrasonic3(34, 34);
Ultrasonic ultrasonic4(36, 36);
Ultrasonic ultrasonic5(38, 38);
Ultrasonic ultrasonic6(40, 40);
Ultrasonic ultrasonic7(42, 42);
Ultrasonic ultrasonic8(44, 44);
Ultrasonic ultrasonic9(46, 46);
Ultrasonic ultrasonic10(48, 48);




int samplestab[11];

int distance = 50;
int compteur = 0;

int song = 0;

void test1()
{
// Print the number of the sound that is playing.

  // digitalWrite(LED_BUILTIN,HIGH);
  // delay(500);
  // digitalWrite(LED_BUILTIN,LOW);
  // delay(500);

  int mesure = ultrasonic1.read();
  if((mesure-distance)>DELTAMAX)
  {
    distance+=DELTAINC;
  }
  else if((distance-mesure)>DELTAMAX)
  {
    distance-=DELTAINC;
  }

  distance = max(distance,MINDIST);
  distance = min(distance,MAXDIST);

    compteur=0;
      Serial.print("distance = ");
      Serial.println(distance);
  

  
  if(distance<30)
  {
    tsunami.trackPlaySolo(1+(song%8), 0); // track = 1 (aka "1.WAV"), output = 0 (aka "1L") 
    song++;
    Serial.println("triggered!");
    
    delay(2000);
  }
  
}


bool takeMesure(int sensornumber, int sensorvalue)
{
  
  if(sensorvalue<3)return false;
  if(sensorvalue<THRESHOLD)
  {
    if(samplestab[sensornumber]>=NUMBERSAMPLES) // play file
    {
      //tsunami.trackGain(sensornumber,-20);
      // if(sensornumber==1)
      // {
      //   tsunami.trackPlaySolo(1+(song%8), 0); // track = 1 (aka "1.WAV"), output = 0 (aka "1L") 
      //   song++;        
      // }
      // else
      // {
      //     tsunami.trackPlaySolo(sensornumber, 0); // track = 1 (aka "1.WAV"), output = 0 (aka "1L") 
      // }      

      tsunami.trackPlaySolo(sensornumber, 0); 
      playing = true;
      trackplaying = sensornumber;
      
      for(int i=0;i<=11;i++)samplestab[i] = 0;
      delay(1000);
      
    }
    else
    {
      samplestab[sensornumber]=samplestab[sensornumber]+1;      
    }
    return true;
  }
  else
  {
    samplestab[sensornumber]=0;    
  }
  return false;


}

bool checkInitSwitch(int sensornumber, int sensorvalue)
{
  
  if(sensorvalue<3)return false;
  if(sensorvalue<ACTIVATIONTHRESHOLD)
  {
    if(samplestab[sensornumber]>=NUMBERSAMPLES) // un capteur est "activé" à l'allumage => configuration
    {

      switch (sensornumber)
      {
      case 1:
        exclusiveplaying = false;
        break;
      case 2:
      THRESHOLD = 10;
        break;
      case 3:
      THRESHOLD = 15;
        break;
      case 4:
      THRESHOLD = 20;
        break;
      case 5:
      THRESHOLD = 25;
        break;
      case 6:
      THRESHOLD = 35;
        break;
      case 7:
      THRESHOLD = 40;
        break;
      case 8:
      THRESHOLD = 45;
        break;
      case 9:
      THRESHOLD = 50;
        break;
      case 10:
      THRESHOLD = 55;
        break;
      }

      // for(int i=0;i<=11;i++)samplestab[i] = 0;
      // delay(1000);
      
    }
    else
    {
      samplestab[sensornumber]=samplestab[sensornumber]+1;      
    }
    return true;
  }
  else
  {
    samplestab[sensornumber]=0;    
  }
  return false;


}

void test2()
{


  int mesure = ultrasonic1.read();
 
  
  if(mesure<THRESHOLD)
  {
    if(samples>=NUMBERSAMPLES)
    {
      tsunami.trackPlaySolo(1+(song%8), 0); // track = 1 (aka "1.WAV"), output = 0 (aka "1L") 
      song++;
      samples = 0;
      delay(2000);
    }
    else
    {
      samples++;
    }
    
  }
  
}

void checkInitSwitches()
{
  checkInitSwitch(1,ultrasonic1.read());
  checkInitSwitch(2,ultrasonic2.read());
  checkInitSwitch(3,ultrasonic3.read());
  checkInitSwitch(4,ultrasonic4.read());
  checkInitSwitch(5,ultrasonic5.read());
  checkInitSwitch(6,ultrasonic6.read());
  checkInitSwitch(7,ultrasonic7.read());
  checkInitSwitch(8,ultrasonic8.read());
  checkInitSwitch(9,ultrasonic9.read());
  checkInitSwitch(10,ultrasonic10.read());
}

void playActiveUltrasonic()
{
  if(takeMesure(1,ultrasonic1.read()))return;
  if(takeMesure(2,ultrasonic2.read()))return;
  if(takeMesure(3,ultrasonic3.read()))return;
  if(takeMesure(4,ultrasonic4.read()))return;
  if(takeMesure(5,ultrasonic5.read()))return;
  if(takeMesure(6,ultrasonic6.read()))return;
  if(takeMesure(7,ultrasonic7.read()))return;
  if(takeMesure(8,ultrasonic8.read()))return;
  if(takeMesure(9,ultrasonic9.read()))return;
  if(takeMesure(10,ultrasonic10.read()))return;
}


void setup() {
  /*les pins 31,33,...,49 servent de VCC*/


for(int i=31; i<=50; i+=2) {
    pinMode(i, OUTPUT);
  digitalWrite(i,HIGH);
 }


  for(int i=0;i<11;i++)
  {
    samplestab[i]=0;
  }
  
  pinMode(LED_BUILTIN,OUTPUT);
  


    //player.begin();
  // Also initiate the hardware serial port so we can use it for debug printing
  // to the console..
   Serial.begin(9600);

  // player.setVolume(30); 
  //  player.setCycleMode(DY::PlayMode::Sequence); // Play all and stop.
  //player.setCycleMode(DY::PlayMode::Repeat); // Play all and repeat.
  //player.setCycleMode(DY::PlayMode::Random); // Play all randomly and repeat.
    //player.next();
 //player.previous (); 
 //player.volumeIncrease();
  //player.volumeDecrease();
 
  // player.stop();

    
  
   Wire.begin();

  // Check to see if Tsunami Qwiic is present on the bus
  // Note, here we are calling begin() with no arguments = defaults (address:0x13, I2C-port:Wire)
  if (tsunami.begin() == false)
  {
    Serial.println("Tsunami Qwiic failed to respond. Please check wiring and possibly the I2C address. Freezing...");
    while (1);      
  }; 

  

  Serial.println("All done!");
  unsigned long starttime = millis();
  while((millis()-starttime)<3000)
  {
    checkInitSwitches();
    delay(100);
  }
   for(int i=0;i<=11;i++)samplestab[i] = 0;

  Serial.print("THRESHOLD = ");
  Serial.println(THRESHOLD);
  Serial.print("exclusiveplaying = ");
  Serial.println(exclusiveplaying);
  
  tsunami.trackPlaySolo(100, 0);
  
  //delay(3000);
  
  
}


void loop()
{
  if (exclusiveplaying && playing)
  {
    if (!tsunami.isTrackPlaying(trackplaying))
    {
      playing = 0;
    }
  }
  else
  {
    playActiveUltrasonic();
  }
}

/******************************************************************************
  Example_01_PlayFile.ino
  Example for the SparkFun Tsunami Super WAV Trigger Qwiic
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: Jamie Robertson (info@robertsonics.com), Pete Lewis
  Date Created: 6/6/2021
  ~

  This example plays the WAV file named "1.wav" on the SD card.
  Note, to learn more about file-naming rules for the Tsunami,
  Please visit: 
  https://learn.sparkfun.com/tutorials/tsunami-hookup-guide/all#tsunami-demonstration

  The WAV file (or files) needed for this example can be downloaded from the
  SparkFun Hookup Guide here:
  https://learn.sparkfun.com/tutorials/tsunami-super-wav-trigger-hookup-guide

  Resources:
  Wire.h (included with Arduino IDE)
  SparkFun_Tsunami_Qwiic.h (included in the src folder) http://librarymanager/All#SparkFun_Tsunami

  Development environment specifics:
  Arduino 1.8.15
  Tsunami Hardware Version v21
  Redboard Qwiic Version v10

  Distributed as-is; no warranty is given.
******************************************************************************/


// void setup() {
  
//   Serial.begin(115200);

//   Serial.println("Tsunami Qwiic Example 1 - Play File");
  
//   Wire.begin();

//   // Check to see if Tsunami Qwiic is present on the bus
//   // Note, here we are calling begin() with no arguments = defaults (address:0x13, I2C-port:Wire)
//   if (tsunami.begin() == false)
//   {
//     Serial.println("Tsunami Qwiic failed to respond. Please check wiring and possibly the I2C address. Freezing...");
//     while (1);      
//   }; 

//   tsunami.trackPlaySolo(1, 0); // track = 1 (aka "1.WAV"), output = 0 (aka "1L") 

//   Serial.println("All done!");
// }

// void loop() 
// {

// }