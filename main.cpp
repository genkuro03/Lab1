#include <Wire.h>
#include <Adafruit_CAP1188.h>
#include <TFT_eSPI.h>
#include <SPI.h>


// Define I2C
#define I2C_SDA 21
#define I2C_SCL 22

TFT_eSPI tft = TFT_eSPI();


// Initialize CAP1188
Adafruit_CAP1188 cap = Adafruit_CAP1188();

void setup() {
    Serial.begin(9600);
    
    // Initialize I2C
    Wire.begin(I2C_SDA, I2C_SCL);

    // Check if CAP1188 is found
    if (!cap.begin(0x28)) {  // Use 0x29 if ADDR pin is tied to 3.3V
        Serial.println("CAP1188 not found!");
        while (1);
    }

    tft.init();
    tft.setRotation(1);  // Adjust orientation if needed
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(4);
}

void loop() {
    uint8_t touched = cap.touched();
    int touchCount = 0;

    // Count how many sensors are touched
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

    delay(100); // Adjust refresh rate
}
