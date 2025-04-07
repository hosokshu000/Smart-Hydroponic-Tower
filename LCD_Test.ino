#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "GlacialIndifference12pt7b.h"

// Include your image data
#include "image_data.h"

// TFT display pins
#define TFT_CS   17
#define TFT_DC   16
#define TFT_RST  20
#define TFT_BL   21

// Create the display object
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup() {
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
  tft.setCursor(78, 250);
  tft.println("1250");
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

void loop() {
  // Your code here
}