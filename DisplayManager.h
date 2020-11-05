#ifndef DISPLAY_MANAGER_H__
#define DISPLAY_MANAGER_H__

#include <SPI.h>
#include <Wire.h>
#include "ILI9341_t3.h"
#include <Adafruit_FT6206.h>
#include "font_Arial.h"

// TFT pins
#define TFT_DC      6
#define TFT_CS      10
#define TFT_RST    255  // 255 = unused, connect to 3.3V
#define TFT_MOSI     7
#define TFT_SCLK    14
#define TFT_MISO    12

class DisplayManager {
  public:
    DisplayManager();
    void Setup();
    void Loop();
    void Init();

  private:
    ILI9341_t3 _tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
    Adafruit_FT6206 _ts = Adafruit_FT6206();
};

DisplayManager::DisplayManager(){

}

void DisplayManager::Setup(){
  Serial.begin(9600);
  _ts.begin(40);
  _tft.begin();
  _tft.setClock(100000000);
  _tft.fillScreen(ILI9341_BLUE);
  _tft.setRotation( 3 );
  _tft.drawRect(10, 5, 140, 110, ILI9341_BLACK);
  _tft.drawRect(170, 5, 140, 110, ILI9341_BLACK);
  _tft.drawRect(10, 125, 140, 110, ILI9341_BLACK);
  _tft.drawRect(170, 125, 140, 110, ILI9341_BLACK);
  
  _tft.setFont(Arial_48);
  _tft.setTextColor(ILI9341_WHITE);
  _tft.setCursor(20, 40);
  _tft.println("SIN");
  _tft.setCursor(190, 40);
  _tft.println("TRI");
  _tft.setCursor(20, 160);
  _tft.println("SAW");
  _tft.setCursor(190, 160);
  _tft.println("PUL");
}

void DisplayManager::Loop(){
  if (_ts.touched())
  {   
    // Retrieve a point  
    TS_Point p = _ts.getPoint(); 
    // rotate coordinate system
    // flip it around to match the screen.
    p.x = map(p.x, 0, 240, 240, 0);
    p.y = map(p.y, 0, 320, 320, 0);
    int y = _tft.height() - p.x;
    int x = p.y;
    Serial.print(x);
    Serial.print(", ");
    Serial.println(y);
  }  
}

#endif
