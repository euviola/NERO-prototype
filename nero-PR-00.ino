//libraries 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT11.h>

//global variables
#define LED_PIN 23
#define DHT_PIN 32
#define SOUND_PIN 36
#define BUTTON_PIN 14

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 
#define SCREEN_ADDRESS 0x3C

//global parts
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT11 dht11(DHT_PIN);

void setup() {
  Serial.begin(9600);
  delay(500);

  //checks whether display is working correctly
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  //display the Adafruit logo, then clear it
  display.display();
  delay(2000);
  display.clearDisplay();
  
  //test message for debugging since I'm new to C
  char test_msg[] = "Test text";
  show_text(test_msg);
}

void loop() {
}

//shows message on a display
void show_text(const char* msg) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.cp437(true); //this line was in the example, idk what it does tbh
  for(int i=0; i<strlen(msg); i++) {
    display.write(msg[i]);
  }
  
  display.display();
}

//reads temperature and humidity via dht11
//returns int to be operated upon further
int temp_and_humidity_read(void) {
  int temperature = 0;
  int humidity = 0;

  int result = dht11.readTemperatureHumidity(temperature, humidity);
  return result;
}
