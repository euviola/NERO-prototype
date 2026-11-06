//libraries 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT11.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

//global variables
//sensors, LEDs, buttons, etc.
#define DHT_PIN 32
#define SOUND_PIN 36
#define BUTTON_PIN 14

//OLED data
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 
#define SCREEN_ADDRESS 0x3C

//RTC data
#define RTC_CLK 18
#define RTC_DAT 19
#define RTC_RST 5

//global variables
int last_button_state = 0;
int current_button_state = 0;
bool led_state = false;
int interface_page = 1;

//global parts
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT11 dht11(DHT_PIN);

ThreeWire RTCWire(RTC_DAT, RTC_CLK, RTC_RST);
RtcDS1302<ThreeWire> rtc(RTCWire);

void setup() {
  Serial.begin(115200);

  //starting the clock
  rtc.Begin();

  rtc.SetIsWriteProtected(false);
  rtc.SetIsRunning(true);

  //Set my own date (07.11.2026, 00:27:30). The abberation is like ~2 seconds because of compilation
  RtcDateTime custom(2026, 11, 7, 0, 27, 30);

  /*the commented line below is the line which should be used only once
  when setting your clock (for those who would use this code, however I
  doubt someone will)*/
  //rtc.SetDateTime(custom);

  pinMode(BUTTON_PIN, INPUT);

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
}

void loop() {
  button_interface();
  if(interface_page == 1) {
    show_temp_and_humidity();
  }
  else if(interface_page == 2) {
    show_clock();
  }
}

//reads temperature and humidity via dht11
void show_temp_and_humidity(void) {
  display.clearDisplay();

  float temperature = dht11.readTemperature();
  float humidity = dht11.readHumidity();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.print("Temperature: ");
  display.print(temperature);
  display.print(" C");

  display.setCursor(0, 20);
  display.print("Humidity: ");
  display.print(humidity);
  display.print("%");

  display.display();
}

//shows date and time
void show_clock(void) {
  RtcDateTime now = rtc.GetDateTime();

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);

  //prints date in a "dd.mm.yyyy" format
  display.printf(
    "%02d.%02d.%04d",
    (int)now.Day(),
    (int)now.Month(),
    (int)now.Year()
  );

  display.setTextSize(2);
  display.setCursor(0, 20);

  //prints time
  display.printf(
    "%02d:%02d:%02d",
    (int)now.Hour(),
    (int)now.Minute(),
    (int)now.Second()
  );

    display.display();
}

//control interface via button
void button_interface(void) {
  current_button_state = digitalRead(BUTTON_PIN);

  if(last_button_state == 0 && current_button_state == 1) {
    if(interface_page == 1) {
      interface_page = 2;
    }
    else if(interface_page == 2) {
      interface_page = 1;
    }
  }

  last_button_state = current_button_state;

  delay(100);
}
