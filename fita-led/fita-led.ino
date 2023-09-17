#include <Adafruit_NeoPixel.h>

#define PIN        2
#define NUMPIXELS 24

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 200

uint8_t cores[] = {0,16,32,64};

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
      do{
        corLed = (Cor){
          cores[random(0,4)], //vermelho
          cores[random(0,4)], //verde
          cores[random(0,4)], //azul
        };
      }while(corLed.r==0 && corLed.g==0 && corLed.b==0);
    }

    pixels.setPixelColor(i, pixels.Color(corLed.r,corLed.g,corLed.b));

    pixels.show();

    delay(DELAYVAL);
  }

}
