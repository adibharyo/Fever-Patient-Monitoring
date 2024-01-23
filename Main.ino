#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <driver/rtc_io.h>

#define WIFI_SSID " " //Insert WiFi SSID
#define WIFI_PASSWORD " " //Insert WiFi Password

#define BOT_TOKEN "  " //Insert Telegram BOT Token
#define CHAT_ID "  " //Insert Telegram Chat ID

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

#define TIME_TO_SLEEP  5        //Time ESP32 will go to sleep (in seconds) 
#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex

RTC_DATA_ATTR int bootCount = 0;
int i;

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, & Wire, OLED_RESET);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float value;
float celcius;
int stats;
int irsensor = 32;


void setup() {
  Serial.begin(115200);
  Serial.println("Serial Initialized");
  delay(10);
  
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  
 // Attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
  
  //MLX90614 Begin
  mlx.begin();
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  delay(1000);
  display.display();
  display.clearDisplay();

  pinMode(irsensor, INPUT_PULLUP); 
  rtc_gpio_pulldown_en((gpio_num_t)GPIO_NUM_32);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_32,0); 

}

void loop() {
  value = mlx.readObjectTempC();
  celcius = value * 1.0625;
  int up = digitalRead(irsensor);
  if (up == 0){  
      Serial.println("Waking Up...");
      bot.sendMessage(CHAT_ID, "Scanning Temperature....", "");
      if (celcius > 37.5) {
        printSuhuTinggi();
        bot.sendMessage(CHAT_ID, "Suhu Tinggi!! suhu terakhir : " + String(celcius)+" °C", "");
        delay(1000);
      } else {
        printSuhuNormal();
        bot.sendMessage(CHAT_ID, "Suhu Normal, suhu terakhir : " + String(celcius)+" °C", "");
        delay(1000);
      }
      display.clearDisplay();
      delay(1000);
      }
  else{
         printKosong();
         delay(5000);
         Serial.println("Going to sleep after " + String(TIME_TO_SLEEP) + " second");
         for(i = TIME_TO_SLEEP; i>0; i--)
         {
          delay(1000);
          printKosong();
          Serial.println(String(i));
          printCountdown(i);
          display.clearDisplay();
          }
         display.clearDisplay();
         printKosong();
         esp_deep_sleep_start();
         Serial.println("This will never be printed");
  }
  delay(500); 
}

void printCountdown(int hitung) {
  display.setFont( & FreeMonoBold12pt7b);
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(110, 60);
  display.print(String(hitung));
  display.display();
}

void printKosong() {
  display.setFont( & FreeSans9pt7b);
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 30);
  display.print("Semoga Sehat");

  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 50);
  display.print("Selalu :D");
  Serial.println("Tidak ada objek");
  delay(1000);
  display.display();
}

void printSuhuNormal() {
  display.setFont( & FreeSans9pt7b);
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 60);
  display.print("Suhu Normal");

  display.setFont( & FreeSans9pt7b);
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 20);
  display.print("Suhu :");

  display.setFont( & FreeMonoBold12pt7b);
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 40);
  display.print(mlx.readObjectTempC() * 1.0625, 1);
  display.drawCircle(92, 35, 3, WHITE);
  display.setCursor(100, 40);
  display.print("C");
  Serial.println(mlx.readObjectTempC() * 1.0625);
  delay(1000);
  display.display();
}
void printSuhuTinggi() {
  display.setFont( & FreeSans9pt7b);
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 60);
  display.print("Suhu Tinggi");

  display.setFont( & FreeSans9pt7b);
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 20);
  display.print("Suhu :");

  display.setFont( & FreeMonoBold12pt7b);
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 40);
  display.print(mlx.readObjectTempC() * 1.0625, 1);
  display.drawCircle(92, 35, 3, WHITE);
  display.setCursor(100, 40);
  display.print("C");
  Serial.println(mlx.readObjectTempC() * 1.0625);
  delay(1000);
  display.display();
}
