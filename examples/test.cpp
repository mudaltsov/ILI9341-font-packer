#include <Arduino.h>
#include <SPI.h>
#include <ILI9341_t3.h>
#include <font_Arial.h>
#include <SerialFlash.h>

#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 17
#define MOSI 11
#define MISO 12
#define SCK 14
#define FLASH_CS 15
#define TFT_BACKLIGHT 20

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, MOSI, SCK, MISO);

void setup() {
    // Tone down the backlight a bit.
    pinMode(TFT_BACKLIGHT, OUTPUT);
    analogWrite(TFT_BACKLIGHT, 64);

    if (!SerialFlash.begin(FLASH_CS)) {
        while (true) {}
    }

    // Set the display to be landscape, with a tasteful yellow background and black text.
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(ILI9341_YELLOW);
    tft.setTextColor(ILI9341_BLACK);

    SerialFlashFile ff = SerialFlash.open("numbers.bin");

    if (ff != 0) {
      // Allocate enough space for the font.
      void* font_data = malloc(ff.size());
      ff.read(font_data, ff.size());

      ILI9341_t3_font_t* myfont = static_cast<ILI9341_t3_font_t*>(font_data);

      // Fix up the pointers.
      myfont->index = static_cast<byte*>(font_data) + sizeof(ILI9341_t3_font_t);
      int index_size_bits = ((myfont->index1_last - myfont->index1_first) + 1) * myfont->bits_index;
      int index_size = ((index_size_bits + 7) / 8);
      myfont->data = myfont->index + index_size;

      // Don't forget to dereference your pointer here.
      tft.setFont(*myfont);
      tft.setCursor(0, 0);
      tft.println("+-0.123456789a/,23 4546326.0");

      // Use one of the fonts that comes with the library.
      tft.setFont(Arial_13);
      tft.setCursor(150, 210);
      tft.print("OK");
    }
}

void loop() {
}
