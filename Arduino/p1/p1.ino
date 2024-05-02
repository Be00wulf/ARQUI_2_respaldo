#include <DHT.h>
#include <DHT_U.h>

#include <stdio.h>
#include <MQUnifiedsensor.h>

// SENSORES MQ DECLARACION DE VARIABLES Y LIBRERIA
#include <MQUnifiedsensor.h> // LIBRERIA SENSORES MQ
String buff;

// #define ARDUINOBoard("Arduino MEGA") // MODELO DE PLACA
#define Board "Arduino UNO" // MODELO DE PLACA ARDUINO 
#define PinMQ135 A0 //ENTRADA ANALOGICA 0 DE  ARDUINO             PINES CONECTADOS A LA PLACA
#define PinMQ7 A1 //ENTRADA ANALOGICA 1 DE   ARDUINO              PINES CONECTADOS A LA PLACA
#define PinMQ131 A2 //ENTRADA ANALOGICA 2 DE               PINES CONECTADOS A LA PLACA
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm
#define RatioMQ7CleanAir 27.5 //RS / R0 = 27.5 ppm
#define RatioMQ131CleanAir 15 //RS / R0 = 15 ppm                  
#define ADC_Bit_Resolution 10 // 10 bit ADC           //(10)
#define Voltage_Resolution 5 // Volt resolution to calc the voltage         //(5)
#define Type_0 "MQ-135" //MQ135
#define Type_1 "MQ-7" //MQ7
#define Type_2 "MQ-131" //MQ131

// DEFINIR VARIABLE PWM
#define turbina 3
// DECLARAMOS LOS SENSORES MQ
MQUnifiedsensor MQ135(Board, Voltage_Resolution, ADC_Bit_Resolution, PinMQ135, Type_0);
MQUnifiedsensor MQ7(Board, Voltage_Resolution, ADC_Bit_Resolution, PinMQ7, Type_1);
MQUnifiedsensor MQ131(Board, Voltage_Resolution, ADC_Bit_Resolution, PinMQ131, Type_2);
//*********************************************************************************************




// LIBRERIA DHT11
//#include <dht.h> // LIBRERIA DHT
#include <DHT.h> // LIBRERIA DHT
#define DHTTYPE    DHT11
// dht DHT;
#define DHT11_PIN 12
//*********************************************************************************************





// SENSOR PMS5003 DECLARACION DE VARIABLES
//#include "PMS.h" // LIBRERIA PMS

#include "PMS.h" // LIBRERIA PMS
PMS pms(Serial3);
PMS::DATA data;

int PM25; // VARIABLE MATERIAL PARTICULADO PM2.5
int PM10; // VARIABLE MATERIAL PARTICULADO PM10
int PM25_Z; // VARIABLE MATERIAL PARTICULADO PM2.5 - PANTALLA HMI
int PM10_Z; // VARIABLE MATERIAL PARTICULADO PM10 - PANTALLA HMI
//*********************************************************************************************





// PANTALLA HMI NEXTION
#include "Nextion.h"

//CREAR OBJETOS
NexNumber n0 = NexNumber(4,8,"n0");
NexNumber n1 = NexNumber(4,3,"n1");
NexNumber n2 = NexNumber(5,9,"n2");
NexNumber n3 = NexNumber(6,10,"n3");
NexNumber n4 = NexNumber(6,9,"n4");
NexNumber n5 = NexNumber(7,10,"n5");
NexNumber n6 = NexNumber(7,8,"n6");
NexNumber n7 = NexNumber(8,2,"n7");
NexNumber n8 = NexNumber(8,4,"n8");
NexGauge z0 = NexGauge(5,4,"z0");
NexGauge z1 = NexGauge(6,6,"z1");
NexGauge z2 = NexGauge(6,7,"z2");
NexGauge z3 = NexGauge(7,7,"z3");
NexGauge z4 = NexGauge(7,6,"z4");
NexGauge z5 = NexGauge(8,9,"z5");
NexGauge z6 = NexGauge(8,8,"z6");
NexCheckbox c0 = NexCheckbox (12, 15, "c0");
NexCheckbox c1 = NexCheckbox (12, 14, "c1");
NexCheckbox c100 = NexCheckbox (12, 3, "c100");
NexCheckbox c75 = NexCheckbox (12, 4, "c75");
NexCheckbox c50 = NexCheckbox (12, 5, "c50");
NexCheckbox c25 = NexCheckbox (12, 6, "c25");
NexDSButton bt0 = NexDSButton(4,10,"bt0");
NexDSButton bt1 = NexDSButton(5,10,"bt1");

//DECLARAMOS VARIABLES PARA LOS CHECKBOX
uint32_t valorc0;
uint32_t valorc1;
uint32_t valorc100;
uint32_t valorc75;
uint32_t valorc50;
uint32_t valorc25;
uint32_t valorbt0;
uint32_t valorbt1;

// LISTA DE EVENTOS TACTILES PANTALLA HMI
NexTouch *nex_listen_list[] =
{
&c0,
&c1,
&c100,
&c75,
&c50,
&c25,
&bt0,
&bt1,
NULL
};

// FUNCION DE ADQUISICIÓN DE DATOS PARA LOS CHECKBOX
void lectura_estado()
{
c0.getValue(&valorc0);
c1.getValue(&valorc1);
c100.getValue(&valorc100);
c75.getValue(&valorc75);
c50.getValue(&valorc50);
c25.getValue(&valorc25);
bt0.getValue(&valorbt0);
bt1.getValue(&valorbt1);
}
//*********************************************************************************************





void setup() {
Serial.begin(9600); // INICIALIZAR PUERTO
Serial2.begin(9600);// INICIALIZAR PUERTO 2
Serial3.begin(9600);// INICIALIZAR PUERTO 3
pinMode(turbina,OUTPUT);
digitalWrite(turbina,LOW);

// ESTABLECER UN MODELO MATEMÁTICO PARA CALCULAR LA
// CONSENTRACIÓN DE PPM Y EL VALOR DE LAS CONSTANTES
//SENSOR MQ 135
/*
Exponential regression:
GAS|a |b
CO| 605.18 | -3.937
Alcohol NO2 | 77.255 | -3.18
CO2| 110.47 | -2.862
Tolueno| 44.947 | -3.445
NH4 - NH3
Acetona
| 102.2 | -2.473
| 34.668 | -3.369
*/

// CO2
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

//SENSOR MQ 7
/*
Exponential regression:
GAS
|a
|b

DEH2| 69.014 | -1.374
LPG| 700000000 | -7.703
CH4| 60000000000000 | -10.54
CO| 99.042 | -1.518
Alcohol | 40000000000000000 | -12.35
*/
// CO
MQ7.setRegressionMethod(1); //_PPM = a*ratio^b
MQ7.setA(99.042); MQ7.setB(-1.518); // CONFIGURACIÓN DE VALORES
// EN LA ECUACIÓN PARA CALCULAR CO
//SENSOR MQ 131
/*
Exponential regression:
GAS|a
|b
NOx| -462.43 | -2.204
CL2| 47.209 | -1.186
O3| 23.943 | -1.11
*/

// O3
MQ131.setRegressionMethod(1); //_PPM = a*ratio^b
MQ131.setA(23.943); MQ131.setB(-1.11); // CONFIGURACIÓN
// VALORES EN LA ECUACIÓN PARA CALCULAR O3

//INICIALIZACIÓM PANTALLA HMI
nexInit();

//LISTA DE ENVIO DE EVENTOS
c0.attachPop(lectura_estado);
c1.attachPop(lectura_estado);
c100.attachPop(lectura_estado);

DEc75.attachPop(lectura_estado);
c50.attachPop(lectura_estado);
c25.attachPop(lectura_estado);
bt0.attachPop(lectura_estado);
bt1.attachPop(lectura_estado);

//PMS 5003
pms.passiveMode();

// Init MQ 135
MQ135.init();

// Init MQ 7
MQ7.init();

// Init MQ 131
MQ131.init();

// CALIBRACIÓN DE RESISTENCIA R0 PARA LOS SENSORES MQ
Serial.print("***** MQ CAlibration *****");
Serial.println("Calibrating please wait.");
float MQ135calcR0 = 0;
float MQ7calcR0 = 0;
float MQ131calcR0 = 0;
for (int i = 1; i <= 10; i++)
{

//Update the voltage lectures
MQ135.update();
MQ7.update();
MQ131.update();
MQ135calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
MQ7calcR0 += MQ7.calibrate(RatioMQ7CleanAir);
MQ131calcR0 += MQ131.calibrate(RatioMQ131CleanAir);
Serial.print(".");
}
MQ135.setR0(MQ135calcR0 / 10);
MQ7.setR0(MQ7calcR0 / 10);
MQ131.setR0(MQ131calcR0 / 10);
Serial.println(" done!.");
Serial.print("MQ135calcR0 :");
Serial.println(MQ135calcR0 / 10);
Serial.print("MQ7calcR0 :");
Serial.println(MQ7calcR0 / 10);
Serial.print("MQ131calcR0 :");
Serial.println(MQ131calcR0 / 10);
Serial.println("***** MQ CAlibration *****");
}
void loop() {
buff="";

//ACTUALIZAR VALORES DE VOLTAJE PARA LOS SENSORES MQ
MQ135.update();
MQ7.update();
MQ131.update();
// LECTURA DE DATOS - SENSORES MQ
//*********************************************************************************************





// DIÓXIDO DE CARBONO (CO2)
int CO2= MQ135.readSensor();
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





// MONÓXIDO DE CARBONO (CO)
int CO=MQ7.readSensor();
if(CO>1999){
CO = 2000;
}
if(CO<0){
CO = 0;
}
int CO_Z = map(CO,0,1000,0,180);
//*********************************************************************************************





// OZONO (O3)
int O3=MQ131.readSensor();
if(O3>999){
O3 = 1000;
}
if(O3<0){
O3 = 0;
}
int O3_Z = map(O3,0,1000,0,180);
//*********************************************************************************************





// LECTURA DE DATOS - TEMPERATURA Y HUMEDAD (DHT11)
DHT.read11(DHT11_PIN);
int temperatura = DHT.temperature;
int humedad = DHT.humidity;
//*********************************************************************************************





// LECTURA DE DATOS - SENSOR PMS 5003 - PM2.5 - PM10
if (pms.readUntil(data))
{
PM25 = data.PM_AE_UG_2_5;
PM10 = data.PM_AE_UG_10_0;
if(PM25>999 || PM10>999){
PM25 = 1000;
PM10 = 1000;
}
if(PM25<0 || PM10<0){
PM25 = 0;
PM10 = 0;
}
PM25_Z = map(PM25,0,1000,0,180);
PM10_Z = map(PM10,0,1000,0,180);
}
//*********************************************************************************************





//MOSTRAR DATOS SENSORES - MONITOR SERIAL
/*Serial.print("Dióxido de Carbono CO2(PPM) : "); Serial.println(CO2);
Serial.print("Dióxido de Nitrógeno NO2(PPM): "); Serial.println(NO2);
Serial.print("Amoniaco NH3(PPM) : "); Serial.println(NH3);
Serial.print("Monóxido de Carbono CO(PPM): "); Serial.println(CO);
Serial.print("Ozono O3(PPM): "); Serial.println(O3);
Serial.print("Temperatura(Cº) : "); Serial.println(temperatura);
Serial.print("Humedad(%): "); Serial.println(humedad);*/
//*********************************************************************************************





// MEMORIA DE ALMACENAMIENTO DE DATOS - SENSORES
buff.concat(',');
buff.concat(CO2);
buff.concat(',');
buff.concat(NO2);
buff.concat(',');
buff.concat(NH3);
buff.concat(',');
buff.concat(CO);
buff.concat(',');
buff.concat(O3);
buff.concat(',');
buff.concat(PM25);
buff.concat(',');
buff.concat(PM10);
buff.concat(',');
buff.concat(temperatura);
buff.concat(',');
buff.concat(humedad);

// ENVIO DE TRAMA DE DATOS AL MODULO ESP 8266
// COMUNICACIÓN SERIAL

-Serial.println(buff);

// TIEMPO DE LECTURA 1 SEGUNDO
delay(1000);

//LISTA DE EVENTOS TACTILES - LINEA DE EJECUCIÓN PANTALLA HMI
nexLoop(nex_listen_list);

//CONDICIONES PARA PANTALLA NEXTION - ENVIO DE DATOS A LA
// PANTALLA HMI

//ACTUALIZAR DATOS EN PANTALLA DE CLIMA
if(valorbt0==1){
n0.setValue(temperatura);
n1.setValue(humedad);
}
else if(valorbt0==0){
//
}

//ACTUALIZAR DATOS EN PANTALLA DE SENSORES
if(valorbt1==1){
n2.setValue(NO2);
z0.setValue(NO2_Z);
n3.setValue(CO2);
z1.setValue(CO2_Z);
n4.setValue(CO);
z2.setValue(CO_Z);
n5.setValue(NH3);
z3.setValue(NH3_Z);
n6.setValue(O3);
z4.setValue(O3_Z);
n7.setValue(PM10);
z5.setValue(PM10_Z);
n8.setValue(PM25);
z6.setValue(PM25_Z);
}
else if(valorbt1==0){
//
}
//*********************************************************************************************






//CONDICIONES PARA PANTALLA NEXTION
//CONTROL Y VELOCIDAD DE TURBINA
// SISTEMA AUTOMATICO DE TURBINA
if(valorc1==1){

//CONDICIONES PARA CO2
if(CO2>400 && CO2<= 600){analogWrite(turbina,150);}
else if(CO2>600 && CO2<= 800){analogWrite(turbina,175);}
else if(CO2>800 && CO2<= 1000){analogWrite(turbina,200);}
else if(CO2>1000){analogWrite(turbina,255);}

//CONDICIONES PARA NO2
else if(NO2>100 && NO2<=200){analogWrite(turbina,150);}
else if(NO2>200 && NO2<=400){analogWrite(turbina,175);}
else if(NO2>400 && NO2<=1000){analogWrite(turbina,200);}
else if(NO2>1000){analogWrite(turbina,255);}

//CONDICIONES PARA NH3
else if(NH3>250 && NH3<=400){analogWrite(turbina,150);}
else if(NH3>400 && NH3<=700){analogWrite(turbina,175);}
else if(NH3>700 && NH3<=1000){analogWrite(turbina,200);}
else if(NH3>1000){analogWrite(turbina,255);}

//CONDICIONES PARA CO
else if(CO>400 && CO<=500){analogWrite(turbina,150);}
else if(CO>500 && CO<=1000){analogWrite(turbina,175);}
else if(CO>1000 && CO<=1500){analogWrite(turbina,200);}
else if(CO>1500){analogWrite(turbina,255);}

//CONDICIONES PARA O3
else if(O3>130 && O3<=240){analogWrite(turbina,150);}
else if(O3>240 && O3<=380){analogWrite(turbina,175);}
else if(O3>380 && O3<=1000){analogWrite(turbina,200);}
else if(O3>1000){analogWrite(turbina,255);}

//CONDICIONES PARA PM10
else if(PM10>50 && PM10<=100){analogWrite(turbina,150);}
else if(PM10>100 && PM10<=150){analogWrite(turbina,175);}
else if(PM10>150 && PM10<=1000){analogWrite(turbina,200);}
else if(PM10>1000){analogWrite(turbina,255);}

//CONDICIONES PARA PM2.5
else if(PM25>25 && PM25<=50){analogWrite(turbina,150);}
else if(PM25>50 && PM25<=75){analogWrite(turbina,175);}
else if(PM25>75 && PM25<=1000){analogWrite(turbina,200);}
else if(PM25>1000){analogWrite(turbina,255);}
else{analogWrite(turbina,0);}
}





//*********************************************************************************************
// SISTEMA MANUAL DE TURBINA
else if(valorc0==1){
if(valorc100==1){
analogWrite(turbina,255);
}
else if(valorc75==1){
analogWrite(turbina,200);
}
// 86
else if(valorc50==1){
analogWrite(turbina,175);
}
else if(valorc25==1){
analogWrite(turbina,150);
}
else{
analogWrite(turbina,0);
}
}
//*********************************************************************************************






else{
analogWrite(turbina,0);}}
