#include "Ubidots.h" //Librería Ubidots
#include <Separador.h> // Librería separador de cadena de caracteres
const
char*
UBIDOTS_TOKEN
=
"BBFF-
1NDf6vPrIrZP548XYMrvDjw6wRU0Op"; // Colocar TOKEN de Cuenta Ubidots
const char* WIFI_SSID = "ALEX MG";
// Colocar Nombre de la red Wifi
const char* WIFI_PASS = "VIDAkkfl1969#";
// Colocar Contraseña de la red
Wifi
Ubidots ubidots(UBIDOTS_TOKEN, UBI_TCP); // Comunicación TCP con la
Plataforma Ubidots
Separador s;
void setup() {
Serial.begin(9600); // Habilitamos el puerto de comunicación serial
ubidots.wifiConnect(WIFI_SSID, WIFI_PASS); // Conexión wifi para el módulo
ESP 8266
}
void loop() {

// Condicional que habilita la comunicación entre Arduino Mega y ESP 8266
if(Serial.available()){
String datos_recibidos = Serial.readString(); // Variable donde se guardará la
cadena de caracteres cada vez que el Arduino Mega envié los datos

// Separamos los datos recibidos en el ESP 8266 que se van formando en una matriz
de acuerdo al orden que se va receptando.
String CO2 = s.separa(datos_recibidos, ',', 1);
String NO2 = s.separa(datos_recibidos, ',', 2);
String NH3 = s.separa(datos_recibidos, ',', 3);
String CO = s.separa(datos_recibidos, ',', 4);
String O3 = s.separa(datos_recibidos, ',', 5);
String PM25 = s.separa(datos_recibidos, ',', 6);
String PM10 = s.separa(datos_recibidos, ',', 7);
String temperatura = s.separa(datos_recibidos, ',', 8);
String humedad = s.separa(datos_recibidos, ',', 9);

// Conversión de datos recibidos de formato String a formato Float
float valor1_CO2=CO2.toFloat();
float valor2_NO2=NO2.toFloat();
float valor3_NH3=NH3.toFloat();
float valor4_CO=CO.toFloat();
float valor5_O3=O3.toFloat();
float valor6_PM25=PM25.toFloat();
float valor7_PM10=PM10.toFloat();
float valor8_temperatura=temperatura.toFloat();
float valor9_humedad=humedad.toFloat();

// Datos que se muestran en el monitor Serial
/*Serial.println(valor1_CO2);
Serial.println(valor2_NO2);
Serial.println(valor3_NH3);
Serial.println(valor4_CO);
Serial.println(valor5_O3);
Serial.println(valor6_PM25);
Serial.println(valor7_PM10);
Serial.println(valor8_temperatura);
Serial.println(valor9_humedad);*/

// Variables que se envían y agregan a la plataforma Ubidots
ubidots.add("Dióxido de Carbono(CO2)", valor1_CO2);// Change for your variable
name
ubidots.add("Dióxido de Nitrógeno(NO2)", valor2_NO2);// Change for your
variable name
ubidots.add("Amoniaco(NH3)", valor3_NH3);// Change for your variable name
ubidots.add("Monóxido de Carbono(CO)", valor4_CO);// Change for your variable
name
ubidots.add("Ozono(O3)", valor5_O3);// Change for your variable name
ubidots.add("PM 2.5", valor6_PM25);// Change for your variable name
ubidots.add("PM 10", valor7_PM10);// Change for your variable name
ubidots.add("Temperatura", valor8_temperatura);// Change for your variable name
ubidots.add("Humedad", valor9_humedad);// Change for your variable name

// Condicional para saber si el módulo se esté comunicando y enviando la data
bool bufferSent = false;
bufferSent = ubidots.send(); // Will send data to a device label that matches the
device Id
if (bufferSent) {

// Do something if values were sent properly
Serial.println("Values sent by the device");
}
} }