#ifndef SPECTRUM_H__
#define SPECTRUM_H__

#include <Arduino.h>
#include <math.h>
#include "ILI9341_t3.h"
#include "BaseAnimation.h"


class Spectrum : public BaseAnimation {
public:
  Spectrum() : BaseAnimation() {};

  void init( ILI9341_t3 tft );
  uint_fast16_t bgColor( void );
  String title();
  void perFrame( ILI9341_t3 tft, FrameParams frameParams );

private:
  uint_fast16_t _step = 0;
  uint_fast8_t _colorPhase = 0;
  uint_fast16_t _bgColor;
  bool _refresh = true;
};

void Spectrum::init( ILI9341_t3 tft ) {
  _bgColor = 0xffffff;
}

uint_fast16_t Spectrum::bgColor( void ) {
  return _bgColor;
}

String Spectrum::title() {
  return "Spectrum";
}

static int count = 0;
static uint16_t line_buffer[320];
static float scale = 10.0;

void Spectrum::perFrame( ILI9341_t3 tft, FrameParams frameParams ) {
    for (int i = 0; i < 240; i++) {
      float red;
      float green;
      float blue;
      float temp = frameParams.levels[i] / 65536.0 * scale;
    
      if (temp < 0.5) {
        red = 0.0;
        green = temp * 2;
        blue = 2 * (0.5 - temp);
      } else {
        red = temp;
        green = (1.0 - temp);
        blue = 0.0;
      }
      
      line_buffer[240 - i - 1] = tft.color565(red * 256, green * 256, blue * 256);;
    }
    tft.writeRect(0, count, 240, 1, (uint16_t*) &line_buffer);
    //tft.setScroll(count++);
    count++;
    count = count % 320;
}

#endif
