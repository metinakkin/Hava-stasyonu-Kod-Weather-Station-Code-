#include <SPI.h>                       // SPI Arayüzünü dahil ettik.
#include "RF24.h"                      // RF24 kütüpanelerini dahil ettik.
#include <LCD5110_Basic.h>
#include<Wire.h> // I2C fonksiyonlarını kullanabilmek için Wire.h kütüphanesi eklendi.
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];
LCD5110 LCD(8, 9, 10, A0, A1);
byte data[10];                          // Göndereceğimiz data değişkenini oluşturduk.
boolean var;                           // Bit şeklinde var değişkeninini oluşturduk.
const uint64_t pipe = 0xF0F0F0F0A1LL;  // Paralel veri yolu adresleri tanımlamasını yaptık.
RF24 radio(6, 7);                     // Modülün CE pinini 6.pin CS pinini de 7.pine bağlayacağımızı belirttik.


void setup()
{

  pinMode(5, OUTPUT);                    // Kırmızı
  pinMode(4, OUTPUT);                    // Mavi
  pinMode(3, OUTPUT);                    // Yeşil
  pinMode(2, OUTPUT);                    // Buzzer
  LCD.InitLCD();
  LCD.setContrast(70);
  LCD.clrScr();

    LCD.setFont(SmallFont);
    LCD.print("HAVA", 30, 10);
    LCD.setFont(SmallFont);
    LCD.print("ISTASYONU", 15, 30);
    delay(4000);
  radio.begin();                         // Kablosuz iletişimi başlattık.
  radio.openReadingPipe(1, pipe);        // Veri alacağımız kanalın ID'sini tanımladık.
  radio.startListening();
  Serial.begin(9600);


}

void loop()
{ 
    LCD.clrScr();
    LCD.setFont(SmallFont);
    LCD.print("NEM", 32, 10);
    LCD.setFont(SmallFont);
    LCD.print("%", 18, 25);
    LCD.setFont(BigNumbers);
    LCD.printNumI(data[0], 28, 30);
    delay(1500);
    LCD.clrScr();
    LCD.setFont(SmallFont);
    LCD.print("SICAKLIK", 23, 10);
    LCD.setFont(BigNumbers);
    LCD.printNumI(data[1], 25, 30);
    LCD.setFont(SmallFont);
    LCD.print(" *C", 54, 30);
    delay(1500);
    LCD.clrScr();
    LCD.setFont(SmallFont);
    LCD.print("SICAKLIK", 23, 10);
    LCD.setFont(BigNumbers);
    LCD.printNumI(data[2], 25, 30);
    LCD.setFont(SmallFont);
    LCD.print(" *F", 54, 30);
    delay(1500);
    LCD.clrScr();
    LCD.setFont(SmallFont);
    LCD.print("HISSEDILEN", 19, 3);
     LCD.setFont(SmallFont);
    LCD.print("SICAKLIK", 23, 15);
    LCD.setFont(BigNumbers);
    LCD.printNumI(data[3], 25, 30);
    LCD.setFont(SmallFont);
    LCD.print(" *C", 54, 30);
    delay(1500);
    LCD.clrScr();
    LCD.setFont(SmallFont);
    LCD.print("BASINC", 23, 10);
    LCD.setFont(BigNumbers);
    LCD.printNumI(data[4]+1000, 0, 30);
    LCD.setFont(SmallFont);
    LCD.print(" HPA", 54, 30);
    delay(1500);
    
    
     
    

  if (radio.available())
  {

    var = false;
    while (!var) {
      var = radio.read(data, 10);

      //LCD.setFont(MediumNumbers);
      //LCD.printNumI(data[0], 15, 30);

      Serial.print("Nem:");
      Serial.print(data[0]);
      Serial.print(" %\t");
      Serial.print("Sicaklik: ");
      Serial.print(data[1]);
      Serial.print(" *C ");
      Serial.print(data[2]);
      Serial.print(" *F\t");
      Serial.print("Hissedilen Sicaklik: ");
      Serial.print(data[3]);
      Serial.print(" *C ");
      Serial.print("Basinc: ");
      Serial.print((data[4] + 1000));
      Serial.println(" hPa ");

      if (data[1] <= 20)
      {
        digitalWrite(5, LOW);
        digitalWrite(4, LOW);
        digitalWrite(3, HIGH);
        digitalWrite(2, LOW);
      }
      if (20 < data[1] <= 25)
      {
        
        digitalWrite(5, LOW);
        digitalWrite(4, HIGH);
        digitalWrite(3, LOW);
        digitalWrite(2, LOW);
      }
      if (25 < data[1])
      {
        digitalWrite(5, HIGH);
        digitalWrite(4, LOW);
        digitalWrite(3, LOW);
       digitalWrite(2, HIGH);
      }

      /* Serial.println(data[1]);
        delay(2000);
        Serial.println(data[2]);
        delay(2000);
        Serial.println(data[3]);
        delay(2000);
        Serial.println(data[4]);
        delay(2000);
      */
    }
  }
}

/*LCD.InitLCD(); Nokia 5110 ekranı başlatır.
  LCD.setFont(SmallFont);Küçük yazı yazmamızı sağlar.
  LCD.setFont(MediumFont); Orta boyda sayı yazmamızı sağlar.
  LCD.setFont(BigFont); büyük boyda sayı yazmamızı sağlar.
  LCD.setContrast(70); Ekran kontrastını ayarlar 0-70 arası.
  LCD.clrScr(); Ekranı temizler.
  LCD.print("Yazılmak istenilen yazı",Dikey Koordinat,Yatay Koordinat);Yazı ve Sayı
  LCD.printNumI("Yazılmak istenilen yazı",Dikey Koordinat,Yatay Koordinat);Sadece Sayı
  LCD.drawBitmap(Yatay Koordinat,Dikey Koordinat,Kayıt edilen resmin adı,Çözünürlük,Çözünürlük); 84x48 olması gerekir.
  LCD.enableSleep(); Ekranı uyku moduna alır.
  LCD.disableSleep();Ekranı uyku moduna çıkarır.
*/
