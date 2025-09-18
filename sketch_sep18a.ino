#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin Definitions
#define ONE_WIRE_BUS 2   // DS18B20 data pin
#define HR202_PIN A0     // HR202 analog pin
#define MQ135_PIN A1     // MQ135 analog pin
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// OLED reset pin
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Setup for DS18B20 Temperature Sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Function to initialize sensors and display
void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Initialize the DS18B20 temperature sensor
  sensors.begin();

  // Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
}

// Function to read data from the HR202 Humidity Sensor
float readHumidity() {
  int sensorValue = analogRead(HR202_PIN);
  float voltage = sensorValue * (5.0 / 1023.0);
  float humidity = map(voltage, 0.8, 3.8, 0, 100); // Adjust mapping based on calibration
  return humidity;
}

// Function to read data from the MQ135 Air Quality Sensor
float readAirQuality() {
  int sensorValue = analogRead(MQ135_PIN);
  float voltage = sensorValue * (5.0 / 1023.0);
  return voltage; // Higher voltage indicates poorer air quality
}

// Function to display data on OLED screen
void displayData(float temperature, float humidity, float airQuality) {
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0);
  display.print("Weather Station");

  display.setCursor(0, 20);
  display.print("Temp: ");
  display.print(temperature);
  display.print(" C");
  
  display.setCursor(0, 30);
  display.print("Humidity: ");
  display.print(humidity);
  display.print(" %");
  
  display.setCursor(0, 40);
  display.print("Air Quality: ");
  display.print(airQuality);
  display.print(" V");

  display.display();
}

// Main loop
void loop() {
  // Request temperature readings from DS18B20 sensor
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  
  // Read humidity from HR202 sensor
  float humidity = readHumidity();

  // Read air quality from MQ135 sensor
  float airQuality = readAirQuality();

  // Display the sensor data on the OLED screen
  displayData(temperature, humidity, airQuality);

  // Print the sensor data to Serial Monitor for debugging
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Air Quality (V): ");
  Serial.println(airQuality);

  // Wait 2 seconds before the next loop
  delay(2000);
}