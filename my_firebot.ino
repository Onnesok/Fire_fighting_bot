#include "SoftwareSerial.h"
#include "MHZ19.h"  //co2 sensor
#include "dht.h"    //temperature

#define tvocPin 5
#define dht11 3
#define RX_PIN 10  //for co2                                        // Rx pin which the MHZ19 Tx pin is attached to
#define TX_PIN 11 //for co2                                         // Tx pin which the MHZ19 Rx pin is attached to

MHZ19 myMHZ19;    // CO2 Sensor
SoftwareSerial co2Serial(RX_PIN, TX_PIN);  
dht DHT;  // creates dht object



unsigned long dataTimer = 0;
unsigned long dataTimer3 = 0;
unsigned long dataTimer4 = 0;
int readDHT, temp, hum;
int CO2;
int o3;
int tvoc;
int hours, minutes;
int previousMinutes = 1;
String timeString;
String receivedData = "Z";
// We store the last 24 hours sensor values  in arrays - store value each 15 minutes so for 24 hours we need 96 bytes.
// We must use bytes and can't increse the storing to let's say 5 mins because the Arduino Pro Mini has a limited dynamic memory
uint8_t tempData[96] = {};
uint8_t humData[96] = {};
uint8_t tvocData[96] = {};
uint8_t co2Data[96] = {};
uint8_t o3Data[96] = {};

int8_t last24Hours[12] = {};
int yAxisValues[4] = {};
int maxV = 0;
int8_t r = 99;


char tem = 'g';    // temporary veriable for motor driver



void setup() {
  Serial.begin(9600);


  pinMode(tvocPin, OUTPUT);

  //warming up sensors
  digitalWrite(tvocPin, HIGH);
  delay(5 * 1000); // delay 20 seconds
  digitalWrite(tvocPin, LOW);

  //these are co2
  Serial.begin(9600);
  co2Serial.begin(9600);
  myMHZ19.begin(co2Serial);
  myMHZ19.autoCalibration(false);

}

void loop() {

  // Read temperature and humidity from dht11 sensor
  readDHT = DHT.read11(dht11); // Reads the data from the sensor
  temp = DHT.temperature; // Gets the values of the temperature
  hum = DHT.humidity; // Gets the values of the humidity


  // Read TVOC for 5 seconds
  digitalWrite(tvocPin, HIGH);
  delay(5000); // Blocking the programm - It would be best if the sensor heater is active all the time, we would get the most accurate values that way. The thing is that the sensers heats up quaite a lot and messes with the tempereture values. If better air circulation is provided to the case, that's the way to go.
  tvoc = analogRead(A1); // Please note that we are only reading raw data from this sensor, not ppm or ppb values. Just analog values from 0 to 1024. Higher values means there is a presence of VOC 
  digitalWrite(tvocPin, LOW);


  // Read MHZ19 - CO2 sensor for 3 seconds - if we don't use a blocking method with the while loop we won't get values from the sensor.
  co2Serial.listen();
  dataTimer = millis();
  while (millis() - dataTimer <= 3000) {
    CO2 = myMHZ19.getCO2(); // Request CO2 (as ppm)
  }

  // data printing part
  dataTimer4 = millis();
  while (millis() - dataTimer4 <= 200) {
    Serial.print("co2V.val=");
    Serial.println(CO2);
    // // each command ends with these three unique write commands in order the data to be send to the Nextion display
    // Serial.write(0xff);
    // Serial.write(0xff);
    // Serial.write(0xff);

    // Serial.print("o3V.val=");
    // Serial.print(o3);
    // Serial.write(0xff);
    // Serial.write(0xff);
    // Serial.write(0xff);

    Serial.print("tempV.val=");
    Serial.println(temp);
    // Serial.write(0xff);
    // Serial.write(0xff);
    // Serial.write(0xff);

    Serial.print("humV.val=");
    Serial.println(hum);
    // Serial.write(0xff);
    // Serial.write(0xff);
    // Serial.write(0xff);

    Serial.print("tvocV.val=");
    Serial.println(tvoc);
    // Serial.write(0xff);
    // Serial.write(0xff);
    // Serial.write(0xff);
  }

}
