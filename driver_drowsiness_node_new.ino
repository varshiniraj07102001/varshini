#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "iotkit"
#define WIFI_PASSWORD "12345678910"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyAJE-cMq_FvEh_vM7sc01N3bMBlFO_kXyQ "

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://driver-drowniness-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "varshn34@gmail.com"
#define USER_PASSWORD "FRIEND!!"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial nanoSerial(5, 4); // RX, TX

char buffer[100];
char Buttons;


void setup()
{
  Serial.begin(9600);
  nanoSerial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  /* Assign the api key (required) */
  config.api_key = API_KEY;
  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  //Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);

}

void loop()
{
 while (Serial.available() > 0) {
   
    if (gps.encode(Serial.read())) {
      if (gps.location.isUpdated()) {
    double lat = gps.location.lat();
    double lng = gps.location.lng();

    double altitude = gps.altitude.meters();

    int year = gps.date.year();
    int month = gps.date.month();
    int day = gps.date.day();

    int hour = gps.time.hour();
    int minute = gps.time.minute();
    int second = gps.time.second();

    snprintf(buffer, sizeof(buffer),
             "Latitude: %.8f, Longitude: %.8f, Altitude: %.2f m, "
             "Date/Time: %d-%02d-%02d %02d:%02d:%02d",
             lat, lng, altitude, year, month, day, hour, minute, second);
    Serial.println(buffer);
    
  }
      
    }
    
  }
  if (nanoSerial.available() > 0) {
    Buttons = nanoSerial.read();
    Serial.println(Buttons);


    if (Buttons == 'H') {

      Firebase.setString(fbdo, F("Driver Drowsiness/Heart Rate Status"), "Heart Value RISK...") ;
      Firebase.setString(fbdo, F("Driver Drowsiness/Location Status"), buffer) ;
      Firebase.setString(fbdo, F("Driver Drowsiness/Alert"), "RISK") ;
    }

    else {

      Firebase.setString(fbdo, F("Driver Drowsiness/Heart Rate Status"), "Heart Value is Normal.") ;
      Firebase.setString(fbdo, F("Driver Drowsiness/Alert"), " ") ;
    }
    if (Buttons == 'A') {

      Firebase.setString(fbdo, F("Driver Drowsiness/Alcohol Status"), "ALCOHOL Detected...") ;
      Firebase.setString(fbdo, F("Driver Drowsiness/Location Status"), buffer) ;
      Firebase.setString(fbdo, F("Driver Drowsiness/Alert"), "RISK") ;
    }

    else {
      Firebase.setString(fbdo, F("Driver Drowsiness/Alcohol Status"), "ALCOHOL not Consumed.") ;
    }


    if (Buttons == 'a')
    {
      Firebase.setString(fbdo, F("Driver Drowsiness/Driver Status"), "Seat empty") ;
      Firebase.setString(fbdo, F("Driver Drowsiness/Alert"), " ") ;
    }
    else if (Buttons == 'b')
    {
      Firebase.setString(fbdo, F("Driver Drowsiness/Driver Status"), "Normal") ;
      Firebase.setString(fbdo, F("Driver Drowsiness/Alert"), " ") ;
    }
    /*else  if ( Buttons == 'c')
      {
      Firebase.setString(fbdo, F("Driver Drowsiness/Driver Status"), "yawn") ;
      Firebase.setString(fbdo, F("Driver Drowsiness/Alert"), "RISK") ;
      }*/
    else if ( Buttons == 'd')
    {
      Firebase.setString(fbdo, F("Driver Drowsiness/Driver Status"), "eyes close") ;
      Firebase.setString(fbdo, F("Driver Drowsiness/Alert"), "RISK") ;
      Firebase.setString(fbdo, F("Driver Drowsiness/Location Status"), buffer) ;
    }
    else  if ( Buttons == 'e')
    {
      Firebase.setString(fbdo, F("Driver Drowsiness/Driver Status"), "drowsiness symtoms") ;
      Firebase.setString(fbdo, F("Driver Drowsiness/Alert"), "RISK") ;
      Firebase.setString(fbdo, F("Driver Drowsiness/Location Status"), buffer) ;
    }

  }
  Firebase.setString(fbdo, F("Driver Drowsiness/Driver Status"), " ") ;

}
