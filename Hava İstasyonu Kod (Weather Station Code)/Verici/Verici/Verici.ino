#include  <SPI.h>                     // SPI Arayüzünü dahil ettik.
#include  "RF24.h"                    // RF24 kütüpanelerini dahil ettik.
#include "DHT.h"
#include "Wire.h"
#include "Adafruit_BMP085.h" //Basınç Sensörü Kütüphanesi
Adafruit_BMP085 mySensor; 

float pressure; 
byte data[10];                         // Göndereceğimiz data değişkenini oluşturduk.
const uint64_t pipe = 0xF0F0F0F0A1LL; // Paralel veri yolu adresleri tanımlamasını yaptık.
RF24 radio(6, 7);                    // Modülün CE pinini 6.pin CS pinini de 7.pine bağlayacağımızı belirttik.
#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
void setup()
{
  
  pinMode(3,OUTPUT);
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  mySensor.begin();
  dht.begin();
  radio.begin();                       // Kablosuz iletişimi başlattık.
  radio.openWritingPipe(pipe);         // Gönderim yapacağımız kanalın ID'sini tanımladık.
 pressure=(mySensor.readPressure())/(100); //Basınç Okunuyor
}
void loop()
{
  
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print("Humidity: ");
  Serial.print(h);
 
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  if(t>=25)
  {
    digitalWrite(3,HIGH);
    }
 if(t<25)
  {
    digitalWrite(3,LOW);
    }
 
  
  data[0] = h;
  data[1] = t;
  data[2] = f;
  data[3] = hic;
  data[4]=(pressure-1000);
  radio.write(data, 10);                 //Sonra da bu mesaj gönderilsin.
}

