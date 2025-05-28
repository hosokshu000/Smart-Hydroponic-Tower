#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "GlacialIndifference12pt7b.h"
#include "TDS_read.h"

// Include your image data
#include "image_data.h"

// TFT display pins
#define TFT_CS   17
#define TFT_DC   16
#define TFT_RST  20
#define TFT_BL   21

#define CUSTOM_COLOR 0xDE30

// Create the display object
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

float tdsvalue = 0;

void setup() {
  TDS_setup();

  Serial.begin(115200);
  
  // Initialize backlight
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  
  // Initialize display
  tft.init(240, 280);
  tft.setRotation(2);
  tft.fillScreen(ST77XX_BLACK);
  
  // Draw the image from the included data
  drawImage(myImage, IMAGE_WIDTH, IMAGE_HEIGHT, 120 - IMAGE_WIDTH / 2, 140 - IMAGE_HEIGHT / 2);

  tft.setFont(&GlacialIndifference_Regular12pt7b);
  tft.setTextColor(ST77XX_WHITE);
}

void loop() {
  writeTDS();
}

void drawImage(const uint16_t *image, int16_t width, int16_t height, int16_t x, int16_t y) {
  tft.startWrite();
  for (int16_t j = 0; j < height; j++) {
    for (int16_t i = 0; i < width; i++) {
      uint16_t color = pgm_read_word(&image[j * width + i]);
      tft.writePixel(x + i, y + j, color);
    }
  }
  tft.endWrite();
}

void writeTDS() {
  // Remove bias
  float newTdsValue = GetTDS() - 5;

  if (newTdsValue < 0) {
    newTdsValue = 0;
  }
  
  String oldValueStr = String(tdsvalue, 1);
  String newValueStr = String(newTdsValue, 1);
  
  int16_t x1, y1;
  uint16_t w, h;
  
  // Get exact bounds of old text
  tft.getTextBounds(oldValueStr, 0, 0, &x1, &y1, &w, &h);
  int oldTextWidth = w;
  
  // Get exact bounds of new text
  tft.getTextBounds(newValueStr, 0, 0, &x1, &y1, &w, &h);
  int newTextWidth = w;
  
  int rightEdge = 125; // Adjust as needed
  
  // Clear old text
  tft.setTextColor(CUSTOM_COLOR);
  tft.setCursor(rightEdge - oldTextWidth, 250);
  tft.print(oldValueStr);
  
  // Print new text
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(rightEdge - newTextWidth, 250);
  tft.print(newValueStr);
  
  tdsvalue = newTdsValue;
}
