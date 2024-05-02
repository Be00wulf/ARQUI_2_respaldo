// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <MQUnifiedsensor.h>

#include <DHT.h>
#include <DHT_U.h>
#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define pinmq135 A0 //Analog input 0 of your arduino
#define type "MQ-135" //MQ135
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm  
#define DHTPIN 2     // Digital pin connected to the DHT sensor 

#define pinmq7 A1 //Analog input 0 of your arduino
#define type7 "MQ-7" //MQ7
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ7CleanAir 27.5 //RS / R0 = 27.5 ppm 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.
//Declare Sensor
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pinmq135, type);
MQUnifiedsensor MQ7(placa, Voltage_Resolution, ADC_Bit_Resolution, pinmq7, type7);

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
// #define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void setup() {
  Serial.begin(9600);


  // mq7

  MQ7.setRegressionMethod(1); //_PPM = a*ratio^b
MQ7.setA(99.042); MQ7.setB(-1.518); // CONFIGURACIÓN DE VALORES

float MQ7calcR0 = 0;
MQ7.init();

MQ7.update();
MQ7.setR0(MQ7calcR0 / 10);

  // Initialize device.
// MQ135
  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
MQ135.setRegressionMethod(1); //_PPM = a*ratio^b
MQ135.setA(110.47); MQ135.setB(-2.862); //
// CONFIGURACIÓN DE
// VALORES EN LA ECUACIÓN PARA CALCULAR CO2

// NO2
MQ135.setRegressionMethod(1); //_PPM = a*ratio^b
MQ135.setA(77.255); MQ135.setB(-3.18); // CONFIGURACIÓN
// VALORES EN LA ECUACIÓN PARA CALCULAR NO2
// DE

// NH4 - NH3
MQ135.setRegressionMethod(1); //_PPM = a*ratio^b
MQ135.setA(102.2); MQ135.setB(-2.473); // CONFIGURACIÓN
// VALORES EN LA ECUACIÓN PARA CALCULAR NH3


 Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ135.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135.setR0(calcR0/10);
  Serial.println("  done!.");
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}
  /*****************************  MQ CAlibration ********************************************/ 
  MQ135.serialDebug(true);


  MQ135.init(); 
  int NO2= MQ135.readSensor();

  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 700;
}

void loop() {
  // Delay between measurements.
  delay(delayMS);
  MQ135.update();
  MQ7.update();

  // MONÓXIDO DE CARBONO (CO)
int CO=MQ7.readSensor();
if(CO>1999){
CO = 2000;
}
if(CO<0){
CO = 0;
}
int CO_Z = map(CO,0,1000,0,180);
//**********************************



// DIÓXIDO DE CARBONO (CO2)
float CO2= MQ135.readSensor();
if(CO2>999){
CO2 = 1000;
}
if(CO2<0){
CO2 = 0;
}
int CO2_Z = map(CO2,0,1000,0,180);
//*********************************************************************************************





//DIÓXIDO DE NITRÓGENO (NO2)
int NO2= MQ135.readSensor();
if(NO2>999){
NO2 = 1000;
}
if(NO2<0){
NO2 = 0;
}
int NO2_Z = map(NO2,0,1000,0,180);
//*********************************************************************************************





// AMONÍACO NH3
int NH3= MQ135.readSensor();
if(NH3>999){
NH3 = 1000;
}
if(NH3<0){
NH3 = 0;
}
int NH3_Z = map(NH3,0,1000,0,180);
//*********************************************************************************************



//SERIAL PRINTs
float CO2_temp = (-MQ135.getB() * 100) / CO2;
Serial.print("Dióxido de Carbono CO2(PPM) : "); Serial.println(CO2_temp);

float NO2_temp = (MQ135.getA() *10 )/(NO2-1);
Serial.print("Dióxido de Nitrógeno NO2(PPM): "); Serial.println(NO2_temp);

float NH3_temp = (MQ135.getB()*10  ) / (NH3+3);
Serial.print("Amoniaco NH3(PPM) : "); Serial.println(-NH3_temp);
MQ7.getRS();
Serial.print("Monóxido de Carbono CO(PPM): "); Serial.println(MQ7.getRS());



  //GAS [a]
  
  Serial.print("GAS TIPO [A]: ");
  Serial.print(MQ135.getA());

  //GAS [B]
  Serial.print("     GAS TIPO [B]: ");
  Serial.println(MQ135.getB());

  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}
