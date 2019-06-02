//modem tanimlaniyor
#define TINY_GSM_MODEM_SIM800
//#define TINY_GSM_RX_BUFFER 512

//kutuphaneler
#include <TinyGPS++.h>//gps kutuphanesi
#include <TinyGsmClient.h>//gsm kutuphanesi
#include <ArduinoHttpClient.h>//http get protokol icin

//GPRS kimlik bilgileri
const char apn[]  = "internet";
const char user[] = "vodafone";
const char pass[] = "vodafone";

//server bilgileri
const char server[] = "takip.mertcolak.com.tr";
const char resource[] = "/mertcolak/gonder.php?veri=test";
const int  port = 80;

String la,lo,st,hd,al,sp,ag; //String veri gonderme icin parametre degiskenleri
bool yeni_veri = false; //gps yeni bilgi kontrolü

TinyGsm modem(Serial2);

//gprs ayarlaniyor
TinyGsmClient client(modem);

//http icin server ve url tanimlaniyor
HttpClient http(client, server, port);

//TinyGPS++ 
TinyGPSPlus gps;

//baslangic_parametreleri
void setup() {
  Serial.begin(9600);  //9600 bps debug icin
  Serial1.begin(9600); //9600 bps gps haberlesme icin
  Serial2.begin(9600); //9500 bps gprs haberlesme icin

  //debug bilgi
  Serial.println("Modem kuruluyor.."); 

  //modem resetleniyor
  modem.restart();

  //modem hakkında bilgi veriliyor
  String modemInfo = modem.getModemInfo();
  Serial.print(F("Modem: "));
  Serial.println(modemInfo);

  TinyGsm modem(Serial2);

  //varsa sim kart pin
  //modem.simUnlock("1234");
  
  //debug icin bilgi
  Serial.println("Ana program baslatildi."); 
  
}

//ana-program
void loop() {
    if (gps.location.isUpdated()){ //gps verileri guncellenirse veri kayda deger
      
      Serial.print("Latitude= "); 
      la=String(gps.location.lat(), 6);
      Serial.print(la);
      Serial.print(" Longitude= "); 
      lo=String(gps.location.lng(), 6);
      Serial.print(lo);
      Serial.print(" Satalite= "); 
      st=String(gps.satellites.value(), 5);
      Serial.print(st);
      Serial.print(" HDOP= "); 
      hd=String(gps.hdop.value(), 5);
      Serial.print(hd);
      Serial.print(" Altitude= "); 
      al=String(gps.altitude.meters(), 7);
      Serial.print(al);
      Serial.print(" Speed/kmph= "); 
      sp=String(gps.speed.kmph(), 6);
      Serial.print(sp);
      Serial.print(" location age= "); 
      ag=String(gps.location.age(), 5);
      Serial.println(ag);//en son guncellenme time
      yeni_veri=true;
    }
  
    smartDelay(1000);//veri güncelleme ve interval
    //gecikme kontrolü
    if (millis() > 5000 && gps.charsProcessed() < 10){
      Serial.println(F("GPS verisi gelmiyor kablolari kontrol edin.!"));
    }
    //yeni guncel veri geldiyse gonderilecek
    if(yeni_veri){
      //guncel gps verisi alindi gprs ile gonderiliyor

      //baz istasyonu kontrol
      Serial.println("Baz istasyonuna baglaniyor..");
        if (!modem.waitForNetwork()) {
            Serial.println(" baglanamadi");
            delay(1000);
            return;
          }
       Serial.println("Baz OK");
       
       //apn baglatisi saglaniyor
       Serial.print(apn);
       Serial.println(" baglaniyor..");
          if (!modem.gprsConnect(apn, user, pass)) {
            Serial.println(" baglanamadi");
            delay(10000);
            return;
          }
       Serial.println("Apn OK");

       //url olusturuluyor
       ///http://takip.mertcolak.com.tr/mertcolak/gonder.php?la=41.220825&lo=32.645947&st=13&hd=215&al=475.7000100&sp=2.981720&ag=34
       Serial.println("Url olusturuluyor");
       String url="/mertcolak/gonder.php?la="+la+"&lo="+lo+"&st="+st+"&hd="+hd+"&al="+al+"&sp="+sp+"&ag="+ag;
       int url_say = url.length() + 1;
       char kaynak[url_say];
       strcpy( kaynak, url.c_str() );
       
       //http-get ile bilgi gonderiliyor
       Serial.print("Http-get_istegi...");
        int err = http.get(kaynak);
        if (err != 0) {
          Serial.println("gonderilemedi");
          delay(10000);
          return;
        }

        //http-get den gelen bilgi debug icin gosteriliyor
          int durum = http.responseStatusCode();
          Serial.println(durum);
          if (!durum) {
            delay(10000);
            return;
          }

          //header_read okunuyor
          while (http.headerAvailable()) {
            String headerName = http.readHeaderName();
            String headerValue = http.readHeaderValue();
            //SerialMon.println(headerName + " : " + headerValue);
          }

          //icerik_uzunlugu_hesaplaniyor
          int uzunluk = http.contentLength();
          if (uzunluk >= 0) {
            Serial.print("Gelen cevap uzunlugu ");
            Serial.println(uzunluk);
          }
          //gelen cevapta kesilme olursa debug bilgi
          if (http.isResponseChunked()) {
            Serial.println("Cevap alinamadi.");
          }

          //gelen veri aliniyor debug bilgi
          String body = http.responseBody();
          Serial.println("Cevap ");
          Serial.println(body);

          //gelen veri uzunlugu hesaplanip debug bilgi 
          Serial.print("Gelen veri uzunlugu");
          Serial.println(body.length());

          //http durma debug bilgi 
          Serial.println("Http durduruldu");
          http.stop();
          

      }else{
        //debug icin veri gelmedini bildir
        Serial.println("Guncel GPS verisi yok");
        Serial.println("Yeniden taranıyor..");
        }
  }
  
//gecikme kontrolü
static void smartDelay(unsigned long ms){
  unsigned long start = millis();
  do 
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}
