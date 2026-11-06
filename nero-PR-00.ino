//libraries 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT11.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

//global variables
#define LED_PIN 23
#define DHT_PIN 32
#define SOUND_PIN 36
#define BUTTON_PIN 14

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 
#define SCREEN_ADDRESS 0x3C

#define RTC_CLK 18
#define RTC_DAT 19
#define RTC_RST 5

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

  rtc.Begin();

  RtcDateTime now = rtc.GetDateTime();

  Serial.printf(
    "%02d:%02d:%02d\n",
    (int)now.Hour(),
    (int)now.Minute(),
    (int)now.Second()
  );

  pinMode(LED_PIN, OUTPUT);
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
  button_and_led();
  if(interface_page == 1) {
    show_temp_and_humidity();
  }
  else if(interface_page == 2) {
    interface_text();
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

//test function for making interface
void interface_text(void) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.print("TEST TEXT TO TEST INTERFACES");
  display.display();
}

//starts the LED if button was pressed
//made to further use button to control an interface
void button_and_led(void) {
  current_button_state = digitalRead(BUTTON_PIN);

  if(last_button_state == 0 && current_button_state == 1) {
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state);
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
