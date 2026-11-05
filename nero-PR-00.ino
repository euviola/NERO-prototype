#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT11.h>

#define LED_PIN 23
#define DHT_PIN 32
#define SOUND_PIN 36
#define BUTTON_PIN 14
#define LED_SCL 22
#define LED_SDA 21

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 
#define SCREEN_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT11 dht11(DHT_PIN);

void setup() {
  Serial.begin(9600);

  delay(500);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();

  char test_msg[] = "Test text";
  show_text(test_msg);
}

void loop() {
}

void show_text(const char* msg) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.cp437(true);
  for(int i=0; i<strlen(msg); i++) {
    display.write(msg[i]);
  }
  
  display.display();
}
