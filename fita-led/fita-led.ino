#include <Adafruit_NeoPixel.h>

#define PIN        2
#define NUMPIXELS 24

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 200

uint8_t cores[] = {2,100,200};

typedef struct{
  uint8_t r;
  uint8_t g;
  uint8_t b;
}Cor;

void setup() {
  pixels.begin();
}

void loop() {
  Cor corLed;

  for(int i=0; i<NUMPIXELS; i++) {

    if(i%4==0){

      //Redifinir as cores
      corLed = (Cor){
        cores[random(0,2)], //vermelho
        cores[random(0,2)], //verde
        cores[random(0,2)], //azul
      };
    }

    pixels.setPixelColor(i, pixels.Color(corLed.r,corLed.g,corLed.b));

    pixels.show();

    delay(DELAYVAL);
  }

}
