#ifndef RGBWSTRIP
#define RGBWSTRIP
#endif
#include <Adafruit_NeoPixel.h>

#define DATA_PIN    13
#define NUM_LEDS    29
#define BRIGHTNESS  10

class RGBWStrip {
private:
    Adafruit_NeoPixel* strip = new Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRBW + NEO_KHZ800);

public:
    RGBWStrip() {
        strip->begin();
        strip->setBrightness(BRIGHTNESS);
    }

    void setBrightness(uint8_t brightness) {
        strip->setBrightness(brightness);
        strip->show();
    }

    void fillRGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
        Serial.println("fillRGBW called");

        strip->fill(strip->Color(r, g, b, w));
        strip->show();
    }
};
