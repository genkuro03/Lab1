#define sensitivity 0x3F  // Sensitivity setting (adjusted in writeRegister)
#include <Arduino.h>
#include <Wire.h>  // I2C interface
#include <Adafruit_CAP1188.h>
#include <TFT_eSPI.h>
#include <SPI.h>

SPIClass mySPI(HSPI);

// Pin Definitions
const int ledPin = 25;  // LED indicator
const int CS_PIN = 25;  // CAP1188 chip select pin

// CAP1188 SPI Pins
#define CAP1188_RESET 32
#define CAP1188_CS 25
#define CAP1188_MOSI 27
#define CAP1188_MISO 26
#define CAP1188_SCK 33

// Initialize CAP1188 using SPI
Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_SCK, CAP1188_MISO, CAP1188_MOSI, CAP1188_CS, CAP1188_RESET);
TFT_eSPI tft = TFT_eSPI();

// Detection flag
bool detected = false;

void setup() {
    Serial.begin(9600);

    // ESP32 SPI Pin Mappings
    mySPI.begin(CAP1188_SCK, CAP1188_MISO, CAP1188_MOSI, CAP1188_CS);

    // Initialize CAP1188
    if (!cap.begin()) {
        Serial.println(F("CAP1188 not found!"));
    } 
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);  


    tft.init();
    tft.setRotation(1);  // Adjust orientation if needed
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(4);

    uint8_t reg = cap.readRegister(0x1F) & 0x0F;
    cap.writeRegister(0x1F, reg | (sensitivity & 0x70));
}

void loop() {
    mySPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN, LOW);
    mySPI.transfer(0x42);  
    digitalWrite(CS_PIN, HIGH);
    mySPI.endTransaction();


    int touchCount = 0;
    uint8_t touched = cap.touched();




    for (int i = 0; i < 8; i++) {
        if (touched & (1 << i)) {
            touchCount++;
        }
    }

    // Print to Serial
    Serial.print("Touched Sensors: ");
    Serial.println(touchCount);

    // Display result on OLED
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 20);
    tft.print("Touches: ");
    tft.print(touchCount);

    delay(100);

}
