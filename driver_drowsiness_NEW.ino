#include <Wire.h>
#include "MAX30105.h"

#include "heartRate.h"
#include <SoftwareSerial.h>
SoftwareSerial dockSerial(12, 11);
MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
int Sensor_input = A0;
int buzzer = 3;

const int BUTTON = 7; // Naming switch button pin

int BUTTONstate = 0;
String data;
char  dockvalue;
void setup()
{
  Serial.begin(115200);
  dockSerial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode (BUTTON, INPUT);




  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    //Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  //Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}


void loop()
{

  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  BUTTONstate = digitalRead(BUTTON);
  //Serial.println(BUTTONstate);

  if ((irValue > 50000) && (BUTTONstate == 1))
  {
    digitalWrite(buzzer, HIGH);
    Serial.println("H");
    delay(1000);
  }
  else {
    digitalWrite(buzzer, LOW);
  }
  int sensor_Aout = analogRead(Sensor_input);  /*Analog value read function*/
  //Serial.print("Gas Sensor: ");
  //Serial.println(sensor_Aout);   /*Read value printed*/
  // delay(1000);

  if (sensor_Aout > 600 )
  {
    digitalWrite(buzzer, HIGH);
    data = "A";
    Serial.println(data);

    delay(1000);
  }
  else {
    digitalWrite(buzzer, LOW);
  }
  /////////////////////////////

  if (dockSerial.available() > 0) {
    dockvalue = dockSerial.read();
    // Serial.println(dockvalue);

  }
  if ( dockvalue == 'a')
  {
    Serial.println("a");


  }
  else if ( dockvalue == 'b')
  {
    Serial.println("b");

    /*else  if ( dockvalue == 'c')
      {
      Serial.println("YAWN");

      }*/
    else if ( dockvalue == 'd')
    {
      Serial.println("d");

      digitalWrite(buzzer, HIGH);
      delay(1000);
    }
    else  if ( dockvalue == 'e')
    {
      Serial.println("e");
      digitalWrite(buzzer, HIGH);
      delay(1000);
    }
    digitalWrite(buzzer, LOW);
    delay(1000);
  }
